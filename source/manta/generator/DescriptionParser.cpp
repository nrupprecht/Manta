//
// Created by Nathaniel Rupprecht on 3/8/23.
//

#include "manta/generator/DescriptionParser.h"
// Other files
#include <Lightning/Lightning.h>

#include "manta/parser/ParseNode.h"

namespace manta {

// ====================================================================================
//  ProductionRulesBuilder.
// ====================================================================================

int ProductionRulesBuilder::registerProduction(const std::string& production) {
  auto it = production_rules_data_->nonterminal_map.find(production);
  if (it == production_rules_data_->nonterminal_map.end()) {
    production_rules_data_->nonterminal_map.emplace(production, production_rules_data_->num_productions);
    production_rules_data_->inverse_nonterminal_map.emplace(production_rules_data_->num_productions,
                                                            production);
    return production_rules_data_->num_productions--;
  }
  return it->second;
}

void ProductionRulesBuilder::registerStartingProduction(int id) {
  production_rules_data_->start_nonterminal = id;
}

void ProductionRulesBuilder::shiftProductionNumbers() {
  // Get the number of terminals.
  int lids = static_cast<int>(production_rules_data_->lexer_generator->GetNumLexemes());

  // Shift the ids in production map.
  for (auto& p : production_rules_data_->nonterminal_map) {
    p.second = lids - p.second;
  }

  // Shift the ids in all productions
  for (auto& item : production_rules_data_->all_productions) {
    // Correct production.
    item.production = lids - item.production;
    // Correct productions in the rhs.
    for (auto& i : item.rhs) {
      if (i < 0) {
        i = lids - i;
      }
    }
  }

  // Shift the ids in inverse map
  std::map<int, std::string> new_inverse_map;
  for (auto& p : production_rules_data_->inverse_nonterminal_map) {
    new_inverse_map.emplace(lids - p.first, p.second);
  }
  production_rules_data_->inverse_nonterminal_map = new_inverse_map;

  // Shift the start state.
  production_rules_data_->start_nonterminal = lids - production_rules_data_->start_nonterminal;

  // Shift the ids in productions_for_.
  std::map<int, State> new_productions_for;
  for (auto& p : production_rules_data_->productions_for) {
    State state;
    for (auto item : p.second) {
      // Correct production.
      item.production = lids - item.production;
      // Correct productions in the rhs.
      for (auto& i : item.rhs) {
        if (i < 0) {
          i = lids - i;
        }
      }
      state.insert(item);
    }
    new_productions_for.insert(std::pair<int, State>(lids - p.first, state));
  }
  production_rules_data_->productions_for = new_productions_for;
  // Set total_symbols_.
  production_rules_data_->total_symbols =
      lids + static_cast<int>(production_rules_data_->nonterminal_map.size());
}

int ProductionRulesBuilder::getLexemeID(const std::string& lexeme_name) const {
  return production_rules_data_->lexer_generator->LexemeID(lexeme_name);
}

Item& ProductionRulesBuilder::makeNextItem(int production_id) {
  current_item_ = {production_id, next_production_label_++};
  current_item_.item_number = item_number_++;
  return current_item_;
}

void ProductionRulesBuilder::storeCurrentItem() {
  auto production_id = current_item_.production;

  // Done finding the rule. Store the rule.
  auto prod = production_rules_data_->productions_for.find(production_id);
  if (prod == production_rules_data_->productions_for.end()) {
    production_rules_data_->productions_for.emplace(production_id, State());
    prod = production_rules_data_->productions_for.find(production_id);
  }

  // Add production to the productions for production_id
  prod->second.insert(current_item_);
  // Add production to all productions.
  production_rules_data_->all_productions.push_back(current_item_);
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
  return current_item_.item_number;
}

// ====================================================================================
//  HandWrittenDescriptionParser.
// ====================================================================================

std::string HandWrittenDescriptionParser::findNextCommand(std::istream& stream) const {
  char c;
  // Find the .Parser indicator.
  stream.get(c);
  while (!stream.eof()) {
    if (c == '#') {  // Pass comments
      stream.get(c);
      while (!stream.eof() and c != '\n') {
        stream.get(c);
      }
    }
    else if (c == '.') {  // Command
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
  int pid;

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
    if (isspace(c)) {
      // Start of production
    }
    else if (isalpha(c)) {
      production_name.clear();
      do {
        production_name.push_back(c);
        stream.get(c);
      } while (!isspace(c) && !stream.eof());

      // We should have stopped because we encountered a space, not because of an eof.
      if (stream.eof()) {
        MANTA_THROW(UnexpectedInput, "encountered eof, expected a space");
      }

      // Get the production number associated with the production name, registering it if it has not
      // already been registered.
      pid = registerProduction(production_name);

      // Find '->'
      stream.get(c);
      while (c != '-' && !stream.eof()) {
        if (!isspace(c)) {  // We expect there to only be spaces leading up to the equals sign.
          MANTA_THROW(UnexpectedInput, "expected a space, encountered '" << c << "'");
        }
        stream.get(c);
      }
      if (stream.eof()) {  // We do not expect to hit EOF.
        MANTA_THROW(UnexpectedInput, "encountered eof");
      }
      // Get the '>' part of the '->'
      stream.get(c);
      if (c != '>') {
        MANTA_THROW(UnexpectedInput, "expected a '>' from a \"->\", found a " << c);
      }
      if (stream.eof()) {  // We do not expect to hit EOF.
        MANTA_THROW(UnexpectedInput, "encountered eof");
      }

      // Get all the production rules. Stream points to character after the '='
      getProductions(stream, pid);
    }
    // OR
    else if (c == '|') {
      getProductions(stream, pid);
    }
    // Start of a comment.
    else if (c == '#') {
      // Pass comments.
      while (c != '\n' && !stream.eof()) {
        stream.get(c);
      }
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
        break;
      }
      // Set the start symbol.
      else if (command == "Start") {
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
    getData(stream);
  }

  // Shift productions, so all terminals and nonterminals have positive numbers.
  shiftProductionNumbers();

  findStartProduction();

  return production_rules_data_;
}

inline void HandWrittenDescriptionParser::getProductions(std::istream& in, int production_id) {
  // Create an "item" to represent the production.
  auto& production = makeNextItem(production_id);

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
        production.add(id);
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

      // Found the production. Get its production number.
      int id = registerProduction(acc);
      // If this is the start state, register the start production id.
      // TODO: Register "start" at the construction of the parser generator so it always
      //  has the first id.
      if (acc == "start") {
        registerStartingProduction(id);
      }
      // Add production to rule.
      production.add(id);
      // Clear accumulator.
      acc.clear();
    }

    // Start of a default lexer type (terminal), or the @null symbol.
    // TODO: Redo this part.
    else if (c == '@') {
      in.get(c);
      while (!isspace(c) && !in.eof()) {
        acc.push_back(c);
        in.get(c);
      }

      // Ask the lexer generator for the lexeme ID of the terminal
      int id = getLexemeID(acc);
      if (id < 0) {
        MANTA_THROW(UnexpectedInput, "visitor_name " << acc << " not a valid lexeme type");
      }
      production.add(id);

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
    // Start of a comment.
    else if (c == '#') {
      // Pass comments.
      while (c != '\n' && !in.eof()) {
        in.get(c);
      }
    }
    // Start of precedence section
    else if (c == '-') {
      in.get(c);  // Expect a '>'
      if (c != '>') {
        MANTA_THROW(UnexpectedInput, "expected a >, got " << c);
      }
      // Fill in the production's resolution info.
      findResInfo(in, production.res_info);
    }
    // Start of the instructions
    else if (c == ':') {
      // Store the rule.
      production.instructions = getInstructions(in, production_id);
      break;
    }
    else if (c == '|') {  // Start of another production for the non-terminal. Put it back and return.
      in.putback(c);
      break;
    }

    // Get next character.
    if (!is_terminator(c)) {
      in.get(c);
    }
  }

  storeCurrentItem();
}

void HandWrittenDescriptionParser::findResInfo(std::istream& in, ResolutionInfo& res_info) {
  auto is_terminator = [](char c) { return c == '\n' || c == '\r' || c == ';'; };

  // Note - comments are not allowed in the resolution info section.
  char c;
  in.get(c);
  std::string word;
  while (!in.eof()) {
    if (c == ' ') {
    }  // Pass spaces
    else if (is_terminator(c)) {  // End of production. No instructions.
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
        MANTA_THROW(UnexpectedInput, "in findResInfo, expected (, found " << c);
      }

      // Get the argument. We expect there to be one argument, since the only functions currently accepted
      // here are prec(.) and assoc(.)
      if (word == "prec") {
        word.clear();
        not_eof = getInteger(in, word);
        res_info.precedence = std::stoi(word);
      }
      else if (word == "assoc") {
        word.clear();
        not_eof = getWord(in, word);
        if (word == "Left") {
          res_info.assoc = Associativity::Left;
        }
        else if (word == "Right") {
          res_info.assoc = Associativity::Right;
        }
        else if (word == "None") {
          res_info.assoc = Associativity::None;
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
    else if (c == ':') {  // Start of the instruction section.
      // Put the ':' back so the calling function will detect the start of the instruction section.
      in.putback(c);
      return;
    }

    // Get next character
    in.get(c);
  }
}

std::shared_ptr<ParseNode> HandWrittenDescriptionParser::getInstructions(std::istream& in, int pid) {
  // Setup.
  char c;
  std::string acc;

  // Start an instruction parser node.
  auto instruction = std::make_shared<ParseNode>("I");

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
      // TODO: We putback c here at least in part so the \n triggers the end of the loop.
      //  I added a "break" here, but need to make sure we don't actually need the \n before
      //  we can remove the putback.
      if (!in.eof()) {
        in.putback(c);
      }
      break;
    }
    // Reduction code.
    else if (c == '%') {
      MANTA_ASSERT(!in.eof(), "unexpected eof while getting reduction code");

      // Get what type of code this is for, a visitor, or a function to be called upon the REDUCE.
      acc.clear();
      do {
        in.get(c);
        acc.push_back(c);
      } while (!in.eof() && c != '{');
      MANTA_ASSERT(!in.eof(), "unexpected eof while getting reduction code");
      in.putback('{');
      acc.pop_back();
      auto target_type = acc;
      if (target_type.empty()) {
        LOG_SEV(Debug) << "Got reduction code for the REDUCE function.";
      }
      else {
        LOG_SEV(Debug) << "Reduction code got the target type '" << acc << "'.";
      }

      MANTA_ASSERT(!in.eof(), "unexpected eof while getting reduction code");

      in.get(c);
      MANTA_ASSERT(!in.eof(), "unexpected eof while getting reduction code");
      if (c != '{') {
        MANTA_THROW(UnexpectedInput, "expected '{' after '%', found '" << c << "'");
      }

      // Get everything up until the closing "%}"
      acc.clear();
      do {
        in.get(c);
        acc.push_back(c);
      } while (!in.eof() && acc.size() < 2 || acc.substr(acc.size() - 2) != "%}");
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

      // Add a node.
      auto node = std::make_shared<ParseNode>(acc);
      instruction->Add(node);
      // Clear accumulator.
      acc.clear();

      // Look for an open parenthesis. The other characters should not occur.
      while (c != '(' && !isalpha(c) && c != '\n') {
        in.get(c);
      }
      if (c != '(') {
        MANTA_THROW(UnexpectedInput,
                    "Error while getting instruction: expected an open parenthesis. Found ["
                        << c << "]. (Trying to find the argument for [" << node->designator
                        << "]. Instruction so far is " << instruction->printTerminals());
      }

      // Gather all arguments.
      in.get(c);
      while (!in.eof() && c != ')') {
        // Pass spaces.
        if (isspace(c))
          ;
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
          node->Add(acc);
          // Clear accumulator.
          acc.clear();
        }
        // String.
        else if (c == '"') {
          in.get(c);
          // TODO: Handle escaped quotes.
          while (!in.eof() && c != '"') {
            acc.push_back(c);
            in.get(c);
          }
          // Add child.
          node->Add(acc);
          // Clear accumulator.
          acc.clear();
        }
        // Argument separators.
        else if (c == ',') {
        };  // Put this on a separate line to silence a warning

        // Get next character.
        in.get(c);
      }
    }

    // Get the next character.
    in.get(c);
  }
  // Return the instruction.
  return instruction;
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
      else {
        MANTA_THROW(UnexpectedInput, "in getData, expected a .End command, found a ." << word);
      }
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
        production_rules_data_->file_data.import_names.push_back(word);
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
      auto visitor_name = word;
      auto& visitor_data = production_rules_data_->visitor_data.visitors;
      auto it = visitor_data.find(word);
      MANTA_ASSERT(it != visitor_data.end(), "could not find visitor '" << word << "' to add code to it");
      auto& visitor = it->second;

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
        visitor.other_definitions = code;
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
        visitor.additional_base_classes.push_back(word);
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