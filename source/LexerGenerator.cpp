//
// Created by Nathaniel Rupprecht on 2/1/21.
//

#include "LexerGenerator.h"
// Other files.
#include "LexerDFA.hpp"

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
    cout << "Failed to open file [" << filename << "].\n";
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
    lexer_dfa_.clear();
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

      /*
      A | B - Or
      (...) or [...] - Grouping.

      a-z   - Character range.

      --- Escape characters ---
      \s    - Space
      \n    - Newline
      \t    - Tab
      \0    - eof
      \\
      \[ \]
      \( \)
      \{ \}
      \|
      \+
      \-
      \*
      \?
      -------------------------
      */

      // Get the lexeme definition.
      auto[start_id, end_id] = get_sequence('\n');
      // Set recent_id node to be accepting.
      lexer_dfa_.AddAcceptance(end_id, static_cast<int>(all_lexemes_.size()) - 1, 1 /* Precedence */);
      // Lambda transition from head node to start_id node.
      lexer_dfa_.AddTransition(head_id_, start_id);
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
      string command;
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

int LexerGenerator::Accepts(const string &word) const {
  auto acceptance = lexer_dfa_.Accepts(word);
  if (acceptance.empty()) {
    return -1;
  }
  return acceptance[0].second;
}

char LexerGenerator::peek() const {
  return lexer_dfa_.peek();
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

std::string LexerGenerator::escapeLiteral(const std::string& literal) {
  std::string output;
  output.reserve(literal.size());

  for (auto c : literal) {
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
      default:
        output.push_back(c);
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

int LexerGenerator::ReservedIndex(const string &keyword) const {
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

int LexerGenerator::AddReserved(const string &keyword, int precedence) {
  // Check if the keyword already exists.
//  auto it = std::find_if(reserved_tokens_.begin(),
//                         reserved_tokens_.end(),
//                         [&keyword](const auto &pr) { return keyword == pr.first; });
//  if (it == reserved_tokens_.end()) {
//    // Since the reserved word is a literal string and AddLexeme expects a regex, we may have to escape some characters.
//    // TODO: Add an AddLiteral function.
//    std::string escaped_keyword;
//    for (auto c: keyword) {
//      if (c == '(') {
//        escaped_keyword += "\\(";
//      }
//      else if (c == ')') {
//        escaped_keyword += "\\)";
//      }
//      else {
//        escaped_keyword.push_back(c);
//      }
//    }
//    int lexeme_number = AddLexeme("RES:" + keyword, escaped_keyword, precedence);
//
//    reserved_tokens_.emplace_back(keyword, lexeme_number);
//    return lexeme_number;
//  }
//  else {
//    return static_cast<int>(std::distance(reserved_tokens_.begin(), it));
//  }

  // Introduce escape characters if necessary.

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
  auto[start_id, end_id] = get_sequence('\0', false);
  // Set recent_id node to be accepting.
  lexer_dfa_.AddAcceptance(end_id, lexeme_number, precedence);
  // Lambda transition from head node to start_id node.
  lexer_dfa_.AddTransition(head_id_, start_id);

  return lexeme_number;
}

std::size_t LexerGenerator::GetNumLexemes() const {
  return all_lexemes_.size();
}

inline std::pair<int, int> LexerGenerator::get_sequence(char terminator, bool useTerminator) {
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
      // Link recent_id node with end_id node with a epsilon transition.
      lexer_dfa_.AddTransition(recent_id, end_id);
      return ends;
    }
      // Pass spaces and tabs
    else if (c == ' ' || c == '\t');
      // Comment (full line)
    else if (c == '#') {
      while (!in_->eof() && c != '\n') {
        in_->get(c);
      }
    }
      // Start of grouping.
    else if (c == '(' || c == '[') {
      auto[start, end] = c == '(' ? get_sequence(')') : get_sequence(']');
      lexer_dfa_.AddTransition(recent_id, start); // Link up with a epsilon transition.
      // Peek if a modifiers modifies the grouping.
      in_->get(c);
      if (!in_->eof()) {
        if (c == '*') make_star(start, end, recent_id);
        else if (c == '?') make_ques(start, end, recent_id);
        else if (c == '+') make_plus(start, end, recent_id);
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
      // NOT - for now, we only allow NOTs of single characters or ranges.
    else if (c == '~') {

    }
      // Newline - we should only encounter this if terminator == '\n'
    else if (c == '\n') {
      throw UnexpectedNewLine();
    }
      // General character. Could be the start of a char range.
    else {
      char d = static_cast<char>(in_->peek());
      if (d == '-') { // Char range
        in_->get(d); // Consume the character
        in_->get(d); // Get the next character.
        if (d != terminator || !useTerminator) {
          add_node(c, d, recent_id);
        }
        else { // Not actually a char range. Just a character and a '-' character.
          add_char(c, recent_id);
          add_char('-', recent_id);
        }
      }
      else {
        add_char(c, recent_id);
      }
    }
    // Get next character
    in_->get(c);
  } while (!in_->eof());
  // In case of eof.
  lexer_dfa_.AddTransition(recent_id, end_id); // For completeness.
  return ends;
}

void LexerGenerator::specialCharacters(char c, int &recent_id) {
  // If we reached the EOF, the character was really just a \\
    // TODO: Should we require \ to still be escaped? Probably.
  if (in_->eof()) {
    add_char('\\', recent_id);
    return;
  }

  // Any letter. Need to add two transitions.
  if (c == '@') {
    int id = lexer_dfa_.AddNode();
    lexer_dfa_.AddTransition(recent_id, id, 'a', 'z');
    lexer_dfa_.AddTransition(recent_id, id, 'A', 'Z');
    check_modifier(recent_id, id, recent_id);
    recent_id = id;
  }
    // Any lower case letter.
  else if (c == 'a') {
    add_node('a', 'z', recent_id);
  }
    // Any upper case letter.
  else if (c == 'A') {
    add_node('A', 'Z', recent_id);
  }
    // Any number
  else if (c == 'd') {
    add_node('0', '9', recent_id);
  }
    // Other escape characters.
  else {
    char d = 0;
    if (c == '\\') d = '\\';
    else if (c == 'n') d = '\n';
    else if (c == 'r') d = '\r';
    else if (c == 't') d = '\t';
    else if (c == 's') d = ' ';
    else if (c == '(') d = '(';
    else if (c == '[') d = '[';
    else if (c == '{') d = '{';
    else if (c == ')') d = ')';
    else if (c == ']') d = ']';
    else if (c == '}') d = '}';
    else if (c == '|') d = '|';
    else if (c == '+') d = '+';
    else if (c == '-') d = '-';
    else if (c == '*') d = '*';
    else if (c == '?') d = '?';
    else if (c == '~') d = '~';
    else if (c == '0') d = 0;
    // Make the node.
    add_char(d, recent_id);
  }
}

void LexerGenerator::make_star(int idi, int idf, int &recent_id) {
  lexer_dfa_.AddTransition(idi, idf);
  lexer_dfa_.AddTransition(idf, idi);
}

void LexerGenerator::make_plus(int idi, int idf, int &recent_id) {
  lexer_dfa_.AddTransition(idf, idi);
}

void LexerGenerator::make_ques(int idi, int idf, int &recent_id) {
  lexer_dfa_.AddTransition(idi, idf);
}

// Add (if needed) a modifier to a sequence of nodes.
void LexerGenerator::check_modifier(int idi, int idf, int &recent_id) {
  char d = in_->peek();
  bool get_next = true;
  if (d == '+') make_plus(idi, idf, recent_id);
  else if (d == '*') make_star(idi, idf, recent_id);
  else if (d == '?') make_ques(idi, idf, recent_id);
  else get_next = false;
  // If necessary, get the next character.
  if (get_next) {
    in_->get(d);
  }
}

// Lambda function that adds another node in the line.
void LexerGenerator::add_node(char ci, char cf, int &recent_id) {
  int id = lexer_dfa_.AddNode();
  lexer_dfa_.AddTransition(recent_id, id, ci, cf);
  check_modifier(recent_id, id, recent_id);
  // Adjust pointer.
  recent_id = id;
}

// Lambda function for adding a character node. Checks whether a */+/? comes after the char.
void LexerGenerator::add_char(char c, int &recent_id) {
  int id = lexer_dfa_.AddNode();
  // Add the normal character transition.
  lexer_dfa_.AddTransition(recent_id, id, c);
  check_modifier(recent_id, id, recent_id);
  // Adjust pointer.
  recent_id = id;
}

inline bool LexerGenerator::is_skip(int lexeme_id) {
  return std::find(skip_lexemes_.begin(), skip_lexemes_.end(), lexeme_id) != skip_lexemes_.end();
}
