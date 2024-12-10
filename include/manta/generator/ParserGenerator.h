//
// Created by Nathaniel Rupprecht on 2/2/21.
//

#pragma once

#include <span>

#include "manta/generator/DescriptionParser.h"
#include "manta/generator/LALRPropagationGraph.h"
#include "manta/generator/LexerGenerator.h"
#include "manta/lexer/LexerDFA.hpp"
#include "manta/parser/ParseNode.h"
#include "manta/parser/ParserData.h"
#include "manta/utility/WorkDeque.h"

namespace manta {

//! \brief Determines what type of parser to create.
enum class ParserType {
  LR0,
  SLR,
  LALR
};

// Forward declare LALR parser.
class LALRParser;

//! \brief Class that can read a description of a parser and create from that a table-driven LALRParser.
//!
//! The start of the parser description with be the indicator ".Parser"
class ParserGenerator {
public:
  //! \brief Create a parser generator of the specified type.
  explicit ParserGenerator(ParserType type = ParserType::SLR);

  //! \brief Mutator to set the description_parser_.
  void SetDescriptionParser(std::shared_ptr<DescriptionParser> description_parser);

  //! \brief Set the ParserGenerator's logger.
  void SetLogger(const lightning::Logger& logger) { logger_ = logger; }

  //! \brief Read the description of a Parser from a stream and create all the data necessary to run that
  //!        parser.
  std::shared_ptr<ParserData> CreateParserData(std::istream& stream);

  //! \brief Parse a description of a grammar from a file to create a parser.
  std::shared_ptr<LALRParser> CreateParserFromFile(const std::string& filename);

  //! \brief Parse a description of a grammar from a string to create a parser.
  std::shared_ptr<LALRParser> CreateParserFromString(const std::string& description);

  //! \brief Parse a description of a grammar from an istream to create a parser.
  std::shared_ptr<LALRParser> CreateParserFromStream(std::istream& stream);

  //! \brief Get the number of non-terminal symbols.
  NO_DISCARD int NumNonTerminals() const;

  //! \brief Get the number of terminal symbols (lexemes).
  NO_DISCARD int NumTerminals() const;

  //! \brief Get the ID of a non-terminal (lexeme), by its name.
  NO_DISCARD int GetNonterminalID(const std::string& non_terminal) const;

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
  std::set<std::string> FirstSet(const std::string& symbol);

  //! \brief Compute the first set of a sentence of symbols. This is the set of all terminals that begin
  //!        strings derivable from the sentence.
  //!
  //! TODO: Fill in notes.
  std::set<int> FirstSet(std::span<const int> symbols);

  //! \brief Compute the follow set of a non-terminal.
  //!
  //! Follow(start) = { $ } (eof follows the start symbol)
  //! For productions X -> p B q, where p, q are any symbols or strings of symbols,
  //!     First(q) \ { @epsilon } \subset Follow(B)
  //! For productions X -> p B, Follow(A) \subset Follow(B)
  //! For productions A -> p B q where q =>* @epsilon, First(q) \ { @epsilon } U Follow(A) \subset Follow(B)
  //!
  //! \param non_terminal The symbol in question.
  //! \return The follow set.
  std::set<int> FollowSet(int non_terminal);
  std::set<std::string> FollowSet(const std::string& symbol);

  // ======================================================
  //  Diagnostics
  // ======================================================

  //! \brief Write all the states (as item sets) to an ostream.
  void WriteStates(std::ostream& out) const;

  //! \brief Get all the states.
  const std::vector<State>& GetStates() const;

  //! \brief Get the parse table.
  NO_DISCARD const std::vector<std::vector<Entry>>& GetParseTable() const;

  //! \brief Write the name of a terminal or non-terminal. Writes reserved terminals with quotes.
  NO_DISCARD std::string NameOf(int id) const;

  //! \brief Write an item (a production rule + bookmark) to a string.
  NO_DISCARD std::string WriteItem(const Item& item) const;

  //! \brief Write a production rule to a string.
  NO_DISCARD std::string Write(const ProductionRule& rule) const;

  // ======================================================
  //  Exceptions
  // ======================================================

  //! \brief An exception class that represents encountering an unexpected character type or state while
  //!        parsing.
  EXCEPTION_MESSAGE_CTOR(UnexpectedInput);

  //! \brief An exception class the signals that a lexeme type (@...) was not recognized by the lexer
  //!        generator.
  EXCEPTION_MESSAGE_CTOR(UnrecognizedLexerItem);

private:
  // ======================================================
  //  Private helper functions.
  // ======================================================

  //! \brief Compute the nonterminal_derives_empty_ vector, which indicates which states can derive empty.
  void createStateDerivesEmpty();

  //! \brief Check if a symbol is a terminal.
  NO_DISCARD bool isTerminal(int id) const;

  //! \brief Check if a symbol is a non-terminal.
  NO_DISCARD bool isNonTerminal(int id) const;

  //! \brief Maps non-terminals (which are numbered starting with NumTerminals
  NO_DISCARD int nonTerminalIndex(int id) const;

  //! \brief Compute the LR0 table from the grammar.
  bool computeLR0();

  //! \brief Adds a state to the parser as it is being built.
  int addState(const State& items, std::deque<int>& work_list);

  //! \brief Fill in the goto for state s.
  void computeGoto(int s, std::deque<int>& work_list);

  //! \brief Compute the closure of state s.
  State closure(int s) const;

  //! \brief Try to advance the bookmark (dot) of a state, returning the resulting state.
  static State advanceDot(const State& state, int symb);

  //! \brief Fill in the rest of the parser table, calculating the reductions.
  void completeTable();

  //! \brief Set an entry in the parse table. If there is no entry, we set it. If there is already an entry,
  //!        resolution is attempted.
  void assertEntry(int state, int symbol, const Entry& action);

  //! \brief Step in LALR parser generation to compute the LALR propagation graph and follow sets.
  ItemFollowSet computeLookahead();

  //! \brief Create the LALR propagation graph and initial follow sets.
  ItemFollowSet buildItemForPropGraph();

  //! \brief Propagate the initial follow sets through the LALR propagation graph to get the complete follow
  //!        sets.
  void evalItemForPropGraph(ItemFollowSet& item_follow) const;

  //! \brief Add an edge to the propagation graph. Used by `buildItemForPropGraph`.
  //!
  //! \returns Returns the start and end vertices.
  std::pair<StateItem, StateItem> addEdge(int state_id, int next_element, const Item& cleaned_item);

  //! \brief Fill out a row in the parser table.
  void tryRuleInState(int state, const AnnotatedProductionRule& rule);

  //! \brief The LALR version of computing what actions should be reduce.
  void tryRuleInStateLALR(int state_index,
                          const AnnotatedProductionRule& rule,
                          const ItemFollowSet& item_follow);

  //! \brief Tries to find a state in all_states_. Returns -1 for failure.
  int findState(const State& items) const;

  //! \brief The internal implementation of the first set calculation.
  //!
  //! Uses a vector to keep track of which symbols were already visited by the first calculation.
  std::set<int> internalFirst(int symbol, std::vector<bool>& visited);
  std::set<int> internalFirst(std::span<const int> symbols, std::vector<bool>& visited);

  //! \brief A function that is called recursively in the compute follow set algorithm.
  //!
  //! Uses a vector to keep track of which symbols were already visited by the follow calculation.
  std::set<int> internalFollow(int non_terminal, std::vector<bool>& visited);

  //! \brief Get the productions and place in the rhs of a production where a non-terminal occurs (on the rhs
  //!        of a production).
  std::vector<std::pair<int, int>> productionsIncluding(int non_terminal) const;

  //! \brief Returns whether the entire tail of a vector of production symbols can derive the empty symbol.
  NO_DISCARD bool allDeriveEmpty(std::span<const int> sentence) const;

  //! \brief Check whether a state can derive empty.
  NO_DISCARD bool stateDerivesEmpty(const State& state) const;

  //! \brief Check whether a symbol (terminal or non-terminal) can derive empty.
  //!
  //! This will always be false for terminals.
  bool derivesEmpty(int symbol) const;

  // ======================================================
  //  Private member variables.
  // ======================================================

  //! \brief A description parser for reading the description of the parser from a stream.
  std::shared_ptr<DescriptionParser> description_parser_ = std::make_shared<HandWrittenDescriptionParser>();

  //! \brief The data that defines the parser to create. Includes the lexer.
  std::shared_ptr<ProductionRulesData> production_rules_data_ {};

  //! \brief The parse table. It is a vector so we can add new states.
  //!
  //! The pair is [ action, state ].
  //!
  //! Note: It is of course possible to have a more concise representation of this table, however, I am
  //! primarily concerned right now with just playing around and getting things to work, not making this into
  //! a full production quality product.
  std::vector<std::vector<Entry>> parse_table_;

  //! \brief Resolution info for all entries in the parse table.
  std::vector<std::vector<ResolutionInfo>> resolution_info_;

  //! \brief All the different states.
  std::vector<State> all_states_;

  //! \brief A flag that should be set to false if something fails.
  bool status_ = true;

  //! \brief What type of parser should be generated.
  ParserType parser_type_ = ParserType::LALR;

  //! \brief The LALR propagation graph.
  LALRPropagationGraph propagation_graph_;

  //! \brief Logger for the parser generation.
  lightning::Logger logger_;
};

}  // namespace manta
