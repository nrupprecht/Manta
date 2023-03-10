//
// Created by Nathaniel Rupprecht on 3/8/23.
//

#include "manta/generator/DescriptionParser.h"
// Other files
#include "manta/parser/ParseNode.h"

using namespace manta;


std::shared_ptr<ProductionRulesData> DescriptionParser::ParseDescription(std::istream &stream) {
  // Create a new production rules object.
  production_rules_data_ = std::make_shared<ProductionRulesData>();

  char c;
  std::string production_name;
  int pid;

  // Create the lexer. We always add @eof as a lexer item.
  production_rules_data_->lexer_generator.CreateLexer(stream, false); // Do not Clear old.

  // Find the .Parser indicator.
  stream.get(c);
  while (!stream.eof()) {
    if (c == '#') { // Pass comments
      stream.get(c);
      while (!stream.eof() and c != '\n') {
        stream.get(c);
      }
    }
    else if (c == '.') { // Command
      std::string command;
      stream.get(c);
      while (!stream.eof() && isalpha(c)) {
        command.push_back(c);
        stream.get(c);
      }
      if (command == "Parser") {
        stream.putback(c); // Just in case c is '.'
        break; // Start lexing.
      }
    }
    else {
      stream.get(c);
    }
  }
  // If we reached the end of the stream without finding a .Parser indicator, that means the stream
  // did not actually define a parser.
  if (stream.eof()) {
    parser_generation_trace_ << "Could not find the .Parser indicator.\n";
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
        throw UnexpectedInput("encountered eof, expected a space");
      }

      // Get the production number associated with the production name, registering it if it has not
      // already been registered.
      pid = registerProduction(production_name);

      // Find '->'
      stream.get(c);
      while (c != '-' && !stream.eof()) {
        if (!isspace(c)) { // We expect there to only be spaces leading up to the equals sign.
          throw UnexpectedInput("expected a space, encountered '" + std::string{c} + "'");
        }
        stream.get(c);
      }
      if (stream.eof()) { // We do not expect to hit EOF.
        throw UnexpectedInput("encountered eof");
      }
      // Get the '>' part of the '->'
      stream.get(c);
      if (c != '>') {
        throw UnexpectedInput("expected a '>' from a \"->\", found a " + std::string{c});
      }
      if (stream.eof()) { // We do not expect to hit EOF.
        throw UnexpectedInput("encountered eof");
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
          throw UnexpectedInput("reached eof while looking for the name of the start production");
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
            throw UnexpectedInput("expected only spaces after .Start command");
          }
          stream.get(c);
        }

        production_rules_data_->start_nonterminal_name = command;
      }
    }

    // Get next character.
    stream.get(c);
  }

  // Shift productions, so all terminals and nonterminals have positive numbers.
  shiftProductionNumbers();

  // Find start production (must do this after we shift production numbers).
  auto it = production_rules_data_->nonterminal_map.find(production_rules_data_->start_nonterminal_name);
  if (it == production_rules_data_->nonterminal_map.end()) {
    throw std::exception();
  }
  production_rules_data_->start_nonterminal = it->second;

  return production_rules_data_;
}


inline void DescriptionParser::getProductions(std::istream &in, int production_id) {
  // Create an "item" to represent the production.
  Item production(production_id, next_production_label_++);

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
        int id = production_rules_data_->lexer_generator.AddReserved(acc);
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

      // Found the production. Get it's production number.
      int id = registerProduction(acc);
      // If this is the start state, register the start production id.
      // TODO: Register "start" at the construction of the parser generator so it always
      //  has the first id.
      if (acc == "start") {
        production_rules_data_->start_nonterminal = id;
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
      int id = production_rules_data_->lexer_generator.LexemeID(acc);
      if (id < 0) {
        throw UnrecognizedLexerItem("word " + acc + " not a valid lexeme type");
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
      in.get(c); // Expect a '>'
      if (c != '>') {
        throw UnexpectedInput("expected a >, got " + std::string{c});
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
    else if (c == '|') { // Start of another production for the non-terminal. Put it back and return.
      in.putback(c);
      break;
    }

    // Get next character.
    if (!is_terminator(c)) {
      in.get(c);
    }
  }

  // Done finding the rule. Store the rule.
  auto prod = production_rules_data_->productions_for.find(production_id);
  if (prod == production_rules_data_->productions_for.end()) {
    production_rules_data_->productions_for.emplace(production_id, State());
    prod = production_rules_data_->productions_for.find(production_id);
  }

  // Add production to the productions for production_id
  prod->second.insert(production);
  // Add production to all productions.
  production_rules_data_->all_productions.push_back(production);
}

void DescriptionParser::findResInfo(std::istream &in, ResolutionInfo &res_info) {
  auto is_terminator = [](char c) { return c == '\n' || c == '\r' || c == ';'; };

  // Note - comments are not allowed in the resolution info section.
  char c;
  in.get(c);
  std::string word;
  while (!in.eof()) {
    if (c == ' ') {} // Pass spaces
    else if (is_terminator(c)) { // End of production. No instructions.
      // Put the character back so the calling function will detect the end of the production
      in.putback(c);
      return;
    }
    else if (isalpha(c)) {
      word.clear();
      word.push_back(c);
      bool not_eof = getWord(in, word);
      if (!not_eof) {
        throw UnexpectedInput("in findResInfo, unexpectedly reached EOF");
      }
      // Expect a '('
      in.get(c);
      if (c != '(') {
        throw UnexpectedInput("in findResInfo, expected (, found " + std::string{c});
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
        if (word == "Left") { res_info.assoc = Associativity::Left; }
        else if (word == "Right") { res_info.assoc = Associativity::Right; }
        else if (word == "None") { res_info.assoc = Associativity::None; }
        else {
          throw UnexpectedInput("in findResInfo, expected Left, Right, or None as an associativity, not " + word);
        }
      }
      else {
        throw UnexpectedInput("in findResInfo function was " + word + ", which is not a valid function");
      }

      // Expect a ')'
      in.get(c);
      if (c != ')') {
        throw UnexpectedInput("in findResInfo, expected )");
      }
    }
    else if (c == ':') { // Start of the instruction section.
      // Put the ':' back so the calling function will detect the start of the instruction section.
      in.putback(c);
      return;
    }

    // Get next character
    in.get(c);
  }
}

inline std::shared_ptr<ParseNode> DescriptionParser::getInstructions(std::istream &in, int pid) {
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
    else if (isalpha(c)) {
      // Get the whole identifier.
      do {
        acc.push_back(c);
        in.get(c);
      } while (isalpha(c) || c == '_'); // letters or underscores

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
        std::string message = "Error: expected an open parenthesis. Found [" + std::string{c};
        message += "]. (Trying to find the argument for [" + node->designator + "].\n";
        message += "Instruction so far is " + instruction->printTerminals();
        throw UnexpectedInput(message);
      }

      // Gather all arguments.
      in.get(c);
      while (!in.eof() && c != ')') {
        // Pass spaces.
        if (isspace(c));
        // The character '$' starts a node reference - that is, it refers to the nodes of the production
        // the instruction corresponds to.
        else if (c == '$') {
          in.get(c);
          if (!isdigit(c)) {
            throw UnexpectedInput("expected a number to come after the '$' (node reference)");
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
        else if (c == ',') {}; // Put this on a separate line to silence a warning

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

bool DescriptionParser::getWord(std::istream &in, std::string &word) {
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

bool DescriptionParser::getInteger(std::istream &in, std::string &word) {
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

int DescriptionParser::registerProduction(const std::string &production) {
  auto it = production_rules_data_->nonterminal_map.find(production);
  if (it == production_rules_data_->nonterminal_map.end()) {
    production_rules_data_->nonterminal_map.emplace(production, production_rules_data_->num_productions);
    production_rules_data_->inverse_nonterminal_map.emplace(production_rules_data_->num_productions, production);
    return production_rules_data_->num_productions--;
  }
  return it->second;
}

void DescriptionParser::shiftProductionNumbers() {
  // Get the number of terminals.
  int lids = static_cast<int>(production_rules_data_->lexer_generator.GetNumLexemes());

  // Shift the ids in production map.
  for (auto &p: production_rules_data_->nonterminal_map) {
    p.second = lids - p.second;
  }

  // Shift the ids in all productions
  for (auto &item: production_rules_data_->all_productions) {
    // Correct production.
    item.production = lids - item.production;
    // Correct productions in the rhs.
    for (auto &i: item.rhs) {
      if (i < 0) {
        i = lids - i;
      }
    }
  }

  // Shift the ids in inverse map
  std::map<int, std::string> new_inverse_map;
  for (auto &p: production_rules_data_->inverse_nonterminal_map) {
    new_inverse_map.emplace(lids - p.first, p.second);
  }
  production_rules_data_->inverse_nonterminal_map = new_inverse_map;

  // Shift the start state.
  production_rules_data_->start_nonterminal = lids - production_rules_data_->start_nonterminal;

  // Shift the ids in productions_for_.
  std::map<int, State> new_productions_for;
  for (auto &p: production_rules_data_->productions_for) {
    State state;
    for (auto item: p.second) {
      // Correct production.
      item.production = lids - item.production;
      // Correct productions in the rhs.
      for (auto &i: item.rhs) {
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
  production_rules_data_->total_symbols = lids + static_cast<int>(production_rules_data_->nonterminal_map.size());
}