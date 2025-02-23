//
// Created by Nathaniel Rupprecht on 3/8/23.
//

#include "manta/generator/DescriptionParser.h"
// Other files
#include <Lightning/Lightning.h>

#include "manta/parser/ParseNode.h"
#include "manta/utility/Formatting.h"

namespace manta {

// ====================================================================================
//  ProductionRulesBuilder.
// ====================================================================================

std::optional<std::string> ProductionRulesBuilder::GetProductionName(NonterminalID id) const {
  auto it = production_rules_data_->inverse_nonterminal_map.find(id);
  if (it != production_rules_data_->inverse_nonterminal_map.end()) {
    return it->second;
  }
  return {};
}

int ProductionRulesBuilder::registerNonterminal(const std::string& production) {
  auto it = production_rules_data_->nonterminal_map.find(production);
  if (it == production_rules_data_->nonterminal_map.end()) {
    production_rules_data_->nonterminal_map.emplace(production, production_rules_data_->num_productions);
    production_rules_data_->inverse_nonterminal_map.emplace(production_rules_data_->num_productions,
                                                            production);
    return production_rules_data_->num_productions--;
  }
  return it->second;
}

NonterminalID ProductionRulesBuilder::finishNonterminal() {
  if (!nonterminal_ids_.empty()) {
    const auto id = nonterminal_ids_.top();
    nonterminal_ids_.pop();
    return id;
  }
  return -1;
}

NonterminalID ProductionRulesBuilder::registerNonterminalDefinition(const std::string& production) {
  nonterminal_ids_.push(registerNonterminal(production));
  return nonterminal_ids_.top();
}

void ProductionRulesBuilder::addToCurrentProduction(int id) {
  getCurrentProduction().rule.AddToProduction(id);
}

NonterminalID ProductionRulesBuilder::createHelperNonterminal() {
  // Helper nonterminals can't be referenced explicitly, since they are created by the parser generator, not
  // by the user. Each one is unique. So we never have to check if on "already exists."
  auto name = lightning::formatting::Format("SUPPORT_{}_{}", getCurrentProductionID(), support_nonterminal_ids_.size());
  return registerNonterminalDefinition(name);
}

void ProductionRulesBuilder::registerStartingProduction(int id) {
  production_rules_data_->start_nonterminal = id;
}

void ProductionRulesBuilder::shiftProductionNumbers() {
  // Get the number of terminals.
  int lids = static_cast<int>(production_rules_data_->lexer_generator->GetNumLexemes());

  // Shift the ids in production map.
  for (auto& [name, id] : production_rules_data_->nonterminal_map) {
    id = lids - id;
  }

  // Shift the ids in all productions
  for (auto& annotated_rule : production_rules_data_->all_productions) {
    // Correct production.
    annotated_rule.rule.produced_nonterminal = lids - annotated_rule.rule.produced_nonterminal;
    // Correct productions in the rhs.
    for (auto& i : annotated_rule.rule.rhs) {
      if (i < 0) {
        i = lids - i;
      }
    }
  }

  // Shift the ids in inverse map
  std::map<int, std::string> new_inverse_map;
  for (auto& [id, name] : production_rules_data_->inverse_nonterminal_map) {
    auto new_id = lids - id;
    new_inverse_map.emplace(new_id, name);
    LOG_SEV(Debug) << "Mapping " << id << " -> " << new_id << " <-> " << formatting::CLBB(name) << ".";
  }
  production_rules_data_->inverse_nonterminal_map = new_inverse_map;

  // Shift the start state.
  production_rules_data_->start_nonterminal = lids - production_rules_data_->start_nonterminal;

  // Shift the ids in productions_for_.
  std::map<int, AnnotatedProductionSet> new_productions_for;
  for (auto& p : production_rules_data_->productions_for) {
    AnnotatedProductionSet production_rules;
    for (auto production_rule : p.second) {
      // Correct production.
      production_rule.rule.produced_nonterminal = lids - production_rule.rule.produced_nonterminal;
      // Correct productions in the rhs.
      for (auto& i : production_rule.rule.rhs) {
        if (i < 0) {
          i = lids - i;
        }
      }
      production_rules.insert(production_rule);
    }
    new_productions_for.emplace(lids - p.first, production_rules);
  }
  production_rules_data_->productions_for = new_productions_for;
  // Set total_symbols_.
  production_rules_data_->total_symbols =
      lids + static_cast<int>(production_rules_data_->nonterminal_map.size());
}

int ProductionRulesBuilder::getLexemeID(const std::string& lexeme_name) const {
  return production_rules_data_->lexer_generator->LexemeID(lexeme_name);
}

void ProductionRulesBuilder::makeNextProductionRule() {
  // Push a rule onto the stack.
  auto production_id = getCurrentProductionID();
  productions_stack_.push(AnnotatedProductionRule{ProductionRule(production_id), item_number_++});
  auto& current = productions_stack_.top();
  current.instructions = std::make_shared<ParseNode>("I");
}

void ProductionRulesBuilder::storeCurrentItem() {
  MANTA_REQUIRE(!productions_stack_.empty(), "productions stack is empty, no item to store.");

  auto& current_production = productions_stack_.top();

  auto nonterminal_id = getCurrentProduction().rule.produced_nonterminal;

  // If there are no instructions, just get rid of the instructions node.
  if (current_production.instructions->children.empty()) {
    current_production.instructions = nullptr;
  }

  // Done finding the rule. Store the rule.
  auto prod = production_rules_data_->productions_for.find(nonterminal_id);
  if (prod == production_rules_data_->productions_for.end()) {
    production_rules_data_->productions_for.emplace(nonterminal_id, AnnotatedProductionSet{});
    prod = production_rules_data_->productions_for.find(nonterminal_id);
  }

  // Add production to the productions for production_id
  prod->second.insert(current_production);
  // Add production to all productions.
  production_rules_data_->all_productions.push_back(current_production);

  LOG_SEV(Trace) << "Storing current item (" << nonterminal_id << "), there are now "
                 << production_rules_data_->all_productions.size() << " productions.";

  // Pop production off the productions stack.
  productions_stack_.pop();
}

void ProductionRulesBuilder::createAction(std::string name) {
  auto& instructions = getCurrentInstructions();
  instructions.Add(std::move(name));
}

void ProductionRulesBuilder::addArgumentToAction(std::string argument) {
  auto& instructions = getCurrentInstructions();
  MANTA_ASSERT(!instructions.children.empty(), "cannot add argument to action, no action added");
  instructions.children.back()->Add(std::move(argument));
}


std::shared_ptr<const ParseNode> ProductionRulesBuilder::getCurrentAction() const {
  return getCurrentProduction().instructions->children.back();
}

void ProductionRulesBuilder::addImport(const std::string& import_name) {
  production_rules_data_->file_data.import_names.push_back(import_name);
}

void ProductionRulesBuilder::addGeneralCodeToVisitor(const std::string& visitor_name,
                                                     const std::string& code) {
  auto& visitor_data = production_rules_data_->visitor_data.visitors;
  auto it            = visitor_data.find(visitor_name);
  MANTA_ASSERT(it != visitor_data.end(), "could not find visitor '" << visitor_name << "' to add code to it");
  auto& visitor = it->second;
  // Add the code to the visitor.
  if (!visitor.other_definitions.empty()) {
    visitor.other_definitions += "\n";
  }
  visitor.other_definitions += code;
}

void ProductionRulesBuilder::addParentClassForVisitor(const std::string& visitor_name,
                                                      const std::string& parent) {
  auto& visitor_data = production_rules_data_->visitor_data.visitors;
  auto it            = visitor_data.find(visitor_name);
  MANTA_ASSERT(it != visitor_data.end(), "could not find visitor '" << visitor_name << "' to add code to it");
  auto& visitor = it->second;
  visitor.additional_base_classes.push_back(parent);
}

void ProductionRulesBuilder::findStartProduction() {
  auto it = production_rules_data_->nonterminal_map.find(production_rules_data_->start_nonterminal_name);
  if (it == production_rules_data_->nonterminal_map.end()) {
    MANTA_FAIL("could not find the start production '" << production_rules_data_->start_nonterminal_name
                                                       << "'");
  }
  production_rules_data_->start_nonterminal = it->second;
}

std::optional<unsigned> ProductionRulesBuilder::getCurrentItemNumber() const {
  return getCurrentProduction().production_item_number;
}

ParseNode& ProductionRulesBuilder::getCurrentInstructions() const {
  return *getCurrentProduction().instructions;
}

AnnotatedProductionRule& ProductionRulesBuilder::getCurrentProduction() {
  MANTA_REQUIRE(!productions_stack_.empty(), "no production rules in stack");
  return productions_stack_.top();
}

const AnnotatedProductionRule& ProductionRulesBuilder::getCurrentProduction() const {
  MANTA_REQUIRE(!productions_stack_.empty(), "no production rules in stack");
  return productions_stack_.top();
}

std::string ProductionRulesBuilder::getCurrentProductionName() const {
  auto name = GetProductionName(getCurrentProductionID());
  MANTA_ASSERT(name, "there is no current production, cannot get current production's name");
  return *name;
}

int ProductionRulesBuilder::getCurrentProductionID() const {
  MANTA_REQUIRE(!nonterminal_ids_.empty(), "no nonterminal is being defined");
  return nonterminal_ids_.top();
}

// ====================================================================================
//  HandWrittenDescriptionParser.
// ====================================================================================

std::string HandWrittenDescriptionParser::findNextCommand(std::istream& stream) const {
  char c;
  // Find the .Parser indicator.
  stream.get(c);
  while (!stream.eof()) {
    if (c == '#') {
      // Pass comments
      stream.get(c);
      while (!stream.eof() and c != '\n') {
        stream.get(c);
      }
    }
    else if (c == '.') {
      // Command
      std::string command;
      stream.get(c);
      while (!stream.eof() && isalpha(c)) {
        command.push_back(c);
        stream.get(c);
      }
      return command;
    }
    else {
      stream.get(c);
    }
  }
  return "";
}

std::shared_ptr<ProductionRulesData> HandWrittenDescriptionParser::ParseDescription(std::istream& stream) {
  // Create a new production rules object.
  production_rules_data_ = std::make_shared<ProductionRulesData>();

  char c;
  std::string production_name;

  // Create the lexer. We always add @eof as a lexer item.
  production_rules_data_->lexer_generator->CreateLexer(stream, false);  // Do not Clear old.

  if (auto cmd = findNextCommand(stream); cmd != "Parser") {
    MANTA_THROW(UnexpectedInput, "expected a .Parser command, found a ." << cmd);
  }

  // If we reached the end of the stream without finding a .Parser indicator, that means the stream
  // did not actually define a parser.
  if (stream.eof()) {
    return nullptr;
  }

  // The .Parser indicator has been found. We now start parsing the description of the parser.

  stream.get(c);
  while (!stream.eof()) {
    // Pass whitespaces.
    if (isspace(c))
      ;
    // Start of production
    else if (isalpha(c)) {
      // We must be done with the last non-terminal (if any). Pop it off the stack.
      finishNonterminal();

      // Get the production name.
      production_name.clear();
      do {
        production_name.push_back(c);
        stream.get(c);
      } while (!isspace(c) && !stream.eof());
      LOG_SEV(Debug) << "Found production " << formatting::CLB(production_name) << ".";

      // We should have stopped because we encountered a space, not because of an eof.
      if (stream.eof()) {
        MANTA_THROW(UnexpectedInput, "encountered eof, expected a space");
      }

      // Get the production number associated with the production name, registering it if it has not
      // already been registered.
      registerNonterminalDefinition(production_name);
      LOG_SEV(Trace) << "Production " << formatting::CLB(production_name) << " given temporary id "
                     << getCurrentProductionID() << ".";

      // Find '->'
      stream.get(c);
      while (c != '-' && !stream.eof()) {
        if (!isspace(c)) {
          // We expect there to only be spaces leading up to the ->.

          // Check, for context.
          stream.putback(c);
          std::string following;
          stream >> following;
          // Then throw.
          MANTA_THROW(UnexpectedInput,
                      "expected a space while looking for the '->' for production '"
                          << production_name << "', encountered '" << c << "'. The string this begins is '"
                          << following << "'");
        }
        stream.get(c);
      }
      if (stream.eof()) {
        // We do not expect to hit EOF.
        MANTA_THROW(UnexpectedInput, "encountered eof");
      }
      // Get the '>' part of the '->'
      stream.get(c);
      if (c != '>') {
        MANTA_THROW(UnexpectedInput, "expected a '>' from a \"->\", found a " << c);
      }
      if (stream.eof()) {
        // We do not expect to hit EOF.
        MANTA_THROW(UnexpectedInput, "encountered eof");
      }

      // Get all the production rules. Stream points to character after the '='
      getProductions(stream);
    }
    // OR
    else if (c == '|') {
      getProductions(stream);
    }
    // Start of a comment.
    else if (c == '#') {
      LOG_SEV(Trace) << "Bypassing comment.";
      // Pass comments.
      while (c != '\n' && !stream.eof()) {
        stream.get(c);
      }
    }
    // A "continuation"
    else if (c == '\\') {
      stream.get(c);
      if (c != '\n') {
        MANTA_THROW(UnexpectedInput, "expected a newline after a backslash, found a " << c);
      }
      // The stream.get(c) at the end of the loop will get the next character.
    }
    // Command
    else if (c == '.') {
      stream.get(c);
      std::string command;
      while (!stream.eof() && isalpha(c)) {
        command.push_back(c);
        stream.get(c);
      }
      if (!stream.eof()) {
        stream.putback(c);
      }

      // The command ".End" ends a section.
      if (command == "End") {
        LOG_SEV(Debug) << "Found the .End command.";
        break;
      }
      // Set the start symbol.
      if (command == "Start") {
        LOG_SEV(Debug) << "Found the .Start command.";
        stream.get(c);
        while (!stream.eof() && c == ' ') {
          stream.get(c);
        }
        if (!stream.eof()) {
          stream.putback(c);
        }
        else {
          MANTA_THROW(UnexpectedInput, "reached eof while looking for the name of the start production");
        }

        // Get the name of the start production.
        command.clear();
        stream.get(c);
        while (!stream.eof() && isalpha(c)) {
          command.push_back(c);
          stream.get(c);
        }
        // EOF is fine here.
        if (!stream.eof()) {
          stream.putback(c);
        }

        // Go to the end of the line.
        stream.get(c);
        while (!stream.eof() && c != '\n' && c != '\r') {
          if (c != ' ') {
            MANTA_THROW(UnexpectedInput, "expected only spaces after .Start command");
          }
          stream.get(c);
        }

        production_rules_data_->start_nonterminal_name = command;
      }
    }

    // Get next character.
    stream.get(c);
  }

  // Get any additional information about visitor classes.
  if (auto cmd = findNextCommand(stream); cmd == "Data") {
    LOG_SEV(Debug) << "Extracting data section.";
    getData(stream);
  }

  // Shift productions, so all terminals and nonterminals have positive numbers.
  shiftProductionNumbers();

  findStartProduction();

  return production_rules_data_;
}

inline void HandWrittenDescriptionParser::getProductions(std::istream& in) {
  // Create an "item" to represent the production.
  makeNextProductionRule();

  auto is_terminator = [](char c) { return c == '\n' || c == '\r' || c == ';'; };

  // Setup.
  char c;
  std::string acc;
  // Get first character.
  in.get(c);
  // Loop.
  while (!in.eof() && !is_terminator(c)) {
    // Start of a literal (terminal). Get the whole thing.
    if (c == '\"') {
      in.get(c);
      while (c != '\"' && !in.eof()) {
        acc.push_back(c);
        in.get(c);
      }
      // Found the literal. Register.
      if (!acc.empty()) {
        int id = production_rules_data_->lexer_generator->AddReserved(acc);
        // Add to production
        addToCurrentProduction(id);
      }
      // Clear accumulator.
      acc.clear();
    }
    // Start of a production.
    else if (isalpha(c)) {
      do {
        acc.push_back(c);
        in.get(c);
      } while (!isspace(c) && !in.eof() && c != '"');

      // If we ended on an open quote, it is the start of a reserved lexeme, and we need to put this back.
      if (c == '"') {
        in.putback(c);
      }

      LOG_SEV(Debug) << "Found production: " << formatting::CLB(acc);

      // Found the production. Get its production number.
      int id = registerNonterminal(acc);
      // If this is the start state, register the start production id.
      // TODO: Register "start" at the construction of the parser generator so it always
      //  has the first id.
      if (acc == "start") {
        registerStartingProduction(id);
      }
      // Add production to rule.
      addToCurrentProduction(id);
      // Clear accumulator.
      acc.clear();
    }
    // Start of a lexeme indicator.
    else if (c == '@') {
      in.get(c);
      while (!isspace(c) && !in.eof()) {
        acc.push_back(c);
        in.get(c);
      }

      LOG_SEV(Trace) << "Found lexeme: @" << formatting::CLBG(acc);

      // Ask the lexer generator for the lexeme ID of the terminal
      int id = getLexemeID(acc);
      if (id < 0) {
        MANTA_THROW(UnexpectedInput, "visitor_name " << acc << " not a valid lexeme type");
      }
      addToCurrentProduction(id);

      // Clear accumulator.
      acc.clear();
    }
    // Other special symbol, like $null
    else if (c == '$') {
      in.get(c);
      while (!isspace(c) && !in.eof()) {
        acc.push_back(c);
        in.get(c);
      }

      LOG_SEV(Trace) << "Found special: $" << acc;

      if (acc == "null") {
        // We handle the null by not adding anything to the production.
        // That covers the case where there is a (useless) null in the
        // middle of a production, A - > B $null C, the case where
        // there are multiple nulls, A -> $null $null, and the case where
        // a production can derive null, A -> $null.
      }
      else {
        MANTA_FAIL("unrecognized special symbol $" << acc);
      }

      // Clear accumulator.
      acc.clear();
    }
    // Start of a grouping.
    else if (c == '(') {
      getGrouping(in);
    }
    // Start of a comment.
    else if (c == '#') {
      // Pass comments.
      while (c != '\n' && !in.eof()) {
        in.get(c);
      }
      LOG_SEV(Trace) << "Passed a comment.";
    }
    // Start of precedence section
    else if (c == '-') {
      in.get(c);  // Expect a '>'
      if (c != '>') {
        MANTA_THROW(UnexpectedInput, "expected a >, got " << c);
      }
      // Fill in the production's resolution info.
      findResInfo(in);
    }
    // Start of the instructions
    else if (c == ':') {
      // Find and store any instructions / actions.
      getInstructions(in);
      break;
    }
    else if (c == '|') {
      // Start of another production for the non-terminal. Put it back and return.
      in.putback(c);
      break;
    }

    // A "continuation" - a \ followed immediately by a newline.
    else if (c == '\\') {
      in.get(c);
      if (c != '\n') {
        MANTA_THROW(UnexpectedInput, "expected a newline after a backslash, found a " << c);
      }
      in.get(c);
    }

    // Get next character.
    if (!is_terminator(c)) {
      in.get(c);
    }
  }

  storeCurrentItem();
}

NonterminalID HandWrittenDescriptionParser::getGrouping(std::istream& in) {
  // Create an ad-hoc non-terminal for the grouping.

  auto id = createHelperNonterminal();

  // Get the grouping, can end with ')'

  char c;
  while (!in.eof()) {
    // OR
    if (c == '|') {
      getProductions(in);
    }
    // A "continuation" - a \ followed immediately by a newline.
    else if (c == '\\') {
      in.get(c);
      if (c != '\n') {
        MANTA_THROW(UnexpectedInput, "expected a newline after a backslash, found a " << c);
      }
      in.get(c);
    }
    else if (c == ')') {
      break;
    }

    in.get(c);
  }

  auto ending_id = finishNonterminal();
  MANTA_ASSERT(id == ending_id, "something went wrong");
  return id;
}

void HandWrittenDescriptionParser::findResInfo(std::istream& in) {
  LOG_SEV(Trace) << "Finding precedence section for " << formatting::CLB(getCurrentProductionName()) << ".";

  auto& res_info = getCurrentProduction().res_info;

  auto is_terminator = [](char c) { return c == '\n' || c == '\r' || c == ';'; };

  // Note - comments are not allowed in the resolution info section.
  char c;
  in.get(c);
  std::string word;
  while (!in.eof()) {
    if (c == ' ') {
    }  // Pass spaces
    else if (is_terminator(c)) {
      // End of production. No instructions.
      // Put the character back so the calling function will detect the end of the production
      in.putback(c);
      return;
    }
    else if (isalpha(c)) {
      word.clear();
      word.push_back(c);
      bool not_eof = getWord(in, word);
      if (!not_eof) {
        MANTA_THROW(UnexpectedInput, "in findResInfo, unexpectedly reached EOF");
      }
      // Expect a '('
      in.get(c);
      if (c != '(') {
        MANTA_THROW(UnexpectedInput,
                    "in findResInfo, expected '(', found '" << c << "', preceding word '" << word << "'");
      }

      // Get the argument. We expect there to be one argument, since the only functions currently accepted
      // here are prec(.) and assoc(.)
      if (word == "prec") {
        word.clear();
        not_eof             = getInteger(in, word);
        res_info.precedence = manta::stoi(word);
      }
      else if (word == "assoc") {
        word.clear();
        not_eof = getWord(in, word);
        if (word == "Left") {
          res_info.assoc = Associativity::LEFT;
        }
        else if (word == "Right") {
          res_info.assoc = Associativity::RIGHT;
        }
        else if (word == "None") {
          res_info.assoc = Associativity::NONE;
        }
        else {
          MANTA_THROW(UnexpectedInput,
                      "in findResInfo, expected Left, Right, or None as an associativity, not " << word);
        }
      }
      else {
        MANTA_THROW(UnexpectedInput,
                    "in findResInfo function was " << word << ", which is not a valid function");
      }

      // Expect a ')'
      in.get(c);
      if (c != ')') {
        MANTA_THROW(UnexpectedInput, "in findResInfo, expected ')'");
      }
    }
    else if (c == ':') {
      // Start of the instruction section.
      // Put the ':' back so the calling function will detect the start of the instruction section.
      in.putback(c);
      break;
    }

    // Get next character
    in.get(c);
  }

  LOG_SEV(Trace) << "Filled in precedence section.";
}

void HandWrittenDescriptionParser::getInstructions(std::istream& in) {
  LOG_SEV(Debug) << "Getting instructions for production " << formatting::CLG(getCurrentProductionName()) << " ("
                 << getCurrentProductionID() << ").";

  // Setup.
  char c;
  std::string acc;

  // Start an instruction parser node.

  // Get first character.
  in.get(c);
  // Loop.
  while (!in.eof() && c != '\n') {
    // Comment
    if (c == '#') {
      // Pass comments.
      while (!in.eof() && c != '\n') {
        in.get(c);
      }
      LOG_SEV(Trace) << "Passed comment.";
      // TODO: We putback c here at least in part so the \n triggers the end of the loop.
      //  I added a "break" here, but need to make sure we don't actually need the \n before
      //  we can remove the putback.
      if (!in.eof()) {
        in.putback(c);
      }
      break;
    }

    if (c == '|') {  // OR - start of another production
      in.putback(c);
      break;
    }

    if (c == '\\') {  // continuation.
      in.get(c);
      if (c != '\n') {
        MANTA_THROW(UnexpectedInput, "expected a newline after a backslash, found a " << c);
      }
      LOG_SEV(Trace) << "Passing continuation.";
    }
    // Reduction / visitor code.
    else if (c == '%') {
      MANTA_ASSERT(!in.eof(), "unexpected eof while getting reduction code");
      LOG_SEV(Debug) << "Found reduction / visitor code for non-terminal " << getCurrentProductionID();

      // Get what type of code this is for, a visitor, or a function to be called upon the REDUCE.
      acc.clear();
      do {
        in.get(c);
        acc.push_back(c);
      } while (!in.eof() && c != '{');
      MANTA_ASSERT(!in.eof(), "unexpected eof while getting reduction code");
      acc.pop_back();
      auto target_type = acc;
      if (target_type.empty()) {
        LOG_SEV(Debug) << "Got reduction code for the REDUCE function.";
      }
      else {
        LOG_SEV(Debug) << "Reduction code got the visitor type '" << acc << "'.";
      }

      MANTA_ASSERT(!in.eof(), "unexpected eof while getting reduction code");

      // Get everything up until the closing "%}"
      acc.clear();
      do {
        in.get(c);
        acc.push_back(c);
      } while ((!in.eof() && acc.size() < 2) || acc.substr(acc.size() - 2) != "%}");
      if (acc.size() < 2) {
        MANTA_THROW(UnexpectedInput, "unexpected eof while getting reduction code");
      }
      acc = acc.substr(0, acc.size() - 2);

      if (auto item = getCurrentItemNumber()) {
        if (target_type.empty() || target_type == "REDUCE") {
          production_rules_data_->reduction_code[*item] = acc;
        }
        else {
          // TODO: (?) Sanitize visitor type name (target_type).
          production_rules_data_->visitor_data.SetBodyForItem(target_type, *item, acc);
          LOG_SEV(Debug) << "Adding code body to visitor '" << formatting::CLBB(target_type)
                         << "' for item number " << *item << ".";
        }
      }
      else {
        MANTA_THROW(UnexpectedInput, "could not get item number for reduction code");
      }
    }
    else if (isalpha(c)) {
      // Get the whole identifier.
      do {
        acc.push_back(c);
        in.get(c);
      } while (isalpha(c) || c == '_');  // letters or underscores

      LOG_SEV(Trace) << "Got identifier: " << formatting::CLBB(acc);

      // Add a node.
      createAction(acc);

      // Clear accumulator.
      acc.clear();

      // Look for an open parenthesis. The other characters should not occur.
      while (c != '(' && !isalpha(c) && c != '\n') {
        in.get(c);
      }
      if (c != '(') {
        MANTA_THROW(UnexpectedInput,
                    "Error while getting instruction: expected an open parenthesis. Found ["
                        << c << "]. (Trying to find the argument for [" << getCurrentAction()->designator
                        << "]. Instruction so far is " << getCurrentInstructions().printTerminals());
      }

      // Gather all arguments.
      in.get(c);
      while (!in.eof() && c != ')') {
        // Pass spaces.
        if (isspace(c)) {
        }
        // The character '$' starts a node reference - that is, it refers to the nodes of the production
        // the instruction corresponds to.
        else if (c == '$') {
          in.get(c);
          if (!isdigit(c)) {
            MANTA_THROW(UnexpectedInput, "expected a number to come after the '$' (node reference)");
          }
          while (!in.eof() && isdigit(c)) {
            acc.push_back(c);
            in.get(c);
          }

          // Allow for arguments like "$0.arg_list"
          if (!in.eof() && c == '.') {
            acc.push_back(c);
            in.get(c);
            while (!in.eof() && (isalpha(c) || c == '_')) {
              acc.push_back(c);
              in.get(c);
            }
          }

          if (!in.eof()) {
            in.putback(c);
          }

          // Add child.
          addArgumentToAction(acc);
          // Clear accumulator.
          acc.clear();
        }
        // String.
        else if (c == '"') {
          in.get(c);
          while (!in.eof() && c != '"') {
            if (c == '\\') {
              in.get(c);
              switch (c) {
                case 't':
                  acc.push_back('\t');
                  break;
                case 'n':
                  acc.push_back('\n');
                  break;
                case '"':
                case '\\':
                default:
                  acc.push_back(c);
                  break;
              }
            }
            else {
              acc.push_back(c);
            }
            in.get(c);
          }
          // Add child.
          addArgumentToAction(acc);
          // Clear accumulator.
          acc.clear();
        }
        // Argument separators.
        else if (c == ',') { /* separator */
        }                    // Put this on a separate line to silence a warning

        // Get next character.
        in.get(c);
      }
    }

    // Get the next character.
    in.get(c);
  }
  LOG_SEV(Trace) << "Done finding instruction.";
}

void HandWrittenDescriptionParser::getData(std::istream& stream) {
  char c;

  // Find the next word, bypassing spaces and comments (which start with '#').
  std::string word;
  stream.get(c);
  while (!stream.eof()) {
    if (c == '#') {
      // Pass comments.
      while (c != '\n' && !stream.eof()) {
        stream.get(c);
      }
    }
    else if (c == '.') {
      word.clear();
      getWord(stream, word);
      if (word == "End") {
        return;
      }
      MANTA_THROW(UnexpectedInput, "in getData, expected a .End command, found a ." << word);
    }
    // Module import.
    else if (c == '@') {
      // Get the command.
      word.clear();
      getWord(stream, word);
      if (word == "import") {
        bypassWhitespace(stream);
        word.clear();
        // Get all characters until a whitespace occurs.
        stream.get(c);
        do {
          word.push_back(c);
          stream.get(c);
        } while (!isspace(c) && !stream.eof());
        // Add the import.
        addImport(word);
        LOG_SEV(Debug) << "Found command to import the module '" << word << "'.";
      }
      else {
        MANTA_THROW(UnexpectedInput, "in getData, expected a @import command, found a @" << word);
      }
    }
    else if (isalpha(c)) {
      word.clear();
      word.push_back(c);
      if (!getWord(stream, word)) {
        MANTA_THROW(UnexpectedInput, "in getData, unexpectedly reached EOF");
      }
      auto command_type = word;

      // Bypass white spaces.
      bypassWhitespace(stream);

      // Next word should be the visitor name.
      word.clear();
      if (!getWord(stream, word)) {
        MANTA_THROW(UnexpectedInput, "in getData, unexpectedly reached EOF");
      }
      auto visitor_name  = word;
      auto& visitor_data = production_rules_data_->visitor_data.visitors;
      auto it            = visitor_data.find(word);
      MANTA_ASSERT(it != visitor_data.end(), "could not find visitor '" << word << "' to add code to it");

      if (command_type == "code") {
        // Bypass white spaces.
        bypassWhitespace(stream);

        // Expect a '%{'
        stream.get(c);
        if (c != '%' && !stream.eof()) {
          MANTA_THROW(UnexpectedInput, "in getData, expected a %, found " << c);
        }
        stream.get(c);
        if (c != '{' && !stream.eof()) {
          MANTA_THROW(UnexpectedInput, "in getData, expected a {, found " << c);
        }
        // Get all the code up until the '%}'
        std::string code;
        stream.get(c);
        while (!stream.eof() && !(c == '}' && code.back() == '%')) {
          code.push_back(c);
          stream.get(c);
        }
        // Last two characters of code are '%}', remove those.
        code.pop_back();
        code.pop_back();

        // Add the code to the visitor.
        addGeneralCodeToVisitor(visitor_name, code);
      }
      else if (command_type == "inherits") {
        // Next word is the class that the visitor inherits from. This is programmer specified, and does not
        // have to be a type the parser knows about.

        // Bypass white spaces.
        bypassWhitespace(stream);

        word.clear();
        // Get all characters until a whitespace occurs.
        stream.get(c);
        do {
          word.push_back(c);
          stream.get(c);
        } while (!isspace(c) && !stream.eof());
        addParentClassForVisitor(visitor_name, word);
        LOG_SEV(Debug) << "Visitor class '" << visitor_name << "' will inherit from '" << word << "'.";
      }
      else {
        MANTA_FAIL("unrecognized command type " << command_type);
      }
    }

    stream.get(c);
  }
}

bool HandWrittenDescriptionParser::getWord(std::istream& in, std::string& word) {
  char c;
  in.get(c);
  while (!in.eof() && std::isalpha(c)) {
    word.push_back(c);
    in.get(c);
  }
  if (!in.eof()) {
    in.putback(c);
    return true;
  }
  return false;
}

bool HandWrittenDescriptionParser::getInteger(std::istream& in, std::string& word) {
  char c;
  in.get(c);
  while (!in.eof() && isdigit(c)) {
    word.push_back(c);
    in.get(c);
  }
  if (!in.eof()) {
    in.putback(c);
    return true;
  }
  return false;
}

void HandWrittenDescriptionParser::bypassWhitespace(std::istream& stream) {
  char c;

  // Bypass white spaces.
  stream.get(c);
  while (!stream.eof() && isspace(c)) {
    stream.get(c);
  }
  stream.putback(c);
}

}  // namespace manta
