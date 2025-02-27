#pragma once

// This website can be used to check things like first sets, follow sets, etc., and to
// generate various parsing tables, LR, LALR, etc.
// http://smlweb.cpsc.ucalgary.ca/start.html
// https://web.cs.dal.ca/~sjackson/lalr1.html

#include <utility>

#include <Lightning/Lightning.h>

#include "manta/parser/ParseNode.h"
#include "manta/lexer/LexerDFA.h"
#include "manta/parser/ParserData.h"
#include "manta/utility/ParserUtility.h"

namespace manta {

//! \brief A combined lexer and parser. This can be a LR0, SLR, or LALR parser, despite
//! the name.
//!
class LALRParser {
  friend class CodeGenerator;

public:
  //! \brief Use the parser to parse input from a file.
  std::shared_ptr<ParseNode> ParseCodeFile(const std::string& file_name);

  //! \brief Use the parser to parse input from a string.
  std::shared_ptr<ParseNode> ParseString(const std::string& input);

  //! \brief Pretty print the transition table.
  NO_DISCARD std::string PrintTable() const;

  static std::string PrintAsMathematica(const std::shared_ptr<ParseNode>& head);

  //! \brief Get the number of steps that the last parse took.
  NO_DISCARD std::size_t NumParseSteps() const;

  //! \brief Get the lexer from the LALR parser.
  //!
  //! Mostly useful for testing.
  //!
  NO_DISCARD std::shared_ptr<LexerDFA> GetLexer() const;

  //! \brief Set the logger that is used to monitor parsing.
  void SetLogger(const lightning::Logger& logger);

  friend class ParserGenerator;

  friend void CompareParsers(const LALRParser& left, const LALRParser& right);

private:
  using Node = std::shared_ptr<ParseNode>;

  // ================================================
  //  Action functions
  // ================================================

  //! \brief Parse whatever the lexer is pointing at.
  Node parse();

  static void instructionNode(Node& self, const std::string& name);
  static void instructionAdd(Node& self, Node& node);
  static void instructionAdopt(Node& self, Node& node);
  static void instructionReplace(Node& self, Node& node);
  static void instructionPush(Node& self, const std::string& name, Node& node);

  std::string entryToString(const Entry& entry);

  //! \brief Convert an ID to a string. The ID may either be a lexeme, or terminal.
  NO_DISCARD std::string idToString(int id) const;

  //! \brief Check whether an ID corresponds to a Lexeme ID (otherwise, it is a
  //! non-terminal ID).
  NO_DISCARD bool isLexeme(int id) const;

  void printFatalParseError(int state);

  //! \brief Construct a parser from its constituents.
  LALRParser(std::map<int, std::string> inverse_production_map,
             int start_production,
             int total_symbols,
             std::vector<std::vector<Entry>> parse_table,
             std::vector<State> all_states,
             std::shared_ptr<LexerDFA> lexer)
      : lexer_(std::move(lexer))
      , inverse_production_map_(std::move(inverse_production_map))
      , start_production_(start_production)
      , total_symbols_(total_symbols)
      , parse_table_(std::move(parse_table))
      , all_states_(std::move(all_states)) {};

  LALRParser(const std::shared_ptr<ParserData>& parser_data)
      : LALRParser(parser_data->production_rules_data->inverse_nonterminal_map,
                   parser_data->production_rules_data->start_nonterminal,
                   parser_data->production_rules_data->total_symbols,
                   parser_data->parse_table,
                   parser_data->all_states,
                   parser_data->GetLexerGenerator().CreateLexer()) {}

  // ================================================
  //  Private member variables.
  // ================================================

  lightning::Logger logger_;

  //! \brief A lexer.
  std::shared_ptr<LexerDFA> lexer_;

  //! \brief Maps production numbers to production names.
  std::map<int, std::string> inverse_production_map_;

  //! \brief Which production start points to.
  int start_production_ = 0;

  //! \brief The total number of lexer ids plus production symbols. The number of columns
  //!        in the parse_table_.
  std::size_t total_symbols_ = 0;

  //! \brief The parse table. It is a vector so we can add new states.
  //!
  //! The pair is [ action, state ].
  //! 0 - Error.
  //! 1 - Shift.
  //! 2 - Reduce.
  //! 3 - Accept.
  std::vector<std::vector<Entry>> parse_table_;

  //! \brief All the different states.
  //!
  //! Used for pretty - printing the transition table.
  std::vector<State> all_states_;

  //! \brief The number of steps that occurred during parsing.
  std::size_t num_parse_steps_ = 0;
};

//! \brief Compare two parsers, printing the differences between them.
void CompareParsers(const LALRParser& left, const LALRParser& right);

}  // namespace manta
