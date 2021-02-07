//
// Created by Nathaniel Rupprecht on 2/2/21.
//

#include "ParserGenerator.h"
// Other files
#include "LALR-Manta.hpp"

namespace Manta {

    std::shared_ptr<LALRParser> ParserGenerator::createParserFromFile(const std::string &filename) {
        std::ifstream fin(filename);
        if (fin.fail()) {
            return nullptr;
        }
        auto parser = createParserFromStream(fin);
        fin.close();
        return parser;
    }

    std::shared_ptr<LALRParser> ParserGenerator::createParserFromStream(std::istream &stream) {
        char c;
        string production_name;
        int pid;

        // Find the .Parser indicator.
        stream.get(c);
        while (!stream.eof()) {
            if (c == '#') {
                stream.get(c);
                while (!stream.eof() and c != '\n') {
                    stream.get(c);
                }
            }
            else if (c == '.') { // Command
                std::string command;
                stream.get(c);
                while (!stream.eof() && isalpha(c)) {
                    command.push_back(c);
                    stream.get(c);
                }
                if (command == "Parser") {
                    stream.putback(c); // Just in case c is '.'
                    break; // Start lexing.
                }
            }
            else {
                stream.get(c);
            }
        }
        if (stream.eof()) {
            std::cout << "Could not find the .Parser indicator.\n";
            return nullptr;
        }

        stream.get(c);
        bool continue_parse = true;
        while (!stream.eof() && continue_parse) {
            // Pass whitespaces.
            if (c == ' ' || c == '\n' || c == '\r');
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
                while (c != '=' && !stream.eof()) {
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
            else if (c == '|') {
                getProductions(stream, pid);
            }
            // Start of a comment.
            else if (c == '#') {
                // Pass comments.
                while (c != '\n' && !stream.eof()) {
                    stream.get(c);
                }
            }
            // Command
            else if (c == '.') {
                stream.get(c);
                std::string command;
                while (!stream.eof() && isalpha(c)) {
                    command.push_back(c);
                    stream.get(c);
                }
                stream.putback(c);

                if (command == "End") {
                    continue_parse = false;
                }
            }
            // Stop parsing description.
            else if (c == '!') {
                continue_parse = false;
            }

            // Get next character.
            if (continue_parse) {
                stream.get(c);
            }
        }

        // Shift productions, so all terminals and nonterminals have positive numbers.
        shiftProductionNumbers();

        // Generate the parse table.
        computeLR0();

        // Complete the table
        completeTable();

        return std::shared_ptr<LALRParser>(new LALRParser(
                inverse_production_map,
                start_production,
                total_symbols,
                parse_table,
                all_states,
                lexer
                ));
    }

    std::string ParserGenerator::generateCodeFile() {
        std::string headerFile = "#ifndef __PARSER_OUT_H__\n#define __PARSER_OUT_H__\n\n";
        // Includes
        headerFile += "#include \"../src/ParseNode.h\"\n#include \"../src/parser-classes.hpp\"\n";

        headerFile += "\n";

        // Declare class
        headerFile += "class Parser {\npublic:\n";

        headerFile += "\n";
        // Private members
        headerFile += "private:\n";
        for (const auto& f : funcs) {
            headerFile += "\t" + f + ";\n";
        }

        // Record of built-ins
        headerFile += "\n\tstd::vector<std::string> builtIns = {";
        for (const auto& f : builtIns) {
            headerFile += "\n\t\t\"" + toChars(f) + "\",";
        }
        headerFile += "\n\t};\n";
        // Built-ins as functions.
        headerFile += "\n\t// Built in functions.\n\n";
        for (const auto& f : builtIns) {
            headerFile += f + "\n";
        }

        headerFile += "};\n\n#endif // __PARSER_OUT_H__\n";
        return headerFile;
    }

    inline void ParserGenerator::getProductions(std::istream &in, int production_id) {
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
            if (c == '\"') {
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
                    } else {
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
            else if (c == '@') {
                in.get(c);
                while (!isspace(c) && !in.eof()) {
                    acc.push_back(c);
                    in.get(c);
                }

                // Found the default lexer type. Register.
                if (acc == "eof") production.add(lexer.getBuiltInType(0));
                else if (acc == "newline") production.add(lexer.getBuiltInType(1));
                else if (acc == "number") production.add(lexer.getBuiltInType(2));
                else if (acc == "identifier") production.add(lexer.getBuiltInType(3));
                else if (acc == "operator") production.add(lexer.getBuiltInType(4));
                else if (acc == "string") production.add(lexer.getBuiltInType(5));
                else if (acc == "null");      // Null. We don't have to do anything.
                else {
                    std::cout << "Unrecognized default lexer type [" << acc << "], exiting.\n";
                    exit(0);
                }

                // Clear accumulator.
                acc.clear();
            }
                // Start of a comment.
            else if (c == '#') {
                // Pass comments.
                while (c != '\n' && !in.eof()) {
                    in.get(c);
                }
            }
                // Start of the instructions
            else if (c == ':') {
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

    inline std::shared_ptr<ParseNode> ParserGenerator::getInstructions(std::istream &in, int pid) {
        // Setup.
        char c;
        std::string acc;
        auto instruction = std::make_shared<ParseNode>("I");

        // Get first character.
        in.get(c);
        // Loop.
        while (!in.eof() && c != '\n') {
            // Comment
            if (c == '#') {
                // Pass comments.
                while (!in.eof() && c != '\n') in.get(c);
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
                if (c != '(') {
                    cout << "Error: expected an open parenthesis. Found [" << c
                         << "]. (Trying to find the argument for [" << node->designator << "].\n";
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

    inline int ParserGenerator::registerProduction(const string &production) {
        auto it = production_map.find(production);
        if (it == production_map.end()) {
            production_map.emplace(production, num_productions);
            inverse_production_map.emplace(num_productions, production);
            return num_productions--;
        }
        return it->second;
    }

    inline void ParserGenerator::shiftProductionNumbers() {
        // Get the number of terminals.
        int lids = lexer.getNumberOfIDs();

        // Shift the ids in production map.
        for (auto &p : production_map) {
            p.second = lids - p.second;
        }

        // Shift the ids in all productions
        for (auto &item : all_productions) {
            // Correct production.
            item.production = lids - item.production;
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
                item.production = lids - item.production;
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

    bool ParserGenerator::computeLR0() {
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

    int ParserGenerator::addState(const State &items) {
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

    void ParserGenerator::computeGoto(int s) {
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

    State ParserGenerator::closure(int s) {
        // Initialize ans.
        State ans = all_states[s];
        int prev_size = 0;

        // While ans is still growing.
        while (ans.size() != prev_size) {
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
                    if (it == productions_for.end()) {
                        continue;
                    }

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

    State ParserGenerator::advanceDot(const State &state, int symb) {
        // symb may be terminal or nonterminal
        State advance_set;
        // Create set: { A -> a X * b | A -> a * X b in state}
        for (const auto& item : state) {
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

    void ParserGenerator::completeTable() {
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

    void ParserGenerator::assertEntry(int state, int symbol, const Entry &action) {
        if (parse_table[state][symbol].isError()) { // == Error
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

    void ParserGenerator::computeLookahead() {
        /* Used in LALR(1) parser */
    }

    void ParserGenerator::tryRuleInState(int state, const Item &rule) {
        // Make rule into LHS(rule) -> RHS(rule)
        rule.endBookmark();
        // If LHS(rule) -> RHS(rule) * is in State(state)
        if (all_states[state].contains(rule))
            for (int sym = 0; sym < total_symbols; ++sym)
                assertEntry(state, sym, Entry(rule));
    }

    inline int ParserGenerator::findState(const State &items) {
        int s = 0;
        for (const auto &state : all_states) {
            if (state == items) return s;
            // Increment state.
            ++s;
        }
        // Return -1 for false;
        return -1;
    }

}