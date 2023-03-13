//
// Created by Nathaniel Rupprecht on 3/8/23.
//

#pragma once

#include "manta/generator/LexerGenerator.h"
#include "manta/utility/ParserUtility.hpp"

namespace manta {

//! \brief Object that contains the definition of a lexer and the productions that make up a grammar.
//!
struct ProductionRulesData {
  //! \brief A lexer generator.
  LexerGenerator lexer_generator;

  //! \brief Maps non-terminal names to non-terminal numbers.
  std::map<std::string, int> nonterminal_map;

  //! \brief Maps non-terminal numbers to non-terminal names.
  std::map<int, std::string> inverse_nonterminal_map;

  //! \brief The productions for each non-terminal. A State (here) is essentially a set of production rules.
  std::map<int, State> productions_for;

  //! \brief All the productions, for all non-terminals.
  std::vector<Item> all_productions;

  //! \brief Whether a non-terminal can derive empty.
  std::vector<bool> nonterminal_derives_empty;

  //! \brief The number of terminals in the correspondence vector.
  int num_productions = 0;

  //! \brief Which non-terminal is the starting non-terminal.
  int start_nonterminal = 0;

  //! \brief The name of the start non-terminal. By default, this is "start."
  std::string start_nonterminal_name = "start";

  //! \brief The total number of lexer ids (terminals) plus non-terminal symbols. This is the number
  //! of columns in the parse_table_.
  int total_symbols = 0;

  // ============================================================================
  //  Helper functions.
  // ============================================================================

  NO_DISCARD const std::string& GetName(int id) const {
    if (id < NumTerminals()) {
      return lexer_generator.LexemeName(id);
    }
    return GetNonterminalName(id);
  }

  NO_DISCARD const std::string& GetNonterminalName(int id) const {
    return inverse_nonterminal_map.at(id);
  }

  NO_DISCARD int NumNonTerminals() const {
    return total_symbols - static_cast<int>(lexer_generator.GetNumLexemes());
  }

  NO_DISCARD int NumTerminals() const {
    return static_cast<int>(lexer_generator.GetNumLexemes());
  }

  NO_DISCARD bool IsNonTerminal(int id) const {
    return lexer_generator.GetNumLexemes() <= id;
  }

  NO_DISCARD bool IsTerminal(int id) const {
    return !IsNonTerminal(id);
  }
};


//! \brief Class that can parse the description of a lexer and a parser from a stream.
//!
//! Note that this is, itself, a specific type of parser.
//!
class DescriptionParser {
 public:
  //! \brief Parse the description of a parser from a stream, creating the ProductionRulesData that can be used to
  //! to make the parser.
  std::shared_ptr<ProductionRulesData> ParseDescription(std::istream &stream);

  //! \brief An exception class that represents encountering an unexpected character type or state while parsing.
  EXCEPTION_MESSAGE_CTOR(UnexpectedInput);

  //! \brief An exception class the signals that a lexeme type (@...) was not recognized by the lexer generator.
  EXCEPTION_MESSAGE_CTOR(UnrecognizedLexerItem);

 private:
  // ============================================================================
  //  Private functions.
  // ============================================================================

  //! \brief Get a production from it's representation in a stream.
  void getProductions(std::istream &in, int production_id);

  //! \brief Find the conflict resolution info for a production.
  static void findResInfo(std::istream &in, ResolutionInfo &res_info);

  //! \brief Get the instruction for a production.
  static inline std::shared_ptr<ParseNode> getInstructions(std::istream &fin, int pid);

  //! \brief Get all alphabetical characters and put them into a word. Returns true if the word was *not* terminated
  //! by the EOF. Does not Clear word at any point.
  static bool getWord(std::istream &in, std::string &word);

  //! Get all numeric characters and put them into a word. Returns true if the word was *not* terminated
  //! by the EOF. Does not Clear word at any point.
  static bool getInteger(std::istream &in, std::string &word);

  //! \brief Get the production number associated with a production name, registering it if it has not
  //! already been registered.
  int registerProduction(const std::string &production);

  //! \brief Shifts the production numbers from being negative to being positive numbers after the last lexer
  //! token number.
  void shiftProductionNumbers();

  // ============================================================================
  //  Private member variables.
  // ============================================================================

  //! \brief The description of the lexer and parser to create.
  std::shared_ptr<ProductionRulesData> production_rules_data_{};

  //! \brief The number to assign to the next production.
  //!
  //! Note: To keep things easy to compare, right now we are starting productions at 1.
  int next_production_label_ = 1;

  //! \brief A string that records the history of the parser generation.
  std::stringstream parser_generation_trace_;
};


} // namespace manta
