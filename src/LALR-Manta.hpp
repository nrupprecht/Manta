#ifndef __LALR_PARSER_GENERATOR_HPP__
#define __LALR_PARSER_GENERATOR_HPP__

// http://smlweb.cpsc.ucalgary.ca/start.html
// https://web.cs.dal.ca/~sjackson/lalr1.html

#include "parser-classes.hpp"

namespace Manta {

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

    //! \brief Pretty print the transition table.
    string printTable();

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
    void assertEntry(int, int, const Entry&);
    void computeLookahead();
    void tryRuleInState(int, Item);

    //! \brief Trys to find a state in all_states. Returns -1 for failure.
    inline int findState(State);

    //! \brief Maps production names to production numbers.
    map<string, int> production_map;
    map<int, string> inverse_production_map;

    //! \brief The number of terminals in the correspondence vector.
    int num_productions = 0;

    //! \brief The productions for each nonterminal. A State (here) is essentially a set of production rules.
    map<int, State> productions_for;

    //! \brief All the productions.
    vector<Item> all_productions;

    //! \brief Which production start points to.
    int start_production = 0;

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
    //! 3 - Accept.
    vector<vector<Entry> > parse_table;

    //! \brief All the different states.
    vector<State> all_states;

    //! \brief Work list for creating table.
    std::deque<int> work_list;
  };

}
#endif // __LALR_PARSER_GENERATOR_HPP__
