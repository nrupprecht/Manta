#ifndef __LALR_PARSER_GENERATOR_HPP__
#define __LALR_PARSER_GENERATOR_HPP__

// http://smlweb.cpsc.ucalgary.ca/start.html
// https://web.cs.dal.ca/~sjackson/lalr1.html

#include "lexer-Manta.hpp"

namespace Manta {

  //! \brief Encodes state items like A -> a * X b, etc.
  //!
  //! A state is a set of state items.
  struct Item {
    Item(int p) : production(p), bookmark(-1) {};
    Item(int p, int b) : production(p), bookmark(b) {};

    inline void add(int r) { rhs.push_back(r); }
    inline int at(int i) { return rhs.at(i); }
    inline const int at(int i) const { return rhs.at(i); }
    inline int size() const { return rhs.size(); }

    friend bool operator < (const Item& a, const Item& b) {
      return a.production<b.production || a.bookmark<b.bookmark || less_than(a.rhs, b.rhs);
    }
    friend bool operator == (const Item& a, const Item& b) {
      return a.production==b.production && a.bookmark==b.bookmark && a.rhs==b.rhs;
    }

    friend ostream& operator<<(ostream& out, const Item& item) {
      out << item.production << " -> ";
      for (int i=0; i<item.size() ; ++i) {
        if (i==item.bookmark) out << "* ";
        out << item.rhs[i] << " ";
      }
      if (item.bookmark<0) out << "[*] ";
      else if (item.bookmark==item.size()) out << "* ";
      return out;
    }

    //! \brief The production this is a rule for.
    int production;

    //! \brief The right hand side of the production.
    vector<int> rhs;

    //! \brief The location of the bookmark. 
    //!
    //! Bookmark is in the place *before* bookmark, so e.g. "A -> a * X b" has bookmark=1.
    //! A -1 means no bookmark, this is used when an item is being used to encode a pure production,
    //! not a state item.
    int bookmark;
  };

  //! \brief Define a state to be a set of Items.
  typedef set<Item> State;

  inline ostream& operator<<(ostream& out, const State& state) {
    if (state.empty()) out << "{}";
    else {
      out << "{";
      auto it = state.begin();
      out << *it;
      ++it;
      for (; it!=state.end(); ++it) out << ", " << *it;
      out << "}";
    }

    return out;
  }

  // Terminals can either be represented as literals (if they are reserved words or keywords)
  // or by by %eof, %newline, %number, %string, or %operator.
  // Productions are written with angled brackets, e.g. <declaration>
  //   e.g. "for" <declaration> "in" <range> ":"
  //   <range> ::= "[" %number "..." %number "]"
  //   etc...

  class LALRGenerator {
  public:
    //! \brief Parse a description of a grammer to create a parser.
    bool parseDescription(const string&);

    //! \brief Use the parser to parse the code in the file.
    bool parseCodeFile(const string&);

  private:
    inline void getProductions(std::ifstream&, int);

    inline void getInstructions(std::ifstream&, int);

    inline int registerProduction(const string&);

    //! \brief Shifts the production numbers from being negative to being positive numbers after the last lexer 
    //! token number.
    inline void shiftProductionNumbers();

    //! \brief A lexer.
    Lexer lexer;

    // Compute the LR0 table from the grammar.
    void computeLR0();

    int addState(State);
    void computeGoto(int);
    State closure(int);
    State advanceDot(const State&, int);

    void completeTable();
    void assertEntry(int, int, int);
    void computeLookahead();
    void tryRuleInState(int, Item);

    //! \brief Trys to find a state in all_states. Returns -1 for failure.
    inline int findState(State);

    //! \brief Maps production names to production numbers.
    map<string, int> production_map;

    //! \brief The number of terminals in the correspondence vector.
    int num_productions = 0;

    //! \brief The productions for each nonterminal. A State (here) is essentially a set of production rules.
    map<int, State> productions_for;

    //! \brief Which state is the start state.
    int start_state = 0;

    //! \brief The total number of lexer ids plus production symbols. The number of columns in the parse_table.
    int total_symbols = 0;

    //! \brief The number to assign to the next production.
    int next_production_number = -1;

    //! \brief The parse table. It is a vector so we can add new states.
    //!
    //! The pair is [ action, state ].
    //! 0 - Error.
    //! 1 - Shift.
    //! 2 - Reduce.
    vector<vector<pair<int, int> > > parse_table;

    //! \brief All the different states.
    vector<State> all_states;

    //! \brief Work list for creating table.
    std::deque<int> work_list;
  };

}
#endif // __LALR_PARSER_GENERATOR_HPP__
