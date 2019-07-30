#include "LALR-Manta.hpp"

namespace Manta {

  bool LALRGenerator::parseDescription(const string& fileName) {
    std::ifstream fin(fileName);
    if (fin.fail()) return false;

    char c;
    string production_name;
    int pid;

    fin.get(c);
    while (!fin.eof()) {
      // Pass whitespaces.
      if (c==' ');
      // Start of production
      else if (c=='<') {
        production_name.clear();
        fin.get(c);
        while (c!='>' && !fin.eof()) {
          if (!isspace(c)) production_name.push_back(c);
          else production_name.push_back('_');
          fin.get(c);
        }

        // We should have stopped because we encountered a '>', not because of an eof.
        if (fin.eof()) throw false;

        // Log production.
        pid = registerProduction(production_name);

        // Find '='
        fin.get(c);
        while (c!='=' && !fin.eof()) {
          if (!isspace(c)) throw false;
          fin.get(c);          
        }
        if (fin.eof()) throw false;

        // Get all the production rules. Stream points to character after the '='
        getProductions(fin, pid);
      }
      // OR 
      else if (c=='|') getProductions(fin, pid);
      // Start of a comment.
      else if (c=='#') {
        // Pass comments.
        while (c!='\n' && !fin.eof()) fin.get(c);
      }

      // Get next character.
      fin.get(c);
    }

    // Shift productions, so all terminals and nonterminals have positive numbers.
    shiftProductionNumbers();

    // Generate the parse table.
    computeLR0();

    // Complete the table
    completeTable();

    return true;
  }

  bool LALRGenerator::parseCodeFile(const string& fileName) {
    if (!lexer.openFile(fileName)) return false;

    // Stack of symbols that the parser has read.
    std::stack<Token> working_stack;
    // Symbols being fed into the parser.
    std::deque<Token> incoming_deque;

    cout << endl << endl << "Number of productions: " << num_productions << endl;
    cout << productions_for.size() << endl;

    lexer.openFile(fileName);

    int state = 0; // Starting state.
    while (true) {

      // Refill incoming_deque.
      if (incoming_deque.empty() && !lexer.isEOF()) {
        Token tok = lexer.getNext();

        cout << "Literal: " << tok.literal << ", Type: " << tok.type << endl;

        incoming_deque.push_back(tok);
      }

      // If shift
      working_stack.push(incoming_deque.front());
      incoming_deque.pop_front();

      // EXIT for now.
      if (lexer.isEOF()) return true;
    }

    return true;
  }

  inline void LALRGenerator::getProductions(std::ifstream& fin, int production_id) {
    // Create an "item" to represent the production.
    Item production(production_id);

    // Setup.
    char c;
    string acc;
    // Get first character.
    fin.get(c);
    // Loop.
    while (!fin.eof() && c!='\n') {
      // Start of a literal (terminal). Get the whole thing.
      if (c=='\"') {
        fin.get(c);
        while (c!='\"' && !fin.eof()) {
          acc.push_back(c);
          fin.get(c);
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
      else if (c=='<') {
        fin.get(c);
        while (c!='>' && !fin.eof()) {
          if (!isspace(c)) acc.push_back(c);
          else acc.push_back('_');
          fin.get(c);
        }
        // Found the production. Make sure it is registered.
        int id = registerProduction(acc);
        // If this is the start state
        if (acc=="start") start_state = id;
        // Add production to rule.
        production.add(id);
        // Clear accumulator.
        acc.clear();
      }
      // Start of a default lexer type (terminal).
      else if (c=='@') {
        fin.get(c);
        while (!isspace(c) && !fin.eof()){
          acc.push_back(c);
          fin.get(c);
        }

        // Found the default lexer type. Register.
        if (acc=="eof")             production.add(0);
        else if (acc=="newline")    production.add(1);
        else if (acc=="number")     production.add(2);
        else if (acc=="identifier") production.add(3);
        else if (acc=="operator")   production.add(4);
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
        while (c!='\n' && !fin.eof()) fin.get(c);
      }
      // Start of the instructions
      else if (c==':') {
        // Store the rule.
        // ----->
        getInstructions(fin, production_id);
        // Get instructions goes to the end of the line. Setting this character will end the parse.
        c = '\n';
      }

      // Get next character.
      if (c!='\n') fin.get(c);
    }

    // Done finding the rule. Store the rule.
    auto prod = productions_for.find(production_id);
    if (prod==productions_for.end()) {
      productions_for.insert(pair<int, State>(production_id, State()));
      prod = productions_for.find(production_id);
    } 
    prod->second.insert(production);
  }

  inline void LALRGenerator::getInstructions(std::ifstream& fin, int pid) {
    // Setup.
    char c;
    string acc;
    // Get first character.
    fin.get(c);
    // Loop.
    while (!fin.eof() && c!='\n') {

      // \todo Fill this in.

      fin.get(c);
    }
  }

  inline int LALRGenerator::registerProduction(const string& production) {
    auto it = production_map.find(production);
    if (it==production_map.end()) {
      production_map.insert(pair<string, int>(production, num_productions--));
      return num_productions+1;
    }
    return it->second;
  }

  inline void LALRGenerator::shiftProductionNumbers() {
    // Get the number of terminals.
    int lids = lexer.getNumberOfIDs();

    // Shift the ids in production map.
    for (auto &p : production_map) p.second = lids - p.second;

    // Shift the start state.
    start_state = lids - start_state;

    // Shift the ids in productions_for.
    map<int, State> new_productions_for;
    for (auto &p : productions_for) {
      State state;
      for (auto item : p.second) {
        // Correct production.
        item.production = lids-item.production;
        // Correct productions in the rhs.
        for (auto &i : item.rhs) 
          if (i<0) i = lids-i;
        state.insert(item);
      }
      new_productions_for.insert(pair<int, State>(lids - p.first, state));
    }
    productions_for = new_productions_for;

    total_symbols = lids + production_map.size();
  }

  void LALRGenerator::computeLR0() {
    /*
    states <- 0;
    StartItems <- { Start -> RHS(p) | p in ProductionsFor(Start) }
    StartState <- addState(States, StartItems);
    while (s <- WorkList.ExtractElement() != empty) 
      computeGoto(s);
    return (States, StartState);
    */ 

    auto st = productions_for.find(start_state);
    if (st==productions_for.end()) {
      cout << "Error - could not find productions for the start state.\n";
      exit(0);
    }
    
    // I can't figure out why the compiler insists on using const objects here, so here's a hacky work around.
    State startItems = st->second;
    for (auto & item : startItems) {
      const int *i = &item.bookmark;
      *const_cast<int*>(i) = 0;
    }

    // Add the start state.
    addState(startItems);

    while (!work_list.empty()) {
      int s = work_list.front();

      cout << "Extracting state from work_list: " << s << endl;

      work_list.pop_front();
      computeGoto(s);
    }

    cout << "Done with computeLR0.\n\n";
  }

  int LALRGenerator::addState(State items) {
    /*
    if (items not in states) {
      s <- newState(items)
      States += {s};
      WorkList += {s}
      Table[s][*] = ERROR; // Error is the default.
    }
    else s = FindState(items);
    return s;
    */

    // Try to find a state that is the given collection of items.
    int s = findState(items); 
    // If the items are not a state, create a state for them.
    if (s==-1) {
      all_states.push_back(items);
      s = all_states.size() - 1;
      parse_table.push_back(vector<pair<int,int> >(total_symbols, pair<int, int>(0,0) ));
      work_list.push_back(s);
    }
    // Return the state number.
    return s;
  }

  void LALRGenerator::computeGoto(int s) {
    /*
    closed <- Closure(s)
    for (x in productions and terminals) {
      relevantItems <- advanceDot(closed, x);
      if (!relevantItems.empty()) {
        Table[s][x] <- shift addState(States, relevantItems);
      }
    }
    */

    cout << "Compute goto for " << s << endl;

    State closed = closure(s);
    cout << "State (" << s << "): " << all_states[s] << endl;
    cout << "Closure = " << closed << endl;

    cout << "Table " << s << ":\t";

    for (int x=0; x<total_symbols; ++x) {
      State relevantItems = advanceDot(closed, x);
      if (!relevantItems.empty()) {
        int sn = addState(relevantItems);
        parse_table[s][x] = pair<int, int>(1, sn);

        cout << sn << " ";
      }
      else cout << "x ";
    }
    cout << "\nDone.\n\n";
  } 

  State LALRGenerator::closure(int s) {
    /*
    ans <- state
    prev <- 0
    while (ans!=prev) {
      prev <- ans
      foreach (A -> a * B b  in ans) {
        foreach (p in ProductionsFor(B))
          ans += B -> * RHS(p)
      }
    }
    return ans;
    */

    State ans = all_states[s];
    int prev_size = 0;

    // While ans is still growing.
    while (ans.size()!=prev_size) {
      // Update size.
      prev_size = ans.size();
      // For all productions in ans.
      for (auto A : ans) {
        int bookmark = A.bookmark;
        int next = -1<bookmark && bookmark<A.rhs.size() ? A.rhs.at(bookmark) : -1;

        // If the bookmark was behind a non-terminal, we need to add that nonterminal to the closure.
        if (lexer.getNumberOfIDs() < next) { 

          auto it = productions_for.find(next);
          if (it==productions_for.end()) continue;

          // Productions for next.
          State &state = it->second;
          // Set productions' bookmarks so they are like next -> * RHS(next)
          for (auto & item : state) {
            const int *i = &item.bookmark;
            *const_cast<int*>(i) = 0;
          }

          for (auto &st : state)
            if (ans.find(st)==ans.end()) ans.insert(st);
        }
      }
    }


    return ans;
  }

  State LALRGenerator::advanceDot(const State& state, int symb) {
    // symb may be terminal or nonterminal

    /*
    return ( { A-> a X * b | A -> a * X b in state})
    */

    State advance_set;
    // Create set: { A -> a X * b | A -> a * X b in state}
    for (const auto item : state) {
      int bookmark = item.bookmark;
      if (-1<bookmark && bookmark<item.size() && item.at(bookmark)==symb) {
        Item new_item = item;
        // Increment the bookmark
        ++new_item.bookmark;
        advance_set.insert(new_item);
      }
    }
    return advance_set;
  }

  void LALRGenerator::completeTable() {
    /*
    foreach (state in Table)
      foreach (rule in Productions)
        tryRuleInState(state, rule)
    assertEntry(startState, goalSymbol, ACCEPT)
    */


    // Used by LALR(k) parser.
    computeLookahead();

    /*
    for (int s=0; s<parse_table.size(); ++s) {
      auto entry = parse_table[s];
      for (int i=0; i<total_symbols; ++i)
        tryRuleInState(s, )
    }
    */
  }

  void LALRGenerator::assertEntry(int state, int symbol, int action) {
    if (parse_table[state][symbol]==pair<int, int>(0, 0)) // == Error
      parse_table[state][symbol].first = action; // <- action
    else {
      cout << "Error - Entry already exists!!!\n";
      exit(0);
    }
  }

  void LALRGenerator::computeLookahead() {
    /* Used in LALR(1) parser */
  }

  void LALRGenerator::tryRuleInState(int state, Item rule) {
    /*
    if (LHS(rule) -> RHS(rule) * in State(state))
      foreach X in productions and terminals
        assertEntry(state, X, reduce rule)
    */
  }

  inline int LALRGenerator::findState(State items) {
    int s=0;
    for (const auto & state : all_states) {
      if (state==items) return s;
      // Increment state.
      ++s;
    }
    // Return -1 for false;
    return -1;
  }

}