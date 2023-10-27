//
// Created by Nathaniel Rupprecht on 3/8/23.
//

#pragma once

#include "manta/generator/LexerGenerator.h"
#include "manta/utility/ParserUtility.hpp"

namespace manta {

struct VisitorData {
  //! \brief Definition for a single visitor.
  struct Visitor {
    //! \brief Map from Item to the code that should go in the visitor for that item.
    std::map<unsigned, std::string> code;

    //! \brief The name of the visitor class.
    std::string name;

    //! \brief Code to inject into the body of the visitor class, could be data definitions or additional
    //! functions.
    std::string other_definitions;
  };

  //! \brief Map from visitor name to a description of the visitor.
  std::map<std::string, Visitor> visitors;

  void SetBodyForItem(const std::string& visitor_name, unsigned item_number, const std::string& body) {
    auto& visitor = visitors[visitor_name];
    visitor.name = visitor_name;
    visitor.code[item_number] = body;
  }
};

struct FileData {
  //! \brief Vector of files or modules to include or import.
  std::vector<std::string> import_names;
};

//! \brief Object that contains the definition of a lexer and the productions that make up
//! a grammar.
//!
struct ProductionRulesData {
  ProductionRulesData()
      : lexer_generator(std::make_shared<LexerGenerator>()) {}

  //! \brief A lexer_generator generator.
  std::shared_ptr<LexerGenerator> lexer_generator {};

  //! \brief Maps non-terminal names to non-terminal numbers.
  std::map<std::string, int> nonterminal_map;

  //! \brief Maps non-terminal numbers to non-terminal names.
  std::map<int, std::string> inverse_nonterminal_map;

  //! \brief The productions for each non-terminal. A State (here) is essentially a set of
  //! production rules.
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

  //! \brief The total number of lexer ids (terminals) plus non-terminal symbols. This is
  //! the number of columns in the parse_table_.
  int total_symbols = 0;

  //! \brief Data about any visitors that should be generated.
  VisitorData visitor_data {};

  //! brief Data about any files that should be generated.
  FileData file_data{};

  // ============================================================================
  //  Helper functions.
  // ============================================================================

  NO_DISCARD const std::string& GetName(int id) const {
    if (id < NumTerminals()) {
      return lexer_generator->LexemeName(id);
    }
    return GetNonterminalName(id);
  }

  NO_DISCARD const std::string& GetNonterminalName(int id) const { return inverse_nonterminal_map.at(id); }

  NO_DISCARD int NumNonTerminals() const {
    return total_symbols - static_cast<int>(lexer_generator->GetNumLexemes());
  }

  NO_DISCARD int NumTerminals() const { return static_cast<int>(lexer_generator->GetNumLexemes()); }

  NO_DISCARD bool IsNonTerminal(int id) const { return lexer_generator->GetNumLexemes() <= id; }

  NO_DISCARD bool IsTerminal(int id) const { return !IsNonTerminal(id); }
};

//! \brief Base class for objects that can parse a stream and produce a description of a parser described by
//! the stream.
//!
//! Note that each description parser, as a parser, accepts its own specific language.
//!
class DescriptionParser {
public:
  //! \brief Parse the description of a parser from a stream, creating the
  //! ProductionRulesData that can be used to to make the parser.
  virtual std::shared_ptr<ProductionRulesData> ParseDescription(std::istream& stream) = 0;
};

//! \brief Class that contains common functionality for building production rules data.
class ProductionRulesBuilder {
public:
  ProductionRulesBuilder()
      : production_rules_data_(std::make_shared<ProductionRulesData>()) {}

  //! \brief Get the production rules data that has been built.
  std::shared_ptr<ProductionRulesData> GetProductionRulesData() { return production_rules_data_; }

protected:
  //! \brief Get the production number associated with a production name, registering it
  //! if it has not already been registered.
  int registerProduction(const std::string& production);

  //! \brief Register a production as starting production.
  void registerStartingProduction(int id);

  //! \brief Shifts the production numbers from being negative to being positive numbers
  //! after the last lexer token number.
  void shiftProductionNumbers();

  //! \brief Find the ID for a lexeme, by the lexeme's name.
  int getLexemeID(const std::string& lexeme_name) const;

  //! \brief Generate an item with the next production label, sets the current_item_ to be this new item.
  Item& makeNextItem(int production_id);

  //! \brief Store the current, completed item.
  void storeCurrentItem();

  //! \brief Find start production. This must be done after we shift production numbers.
  void findStartProduction();

  //! \brief Get the number of the current item.
  std::optional<unsigned> getCurrentItemNumber() const;

  //! \brief The current item being built.
  Item current_item_ {};

  //! \brief Keep track of the next production item.
  unsigned item_number_ = 0;

  //! \brief The description of the lexer and parser to create.
  std::shared_ptr<ProductionRulesData> production_rules_data_ {};

  //! \brief The number to assign to the next production.
  //!
  //! Note: To keep things easy to compare, right now we are starting productions at 1.
  int next_production_label_ = 1;
};

//! \brief Class that can parse the description of a lexer and a parser from a stream.
//! Written "by hand," not produced by Manta itself. You have to have a parser to make a parser generator...
//!
//! Note that this is, itself, a specific type of parser.
//!
class HandWrittenDescriptionParser
    : public DescriptionParser
    , public ProductionRulesBuilder {
public:
  //! \brief Parse the description of a parser from a stream, creating the
  //! ProductionRulesData that can be used to to make the parser.
  std::shared_ptr<ProductionRulesData> ParseDescription(std::istream& stream);

  //! \brief An exception class that represents encountering an unexpected character type
  //! or state while parsing.
  EXCEPTION_MESSAGE_CTOR(UnexpectedInput);

  //! \brief An exception class the signals that a lexeme type (@...) was not recognized
  //! by the lexer generator.
  EXCEPTION_MESSAGE_CTOR(UnrecognizedLexerItem);

private:
  // ============================================================================
  //  Private functions.
  // ============================================================================

  //! \brief Get a production from it's representation in a stream.
  void getProductions(std::istream& in, int production_id);

  //! \brief Find the conflict resolution info for a production.
  void findResInfo(std::istream& in, ResolutionInfo& res_info);

  //! \brief Get the instruction for a production.
  std::shared_ptr<ParseNode> getInstructions(std::istream& fin, int pid);

  //! \brief Get all alphabetical characters and put them into a word. Returns true if the
  //! word was *not* terminated by the EOF. Does not Clear word at any point.
  static bool getWord(std::istream& in, std::string& word);

  //! Get all numeric characters and put them into a word. Returns true if the word was
  //! *not* terminated by the EOF. Does not Clear word at any point.
  static bool getInteger(std::istream& in, std::string& word);
};

}  // namespace manta
