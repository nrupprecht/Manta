#pragma once

// This website can be used to check things like first sets, follow sets, etc., and to generate various parsing tables, LR, LALR, etc.
// http://smlweb.cpsc.ucalgary.ca/start.html
// https://web.cs.dal.ca/~sjackson/lalr1.html

#include <utility>
#include "ParseNode.h"
#include "ParserUtility.hpp"
#include "LexerDFA.hpp"

namespace manta {


class LALRParser {
  friend class CodeGenerator;
 public:
  //! \brief Use the parser to parse input from a file.
  std::shared_ptr<ParseNode> ParserCodeFile(const string &file_name);

  //! \brief Use the parser to parse input from a string.
  std::shared_ptr<ParseNode> ParseString(const string &input);

  //! \brief Pretty print the transition table.
  NO_DISCARD std::string PrintTable() const;

  static std::string PrintAsMathematica(const std::shared_ptr<ParseNode> &head);

  //! \brief Get the parse trace string.
  NO_DISCARD const std::string &GetParseTrace() const;

  friend class ParserGenerator;

 private:
  using Node = std::shared_ptr<ParseNode>;

  // ================================================
  //  Action functions
  // ================================================

  //! \brief Parse whatever the lexer is pointing at.
  Node parse();

  static void instructionNode(Node &self, const std::string &name);
  static void instructionAdd(Node &self, Node &node);
  static void instructionAdopt(Node &self, Node &node);
  static void instructionReplace(Node &self, Node &node);
  static void instructionPush(Node &self, const std::string& name, Node &node);

  std::string entryToString(const Entry &entry);

  //! \brief Convert an ID to a string. The ID may either be a lexeme, or terminal.
  NO_DISCARD std::string toString(int id) const;

  void printFatalParseError(int state);

  //! \brief Construct a parser from its constituents.
  LALRParser(std::map<int, string> inverse_production_map_,
             int start_production_,
             int total_symbols_,
             std::vector<std::vector<Entry>> parse_table_,
             std::vector<State> all_states_,
             std::shared_ptr<LexerDFA> lexer_)
      : inverse_production_map(std::move(inverse_production_map_)),
        start_production(start_production_),
        total_symbols(total_symbols_),
        parse_table(std::move(parse_table_)),
        all_states(std::move(all_states_)),
        lexer(std::move(lexer_)) {};

  // ================================================
  //  Private member variables.
  // ================================================

  //! \brief A lexer.
  std::shared_ptr<LexerDFA> lexer;

  //! \brief Maps production numbers to production names.
  std::map<int, std::string> inverse_production_map;

  //! \brief Which production start points to.
  int start_production = 0;

  //! \brief The total number of lexer ids plus production symbols. The number of columns in the parse_table.
  int total_symbols = 0;

  //! \brief The parse table. It is a vector so we can add new states.
  //!
  //! The pair is [ action, state ].
  //! 0 - Error.
  //! 1 - Shift.
  //! 2 - Reduce.
  //! 3 - Accept.
  std::vector<std::vector<Entry>> parse_table;

  //! \brief All the different states.
  //!
  //! Used for pretty - printing the transition table.
  std::vector<State> all_states;

  //! \brief A string that records the history of the parse.
  std::string parse_trace;
};

}
