//
// Created by Nathaniel Rupprecht on 2/1/21.
//

#include "manta/generator/LexerGenerator.h"
// Other files.
#include <Lightning/Lightning.h>

#include "manta/lexer/LexerDFA.h"
#include "manta/utility/Formatting.h"
#include "manta/utility/StreamFunctions.h"

using namespace manta::formatting;

namespace manta {

LexerGenerator::LexerGenerator(bool eof_token) {
  head_id_ = lexer_dfa_.AddNode();
  if (eof_token) {
    AddLexeme("eof", "\0", 2);
  }
}

std::shared_ptr<LexerDFA> LexerGenerator::CreateLexer(const std::string& filename, bool clear_old) {
  std::ifstream fin(filename);
  if (fin.fail()) {
    LOG_SEV(Error) << "Failed to open file [" << filename << "].";
    return nullptr;
  }
  // Create parser.
  return CreateLexer(fin, clear_old);
}

std::shared_ptr<LexerDFA> LexerGenerator::CreateLexer(std::istream& instream, bool clear_old) {
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
    else if (c == '.') {
      // Command
      std::string command;
      in_->get(c);
      while (!in_->eof() && isalpha(c)) {
        command.push_back(c);
        in_->get(c);
      }
      if (command == "Lexer") {
        in_->putback(c);  // Just in case c is '.'
        break;            // Start lexing.
      }
      // Not the Lexer command. Keep looking.
    }
    else {
      in_->get(c);
    }
  }
  if (in_->eof()) {
    LOG_SEV(Error) << "Could not find the '.Lexer' indicator.";
    return nullptr;
  }

  // Parse parser description.
  in_->get(c);
  while (!in_->eof()) {
    // Start of a lexeme definition.
    if (c == '@') {
      auto lexeme_name = getLexemeName();
      LOG_SEV(Debug) << "Got lexeme name " << CLBG(lexeme_name) << ".";

      // Make sure this is not a repeat lexeme_name. If not, store it.
      if (std::find(all_lexemes_.begin(), all_lexemes_.end(), lexeme_name) != all_lexemes_.end()) {
        LOG_SEV(Warning) << "The lexeme_name ('@" << CLBG(lexeme_name)
                         << "') already exists. The new definition will not overwrite "
                            "the old definition";
      }

      pass_white_space();

      // Get the 'r'
      in_->get(c);
      MANTA_ASSERT(c == 'r', "did not find 'r' in regex definition");
      MANTA_ASSERT(!in_->eof(), "reached EOF while getting lexeme_name definition");

      // Get the '`'
      in_->get(c);
      MANTA_ASSERT(c == '`', "did not find opening \" in regex definition");
      MANTA_ASSERT(!in_->eof(), "reached EOF while getting lexeme_name definition");

      // ========================================
      // Get the string that defines the regex.
      // ========================================

      // Get the regex expression defining the lexeme_name.

      auto regex_pattern = GetUntil(in_, '`');
      LOG_SEV(Debug) << "Extracted regex pattern " << CLBB(regex_pattern) << " for lexeme "
                     << CLBG(lexeme_name) << ".";

      // Add the lexeme_name.
      AddLexeme(lexeme_name, regex_pattern, 1);

      // Consume spaces until a newline or EOF.
      in_->get(c);
      while (!in_->eof()) {
        if (c == ' ' || c == '\t')
          ;
        else if (c == '\n') {
          break;
        }
        else {
          LOG_SEV(Error) << "Unexpected character '" << c << "'.";
          MANTA_FAIL("unexpected characters");
        }
        in_->get(c);
      }
    }
    // Full line comments.
    else if (c == '#') {
      LOG_SEV(Debug) << "Passing over full-line comment.";
      while (!in_->eof() && c != '\n') {
        in_->get(c);
      }
    }
    // Command.
    else if (c == '.') {
      LOG_SEV(Debug) << "Extracting command.";
      pass_white_space();
      std::string command;
      in_->get(c);
      // Get the command and check what it is.
      while (!in_->eof() && isalpha(c)) {
        command.push_back(c);
        in_->get(c);
      }
      LOG_SEV(Debug) << "Command is " << CLY(command) << ".";

      // End parsing.
      if (command == "End" || command == "Parser") {
        break;
      }
      // Designate a skip lexeme
      if (command == "Skip") {
        // Get the lexeme to skip.
        auto skip = getLexemeName();
        LOG_SEV(Debug) << "Adding skip lexeme " << CLBG(skip) << ".";
        AddSkip(skip);
      }
      else {
        LOG_SEV(Warning) << "Unrecognized command " << CLY(command) << ". Bypassing.";
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
  return std::shared_ptr<LexerDFA>(new LexerDFA(lexer_dfa_, all_lexemes_, reserved_tokens_, skip_lexemes_));
}

const std::map<std::string, std::pair<std::string, int>>& LexerGenerator::GetDefiningExpressions() const {
  return defining_expressions_;
}

int LexerGenerator::Accepts(const std::string& word) const {
  auto acceptance = lexer_dfa_.Accepts(word);
  if (acceptance.empty()) {
    return -1;
  }
  return acceptance[0].second;
}

char LexerGenerator::peek() const {
  return lexer_dfa_.Peek();
}

std::string LexerGenerator::getLexemeName() const {
  std::string lexeme;
  char c {};
  in_->get(c);
  while ((!in_->eof() && isalpha(c)) || isdigit(c) || c == '_') {
    // Check for errors.
    MANTA_ASSERT(!(c == '\n' || c == '\r'), "did not expect newline while getting lexeme name");
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
        output += R"(\\)";
        break;
      case '|':
        output += R"(\|)";
        break;
      case '(':
        output += R"(\()";
        break;
      case ')':
        output += "\\)";
        break;
      case '[':
        output += R"(\[)";
        break;
      case ']':
        output += R"(\])";
        break;
      default:
        output.push_back(c);
        break;
    }
  }

  return output;
}

const std::string& LexerGenerator::LexemeName(std::size_t index) const {
  MANTA_REQUIRE(0 <= index && index < all_lexemes_.size(), "index " << index << " out of bounds");
  return all_lexemes_[index];
}

int LexerGenerator::LexemeID(const std::string& name) const {
  auto it = std::find(all_lexemes_.begin(), all_lexemes_.end(), name);
  MANTA_ASSERT(it != all_lexemes_.end(), "could not find lexeme named " << name);
  return static_cast<int>(std::distance(all_lexemes_.begin(), it));
}

int LexerGenerator::ReservedIndex(const std::string& keyword) const {
  auto it = reserved_tokens_.find(keyword);
  if (it == reserved_tokens_.end()) {
    return -1;
  }
  else {
    return it->second;
  }
}

int LexerGenerator::AddReserved(const std::string& keyword, int precedence) {
  // The call to escapeLiteral escapes characters, if necessary.
  auto reserved_name = "RES:" + keyword;
  auto index         = AddLexeme(reserved_name, escapeLiteral(keyword), precedence);
  reserved_tokens_.emplace(reserved_name, index);
  return index;
}

void LexerGenerator::AddSkip(const std::string& keyword) {
  auto it = std::find(all_lexemes_.begin(), all_lexemes_.end(), keyword);

  MANTA_REQUIRE(it != all_lexemes_.end(),
                "could not add skip lexeme '" << keyword << "', there is no such lexeme");

  auto index = static_cast<int>(std::distance(all_lexemes_.begin(), it));
  // Make sure the lexeme was not already added as a skip lexeme.
  if (!skip_lexemes_.contains(index)) {
    skip_lexemes_.insert(index);
  }
}

int LexerGenerator::AddLexeme(const std::string& lexeme_name, const std::string& regex, int precedence) {
  // Check if the lexeme already exists
  auto it = std::find(all_lexemes_.begin(), all_lexemes_.end(), lexeme_name);
  if (it != all_lexemes_.end()) {
    return static_cast<int>(std::distance(all_lexemes_.begin(), it));
  }

  // Lexeme does not exist, add it.
  all_lexemes_.push_back(lexeme_name);
  int lexeme_number = static_cast<int>(all_lexemes_.size()) - 1;

  // Get the lexeme definition as part of a NDFA.
  auto container          = utility::IStreamContainer::StreamString(regex);
  auto [start_id, end_id] = getSequence(container, '\0', false);
  // Set recent_id node to be accepting.
  lexer_dfa_.AddAcceptance(end_id, lexeme_number, precedence);
  // Lambda transition from head node to start_id node.
  lexer_dfa_.AddTransition(head_id_, start_id);

  // Store the defining expression for lexemes.
  defining_expressions_[lexeme_name] = {regex, precedence};

  return lexeme_number;
}

std::vector<std::tuple<std::string, std::string, int>> LexerGenerator::GetOrderedLexemeDefinitions() const {
  auto N = all_lexemes_.size();
  std::vector<std::tuple<std::string, std::string, int>> output;
  output.reserve(N);
  for (auto& name : all_lexemes_) {
    auto& [regex_pattern, prec] = defining_expressions_.at(name);
    output.emplace_back(name, regex_pattern, prec);
  }
  return output;
}

std::vector<std::string> LexerGenerator::GetSkipLexemeNames() const {
  std::vector<std::string> output;
  for (auto id : skip_lexemes_) {
    output.emplace_back(LexemeName(id));
  }
  return output;
}

std::vector<std::string> LexerGenerator::GetReservedLexemeNames() const {
  std::vector<std::string> output;
  for (auto& [name, _] : reserved_tokens_) {
    output.emplace_back(name);
  }
  return output;
}

std::size_t LexerGenerator::GetNumLexemes() const {
  return all_lexemes_.size();
}

std::string LexerGenerator::GetLexemeName(std::size_t index) const {
  if (index < 0 || all_lexemes_.size() <= index) {
    return "";
  }
  return all_lexemes_[index];
}

bool LexerGenerator::IsReserved(const std::string& lexeme_name) const {
  return reserved_tokens_.contains(lexeme_name);
}

std::pair<int, int> LexerGenerator::getSequence(utility::IStreamContainer& stream_container,
                                                char terminator,
                                                bool useTerminator) {
  if (stream_container->eof()) {
    return std::make_pair(-1, -1);
  }

  int start_id  = lexer_dfa_.AddNode();
  int end_id    = lexer_dfa_.AddNode();
  int recent_id = start_id;
  std::pair<int, int> ends(start_id, end_id);

  char c {};  // Initialize to 0
  stream_container->get(c);
  do {
    // Terminator character.
    if (useTerminator && c == terminator) {
      // Link recent_id node with end_id node with an epsilon transition.
      lexer_dfa_.AddTransition(recent_id, end_id);
      return ends;
    }
    // Pass spaces and tabs
    else if (c == ' ' || c == '\t')
      ;
    // Start of grouping.
    else if (c == '(') {
      auto [start, end] = getSequence(stream_container, ')');
      lexer_dfa_.AddTransition(recent_id, start);  // Link up with an epsilon transition.

      // Peek if a modifiers modifies the grouping.
      stream_container->get(c);
      if (!stream_container->eof()) {
        if (c == '*') {
          makeStar(start, end, recent_id);
        }
        else if (c == '?') {
          makeQues(start, end, recent_id);
        }
        else if (c == '+') {
          makePlus(start, end, recent_id);
        }
        else {
          stream_container->putback(c);
        }
      }
      recent_id = end;
    }
    // Escape character
    else if (c == '\\') {
      stream_container->get(c);  // Get next letter.
      specialCharacters(stream_container, c, recent_id);
    }
    // OR
    else if (c == '|') {
      // Link recent_id node with end_id node via a epsilon transition, set up to get the
      // next bin in the OR statement
      lexer_dfa_.AddTransition(recent_id, end_id);
      recent_id = start_id;
    }
    // Character class.
    else if (c == '[') {
      // Character classes can accept single characters based on whether they fall into a
      // range, do not fall into a range, or do not match a certain string.
      //
      // Fall into range: [a-z] ==> Accept the char if it is in the range a-z,
      // inclusively. Don't fall into range: [^a-z] ==> Accept char if it is not in the
      // range a-z, inclusively. Dont match string: [~*/] ==> Accept char if the next two
      // chars are not "*/"

      auto [start, end] = getCharacterClass(stream_container);
      lexer_dfa_.AddTransition(recent_id, start);  // Link up with an epsilon transition.

      // Peek if a modifiers modifies the grouping.
      stream_container->get(c);
      if (!stream_container->eof()) {
        if (c == '*') {
          makeStar(start, end, recent_id);
        }
        else if (c == '?') {
          makeQues(start, end, recent_id);
        }
        else if (c == '+') {
          makePlus(start, end, recent_id);
        }
        else {
          stream_container->putback(c);
        }
      }
      recent_id = end;
    }
    // Newline - we should only encounter this if terminator == '\n'
    else if (c == '\n') {
      MANTA_FAIL("character \\n unexpected while getting sequence");
    }
    // General character. Could be the start of a char range.
    else {
      addChar(stream_container, c, recent_id);
    }
    // Get next character
    stream_container->get(c);
  } while (!stream_container->eof());
  // In case of eof.
  lexer_dfa_.AddTransition(recent_id, end_id);  // For completeness.
  return ends;
}

std::pair<int, int> LexerGenerator::getCharacterClass(utility::IStreamContainer& stream_container) {
  MANTA_REQUIRE(!stream_container->eof(), "cannot encounter EOF in the middle of a character class");
  char c = stream_container->peek();
  // Check if this is a normal character class, complement character class, or string
  // complement.
  if (c == '^') {
    // Complement character class.
    stream_container->get(c);  // Consume.
    return characterClass(stream_container, true);
  }
  if (c == '~') {
    // String complement.
    stream_container->get(c);  // Consume.
    return stringComplement(stream_container);
  }

  // Normal character class.
  return characterClass(stream_container);
}

std::pair<int, int> LexerGenerator::stringComplement(utility::IStreamContainer& stream_container) {
  MANTA_REQUIRE(!stream_container->eof(), "cannot encounter EOF in string complement");

  // Get characters until (unescaped) ]. Do not allow newlines.
  char c;
  stream_container->get(c);
  std::string str;
  while (!stream_container->eof()) {
    MANTA_ASSERT(c != '\n', "cannot have a newline in the middle of a string complement");
    if (c == '\\') {
      // Escaped character
      MANTA_ASSERT(!stream_container->eof(), "cannot encounter EOF in string complement");
      stream_container->get(c);
      str.push_back(escapedCharacter(c));
    }
    else if (c == ']') {
      // End of character class.
      break;
    }
    else if (c == ' ')
      ;  // Skip space.
    else {
      str.push_back(c);
    }
    stream_container->get(c);
  }

  // Construct the NFA for the complement of the string.
  int start_id  = lexer_dfa_.AddNode();
  int end_id    = lexer_dfa_.AddNode();
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

  return {start_id, end_id};
}

std::pair<int, int> LexerGenerator::characterClass(utility::IStreamContainer& stream_container,
                                                   bool make_complement) {
  MANTA_REQUIRE(!stream_container->eof(), "cannot be at EOF when reading character class");

  std::vector<std::pair<char, char>> ranges;
  std::vector<char> characters;

  char c {};
  stream_container->get(c);

  do {
    // End of the range.
    if (c == ']') {
      break;
    }
    else if (std::isspace(c))
      ;  // Bypass whitespace.
    // Other character.
    else {
      // Check if this a character range.
      char d = static_cast<char>(stream_container->peek());
      if (d == '-') {
        stream_container->get(d);  // Consume.

        stream_container->get(d);
        // If the character is ']', the '-' is just another character.
        if (d == ']') {
          characters.push_back(c);
          characters.push_back('-');

          break;
        }
        // Start of an escaped character.
        else if (d == '\\') {
          // Get escaped character.
          stream_container->get(d);
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

    stream_container->get(c);
  } while (!stream_container->eof());

  int start_id = lexer_dfa_.AddNode();
  int end_id   = lexer_dfa_.AddNode();

  if (make_complement) {
    MANTA_FAIL("complement range not implemented");
  }
  else {
    // Normal ranges and selections.
    // NOTE: This is not the absolute best thing to do, since you could have overlapping
    // ranges and chars within other ranges. However, it is probably pretty good, since
    // most people aren't going to write things like [a-z c-z] or [abcdef...z]
    for (auto& [start_range, end_range] : ranges) {
      lexer_dfa_.AddTransition(start_id, end_id, start_range, end_range);
    }
    for (auto ch : characters) {
      lexer_dfa_.AddTransition(start_id, end_id, ch);
    }
  }

  return {start_id, end_id};
}

void LexerGenerator::specialCharacters(utility::IStreamContainer& stream_container, char c, int& recent_id) {
  // Backslashes must be escaped.
  if (stream_container->eof()) {
    MANTA_FAIL("found single backslash ('\\') right before EOF, backslashes must be escaped");
  }

  // Any letter. Need to add two transitions.
  if (c == '@') {
    int id = lexer_dfa_.AddNode();
    lexer_dfa_.AddTransition(recent_id, id, 'a', 'z');
    lexer_dfa_.AddTransition(recent_id, id, 'A', 'Z');
    checkModifier(stream_container, recent_id, id, recent_id);
    recent_id = id;
  }
  // Any lower case letter.
  else if (c == 'a') {
    addNode(stream_container, 'a', 'z', recent_id);
  }
  // Any upper case letter.
  else if (c == 'A') {
    addNode(stream_container, 'A', 'Z', recent_id);
  }
  // Any number
  else if (c == 'd') {
    addNode(stream_container, '0', '9', recent_id);
  }
  // Other escape characters.
  else {
    char d = escapedCharacter(c);
    // Make the node.
    addChar(stream_container, d, recent_id);
  }
}

void LexerGenerator::makeStar(int idi, int idf, [[maybe_unused]] int& recent_id) {
  lexer_dfa_.AddTransition(idi, idf);
  lexer_dfa_.AddTransition(idf, idi);
}

void LexerGenerator::makePlus(int idi, int idf, [[maybe_unused]] int& recent_id) {
  lexer_dfa_.AddTransition(idf, idi);
}

void LexerGenerator::makeQues(int idi, int idf, [[maybe_unused]] int& recent_id) {
  lexer_dfa_.AddTransition(idi, idf);
}

// Add (if needed) a modifier to a sequence of nodes.
void LexerGenerator::checkModifier(utility::IStreamContainer& stream_container,
                                   int idi,
                                   int idf,
                                   int& recent_id) {
  char d        = stream_container->peek();
  bool get_next = true;
  if (d == '+') {
    makePlus(idi, idf, recent_id);
  }
  else if (d == '*') {
    makeStar(idi, idf, recent_id);
  }
  else if (d == '?') {
    makeQues(idi, idf, recent_id);
  }
  else {
    get_next = false;
  }
  // If necessary, get the next character.
  if (get_next) {
    stream_container->get(d);
  }
}

// Lambda function that adds another node in the line.
void LexerGenerator::addNode(utility::IStreamContainer& stream_container, char ci, char cf, int& recent_id) {
  int id = lexer_dfa_.AddNode();
  lexer_dfa_.AddTransition(recent_id, id, ci, cf);
  checkModifier(stream_container, recent_id, id, recent_id);
  // Adjust pointer.
  recent_id = id;
}

// Lambda function for adding a character node. Checks whether a */+/? comes after the
// char.
void LexerGenerator::addChar(utility::IStreamContainer& stream_container, char c, int& recent_id) {
  int id = lexer_dfa_.AddNode();
  // Add the normal character transition.
  lexer_dfa_.AddTransition(recent_id, id, c);
  checkModifier(stream_container, recent_id, id, recent_id);
  // Adjust pointer.
  recent_id = id;
}

inline bool LexerGenerator::isSkip(int lexeme_id) {
  return std::find(skip_lexemes_.begin(), skip_lexemes_.end(), lexeme_id) != skip_lexemes_.end();
}

char LexerGenerator::escapedCharacter(char c) {
  switch (c) {
    // "Normal" escaped characters.
    case '\\':
      return '\\';
    case 'n':
      return '\n';
    case 'r':
      return '\r';
    case 't':
      return '\t';
    // Atypical escaped characters.
    case 's':
      return ' ';
    case '0':
      // Represents EOF.
      return char(0);
    default:
      // Otherwise, the return the input character, it is not a special escaped char.
      return c;
  }
}

}  // namespace manta