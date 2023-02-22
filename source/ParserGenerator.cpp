//
// Created by Nathaniel Rupprecht on 2/2/21.
//

#include "ParserGenerator.h"
// Other files
#include "LALRParser.hpp"

using namespace manta;

ParserGenerator::ParserGenerator(ParserType type)
    : parser_type_(type) {}

std::shared_ptr<LALRParser> ParserGenerator::CreateParserFromFile(const std::string &filename) {
  std::ifstream fin(filename);
  if (fin.fail()) {
    return nullptr;
  }
  auto parser = CreateParserFromStream(fin);
  fin.close();
  return parser;
}

std::shared_ptr<class LALRParser> ParserGenerator::CreateParserFromString(const std::string &description) {
  std::stringstream stream;
  stream << description;
  return CreateParserFromStream(stream);
}

std::shared_ptr<LALRParser> ParserGenerator::CreateParserFromStream(std::istream &stream) {
  char c;
  string production_name;
  int pid;

  // Create the lexer. We always add @eof as a lexer item.
  lexer_generator_.CreateLexer(stream, false); // Do not clear old.

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

        start_production_name_ = command;
      }
    }

    // Get next character.
    stream.get(c);
  }

  // Shift productions, so all terminals and nonterminals have positive numbers.
  shiftProductionNumbers();

  // Find start production (must do this after we shift production numbers).
  auto it = production_map_.find(start_production_name_);
  if (it == production_map_.end()) {
    throw std::exception();
  }
  start_production_ = it->second;

  // Compute which productions can derive empty.
  createStateDerivesEmpty();

  // Generate the parse table.
  computeLR0();

  // Complete the table
  completeTable();

  // Reduce the number of states.

  auto lexer = lexer_generator_.CreateLexer();
  // Note - this uses a private constructor.
  return std::shared_ptr<LALRParser>(
      new LALRParser(
          inverse_production_map_,
          start_production_,
          total_symbols_,
          parse_table_,
          all_states_,
          lexer));
}

int ParserGenerator::NumNonTerminals() const {
  return total_symbols_ - static_cast<int>(lexer_generator_.GetNumLexemes());
}

int ParserGenerator::NumTerminals() const {
  return static_cast<int>(lexer_generator_.GetNumLexemes());
}

int ParserGenerator::GetNonterminalID(const std::string &non_terminal) const {
  return production_map_.at(non_terminal);
}

std::set<int> ParserGenerator::FirstSet(int symbol) {
  std::vector<bool> visited(NumNonTerminals(), false);
  return internalFirst(symbol, visited);
}

std::set<std::string> ParserGenerator::FirstSet(const std::string &symbol) {
  auto first_set = FirstSet(production_map_[symbol]);
  std::set<std::string> output;
  std::for_each(
      first_set.begin(), first_set.end(),
      [&](int x) { output.insert(lexer_generator_.LexemeName(x)); });
  return output;
}

std::set<int> ParserGenerator::FollowSet(int symbol) {
  // See p. 135 of "Crafting a Compiler"

  // The follow set is the set of terminals that can follow a non-terminal
  // Formally, the follow set of A is defined to be
  //    Follow(A) = { b ∈ Σ | S ⇒+ α A b β }.
  //  for any (potentially empty) strings α, β (and Σ being the set of terminal symbols)

  // NOTE: It is assumed that the TERMINAL character for EOF is an element of the grammar, and that we
  // only look at the follow set of non-terminals. Therefore, every non-terminal must be followed by some terminal.

  // Keep track of which terminals have already had their follow sets computed.
  std::vector<bool> did_visit(NumNonTerminals(), false);
  return internalFollow(symbol, did_visit);
}

std::set<std::string> ParserGenerator::FollowSet(const std::string &symbol) {
  auto follow_set = FollowSet(production_map_[symbol]);
  std::set<std::string> output;
  std::for_each(
      follow_set.begin(), follow_set.end(),
      [&](int x) { output.insert(lexer_generator_.LexemeName(x)); });
  return output;
}

void ParserGenerator::WriteStates(std::ostream &out) const {
  int it = 0;
  for (const auto &state: all_states_) {
    out << "---- State " << it << " -----------\n";
    for (auto &item: state) {
      out << "  " << writeItem(item);
      out << "\n";
    }
    out << "---------------------------\n\n";
    ++it;
  }
}

std::string ParserGenerator::GetParserGenerationTrace() const {
  return parser_generation_trace_.str();
}

std::string ParserGenerator::nameOf(int id) const {
  if (isTerminal(id)) {
    auto lex_name = lexer_generator_.LexemeName(id);
    if (lex_name.find("RES:") == 0) {
      std::copy(lex_name.begin() + 4, lex_name.end(), lex_name.begin() + 1);
      lex_name[0] = '"';
      lex_name.resize(lex_name.size() - 3);
      return lex_name + '"';
    }
    return lex_name;
  }
  else {
    return inverse_production_map_.at(id);
  }
}

std::string ParserGenerator::writeItem(const Item &item) const {
  // This may be a null production, just a placeholder for a shift.
  if (item.production < 0) {
    return "";
  }
  std::string output = nameOf(item.production) + " -> ";
  int j = 0;
  for (auto symbol: item.rhs) {
    if (j == item.bookmark) {
      output += "* ";
    }
    output += nameOf(symbol) + " ";
    ++j;
  }
  if (j == item.bookmark) {
    output += "*";
  }
  return output;
}

void ParserGenerator::writeState(const State &state, ostream &out, int id) const {
  out << "---- State " << id << " -----------\n";
  for (auto &item: state) {
    out << "  " << writeItem(item);
    out << "\n";
  }
  out << "---------------------------\n\n";
}

inline void ParserGenerator::getProductions(std::istream &in, int production_id) {
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
        int id = lexer_generator_.AddReserved(acc);
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
      //  has the first id. This is cle
      if (acc == "start") {
        start_production_ = id;
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

      // TODO: Handle @null

      // Ask the lexer generator for the lexeme ID of the terminal
      int id = lexer_generator_.LexemeID(acc);
      if (id < 0) {
        throw UnrecognizedLexerItem("word " + acc + " not a valid lexeme type");
      }
      production.add(id);

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
  auto prod = productions_for_.find(production_id);
  if (prod == productions_for_.end()) {
    productions_for_.emplace(production_id, State());
    prod = productions_for_.find(production_id);
  }
  // Add production to the productions for production_id
  prod->second.insert(production);
  // Add production to all productions.
  all_productions_.push_back(production);
}

void ParserGenerator::findResInfo(std::istream &in, ResolutionInfo &res_info) {
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

inline std::shared_ptr<ParseNode> ParserGenerator::getInstructions(std::istream &in, int pid) {
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
      instruction->add(node);
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
        if (isspace(c)) { // Put this on a separate line to silence a warning
          // The character '$' starts a node reference - that is, it refers to the nodes of the production
          // the instruction corresponds to.
        }
        else if (c == '$') {
          in.get(c);
          if (!isdigit(c)) {
            throw UnexpectedInput("expected a number to come after the '$' (node reference)");
          }
          while (!in.eof() && isdigit(c)) {
            acc.push_back(c);
            in.get(c);
          }
          if (!in.eof()) {
            in.putback(c);
          }
          // Add child.
          node->add(acc);
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
          node->add(acc);
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

bool ParserGenerator::getWord(std::istream &in, std::string &word) {
  char c;
  in.get(c);
  while (!in.eof() && isalpha(c)) {
    word.push_back(c);
    in.get(c);
  }
  if (!in.eof()) {
    in.putback(c);
    return true;
  }
  return false;
}

bool ParserGenerator::getInteger(std::istream &in, std::string &word) {
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

inline int ParserGenerator::registerProduction(const string &production) {
  auto it = production_map_.find(production);
  if (it == production_map_.end()) {
    production_map_.emplace(production, num_productions_);
    inverse_production_map_.emplace(num_productions_, production);
    return num_productions_--;
  }
  return it->second;
}

inline void ParserGenerator::shiftProductionNumbers() {
  // Get the number of terminals.
  int lids = static_cast<int>(lexer_generator_.GetNumLexemes());

  // Shift the ids in production map.
  for (auto &p: production_map_) {
    p.second = lids - p.second;
  }

  // Shift the ids in all productions
  for (auto &item: all_productions_) {
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
  std::map<int, string> new_inverse_map;
  for (auto &p: inverse_production_map_) {
    new_inverse_map.emplace(lids - p.first, p.second);
  }
  inverse_production_map_ = new_inverse_map;

  // Shift the start state.
  start_production_ = lids - start_production_;

  // Shift the ids in productions_for_.
  std::map<int, State> new_productions_for;
  for (auto &p: productions_for_) {
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
    new_productions_for.insert(pair<int, State>(lids - p.first, state));
  }
  productions_for_ = new_productions_for;
  // Set total_symbols_.
  total_symbols_ = lids + static_cast<int>(production_map_.size());
}

void ParserGenerator::createStateDerivesEmpty() {
  // See p. 128 - 130 of "Crafting a Compiler"

  // TODO: Test.

  WorkDeque<ProductionID> work_deque;
  std::map<Item, bool> rule_derives_empty;
  // Count the symbols on the RHS of each production that do not derive empty.
  //  This is updated throughout the algorithm.
  std::map<Item, int> counts;

  auto check_for_empty = [&](const Item &item, int count) {
    if (count == 0) {
      rule_derives_empty[item] = true;
      if (!nonterminal_derives_empty_[item.production]) {
        nonterminal_derives_empty_[item.production] = true;
        work_deque.Add(item.production);
      }
    }
  };

  // Start everything as false.
  nonterminal_derives_empty_.assign(production_map_.size(), false);

  // Record all the production rules that contain instances of a symbol
  std::map<ProductionID, std::set<int>> productions_containing_symbol;

  int i = 0;
  for (auto &production: all_productions_) {
    // Update the map.
    for (auto r: production.rhs) {
      productions_containing_symbol[r].insert(i);
    }

    rule_derives_empty[production] = false;
    auto count = static_cast<int>(production.rhs.size());
    counts[production] = count;
    check_for_empty(production, count);

    ++i;
  }

  while (!work_deque.Empty()) {
    auto next = work_deque.PopNext();

    // Iterate through all productions that include [next]. It is ok if we create an empty entry.
    for (auto production_id: productions_containing_symbol[next]) {
      auto &production = all_productions_[production_id];
      --counts[production];
      check_for_empty(production, counts[production]);
    }
  }
}

bool ParserGenerator::isTerminal(int id) const {
  return id < NumTerminals();
}

bool ParserGenerator::isNonTerminal(int id) const {
  return !isTerminal(id);
}

int ParserGenerator::nonTerminalIndex(int id) const {
  return id - NumTerminals();
}

int ParserGenerator::getProductionIndex(const Item &item) const {
  auto it = std::find(all_productions_.begin(), all_productions_.end(), item);
  if (it == all_productions_.end()) {
    throw std::runtime_error("could not find the item in the productions");
  }
  return static_cast<int>(std::distance(all_productions_.begin(), it));
}

bool ParserGenerator::computeLR0() {
  status_ = true;

  // Find productions for the state.
  auto st = productions_for_.find(start_production_);
  if (st == productions_for_.end()) {
    parser_generation_trace_ << "Error - could not find productions for the start state.\n";
    status_ = false;
    return status_;
  }

  // I can't figure out why the compiler insists on using const objects here, so here's a hacky work around.
  State startItems = st->second;
  startItems.zero_bookmarks();

  // Add the start state.
  work_list_.clear();
  addState(startItems);
  // Go through the work list until it is empty.
  while (!work_list_.empty() && status_) {
    int s = work_list_.front();
    work_list_.pop_front();
    computeGoto(s);
  }

  // Return success.
  return status_;
}

int ParserGenerator::addState(const State &items) {
  // Try to find a state that is the given collection of items.
  int s = findState(items);
  // If the items are not a state, create a state for them.
  if (s == -1) {
    all_states_.push_back(items);
    s = static_cast<int>(all_states_.size()) - 1;
    // Initialize entries to Error.
    parse_table_.emplace_back(total_symbols_, Entry());
    work_list_.push_back(s);
  }
  // Return the state number.
  return s;
}

void ParserGenerator::computeGoto(int s) {
  // Find the closure of state s.
  State closed = closure(s);

  // Try advancing the dot for every symbol.
  for (int x = 0; x < total_symbols_; ++x) {
    State relevantItems = advanceDot(closed, x);
    if (!relevantItems.empty()) {
      int sn = addState(relevantItems);
      // Get the resolution info for the shift.
      ResolutionInfo res_info{};
      bool found_res_info = false, differing_res_info = false;
      for (const auto &item: relevantItems) {
        if (item.res_info != NullResolutionInfo) {
          if (found_res_info && res_info != item.res_info) {
            differing_res_info = true;
          }
          res_info = item.res_info;
          found_res_info = true;
        }
      }

      // Add shift entry, possibly with resolution info.
      if (found_res_info && !differing_res_info) {
        parse_table_[s][x] = Entry(sn, res_info);
      }
      else {
        parse_table_[s][x] = Entry(sn);
      }
    }
  }
}

State ParserGenerator::closure(int s) {
  // Initialize ans.
  State ans = all_states_[s];
  int prev_size = 0;

  // While ans is still growing.
  while (ans.size() != prev_size) {
    // Update size.
    prev_size = ans.size();
    // For all productions in ans.
    for (auto A: ans) {
      int bookmark = A.bookmark;
      int next = -1 < bookmark && bookmark < A.rhs.size() ? A.rhs.at(bookmark) : -1;

      // If the bookmark was behind a non-terminal, we need to add that nonterminal to the closure.
      if (lexer_generator_.GetNumLexemes() < next) {
        // Find the production for next.
        auto it = productions_for_.find(next);
        if (it == productions_for_.end()) {
          continue;
        }

        // Productions for next.
        State &state = it->second;
        // Set productions' bookmarks so they are like next -> * RHS(next)
        state.zero_bookmarks();

        for (const auto &st: state) {
          if (ans.find(st) == ans.end()) {
            ans.insert(st);
          }
        }
      }
    }
  }
  return ans;
}

State ParserGenerator::advanceDot(const State &state, int symb) {
  // symb may be terminal or nonterminal
  State advance_set;
  // Create set: { A -> a X * b | A -> a * X b in state}
  for (const auto &item: state) {
    int bookmark = item.bookmark;
    if (-1 < bookmark && bookmark < item.size() && item.at(bookmark) == symb) {
      Item new_item = item;
      // Increment the bookmark
      ++new_item.bookmark;
      advance_set.insert(new_item);
    }
  }
  return advance_set;
}

void ParserGenerator::completeTable() {
  // Used by LALR(k) parser.
  computeLookahead();

  for (int state_index = 0; state_index < all_states_.size(); ++state_index) {
    for (const auto &rule: all_productions_) {
      tryRuleInState(state_index, rule);
    }
  }

  // Assert the accept state for the starting production.
  assertEntry(0, start_production_, Entry(true));
}

void ParserGenerator::assertEntry(int state, int symbol, const Entry &action) {
  auto &current_entry = parse_table_[state][symbol];
  // If the current entry is unfilled (Error), fill it with the entry.
  if (current_entry.isError()) { // == Error
    current_entry = action; // <- action
  }
  else {
    // Resolve shift/reduce conflicts with precedence. I found a good summary of this scheme at
    // https://softwareengineering.stackexchange.com/questions/178187/how-to-add-precedence-to-lalr-parser-like-in-yacc
    // though this is in other places, like the dragon book.

    auto current_res_info = current_entry.GetResInfo();
    auto res_info = action.GetResInfo();

    // Record the potential conflict in the parser_generation_trace_.
    std::string bf(20, ' ');
    parser_generation_trace_
        << "Conflict for state " << state << ", symbol " << nameOf(symbol) << "\n"
        << "  > Current entry:  " << ToString(current_entry.getAction()) << " " << current_entry.getState()
        << (current_entry.isReduce() ? "\n" + bf + "Reduction" + writeItem(current_entry.getRule()) : "") << "\n"
        << bf << "Prec: " << current_res_info.precedence << ", Assoc: " << ToString(current_res_info.assoc)
        << "\n"
        << "  > Proposed entry: " << ToString(action.getAction()) << " " << action.getState()
        << (action.isReduce() ? "\n" + bf + "Reduction: " + writeItem(action.getRule()) : "") << "\n"
        << bf << "Prec: " << res_info.precedence << ", Assoc: " << ToString(res_info.assoc) << "\n";

    // This is resolution is a generalization of comparing the precedence of first and second operators, e.g.
    // things like A + A * A. We compare the precedence of the first and second operator. The lookahead is the
    // second operator, and the proposed reduction will contain the first operator, e.g. X -> A "+" A.
    // In other words, the second operator's precedence is in the current_res_info, the first operator's precedence is
    // in the incoming action.

    auto first_prec = res_info.precedence;
    auto second_prec = current_res_info.precedence;
    auto first_assoc = res_info.assoc;
    auto second_assoc = current_res_info.assoc;

    // If the first operator has lower precedence, shift.
    if (first_prec < second_prec) {
      bool current_shift = current_entry.isShift();
      current_entry = (current_shift ? current_entry : action);
      parser_generation_trace_ << "  @Res: Current entry is lower precedence. Using Shift ("
                              << (current_shift ? "current entry" : "replacing entry") << ").\n";
    }
      // If the first operator has higher precedence, reduce.
    else if (second_prec < first_prec) {
      bool current_reduce = current_entry.isReduce();
      current_entry = (current_reduce ? current_entry : action);
      parser_generation_trace_ << "  @Res: Current entry is higher precedence. Using Reduce ("
                              << (current_reduce ? "current entry" : "replacing entry") << ").\n";
    }
      // If they are the same, check the associativity.
    else {
      // Shift
      if (first_assoc == Associativity::Right && second_assoc == Associativity::Right) {
        bool current_shift = current_entry.isShift();
        current_entry = (current_shift ? current_entry : action);
        parser_generation_trace_ << "  @Res: Both entries are Right associative. Using Shift ("
                                << (current_shift ? "current entry" : "replacing entry") << ").\n";
      }
        // Reduce
      else if (first_assoc == Associativity::Left && second_assoc == Associativity::Left) {
        bool current_reduce = current_entry.isReduce();
        current_entry = (current_reduce ? current_entry : action);
        parser_generation_trace_ << "  @Res: Both entries are Left associative. Using Reduce ("
                                << (current_reduce ? "current entry" : "replacing entry") << ").\n";
      }
        // Otherwise, error.
      else {
        parser_generation_trace_ << "Error - Entry already exists!!!\n";
        parser_generation_trace_ << "State: " << state << ", Symbol: " << symbol
                                 << ". Old entry: " << parse_table_[state][symbol];
        parser_generation_trace_ << ", " << " New entry: " << action << "\n";
        // Set status_ to false.
        status_ = false;
      }
    }
  }
}

void ParserGenerator::computeLookahead() {
  /* Used in LALR(1) parser */
}

void ParserGenerator::tryRuleInState(int state, const Item &rule) {
  // Make rule into LHS(rule) -> RHS(rule)
  rule.endBookmark();
  const auto &state_set = all_states_[state];

  switch (parser_type_) {
    case ParserType::LR0: {
      // === LR(0) ===
      if (state_set.contains(rule)) { // If LHS(rule) -> RHS(rule) * is in State(state)
        for (int sym = 0; sym < total_symbols_; ++sym) {
          assertEntry(state, sym, Entry(rule));
        }
      }
      break;
    }
    default:
    case ParserType::LALR: // Not implemented yet.
      // TODO: Implement.
    case ParserType::SLR: {
      // === SLR ===
      if (state_set.contains(rule)) { // If LHS(rule) -> RHS(rule) * is in State(state)
        auto follow_set = FollowSet(rule.production);
        for (int sym: follow_set) {
          assertEntry(state, sym, Entry(rule));
        }
      }
    }
  }
}

int ParserGenerator::findState(const State &items) const {
  int s = 0;
  for (const auto &state: all_states_) {
    if (state == items) return s;
    // Increment state.
    ++s;
  }
  // Return -1 for false;
  return -1;
}

std::set<int> ParserGenerator::internalFirst(int symbol, std::vector<bool> &visited) {
  if (isTerminal(symbol)) {
    return {symbol};
  }
  if (!visited[nonTerminalIndex(symbol)]) {
    std::set<int> output;
    visited[nonTerminalIndex(symbol)] = true;
    const auto &state = productions_for_[symbol];
    for (const auto &production: state) {
      auto new_set = internalFirst(production.rhs.at(0), visited);
      output.insert(new_set.begin(), new_set.end());
    }
    // TODO: Check if this needs to be included.
    /* if derivesEmpty(firstSymbol)
     * output += internalFirst(begin + 1, end, visited)
     */
    return output;
  }
  return {};
}

std::set<int> ParserGenerator::internalFollow(int symbol, std::vector<bool> &visited) {
  // See p. 135 of "Crafting a Compiler"

  if (symbol == start_production_) {
    return {0}; // @eof
  }

  std::set<int> output;

  int index = symbol - NumTerminals(); // NonTerminals start at NumTerminals() (i.e. after all the terminals)
  if (!visited[index]) {
    visited[index] = true;

    // We need to find all occurrences of symbol in production rules, and add the First of the next symbol.
    for (const auto &production: all_productions_) {
      for (std::size_t i = 0; i < production.rhs.size(); ++i) {
        const auto &sym = production.rhs[i];
        // If this is an occurrence of symbol, look at the "tail" after this.
        if (sym == symbol) {
          if (i + 1 < production.rhs.size()) {
            auto first_set = FirstSet(production.rhs[i + 1]);
            output.insert(first_set.begin(), first_set.end());
          }
          // Note that this is automatically true if the symbol is the last in the production.
          if (allDeriveEmpty(production.rhs, i + 1)) {
            auto follow_set = internalFollow(production.production, visited);
            output.insert(follow_set.begin(), follow_set.end());
          }
        }
      }
    }
  }

  return output;
}

bool ParserGenerator::allDeriveEmpty(const std::vector<int> &rhs, std::size_t start_index) const {
  // See p. 135 of "Crafting a Compiler"

  // If tail is empty, then by definition, the remainder of the tail is empty.
  if (start_index == rhs.size()) {
    return true;
  }

  for (int i = static_cast<int>(start_index); i < static_cast<int>(rhs.size()); ++i) {
    if (auto it = productions_for_.find(i); it != productions_for_.end()) {
      if (!stateDerivesEmpty(it->second)) {
        return false;
      }
    }
    else { // If Tail[i] is a terminal
      return false;
    }
  }

  return true; // all_derive_empty;
}

bool ParserGenerator::stateDerivesEmpty(const State &state) const {
  return nonterminal_derives_empty_[findState(state)];
}
