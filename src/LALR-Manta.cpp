#include "LALR-Manta.hpp"

namespace Manta {

    bool LALRGenerator::parseDescription(const string& fileName) {
        std::ifstream fin(fileName);
        if (fin.fail()) {
            return false;
        }
        parseDescription(fin);
        fin.close();
        return true;
    }

    bool LALRGenerator::parseDescription(std::istream& stream) {
        char c;
        string production_name;
        int pid;

        stream.get(c);
        bool continue_parse = true;
        while (!stream.eof() && continue_parse) {
            // Pass whitespaces.
            if (c==' ');
                // Start of production
            else if (isalpha(c)) {
                production_name.clear();
                do {
                    production_name.push_back(c);
                    stream.get(c);
                } while (!isspace(c) && !stream.eof());

                // We should have stopped because we encountered a space, not because of an eof.
                if (stream.eof()) {
                    throw false;
                }

                // Log production.
                pid = registerProduction(production_name);

                // Find '='
                stream.get(c);
                while (c!='=' && !stream.eof()) {
                    if (!isspace(c)) {
                        throw false;
                    }
                    stream.get(c);
                }
                if (stream.eof()) {
                    throw false;
                }

                // Get all the production rules. Stream points to character after the '='
                getProductions(stream, pid);
            }
                // OR
            else if (c=='|') getProductions(stream, pid);
                // Start of a comment.
            else if (c=='#') {
                // Pass comments.
                while (c!='\n' && !stream.eof()) stream.get(c);
            }
                // Stop parsing description.
            else if (c=='!') continue_parse = false;

            // Get next character.
            stream.get(c);
        }

        // Shift productions, so all terminals and nonterminals have positive numbers.
        shiftProductionNumbers();

        // Generate the parse table.
        computeLR0();

        // Complete the table
        completeTable();

        return true;
    }

    std::shared_ptr<ParseNode> LALRGenerator::parseCodeFile(const string& fileName) {
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

            if (incoming_symbol<0 || total_symbols<=incoming_symbol) {
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
                for (int i=0; i<size; ++i) {
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
                                production_node->add(collect[index]);
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
    string LALRGenerator::printTable() {
        std::string str;

        // Print out definitions.
        str += repeat('_', (total_symbols+1)*5) + '_';
        str += '\n';
        str += "Token and production definitions:\n";
        str += repeat('-', (total_symbols+1)*5) + '-';
        str += '\n';
        int i=0;
        for (; i<lexer.size(); ++i) {
            str += buffered(i, 4) + ": " + lexer.getTokenLiteral(i) + "\n";
        }
        str += "   -- (Productions) -- \n";
        for (; i<total_symbols; ++i) {
            str += buffered(i, 4) + ": " + inverse_production_map.find(i)->second + "\n";
        }

        // Print table header.
        str += repeat('_', (total_symbols+1)*5) + '_';
        str += '\n';
        str += "St.  |";
        for (int i=0; i<total_symbols; ++i) str += buffered(i, 5);
        str += "\n";
        str += repeat('-', (total_symbols+1)*5) + '-';
        str += '\n';
        // Print transition table.
        for (int s=0; s<all_states.size(); ++s) {
            str += buffered(s, 4) + " | ";
            for (int i=0; i<total_symbols; ++i) str += parse_table[s][i].write(4) + " ";
            str += "\n";
        }
        str += repeat('_', (total_symbols+1)*5) + '_';
        str += '\n';

        // Return the table string.
        return str;
    }

    string LALRGenerator::getParseTrace() {
        return parse_trace;
    }

    inline void LALRGenerator::getProductions(std::istream& in, int production_id) {
        // Create an "item" to represent the production.
        Item production(production_id);

        // Setup.
        char c;
        std::string acc;
        // Get first character.
        in.get(c);
        // Loop.
        while (!in.eof() && c != '\n') {
            // Start of a literal (terminal). Get the whole thing.
            if (c=='\"') {
                in.get(c);
                while (c != '\"' && !in.eof()) {
                    acc.push_back(c);
                    in.get(c);
                }
                // Found the literal. Register.
                if (!acc.empty()) {
                    if (isalpha(acc[0])) {
                        int id = lexer.addKeyword(acc);
                        // Add to production
                        production.add(id);
                    }
                    else {
                        int id = lexer.addOperator(acc);
                        // Add to production
                        production.add(id);
                    }
                }
                // Clear accumulator.
                acc.clear();
            }
                // Start of a production.
            else if (isalpha(c)) {
                do {
                    acc.push_back(c);
                    in.get(c);
                } while (!isspace(c) && !in.eof());
                // Found the production. Make sure it is registered.
                int id = registerProduction(acc);
                // If this is the start state
                if (acc == "start") {
                    start_production = id;
                }
                // Add production to rule.
                production.add(id);
                // Clear accumulator.
                acc.clear();
            }
            // Start of a default lexer type (terminal), or the @null symbol.
            else if (c=='@') {
                in.get(c);
                while (!isspace(c) && !in.eof()){
                    acc.push_back(c);
                    in.get(c);
                }

                // Found the default lexer type. Register.
                if      (acc=="eof")        production.add(lexer.getBuiltInType(0));
                else if (acc=="newline")    production.add(lexer.getBuiltInType(1));
                else if (acc=="number")     production.add(lexer.getBuiltInType(2));
                else if (acc=="identifier") production.add(lexer.getBuiltInType(3));
                else if (acc=="operator")   production.add(lexer.getBuiltInType(4));
                else if (acc=="string")     production.add(lexer.getBuiltInType(5));
                else if (acc=="null");      // Null. We don't have to do anything.
                else {
                    cout << "Unrecognized default lexer type [" << acc << "], exiting.\n";
                    exit(0);
                }

                // Clear accumulator.
                acc.clear();
            }
            // Start of a comment.
            else if (c=='#') {
                // Pass comments.
                while (c != '\n' && !in.eof()) {
                    in.get(c);
                }
            }
            // Start of the instructions
            else if (c==':') {
                // Store the rule.
                production.instructions = getInstructions(in, production_id);
                // Get instructions goes to the end of the line. Setting this character will end the parse.
                c = '\n';
            }

            // Get next character.
            if (c != '\n') {
                in.get(c);
            }
        }

        // Done finding the rule. Store the rule.
        auto prod = productions_for.find(production_id);
        if (prod == productions_for.end()) {
            productions_for.emplace(production_id, State());
            prod = productions_for.find(production_id);
        }
        // Add production to the productions for production_id
        prod->second.insert(production);
        // Add production to all productions.
        all_productions.push_back(production);
    }

    inline std::shared_ptr<ParseNode> LALRGenerator::getInstructions(std::istream& in, int pid) {
        // Setup.
        char c;
        string acc;
        auto instruction = std::make_shared<ParseNode>("I");

        // Get first character.
        in.get(c);
        // Loop.
        while (!in.eof() && c != '\n') {
            // Comment
            if (c=='#') {
                // Pass comments.
                while(!in.eof() && c != '\n') in.get(c);
                if (!in.eof()) in.putback(c);
            }
            else if (isalpha(c)) {
                // Get the whole identifier.
                do {
                    acc.push_back(c);
                    in.get(c);
                } while (isalpha(c));

                // Add a node.
                auto node = std::make_shared<ParseNode>(acc);
                instruction->add(node);
                // Clear accumulator.
                acc.clear();

                // Look for an open parenthesis. The other characters should not occur.
                while (c != '(' && !isalpha(c) && c != '\n') {
                    in.get(c);
                }
                if (c!='(') {
                    cout << "Error: expected an open parenthesis. Found [" << c << "]. (Trying to find the argument for [" << node->designator << "].\n";
                    cout << "Instruction so far is " << *instruction << endl;
                    exit(0);
                }

                // Gather all arguments.
                in.get(c);
                while (!in.eof() && c != ')') {
                    // Pass spaces.
                    if (isspace(c));
                    // Node reference.
                    else if (c == '$') {
                        in.get(c);
                        while (!in.eof() && isdigit(c)) {
                            acc.push_back(c);
                            in.get(c);
                        }
                        if (!in.eof()) in.putback(c);
                        // Add child.
                        node->add(acc);
                        // Clear accumulator.
                        acc.clear();
                    }
                    // String.
                    else if (c == '"') {
                        in.get(c);
                        while (!in.eof() && c != '"') {
                            acc.push_back(c);
                            in.get(c);
                        }
                        // Add child.
                        node->add(acc);
                        // Clear accumulator.
                        acc.clear();
                    }
                        // Argument separators.
                    else if (c == ',') {};

                    // Get next character.
                    in.get(c);
                }
            }

            // Get the next character.
            in.get(c);
        }
        // Return the instruction.
        return instruction;
    }

    inline int LALRGenerator::registerProduction(const string& production) {
        auto it = production_map.find(production);
        if (it == production_map.end()) {
            production_map.emplace(production, num_productions);
            inverse_production_map.emplace(num_productions, production);
            return num_productions--;
        }
        return it->second;
    }

    inline void LALRGenerator::shiftProductionNumbers() {
        // Get the number of terminals.
        int lids = lexer.getNumberOfIDs();

        // Shift the ids in production map.
        for (auto &p : production_map) {
            p.second = lids - p.second;
        }

        // Shift the ids in all productions
        for (auto &item : all_productions) {
            // Correct production.
            item.production = lids-item.production;
            // Correct productions in the rhs.
            for (auto &i : item.rhs) {
                if (i < 0) {
                    i = lids - i;
                }
            }
        }

        // Shift the ids in inverse map
        std::map<int, string> new_inverse_map;
        for (auto &p : inverse_production_map) {
            new_inverse_map.emplace(lids - p.first, p.second);
        }
        inverse_production_map = new_inverse_map;

        // Shift the start state.
        start_production = lids - start_production;

        // Shift the ids in productions_for.
        std::map<int, State> new_productions_for;
        for (auto &p : productions_for) {
            State state;
            for (auto item : p.second) {
                // Correct production.
                item.production = lids-item.production;
                // Correct productions in the rhs.
                for (auto &i : item.rhs) {
                    if (i < 0) {
                        i = lids - i;
                    }
                }
                state.insert(item);
            }
            new_productions_for.insert(pair<int, State>(lids - p.first, state));
        }
        productions_for = new_productions_for;
        // Set total_symbols.
        total_symbols = lids + static_cast<int>(production_map.size());
    }

    bool LALRGenerator::computeLR0() {
        status = true;

        // Find productions for the state state.
        auto st = productions_for.find(start_production);
        if (st == productions_for.end()) {
            std::cout << "Error - could not find productions for the start state.\n";
            status = false;
            return status;
        }

        // I can't figure out why the compiler insists on using const objects here, so here's a hacky work around.
        State startItems = st->second;
        startItems.zero_bookmarks();

        // Add the start state.
        work_list.clear();
        addState(startItems);
        // Go through the work list until it is empty.
        while (!work_list.empty() && status) {
            int s = work_list.front();
            work_list.pop_front();
            computeGoto(s);
        }

        // Return success.
        return status;
    }

    int LALRGenerator::addState(const State& items) {
        // Try to find a state that is the given collection of items.
        int s = findState(items);
        // If the items are not a state, create a state for them.
        if (s == -1) {
            all_states.push_back(items);
            s = static_cast<int>(all_states.size()) - 1;
            // Initialize entries to Error.
            parse_table.emplace_back(total_symbols, Entry());
            work_list.push_back(s);
        }
        // Return the state number.
        return s;
    }

    void LALRGenerator::computeGoto(int s) {
        // Find the closure of state s.
        State closed = closure(s);
        // Try advancing the dot for every symbol.
        for (int x = 0; x < total_symbols; ++x) {
            State relevantItems = advanceDot(closed, x);
            if (!relevantItems.empty()) {
                int sn = addState(relevantItems);
                parse_table[s][x] = Entry(sn);
            }
        }
    }

    State LALRGenerator::closure(int s) {
        // Initialize ans.
        State ans = all_states[s];
        int prev_size = 0;

        // While ans is still growing.
        while (ans.size()!=prev_size) {
            // Update size.
            prev_size = ans.size();
            // For all productions in ans.
            for (auto A : ans) {
                int bookmark = A.bookmark;
                int next = -1 < bookmark && bookmark < A.rhs.size() ? A.rhs.at(bookmark) : -1;

                // If the bookmark was behind a non-terminal, we need to add that nonterminal to the closure.
                if (lexer.getNumberOfIDs() < next) {
                    // Find the production for next.
                    auto it = productions_for.find(next);
                    if (it == productions_for.end()) continue;

                    // Productions for next.
                    State &state = it->second;
                    // Set productions' bookmarks so they are like next -> * RHS(next)
                    state.zero_bookmarks();

                    for (const auto &st : state) {
                        if (ans.find(st) == ans.end()) {
                            ans.insert(st);
                        }
                    }
                }
            }
        }
        return ans;
    }

    State LALRGenerator::advanceDot(const State& state, int symb) {
        // symb may be terminal or nonterminal
        State advance_set;
        // Create set: { A -> a X * b | A -> a * X b in state}
        for (const auto item : state) {
            int bookmark = item.bookmark;
            if (-1 < bookmark && bookmark < item.size() && item.at(bookmark) == symb) {
                Item new_item = item;
                // Increment the bookmark
                ++new_item.bookmark;
                advance_set.insert(new_item);
            }
        }
        return advance_set;
    }

    void LALRGenerator::completeTable() {
        for (int s = 0; s < all_states.size(); ++s) {
            for (const auto &rule : all_productions) {
                tryRuleInState(s, rule);
            }
        }
        // Assert the accept state.
        assertEntry(0, start_production, Entry(true));

        // Used by LALR(k) parser.
        computeLookahead();
    }

    void LALRGenerator::assertEntry(int state, int symbol, const Entry& action) {
        if (parse_table[state][symbol].isError()) {// == Error
            parse_table[state][symbol] = action; // <- action
        }
        else {
            std::cout << "Error - Entry already exists!!!\n";
            std::cout << "State: " << state << ", Symbol: " << symbol << ". Old entry: " << parse_table[state][symbol];
            std::cout << ", " << " New entry: " << action << endl;
            // Set status to false.
            status = false;
        }
    }

    void LALRGenerator::computeLookahead() {
        /* Used in LALR(1) parser */
    }

    void LALRGenerator::tryRuleInState(int state, const Item& rule) {
        // Make rule into LHS(rule) -> RHS(rule)
        rule.endBookmark();
        // If LHS(rule) -> RHS(rule) * is in State(state)
        if (all_states[state].contains(rule))
            for (int sym=0; sym<total_symbols; ++sym)
                assertEntry(state, sym, Entry(rule));
    }

    inline int LALRGenerator::findState(const State& items) {
        int s = 0;
        for (const auto & state : all_states) {
            if (state==items) return s;
            // Increment state.
            ++s;
        }
        // Return -1 for false;
        return -1;
    }

}