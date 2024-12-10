//
// Created by Nathaniel Rupprecht on 3/8/23.
//

#pragma once

#include <deque>
#include <stack>

#include "manta/generator/LexerGenerator.h"
#include "manta/utility/ParserUtility.hpp"

namespace manta {

struct VisitorData {
  //! \brief Definition for a single visitor.
  struct Visitor {
    //! \brief Map from Item to the code that should go in the visitor for that item.
    std::map<ItemID, std::string> code;

    //! \brief The name of the visitor class.
    std::string name;

    //! \brief Code to inject into the body of the visitor class, could be data definitions or additional
    //!        functions.
    std::string other_definitions;

    //! \brief Additional base classes for the visitor.
    std::vector<std::string> additional_base_classes;
  };

  //! \brief Map from visitor name to a description of the visitor.
  std::map<std::string, Visitor> visitors;

  void SetBodyForItem(const std::string& visitor_name, unsigned item_number, const std::string& body) {
    auto& visitor             = visitors[visitor_name];
    visitor.name              = visitor_name;
    visitor.code[item_number] = body;
  }
};

struct FileData {
  //! \brief Vector of files or modules to include or import.
  std::vector<std::string> import_names;
};

//! \brief Object that contains the definition of a lexer and the productions that make up a grammar.
struct ProductionRulesData {
  ProductionRulesData() : lexer_generator(std::make_shared<LexerGenerator>()) {}

  //! \brief A lexer_generator generator.
  std::shared_ptr<LexerGenerator> lexer_generator {};

  //! \brief Maps non-terminal names to non-terminal numbers.
  std::map<std::string, NonterminalID> nonterminal_map;

  //! \brief Maps non-terminal numbers to non-terminal names.
  std::map<NonterminalID, std::string> inverse_nonterminal_map;

  // TODO: Do we need to do anything to keep track of support non-terminals?

  //! \brief The productions for each non-terminal.
  std::map<NonterminalID, AnnotatedProductionSet> productions_for;

  //! \brief All the productions, for all non-terminals.
  std::vector<AnnotatedProductionRule> all_productions;

  //! \brief Whether a non-terminal can derive empty.
  std::vector<bool> nonterminal_derives_empty;

  //! \brief The number of terminals in the correspondence vector.
  int num_productions = 0;

  //! \brief Which non-terminal is the starting non-terminal.
  NonterminalID start_nonterminal = 0;

  //! \brief The name of the start non-terminal. By default, this is "start."
  std::string start_nonterminal_name = "start";

  //! \brief The total number of lexer ids (terminals) plus non-terminal symbols. This is the number of
  //!        columns in the parse_table_.
  int total_symbols = 0;

  //! \brief Code that should be injected after an item reduce occurs.
  std::map<ItemID, std::string> reduction_code;

  //! \brief Data about any visitors that should be generated.
  VisitorData visitor_data {};

  //! brief Data about any files that should be generated.
  FileData file_data {};

  // ============================================================================
  //  Helper functions.
  // ============================================================================

  NO_DISCARD const std::string& GetName(int id) const {
    if (id < NumTerminals()) {
      return lexer_generator->LexemeName(id);
    }
    return GetNonterminalName(id);
  }

  NO_DISCARD std::string GetPrettyName(int id) const {
    if (id < NumTerminals()) {
      using namespace std::string_literals;
      using namespace std::string_view_literals;
      using namespace lightning::formatting;

      auto& lexeme_name = lexer_generator->LexemeName(id);
      if (std::string_view(lexeme_name).substr(0, 4) == "RES:"sv) {
        // TODO: Update Lightning to get Format() to work with string_view.
        return Format("\"{}\"", std::string(std::string_view(lexeme_name).substr(4)));
      }
      return "@"s + lexeme_name;
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

  std::string WriteItem(const Item& item) const {
    // This may be a null production, just a placeholder for a shift.
    if (item.produced_nonterminal < 0) {
      return "";
    }
    std::string output = GetPrettyName(item.produced_nonterminal) + " -> ";
    int j              = 0;
    for (const auto symbol : item.rhs) {
      if (j == item.bookmark) {
        output += "* ";
      }
      output += GetPrettyName(symbol) + " ";
      ++j;
    }
    if (j == item.bookmark) {
      output += "*";
    }
    return output;
  }

  std::string Write(const ProductionRule& rule) const {
    // This may be a null production, just a placeholder for a shift.
    if (rule.produced_nonterminal < 0) {
      return "";
    }
    std::string output = GetPrettyName(rule.produced_nonterminal) + " -> ";
    int j              = 0;
    for (const auto symbol : rule.rhs) {
      output += GetPrettyName(symbol) + " ";
      ++j;
    }
    return output;
  }
};

//! \brief Base class for objects that can parse a stream and produce a description of a parser described by
//!        the stream.
//!
//! Note that each description parser, as a parser, accepts its own specific language.
class DescriptionParser {
public:
  virtual ~DescriptionParser() = default;

  //! \brief Parse the description of a parser from a stream, creating the ProductionRulesData that can be
  //!        used to to make the parser.
  virtual std::shared_ptr<ProductionRulesData> ParseDescription(std::istream& stream) = 0;
};

//! \brief Class that contains common functionality for building production rules data.
class ProductionRulesBuilder {
public:
  ProductionRulesBuilder() : production_rules_data_(std::make_shared<ProductionRulesData>()) {}

  virtual ~ProductionRulesBuilder() = default;

  //! \brief Get the production rules data that has been built.
  std::shared_ptr<ProductionRulesData> GetProductionRulesData() { return production_rules_data_; }

  //! \brief Reset the production rules data, creating a new one.
  void ResetProductionRulesData() { production_rules_data_ = std::make_shared<ProductionRulesData>(); }

  //! \brief Get the name of a nonterminal by its ID.
  std::optional<std::string> GetProductionName(NonterminalID id) const;

protected:
  //! \brief Get the non-terminal id associated with a non-terminal name, registering it if it has not already
  //!        been registered.
  NonterminalID registerNonterminal(const std::string& production);

  //! \brief Register a non-terminal whose productions are being defined.
  NonterminalID registerNonterminalDefinition(const std::string& production);

  void addToProduction(int id) { getCurrentProduction().rule.AddToProduction(id); }

  //! \brief Create a new helper nonterminal.
  NonterminalID createHelperNonterminal(NonterminalID parent_id);

  //! \brief Register a production as starting production.
  void registerStartingProduction(int id);

  //! \brief Shifts the production numbers from being negative to being positive numbers after the last lexer
  //!        token number.
  void shiftProductionNumbers();

  //! \brief Find the ID for a lexeme, by the lexeme's name.
  int getLexemeID(const std::string& lexeme_name) const;

  //! \brief Generate an item with the next production label, sets the current_item_ to be this new item.
  //!        Also generates a new current_instructions_.
  void makeNextProductionRule();

  //! \brief Store the current, completed item.
  void storeCurrentItem();

  //! \brief Register an action for the current production.
  void createAction(std::string name);

  //! \brief Add an argument to the current action.
  void addArgumentToAction(std::string argument);

  std::shared_ptr<const ParseNode> getCurrentAction() const;

  void addImport(const std::string& import_name);

  void addGeneralCodeToVisitor(const std::string& visitor_name, const std::string& code);

  void addParentClassForVisitor(const std::string& visitor_name, const std::string& parent);

  //! \brief Find start production. This must be done after we shift production numbers.
  void findStartProduction();

  //! \brief Get the number of the current item.
  std::optional<unsigned> getCurrentItemNumber() const;

  //! \brief Get the instructions node for the current item.
  ParseNode& getCurrentInstructions() const;

  AnnotatedProductionRule& getCurrentProduction();
  const AnnotatedProductionRule& getCurrentProduction() const;

  std::string getCurrentProductionName() const;

  int getCurrentProductionID() const;

  //! \brief Keep track of the next production item's ID.
  ItemID item_number_ = 0;

  //! \brief The production id of the currently registered production.
  ItemID current_production_id_ = 0;

  //! \brief Keep track of which non-terminals are support non-terminals.
  //!
  //! Support nonterminals are intended on being used to support things like
  //! turning parser patterns like A -> (B C)* into something like
  //! A -> Support_B, Support_B -> Support_B B C | $null.
  std::set<NonterminalID> support_nonterminal_ids_ {};

  //! \brief The description of the lexer and parser to create.
  std::shared_ptr<ProductionRulesData> production_rules_data_ {};

  //! \brief Items currently being constructed. There will be more than one item in the stack if there are
  //!        special patterns in the grammar, like optional or repeating patterns.
  std::stack<AnnotatedProductionRule> productions_stack_ {};
};

//! \brief Class that can parse the description of a lexer and a parser from a stream.
//!
//! Written "by hand," not produced by Manta itself. You have to have a parser to make a parser generator...
//!
//! Note that this is, itself, a specific type of parser.
class HandWrittenDescriptionParser final
    : public DescriptionParser
    , public ProductionRulesBuilder {
public:
  //! \brief Parse the description of a parser from a stream, creating the ProductionRulesData that can be
  //!        used to to make the parser.
  std::shared_ptr<ProductionRulesData> ParseDescription(std::istream& stream);

  //! \brief An exception class that represents encountering an unexpected character type or state while
  //!        parsing.
  EXCEPTION_MESSAGE_CTOR(UnexpectedInput);

  //! \brief An exception class the signals that a lexeme type (@...) was not recognized by the lexer
  //!        generator.
  EXCEPTION_MESSAGE_CTOR(UnrecognizedLexerItem);

private:
  // ============================================================================
  //  Private functions.
  // ============================================================================

  std::string findNextCommand(std::istream& stream) const;

  //! \brief Get a production from it's representation in a stream.
  void getProductions(std::istream& in);

  //! \brief Find the conflict resolution info for a production.
  void findResInfo(std::istream& in);

  //! \brief Get the instruction for a production.
  void getInstructions(std::istream& in);

  //! \brief Get additional data, including visitor related data and module import data.
  void getData(std::istream& in);

  //! \brief Get all alphabetical characters and put them into a word.
  //!
  //! \return Returns true if the word was *not* terminated by the EOF. Does not Clear word at any point.
  static bool getWord(std::istream& in, std::string& word);

  //! Get all numeric characters and put them into a word.
  //!
  //! \return Returns true if the word was *not* terminated by the EOF. Does not Clear word at any point.
  static bool getInteger(std::istream& in, std::string& word);

  //! \brief Bypass white spaces in a stream.
  static void bypassWhitespace(std::istream& stream);
};

}  // namespace manta
