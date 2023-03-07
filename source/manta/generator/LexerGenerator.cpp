//
// Created by Nathaniel Rupprecht on 2/1/21.
//

#include "manta/generator/LexerGenerator.h"
// Other files.
#include "manta/lexer/LexerDFA.hpp"

using namespace manta;

LexerGenerator::LexerGenerator(bool eof_token) {
  head_id_ = lexer_dfa_.AddNode();
  if (eof_token) {
    // Add @eof as a node.
    all_lexemes_.emplace_back("eof");
    auto eof_node = lexer_dfa_.AddNode();
    // Add a transition from the head node to an EOF accepting state.
    lexer_dfa_.AddTransition(head_id_, eof_node, '\0');
    lexer_dfa_.AddAcceptance(eof_node, 0 /* "EOD" ID == 0 */, 1);
  }
}

std::shared_ptr<LexerDFA> LexerGenerator::CreateLexer(const std::string &filename, bool clear_old) {
  std::ifstream fin(filename);
  if (fin.fail()) {
    std::cout << "Failed to open file [" << filename << "].\n";
    return nullptr;
  }
  // Create parser.
  return CreateLexer(fin, clear_old);
}

std::shared_ptr<LexerDFA> LexerGenerator::CreateLexer(std::istream &instream, bool clear_old) {
  in_ = instream;

  // Lambda function for passing whitespaces.
  auto pass_white_space = [&]() {
    char c;
    in_->get(c);
    while (!in_->eof() && (c == ' ' || c == '\t')) {
      in_->get(c);
    }
    in_->putback(c);
  };

  // Clear old data, add a new head node.
  if (clear_old) {
    lexer_dfa_.Clear();
    all_lexemes_.clear();
  }

  char c;

  // Find the .Lexer indicator
  in_->get(c);
  while (!in_->eof()) {
    if (c == '#') {
      in_->get(c);
      while (!in_->eof() and c != '\n') {
        in_->get(c);
      }
    }
    else if (c == '.') { // Command
      std::string command;
      in_->get(c);
      while (!in_->eof() && isalpha(c)) {
        command.push_back(c);
        in_->get(c);
      }
      if (command == "Lexer") {
        in_->putback(c); // Just in case c is '.'
        break; // Start lexing.
      }
      // Not the Lexer command. Keep looking.
    }
    else {
      in_->get(c);
    }
  }
  if (in_->eof()) {
    std::cout << "Could not find the .Lexer indicator.\n";
    return nullptr;
  }

  // Parse parser description.
  in_->get(c);
  while (!in_->eof()) {
    // Start of a lexeme definition.
    if (c == '@') {
      auto lexeme = getLexeme();

      // Make sure this is not a repeat lexeme. If not, store it.
      if (std::find(all_lexemes_.begin(), all_lexemes_.end(), lexeme) != all_lexemes_.end()) {
        std::cout << "WARNING: This lexeme ('@" << lexeme << "') already exists.\n";
      }
      else {
        all_lexemes_.push_back(lexeme);
      }

      pass_white_space();

      // Get the 'r'
      in_->get(c);
      MANTA_ASSERT(c == 'r', "did not find 'r' in regex definition");
      MANTA_ASSERT(!in_->eof(), "reached EOF while getting lexeme definition");

      // Get the '`'
      in_->get(c);
      MANTA_ASSERT(c == '`', "did not find opening \" in regex definition");
      MANTA_ASSERT(!in_->eof(), "reached EOF while getting lexeme definition");

      // ========================================
      // Get the string that defines the regex.
      // ========================================

      // Get the lexeme definition.
      auto [start_id, end_id] = getSequence('`');
      // Set recent_id node to be accepting.
      lexer_dfa_.AddAcceptance(end_id, static_cast<int>(all_lexemes_.size()) - 1, 1 /* Precedence */);
      // Lambda transition from head node to start_id node.
      lexer_dfa_.AddTransition(head_id_, start_id);

      // Consume spaces until a newline or EOF.
      in_->get(c);
      while (!in_->eof()) {
        if (c == ' ' || c == '\t');
        else if (c == '\n') {
          break;
        }
        else {
          MANTA_FAIL("unexpected characters");
        }
        in_->get(c);
      }
    }
      // Full line comments.
    else if (c == '#') {
      while (!in_->eof() && c != '\n') {
        in_->get(c);
      }
    }
      // Command.
    else if (c == '.') {
      pass_white_space();
      std::string command;
      in_->get(c);
      // Get the command and check what it is.
      while (!in_->eof() && isalpha(c)) {
        command.push_back(c);
        in_->get(c);
      }
      // End parsing.
      if (command == "End" || command == "Parser") {
        break;
      }
      // Designate a skip lexeme
      if (command == "Skip") {
        // Get the lexeme to skip.
        auto skip = getLexeme();
        skip_lexemes_.push_back(LexemeID(skip));
      }
      else {
        std::cout << "Unrecognized command: " << command << "\n";
      }
    }
    // Get next character.
    in_->get(c);
  }
  // Create a lexer from the current NDFA.
  return CreateLexer();
}

std::shared_ptr<LexerDFA> LexerGenerator::CreateLexer() {
  // Reduce the NFA to a DFA.
  lexer_dfa_ = lexer_dfa_.NFAToDFA();
  return std::shared_ptr<LexerDFA>(
      new LexerDFA(
          lexer_dfa_,
          all_lexemes_,
          reserved_tokens_,
          skip_lexemes_));
}

int LexerGenerator::Accepts(const std::string &word) const {
  auto acceptance = lexer_dfa_.Accepts(word);
  if (acceptance.empty()) {
    return -1;
  }
  return acceptance[0].second;
}

char LexerGenerator::peek() const {
  return lexer_dfa_.Peek();
}

std::string LexerGenerator::getLexeme() const {
  std::string lexeme;
  char c;
  in_->get(c);
  while (!in_->eof() && isalpha(c) || isdigit(c) || c == '_') {
    // Check for errors.
    if (c == '\n' || c == '\r') {
      throw UnexpectedNewLine();
    }
    // Store char
    lexeme.push_back(c);
    in_->get(c);
  }
  return lexeme;
}

std::string LexerGenerator::escapeLiteral(const std::string &literal) {
  std::string output;
  output.reserve(literal.size());

  for (auto c: literal) {
    switch (c) {
      case '\\':
        output += "\\\\";
        break;
      case '|':
        output += "\\|";
        break;
      case '(':
        output += "\\(";
        break;
      case ')':
        output += "\\)";
        break;
      case '[':
        output += "\\[";
        break;
      case ']':
        output += "\\]";
        break;
      default:output.push_back(c);
        break;
    }
  }

  return output;
}

const std::string &LexerGenerator::LexemeName(int index) const {
  if (index < 0 || all_lexemes_.size() <= index) {
    throw InvalidIndex();
  }
  return all_lexemes_[index];
}

int LexerGenerator::LexemeID(const std::string &name) const {
  auto it = std::find(all_lexemes_.begin(), all_lexemes_.end(), name);
  if (it == all_lexemes_.end()) {
    return -1;
  }
  else {
    return static_cast<int>(std::distance(all_lexemes_.begin(), it));
  }
}

int LexerGenerator::ReservedIndex(const std::string &keyword) const {
  auto it = std::find_if(reserved_tokens_.begin(),
                         reserved_tokens_.end(),
                         [&keyword](const auto &pr) { return keyword == pr.first; });
  if (it == reserved_tokens_.end()) {
    return -1;
  }
  else {
    return it->second;
  }
}

int LexerGenerator::AddReserved(const std::string &keyword, int precedence) {
  // The call to escapeLiteral escapes characters, if necessary.
  return AddLexeme("RES:" + keyword, escapeLiteral(keyword), precedence);
}

void LexerGenerator::AddSkip(const std::string &keyword) {
  auto it = std::find(all_lexemes_.begin(), all_lexemes_.end(), keyword);
  if (it == all_lexemes_.end()) {
    throw InvalidIndex{};
  }

  auto index = static_cast<int>(std::distance(all_lexemes_.begin(), it));
  // Make sure the lexeme was not already added as a skip lexeme.
  if (std::find(skip_lexemes_.begin(), skip_lexemes_.end(), index) == skip_lexemes_.end()) {
    skip_lexemes_.push_back(index);
  }
}

int LexerGenerator::AddLexeme(const std::string &lexeme, const std::string &regex, int precedence) {
  // Check if the lexeme already exists
  auto it = std::find(all_lexemes_.begin(), all_lexemes_.end(), lexeme);
  if (it != all_lexemes_.end()) {
    return static_cast<int>(std::distance(all_lexemes_.begin(), it));
  }

  // Lexeme does not exist, add it.
  all_lexemes_.push_back(lexeme);
  int lexeme_number = static_cast<int>(all_lexemes_.size()) - 1;

  // Get the lexeme definition as part of a NDFA.
  std::stringstream stream;
  stream << regex;
  in_ = stream;
  auto[start_id, end_id] = getSequence('\0', false);
  // Set recent_id node to be accepting.
  lexer_dfa_.AddAcceptance(end_id, lexeme_number, precedence);
  // Lambda transition from head node to start_id node.
  lexer_dfa_.AddTransition(head_id_, start_id);

  return lexeme_number;
}

std::size_t LexerGenerator::GetNumLexemes() const {
  return all_lexemes_.size();
}

std::pair<int, int> LexerGenerator::getSequence(char terminator, bool useTerminator) {
  if (in_->eof()) {
    return std::make_pair(-1, -1);
  }

  int start_id = lexer_dfa_.AddNode();
  int end_id = lexer_dfa_.AddNode();
  int recent_id = start_id;
  std::pair<int, int> ends(start_id, end_id);

  char c;
  in_->get(c);
  do {
    // Terminator character.
    if (useTerminator && c == terminator) {
      // Link recent_id node with end_id node with an epsilon transition.
      lexer_dfa_.AddTransition(recent_id, end_id);
      return ends;
    }
    // Pass spaces and tabs
    else if (c == ' ' || c == '\t');
      // Start of grouping.
    else if (c == '(') {
      auto[start, end] = getSequence(')');
      lexer_dfa_.AddTransition(recent_id, start); // Link up with an epsilon transition.

      // Peek if a modifiers modifies the grouping.
      in_->get(c);
      if (!in_->eof()) {
        if (c == '*') makeStar(start, end, recent_id);
        else if (c == '?') makeQues(start, end, recent_id);
        else if (c == '+') makePlus(start, end, recent_id);
        else in_->putback(c);
      }
      recent_id = end;
    }
      // Escape character
    else if (c == '\\') {
      in_->get(c); // Get next letter.
      specialCharacters(c, recent_id);
    }
      // OR
    else if (c == '|') {
      // Link recent_id node with end_id node via a epsilon transition, set up to get the next bin in the OR statement
      lexer_dfa_.AddTransition(recent_id, end_id);
      recent_id = start_id;
    }
      // Character class.
    else if (c == '[') {
      // Character classes can accept single characters based on whether they fall into a range, do not fall into a range,
      // or do not match a certain string.
      //
      // Fall into range: [a-z] ==> Accept the char if it is in the range a-z, inclusively.
      // Don't fall into range: [^a-z] ==> Accept char if it is not in the range a-z, inclusively.
      // Dont match string: [~*/] ==> Accept char if the next two chars are not "*/"

      auto [start, end] = getCharacterClass();
      lexer_dfa_.AddTransition(recent_id, start); // Link up with an epsilon transition.

      // Peek if a modifiers modifies the grouping.
      in_->get(c);
      if (!in_->eof()) {
        if (c == '*') makeStar(start, end, recent_id);
        else if (c == '?') makeQues(start, end, recent_id);
        else if (c == '+') makePlus(start, end, recent_id);
        else in_->putback(c);
      }
      recent_id = end;
    }
      // Newline - we should only encounter this if terminator == '\n'
    else if (c == '\n') {
      throw UnexpectedNewLine();
    }
      // General character. Could be the start of a char range.
    else {
      addChar(c, recent_id);
    }
    // Get next character
    in_->get(c);
  } while (!in_->eof());
  // In case of eof.
  lexer_dfa_.AddTransition(recent_id, end_id); // For completeness.
  return ends;
}

std::pair<int, int> LexerGenerator::getCharacterClass() {
  MANTA_REQUIRE(!in_->eof(), "cannot encounter EOF in the middle of a character class")
  char c = in_->peek();
  // Check if this is a normal character class, complement character class, or string complement.
  if (c == '^') { // Complement character class.
    in_->get(c); // Consume.
    return characterClass(true);
  }
  else if (c == '~') { // String complement.
    in_->get(c); // Consume.
    return stringComplement();
  }
  else { // Normal character class.
    return characterClass();
  }
}

std::pair<int, int> LexerGenerator::stringComplement() {
  MANTA_REQUIRE(!in_->eof(), "cannot encounter EOF in string complement");

  // Get characters until (unescaped) ]. Do not allow newlines.
  char c;
  in_->get(c);
  std::string str;
  while (!in_->eof()) {
    MANTA_ASSERT(c != '\n', "cannot have a newline in the middle of a string complement");
    if (c == '\\') { // Escaped character
      MANTA_ASSERT(!in_->eof(), "cannot encounter EOF in string complement");
      in_->get(c);
      str.push_back(escapedCharacter(c));
    }
    else if (c == ']') { // End of character class.
      break;
    }
    else if (c == ' '); // Skip space.
    else {
      str.push_back(c);
    }
    in_->get(c);
  }

  // Construct the NFA for the complement of the string.
  int start_id = lexer_dfa_.AddNode();
  int end_id = lexer_dfa_.AddNode();
  int recent_id = start_id;

  //  Example for [~INT]
  //  Note that the start node has a self-transition for "Not I"
  //
  //           <----------------Not T-----
  //           <---Not N-----            |
  //           |            |            |
  //  [Start] (*) -- I --> (*) -- N --> (*) -- T --> (*)
  //           |            |            |
  //           ----------->(*)<-----------
  //                      [End]

  for (char ch : str) {
    // Add the transition back to start if "Not [ch]" occurred.
    lexer_dfa_.AddTransition(recent_id, start_id, ch, true);
    // Add the epsilon transition from the last ID to the end.
    lexer_dfa_.AddTransition(recent_id, end_id);

    // Add the transition from the last ID to the ID if [ch] occurred
    int next_id = lexer_dfa_.AddNode();
    lexer_dfa_.AddTransition(recent_id, next_id, ch);

    recent_id = next_id;
  }

  return { start_id, end_id };
}

std::pair<int, int> LexerGenerator::characterClass(bool make_complement) {
  MANTA_REQUIRE(!in_->eof(), "cannot be at EOF when reading character class");

  std::vector<std::pair<char, char>> ranges;
  std::vector<char> characters;

  char c;
  in_->get(c);

  do {
    // End of the range.
    if (c == ']') {
      break;
    }
    else if (std::isspace(c)); // Bypass whitespace.
    // Other character.
    else {
      // Check if this a character range.
      char d = static_cast<char>(in_->peek());
      if (d == '-') {
        in_->get(d); // Consume.

        in_->get(d);
        // If the character is ']', the '-' is just another character.
        if (d == ']') {
          characters.push_back(c);
          characters.push_back('-');

          break;
        }
        // Start of an escaped character.
        else if (d == '\\') {
          // Get escaped character.
          in_->get(d);
          d = escapedCharacter(d);

          ranges.emplace_back(c, d);
        }
        // Ordinary character, end of the range.
        else {
          ranges.emplace_back(c, d);
        }

      }
      // Not a character range.
      else {
        characters.push_back(c);
      }
    }

    in_->get(c);
  } while (!in_->eof());

  int start_id = lexer_dfa_.AddNode();
  int end_id = lexer_dfa_.AddNode();

  if (make_complement) {
    MANTA_FAIL("complement range not implemented");
  }
  else { // Normal ranges and selections.
    // NOTE: This is not the absolute best thing to do, since you could have overlapping ranges
    // and chars within other ranges. However, it is probably pretty good, since most people
    // aren't going to write things like [a-z c-z] or [abcdef...z]
    for (auto& [s, e] : ranges) {
      lexer_dfa_.AddTransition(start_id, end_id, s, e);
    }
    for (auto ch : characters) {
      lexer_dfa_.AddTransition(start_id, end_id, c);
    }
  }

  return { start_id, end_id };
}

void LexerGenerator::specialCharacters(char c, int &recent_id) {
  // Backslashes must be escaped.
  if (in_->eof()) {
    MANTA_FAIL("found single backslash (\\) right before EOF, backslashes must be escaped");
  }

  // Any letter. Need to add two transitions.
  if (c == '@') {
    int id = lexer_dfa_.AddNode();
    lexer_dfa_.AddTransition(recent_id, id, 'a', 'z');
    lexer_dfa_.AddTransition(recent_id, id, 'A', 'Z');
    checkModifier(recent_id, id, recent_id);
    recent_id = id;
  }
  // Any lower case letter.
  else if (c == 'a') {
    addNode('a', 'z', recent_id);
  }
  // Any upper case letter.
  else if (c == 'A') {
    addNode('A', 'Z', recent_id);
  }
  // Any number
  else if (c == 'd') {
    addNode('0', '9', recent_id);
  }
  // Other escape characters.
  else {
    char d = escapedCharacter(c);
    // Make the node.
    addChar(d, recent_id);
  }
}

void LexerGenerator::makeStar(int idi, int idf, int &recent_id) {
  lexer_dfa_.AddTransition(idi, idf);
  lexer_dfa_.AddTransition(idf, idi);
}

void LexerGenerator::makePlus(int idi, int idf, int &recent_id) {
  lexer_dfa_.AddTransition(idf, idi);
}

void LexerGenerator::makeQues(int idi, int idf, int &recent_id) {
  lexer_dfa_.AddTransition(idi, idf);
}

// Add (if needed) a modifier to a sequence of nodes.
void LexerGenerator::checkModifier(int idi, int idf, int &recent_id) {
  char d = in_->peek();
  bool get_next = true;
  if (d == '+') makePlus(idi, idf, recent_id);
  else if (d == '*') makeStar(idi, idf, recent_id);
  else if (d == '?') makeQues(idi, idf, recent_id);
  else get_next = false;
  // If necessary, get the next character.
  if (get_next) {
    in_->get(d);
  }
}

// Lambda function that adds another node in the line.
void LexerGenerator::addNode(char ci, char cf, int &recent_id) {
  int id = lexer_dfa_.AddNode();
  lexer_dfa_.AddTransition(recent_id, id, ci, cf);
  checkModifier(recent_id, id, recent_id);
  // Adjust pointer.
  recent_id = id;
}

// Lambda function for adding a character node. Checks whether a */+/? comes after the char.
void LexerGenerator::addChar(char c, int &recent_id) {
  int id = lexer_dfa_.AddNode();
  // Add the normal character transition.
  lexer_dfa_.AddTransition(recent_id, id, c);
  checkModifier(recent_id, id, recent_id);
  // Adjust pointer.
  recent_id = id;
}

inline bool LexerGenerator::isSkip(int lexeme_id) {
  return std::find(skip_lexemes_.begin(), skip_lexemes_.end(), lexeme_id) != skip_lexemes_.end();
}

char LexerGenerator::escapedCharacter(char c) {
  if (c == '\\') return '\\';
  else if (c == 'n') return '\n';
  else if (c == 'r') return '\r';
  else if (c == 't') return '\t';
  else if (c == 's') return ' ';
  else if (c == '(') return '(';
  else if (c == '[') return '[';
  else if (c == '{') return '{';
  else if (c == ')') return ')';
  else if (c == ']') return ']';
  else if (c == '}') return '}';
  else if (c == '|') return '|';
  else if (c == '+') return '+';
  else if (c == '-') return '-';
  else if (c == '*') return '*';
  else if (c == '?') return '?';
  else if (c == '~') return '~';
  else if (c == '0') return static_cast<char>(0);
  // Otherwise, the return the input character, it is not a special escaped char.
  return c;
}
