//
// Created by Nathaniel Rupprecht on 2/2/21.
//

#ifndef MANTACLION_PARSERGENERATOR_H
#define MANTACLION_PARSERGENERATOR_H

#include <utility>

#include "ParseNode.h"
#include "LexerGenerator.h"

namespace manta {

enum class ParserType { LR0, SLR, LALR };

//! \brief Object that acts as a deque of work, but you can only add new items if
//! they have never been added before.
template<typename T>
struct WorkDeque {
 public:
  bool Add(const T& item) {
    if (auto it = marked_.find(item); it == marked_.end()) {
      work_.push_back(item);
      marked_.insert(item);
      return true;
    }
    return false;
  }

  T PopNext() {
    auto temp = std::move(work_.front());
    work_.pop_front();
    return temp;
  }

  NO_DISCARD bool Empty() const {
    return work_.empty();
  }

 private:
  //! \brief Current work items.
  std::deque<T> work_;
  //! \brief All items that have ever been seen.
  std::set<T> marked_;
};


class LALRParser;

//! \brief Class that can read a description of a parser and create from that a table-driven LALRParser.
//!
//!     The start of the parser description with be the indicator ".Parser"
class ParserGenerator {
 public:

  //! \brief Create a parser generator of the specified type.
  explicit ParserGenerator(ParserType type = ParserType::SLR);

  //! \brief Parse a description of a grammar from a file to create a parser.
  std::shared_ptr<LALRParser> CreateParserFromFile(const std::string &filename);

  //! \brief Parse a description of a grammar from a string to create a parser.
  std::shared_ptr<LALRParser> CreateParserFromString(const std::string &description);

  //! \brief Parse a description of a grammar from an istream to create a parser.
  std::shared_ptr<LALRParser> CreateParserFromStream(std::istream &stream);

  //! \brief Get the number of non-terminal symbols.
  NO_DISCARD int NumNonTerminals() const;

  //! \brief Get the number of terminal symbols (lexemes).
  NO_DISCARD int NumTerminals() const;

  //! \brief Get the ID of a non-terminal (lexeme).
  NO_DISCARD int GetNonterminalID(const std::string &non_terminal) const;

  //! \brief Compute the first set of a symbol. This is the set of terminals that begin
  //! strings derivable from X.
  //!
  //! If x is a terminal, then First(x) = { x }
  //! If X -> <null> is a production, then <null> \in First(X)
  //! If X -> A_1 A_2 A_3 ... is a production, then
  //!     If <null> \notin First(A_2), then First(X) = First(A_1)
  //!     Else if <null> \notin First(A_3), then First(X) = First(A_1) \ <null> \union First(A_2)
  //!     etc...
  //!     If <null> \in First(A_k) for all k, then <null> \in First(X)
  //!
  //! \param symbol The symbol, as an integer ID.
  //! \return The follow set of symbol.
  std::set<int> FirstSet(int symbol);
  std::set<std::string> FirstSet(const std::string &symbol);

  //! \brief Compute the follow set of a symbol.
  //!
  //! Follow(start) = { $ } (eof follows the start symbol)
  //! For productions X -> p B q, where p, q are any symbols or strings of symbols,
  //!     First(q) \ { @epsilon } \subset Follow(B)
  //! For productions X -> p B, Follow(A) \subset Follow(B)
  //! For productions A -> p B q where q =>* @epsilon, First(q) \ { @epsilon } U Follow(A) \subset Follow(B)
  //!
  //! \param symbol The symbol in question.
  //! \return The follow set.
  std::set<int> FollowSet(int symbol);
  std::set<std::string> FollowSet(const std::string &symbol);

  // ======================================================
  //  Diagnostics
  // ======================================================

  //! \brief Write all the states (as item sets) to an ostream.
  void WriteStates(std::ostream &out) const;

  //! \brief Get the parser generation trace.
  std::string GetParserGenerationTrace() const;

  // ======================================================
  //  Exceptions
  // ======================================================

  //! \brief An exception class that represents encountering an unexpected character type or state while parsing.
  EXCEPTION_MESSAGE_CTOR(UnexpectedInput);

  //! \brief An exception class the signals that a lexeme type (@...) was not recognized by the lexer generator.
  EXCEPTION_MESSAGE_CTOR(UnrecognizedLexerItem);

 private:
  // ======================================================
  //  Private helper functions.
  // ======================================================

  //! \brief Write the name of a terminal or non-terminal. Writes reserved terminals with quotes.
  NO_DISCARD std::string nameOf(int id) const;

  //! \brief Write an item (a production rule + bookmark) to a string.
  NO_DISCARD std::string writeItem(const Item &item) const;

  //! \brief Write a representation of an entire state to an ostream.
  void writeState(const State &state, ostream &out, int id) const;

  //! \brief Get a production from it's representation in a stream.
  inline void getProductions(std::istream &in, int production_id);

  //! \brief Find the conflict resolution info for a production.
  static void findResInfo(std::istream &in, ResolutionInfo &res_info);

  //! \brief Get the instruction for a production.
  static inline std::shared_ptr<ParseNode> getInstructions(std::istream &fin, int pid);

  //! \brief Get all alphabetical characters and put them into a word. Returns true if the word was *not* terminated
  //! by the EOF. Does not clear word at any point.
  static bool getWord(std::istream &in, std::string &word);

  //! Get all numeric characters and put them into a word. Returns true if the word was *not* terminated
  //! by the EOF. Does not clear word at any point.
  static bool getInteger(std::istream &in, std::string &word);

  //! \brief Get the production number associated with a production name, registering it if it has not
  //! already been registered.
  inline int registerProduction(const string &production);

  //! \brief Shifts the production numbers from being negative to being positive numbers after the last lexer
  //! token number.
  inline void shiftProductionNumbers();

  //! \brief Compute the nonterminal_derives_empty_ vector, which indicates which states can derive empty.
  void createStateDerivesEmpty();

  //! \brief Check if a symbol is a terminal.
  NO_DISCARD bool isTerminal(int id) const;

  //! \brief Check if a symbol is a non-terminal.
  NO_DISCARD bool isNonTerminal(int id) const;

  //! \brief Maps non-terminals (which are numbered starting with NumTerminals
  NO_DISCARD int nonTerminalIndex(int id) const;

  //! \brief Get the index of a production.
  NO_DISCARD int getProductionIndex(const Item& item) const;

  //! \brief Compute the LR0 table from the grammar.
  bool computeLR0();

  //! \brief Adds a state to the parser as it is being built.
  int addState(const State &items);

  //! \brief Fill in the goto for state s.
  void computeGoto(int s);

  //! \brief Compute the closure of state s.
  State closure(int s);

  //! \brief Try to advance the bookmark (dot) of a state, returning the resulting state.
  static State advanceDot(const State &state, int symb);

  //! \brief Fill in the rest of the parser table, calculating the reductions.
  void completeTable();

  //! \brief Set an entry in the parse table. If there is no entry, we set it. If there is
  //! already an entry, resolution is attempted.
  void assertEntry(int state, int symbol, const Entry &action);

  void computeLookahead();

  //! \brief Fill out a row in the parser table.
  void tryRuleInState(int state, const Item &rule);

  //! \brief Tries to find a state in all_states. Returns -1 for failure.
  int findState(const State &items) const;

  //! \brief The internal implementation of the first set calculation. Uses a vector
  //! to keep track of which symbols were already visited by the first calculation.
  std::set<int> internalFirst(int symbol, std::vector<bool> &visited);

  //! \brief A function that is called recursively in the compute follow set algorithm.
  //! Uses a vector to keep track of which symbols were already visited by the follow calculation.
  std::set<int> internalFollow(int symbol, std::vector<bool> &visited);

  //! \brief Returns whether the entire tail of a vector of production symbols can derive
  //! the empty symbol.
  NO_DISCARD bool allDeriveEmpty(const std::vector<int>& rhs, std::size_t start_index) const;

  //! \brief Check whether a state can derive empty.
  NO_DISCARD bool stateDerivesEmpty(const State& state) const;

  // ======================================================
  //  Private member variables.
  // ======================================================

  //! \brief A lexer generator.
  LexerGenerator lexer_generator;

  //! \brief Maps production names to production numbers.
  std::map<string, int> production_map;

  //! \brief Maps production numbers to production names.
  std::map<int, string> inverse_production_map;

  //! \brief The productions for each non-terminal. A State (here) is essentially a set of production rules.
  std::map<int, State> productions_for;

  //! \brief All the productions.
  std::vector<Item> all_productions;

  //! \brief Whether a non-terminal derives empty.
  std::vector<bool> nonterminal_derives_empty_;

  //! \brief The number of terminals in the correspondence vector.
  int num_productions = 0;

  //! \brief Which production is the starting production.
  int start_production = 0;

  //! \brief The name of the start production. By default, this is "start."
  std::string start_production_name = "start";

  //! \brief The total number of lexer ids plus production symbols. The number of columns in the parse_table.
  int total_symbols = 0;

  //! \brief The number to assign to the next production.
  //!
  //! Note: To keep things easy to compare, right now we are starting productions at 1.
  int next_production_label = 1;

  //! \brief The parse table. It is a vector so we can add new states.
  //!
  //! The pair is [ action, state ].
  std::vector<std::vector<Entry>> parse_table;

  //! \brief All the different states.
  std::vector<State> all_states;

  //! \brief Work list for creating table.
  std::deque<int> work_list;

  //! \brief A flag that should be set to false if something fails.
  bool status = true;

  //! \brief What type of parser should be generated.
  ParserType parser_type_;

  //! \brief A string that records the history of the parser generation.
  std::stringstream parser_generation_trace;
};

} // manta
#endif // MANTACLION_PARSERGENERATOR_H
