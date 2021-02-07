#include "LALR-Manta.hpp"

namespace Manta {

    std::shared_ptr<ParseNode> LALRParser::parseCodeFile(const string& fileName) {
        if (!lexer.openFile(fileName)) {
            return nullptr;
        }

        // Clear any old parse trace data.
        parse_trace.clear();

        // Stack of symbols that the parser has read.
        std::stack<Token> working_stack;
        // Symbols being fed into the parser.
        std::deque<Token> incoming_deque;

        // Building the AST.
        std::deque<std::shared_ptr<ParseNode>> incoming_parse_deque;
        std::deque<std::shared_ptr<ParseNode>> working_parse_deque;

        std::list<int> working_stack_types; // For debugging.

        // A vector for collecting nodes during a reduction.
        std::vector<std::shared_ptr<ParseNode>> collect;

        std::shared_ptr<ParseNode> start_node = nullptr;

        // Push starting state onto the stack.
        working_stack.emplace(start_production, 0);
        working_parse_deque.push_front(start_node);
        working_stack_types.push_back(start_production); // For debugging.

        int count = 0;
        bool accept = false;
        for ( ; !accept; ++count) {
            // Refill incoming_deque if necessary.
            if (incoming_deque.empty()) {
                Token tok = lexer.getNext();
                incoming_deque.push_back(tok);
                incoming_parse_deque.push_back(std::make_shared<ParseNode>(tok.literal)); // For now, use literal.

                // Record getting a new token.
                auto literal = tok.literal;
                if (tok.literal=="\n") literal = "\\n";
                parse_trace += "Getting token: " + toString(tok.type) + ", Literal: [" + literal + "]\n";
            }

            int state = working_stack.top().state;
            int incoming_symbol = incoming_deque.front().type;

            // Record the step and state of the parser.
            parse_trace += "Step: " + toString(count) + ", State: " + toString(state) + ".\n";

            if (incoming_symbol < 0 || total_symbols <= incoming_symbol) {
                cout << "ERROR - bad symbol: " << incoming_symbol << ", Literal: [" << incoming_deque.front().literal << "]. Exiting.\n";
                break;
            }

            // Print the state of the stack.
            for (auto ty : working_stack_types) parse_trace += toString(ty) + " ";
            parse_trace += " | ";
            parse_trace += toString(incoming_deque.front().type) + "\n";

            // Get action from the parse table.
            Entry action = parse_table[state][incoming_symbol];
            Token transfer = incoming_deque.front();

            // If shift
            if (action.isShift()) {
                transfer.state = action.getState(); // Set state
                incoming_deque.pop_front();    // Pop off the incoming stack...
                working_stack.push(transfer);  // and shift onto the working stack.
                working_stack_types.push_back(transfer.type); // For debugging.

                // Shift ParseNode
                working_parse_deque.push_front(incoming_parse_deque.front());
                incoming_parse_deque.pop_front();

                // Record a shift occurring.
                parse_trace += "Shift. State is now " + toString(action.getState()) + ".\n";
            }
            else if (action.isReduce()) {
                int size = action.getRule().size();
                int production = action.getRule().production;

                // Put (newly reduced) production onto the input stack.
                incoming_deque.push_front(Token(production, ""));
                // Create a parse node.
                auto production_node = std::make_shared<ParseNode>(inverse_production_map.find(production)->second);

                // Take nodes that are to be reduced off the stack, and temporarily store them in the collect vector.
                collect.resize(size);
                for (int i = 0; i < size; ++i) {
                    collect[size-i-1] = working_parse_deque.front();
                    working_parse_deque.pop_front();
                    working_stack.pop();
                    working_stack_types.pop_back(); // For debugging.
                }

                // Carry out reduction instructions.
                auto instructions = action.getRule().instructions;
                if (instructions) {
                    for (const auto& instruction : instructions->children) {
                        // Get the designator.
                        string name = instruction->designator;
                        // Rename the new node.
                        if (name == "node") {
                            production_node->designator = instruction->children[0]->designator;
                        }
                        // Add a child to the node.
                        else if (name == "add") {
                            int index = toInt(instruction->children[0]->designator);
                            if (collect[index]) {
                                instruction_add(production_node, collect[index]);

                                // production_node->add(collect[index]);
                                collect[index] = nullptr;
                            }
                        }
                        // Add all the children of the specified token to the node.
                        else if (name == "adopt") {
                            int index = toInt(instruction->children[0]->designator);
                            auto& children = production_node->children;
                            auto& vec = collect[index]->children;
                            children.insert(children.end(), vec.begin(), vec.end());
                            vec.clear();
                        }
                        // Replace the new node with one of the tokens.
                        else if (name == "replace") {
                            int index = toInt(instruction->children[0]->designator);
                            if (collect[index]) {
                                production_node = collect[index];
                                collect[index] = nullptr;
                            }
                        }
                    }
                }
                else {
                    for (auto &node : collect) {
                        production_node->add(node);
                    }
                }
                // Clear collection vector.
                collect.clear();

                // Push node onto the incoming deque.
                incoming_parse_deque.push_front(production_node);

                // Record the reduction occurring.
                parse_trace += "Reduce by " + toString(size) + ". Reduce to a " + toString(production)
                               + " (via " + toString(action.getRule()) + ").\n";
            }
            else if (action.isAccept()) {
                // Set start node to be the parsed program.
                start_node = incoming_parse_deque.front();
                incoming_parse_deque.pop_front();
                // Set accept to true.
                accept = true;

                // Write the acceptance to the parse trace.
                parse_trace += "Accept!\n";
            }
            else if (action.isError()) {
                // Record error in parse trace.
                parse_trace += "ERROR - lexer is at Line " + toString(lexer.getLine()) + ", Column " + toString(lexer.getCharacter()) + ".\n";
                // Also print error to the screen.
                cout << "ERROR - lexer is at Line " << lexer.getLine() << ", Column " << lexer.getCharacter() << ".\n";
                cout << "Exiting.\n\n";
                break;
            }
            // Put a newline into the parser trace.
            parse_trace += "\n";
        }

        // If the parser accepted, return the AST node
        if (accept) {
            return start_node;
        }
        else {
            return nullptr;
        }
    }

    //! \brief Pretty print the transition table.
    string LALRParser::printTable() {
        std::string str;

        // Print out definitions.
        str += repeat('_', (total_symbols+1)*5) + '_';
        str += '\n';
        str += "Token and production definitions:\n";
        str += repeat('-', (total_symbols+1)*5) + '-';
        str += '\n';
        int i = 0;
        for (; i < lexer.size(); ++i) {
            str += buffered(i, 4) + ": " + lexer.getTokenLiteral(i) + "\n";
        }
        str += "   -- (Productions) -- \n";
        for (; i < total_symbols; ++i) {
            str += buffered(i, 4) + ": " + inverse_production_map.find(i)->second + "\n";
        }

        // Print table header.
        str += repeat('_', (total_symbols+1)*5) + '_';
        str += '\n';
        str += "St.  |";
        for (int j = 0; j < total_symbols; ++j) {
            str += buffered(j, 5);
        }
        str += "\n";
        str += repeat('-', (total_symbols+1)*5) + '-';
        str += '\n';
        // Print transition table.
        for (int s = 0; s < all_states.size(); ++s) {
            str += buffered(s, 4) + " | ";
            for (int j = 0; j < total_symbols; ++j) {
                str += parse_table[s][j].write(4) + " ";
            }
            str += "\n";
        }
        str += repeat('_', 5 * (total_symbols + 1)) + '_';
        str += '\n';

        // Return the table string.
        return str;
    }

    std::string LALRParser::printAsMathematica(const std::shared_ptr<ParseNode>& head) {
        std::vector<std::pair<int, int>> connectivity;
        std::vector<std::pair<int, std::string>> labels;

        std::deque<std::pair<std::shared_ptr<ParseNode>, int>> stack;
        stack.emplace_back(head, 0);
        labels.emplace_back(0, head->designator);

        int nodeNumber = 1; // Head was 0.

        while (!stack.empty()) {
            auto& pr = stack.front();
            auto& node = pr.first;
            int number = pr.second;
            for (const auto& child : node->children) {
                connectivity.emplace_back(number, nodeNumber);
                labels.emplace_back(nodeNumber, child->designator);
                stack.emplace_back(child, nodeNumber);
                ++nodeNumber;
            }
            stack.pop_front();
        }

        // --- Form the mathematica command that will print this as a tree.

        auto clean = [] (const std::string& str) {
            std::string out;
            for (char c : str) {
                if (c == '\t') out += "\\t";
                else if (c == '\n') out += "\\n";
                else if (c == '\r') out += "\\r";
                else out += c;
            }
            return out;
        };

        // First, form connectivity.
        std::string mathematicaCommand = "TreePlot[{";
        int count = 0;
        for (int i = 0; i < connectivity.size(); ++i) {
            auto [first, second] = connectivity[i];
            mathematicaCommand += std::to_string(first) + "->" + std::to_string(second) + "";
            if (i != connectivity.size() - 1) {
                mathematicaCommand += ",";
            }
        }
        mathematicaCommand += "},Top,0,VertexLabels->{";
        // Then form vertex labels.
        for (int i = 0; i < labels.size(); ++i) {
            auto [first, name] = labels[i];
            mathematicaCommand += std::to_string(first) + "->\"" + clean(name) + "\"";
            if (i != labels.size() - 1) {
                mathematicaCommand += ",";
            }
        }
        mathematicaCommand += "},ImageSize->Large]";

        return mathematicaCommand;
    }

    string LALRParser::getParseTrace() {
        return parse_trace;
    }

    //

    void LALRParser::instruction_node(Node& self, Node& node) const {

    }

    void LALRParser::instruction_add(Node& self, Node& node) const {
        self->add(node);
    }

    void LALRParser::instruction_adopt(Node& self, Node& node) const {

    }

    void LALRParser::instruction_replace(Node& self, Node& node) const {

    }

}