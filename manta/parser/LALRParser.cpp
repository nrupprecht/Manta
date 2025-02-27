#include "manta/parser/LALRParser.h"
// Other files
#include <string_view>

#include "manta/utility/Formatting.h"

namespace manta {

using namespace manta::formatting;
using namespace lightning;

namespace {

std::string escape(const std::string& input) {
  std::string output;
  output.reserve(input.size());
  std::for_each(input.begin(), input.end(), [&](char c) {
    if (c == '\n')
      output += "\\n";
    else if (c == '\t')
      output += "\\t";
    else
      output += c;
  });
  return output;
}

}  // namespace

std::shared_ptr<ParseNode> LALRParser::ParseString(const std::string& input) {
  lexer_->SetStringToLex(input);
  return parse();
}

std::shared_ptr<ParseNode> LALRParser::ParseCodeFile(const std::string& file_name) {
  if (!lexer_->SetFileToLex(file_name)) {
    LOG_SEV_TO(logger_, Error) << "File '" << file_name << "' does not exist.";
    return nullptr;
  }
  return parse();
}

std::shared_ptr<ParseNode> LALRParser::parse() {
  lexer_->SetRepeatEOF(true);

  // Stack of symbols that the parser has read.
  std::stack<Token> working_stack;
  // Symbols being fed into the parser.
  std::deque<Token> incoming_deque;

  // Building the AST.
  std::deque<std::shared_ptr<ParseNode>> incoming_parse_deque;
  std::deque<std::shared_ptr<ParseNode>> working_parse_deque;

  /// For Debugging.
  std::list<int> working_stack_types;

  // A vector for collecting nodes during a reduction.
  std::vector<std::shared_ptr<ParseNode>> collect;
  std::shared_ptr<ParseNode> start_node = nullptr;

  // Push starting state onto the stack.
  working_stack.emplace(start_production_, 0);
  working_parse_deque.push_front(start_node);
  working_stack_types.push_back(start_production_);  // For debugging.

  bool accept = false;
  for (num_parse_steps_ = 1; !accept; ++num_parse_steps_) {
    // Get the current state.
    int state = working_stack.top().state;

    // Refill incoming_deque if necessary.
    if (incoming_deque.empty()) {
      // Check, in order of precedence (this is the order in the result), if any results
      // result in valid actions for the parser. Use this as the assumed token from the
      // lexer.
      auto result = lexer_->LexNext();
      if (!result) {
        // Error lexing.
        LOG_SEV_TO(logger_, Error) << "Could not lex another symbol.";
        printFatalParseError(state);
        break;
      }

      if (auto handler = logger_.Log(Severity::Debug)) {
        handler << "Lexed the literal \"" << CLBG(escape(result->literal)) << "\". Matched "
                << result->accepted_lexemes.size() << " lexeme(s):";
        int i = 0;
        for (auto& [id, _] : result->accepted_lexemes) {
          if (i != 0) {
            handler << ",";
          }
          handler << " " << (i % 2 == 0 ? CLB(lexer_->LexemeName(id)) : CLM(lexer_->LexemeName(id)))
                  << " (id = " << id << ")";
          ++i;
        }
      }

      bool any_valid = false;
      for (auto& [lexeme_id, _] : result->accepted_lexemes) {
        if (!parse_table_.at(state).at(lexeme_id).IsError()) {
          incoming_deque.emplace_back(lexeme_id, result->literal);
          incoming_parse_deque.push_back(std::make_shared<ParseNode>(result->literal));

          any_valid = true;
          break;
        }
      }

      // Check if no valid options could be found.
      if (!any_valid) {
        if (auto handle = logger_.Log(lightning::Severity::Error)) {
          handle << "No valid transitions could be found for the input." << lightning::NewLineIndent
                 << "Accepted lexeme(s) (for literal " << CLBG(result->literal) << ") were: ";
          for (auto& [lexeme_id, _] : result->accepted_lexemes) {
            handle << NewLineIndent << CLBB(idToString(lexeme_id));
          }
        }
        printFatalParseError(state);
        break;
      }
    }

    MANTA_ASSERT(!incoming_deque.empty(), "incoming deque cannot be empty");
    int incoming_symbol = incoming_deque.front().type;

    // For Debugging: Record the step and state of the parser.
    // For Debugging: Record the step and state of the parser.
    LOG_SEV_TO(logger_, Debug) << "Step " << num_parse_steps_ << ", State: " << state << ", lexer is at line "
                               << lexer_->GetLine() << ", column " << lexer_->GetColumn();

    if (incoming_symbol < 0 || static_cast<int>(total_symbols_) <= incoming_symbol) {
      LOG_SEV_TO(logger_, Error) << "Bad symbol: " << incoming_symbol << ", Literal: ["
                                 << incoming_deque.front().literal << "]. Exiting.";
      break;
    }

    // For Debugging: Print the state of the stack.
    if (auto handler = logger_.Log(Severity::Debug)) {
      handler << "Current stack:" << NewLineIndent;
      for (auto& ty : working_stack_types) {
        handler << "[" << (isLexeme(ty) ? CLM(idToString(ty)) : CLB(idToString(ty))) << "] ";
      }
      // Separate stack incoming deque.
      handler << " <-->  ";
      // Incoming deque.
      for (auto& d : incoming_deque) {
        handler << "[" << (isLexeme(d.type) ? CLM(idToString(d.type)) : CLB(idToString(d.type))) << "] ";
      }
    }

    // Get action from the parse table.
    Entry action   = parse_table_[state][incoming_symbol];
    Token transfer = incoming_deque.front();

    // If shift
    if (action.IsShift()) {
      transfer.state = action.GetState();  // Set state
      incoming_deque.pop_front();          // Pop off the incoming stack...
      working_stack.push(transfer);        // and shift onto the working stack.

      // Shift ParseNode
      working_parse_deque.push_front(incoming_parse_deque.front());
      incoming_parse_deque.pop_front();

      // For debugging: Record a shift occurring.
      working_stack_types.push_back(transfer.type);
      LOG_SEV_TO(logger_, Debug) << "SHIFT. State is now " << action.GetState() << ".";
    }
    else if (action.IsReduce()) {
      int size       = action.GetRule().Size();
      int production = action.GetRule().produced_nonterminal;

      // Put (newly reduced) production onto the input stack.
      incoming_deque.push_front(Token(production, ""));
      // Create a parse node.
      auto production_node = std::make_shared<ParseNode>(inverse_production_map_.find(production)->second);

      // Take nodes that are to be reduced off the stack, and temporarily store them in
      // the collect vector.
      collect.resize(size);
      for (int i = 0; i < size; ++i) {
        collect[size - i - 1] = working_parse_deque.front();
        working_parse_deque.pop_front();
        working_stack.pop();
        working_stack_types.pop_back();  // For debugging.
      }

      LOG_SEV_TO(logger_, Debug) << "Reducing " << collect.size()
                                 << " collected nodes, checking for instructions.";

      // Carry out reduction instructions.
      if (auto instructions = action.GetAnnotatedRule().instructions) {
        for (const auto& instruction : instructions->children) {
          // Get the designator.
          std::string functionName = instruction->designator;

          // Rename the new node.
          if (functionName == "node") {
            MANTA_ASSERT(!instruction->children.empty(),
                         "instruction must not have no children");
            instructionNode(production_node, instruction->children[0]->designator);
            LOG_SEV_TO(logger_, Debug) << "Executing NODE instruction.";
          }
            // Add a child to the node.
          else if (functionName == "add") {
            MANTA_ASSERT(!instruction->children.empty(),
                         "instruction must not have no children");
            int index = manta::stoi(instruction->children[0]->designator);
            if (0 <= index && index < static_cast<int>(collect.size())) {
              instructionAdd(production_node, collect[index]);
            }
            LOG_SEV_TO(logger_, Debug) << "Executing ADD instruction.";
          }
            // Add all the children of the specified token to the node.
          else if (functionName == "adopt") {
            MANTA_ASSERT(!instruction->children.empty(),
                         "instruction must not have no children");
            int index = manta::stoi(instruction->children[0]->designator);
            if (0 <= index && index < static_cast<int>(collect.size())) {
              instructionAdopt(production_node, collect[index]);
            }
            LOG_SEV_TO(logger_, Debug) << "Executing ADOPT instruction.";
          }
            // Replace the new node with one of the tokens.
          else if (functionName == "replace") {
            MANTA_ASSERT(!instruction->children.empty(),
                         "instruction must not have no children");
            int index = manta::stoi(instruction->children[0]->designator);
            if (0 <= index && index < static_cast<int>(collect.size())) {
              instructionReplace(production_node, collect[index]);
              collect[index] = nullptr;
            }
            LOG_SEV_TO(logger_, Debug) << "Executing REPLACE instruction.";
          }
            // Add a named node as a child to this node, adding the children of a node to
            // the new node
          else if (functionName == "push") {
            MANTA_ASSERT(1 < instruction->children.size(),
                         "instruction must have at least two children");
            int index = manta::stoi(instruction->children[1]->designator);
            if (0 <= index && index < static_cast<int>(collect.size())) {
              instructionPush(
                  production_node, instruction->children[0]->designator, collect[index]);
            }
            LOG_SEV_TO(logger_, Debug) << "Executing PUSH instruction.";
          }
        }
      }
      else {
        for (auto& node : collect) {
          production_node->Add(node);
        }
      }
      // Clear collection vector.
      collect.clear();

      // Push node onto the incoming deque.
      incoming_parse_deque.push_front(production_node);

      // Record the reduction occurring.
      LOG_SEV_TO(logger_, Debug) << "REDUCE " << size << " collected node. Reduce to a '"
                                 << inverse_production_map_.at(production) << "' (non-terminal " << production
                                 << ") via item " << action.GetAnnotatedRule().production_item_number << ":"
                                 << NewLineIndent << entryToString(action) << ".";
    }
    else if (action.IsAccept()) {
      // Set start node to be the parsed program.
      start_node = incoming_parse_deque.front();
      incoming_parse_deque.pop_front();
      // Set 'accept' to true.
      accept = true;

      // Write the acceptance to the parse trace.
      LOG_SEV_TO(logger_, Debug) << "ACCEPT!";
    }
    else if (action.IsError()) {
      printFatalParseError(state);
      break;
    }
  }

  // If the parser accepted, return the AST node
  if (accept) {
    LOG_SEV_TO(logger_, Debug) << "Ended in acceptance, returning head node.";
    return start_node;
  }
  else {
    LOG_SEV_TO(logger_, Debug) << "Ended NON accepting. Returning empty.";
    return nullptr;
  }
}

//! \brief Pretty print the transition table.
std::string LALRParser::PrintTable() const {
  std::string str;

  // Print out definitions.
  str += repeat('_', (total_symbols_ + 1) * 5) + "__";
  str += '\n';
  str += "Token and production definitions:\n";
  str += repeat('-', (total_symbols_ + 1) * 5) + "--";
  str += '\n';
  str += "   -- (Lexemes) -- \n";
  std::size_t i = 0;
  for (; i < lexer_->GetNumLexemes(); ++i) {
    str += buffered(i, 4) + ": " + lexer_->LexemeName(i) + "\n";
  }
  str += "   -- (Productions) -- \n";
  for (; i < total_symbols_; ++i) {
    str += buffered(i, 4) + ": " + inverse_production_map_.find(i)->second + "\n";
  }

  // Print table header.
  str += repeat('_', 5 * (total_symbols_ + 2)) + "__";
  str += '\n';

  // Store the (space) buffered names. We need to find how long the longest name is,
  // so we know how many spaces we need to buffer with.
  std::vector<std::string> buffered_names;

  std::size_t max_size = 0;
  for (std::size_t j = 0; j < lexer_->GetNumLexemes(); ++j) {
    auto name = lexer_->LexemeName(static_cast<int>(j));
    if (name.substr(0, 4) == "RES:") {
      max_size = std::max(max_size, name.size() - 4);
    }
    else {
      max_size = std::max(max_size, name.size());
    }
  }
  for (auto& [id, name] : inverse_production_map_) {
    max_size = std::max(max_size, name.size());
  }

  for (std::size_t j = 0; j < lexer_->GetNumLexemes(); ++j) {
    auto name = lexer_->LexemeName(j);

    if (name.substr(0, 4) == "RES:") {
      std::copy(name.begin() + 4, name.end(), name.begin());
      name.resize(name.size() - 4);
    }
    buffered_names.push_back(repeat(' ', static_cast<int>(max_size - name.size())) + name);
  }
  for (auto& [id, name] : inverse_production_map_) {
    buffered_names.push_back(repeat(' ', static_cast<int>(max_size - name.size())) + name);
  }

  for (std::size_t j = 0; j < max_size; ++j) {
    str += "     |";
    for (std::size_t k = 0; k < total_symbols_; ++k) {
      str += "    " + std::string {buffered_names[k][j]};
      if (k == lexer_->GetNumLexemes() - 1) {
        str += "  |";
      }
    }
    str += "  |\n";
  }

  str += repeat('-', 5 * (total_symbols_ + 2)) + "--";
  str += '\n';
  // Print transition table.
  for (std::size_t s = 0; s < all_states_.size(); ++s) {
    str += buffered(s, 4) + " | ";
    for (std::size_t j = 0; j < total_symbols_; ++j) {
      str += parse_table_[s][j].Write(4) + " ";

      if (j == lexer_->GetNumLexemes() - 1) {
        str += " | ";
      }
    }
    str += " |\n";
  }
  str += repeat('-', 5 * (total_symbols_ + 2)) + "--";
  str += '\n';

  // Return the table string.
  return str;
}

std::string LALRParser::PrintAsMathematica(const std::shared_ptr<ParseNode>& head) {
  std::vector<std::pair<int, int>> connectivity;
  std::vector<std::pair<int, std::string>> labels;

  std::deque<std::pair<std::shared_ptr<ParseNode>, int>> stack;
  stack.emplace_back(head, 0);
  labels.emplace_back(0, head->designator);

  int nodeNumber = 1;  // Head was 0.

  while (!stack.empty()) {
    auto& pr   = stack.front();
    auto& node = pr.first;
    int number = pr.second;
    for (const auto& child : node->children) {
      connectivity.emplace_back(number, nodeNumber);
      labels.emplace_back(nodeNumber, child->designator);
      stack.emplace_back(child, nodeNumber);
      ++nodeNumber;
    }
    stack.pop_front();
  }

  // --- Form the mathematica command that will print this as a tree.

  auto clean = [](const std::string& str) {
    std::string out;
    for (char c : str) {
      if (c == '\t')
        out += "\\t";
      else if (c == '\n')
        out += "\\n";
      else if (c == '\r')
        out += "\\r";
      else
        out += c;
    }
    return out;
  };

  // First, form connectivity.
  std::string mathematicaCommand = "TreePlot[{";
  for (std::size_t i = 0; i < connectivity.size(); ++i) {
    auto [first, second] = connectivity[i];
    mathematicaCommand += std::to_string(first) + "->" + std::to_string(second) + "";
    if (i != connectivity.size() - 1) {
      mathematicaCommand += ",";
    }
  }
  mathematicaCommand += "},Top,0,VertexLabels->{";
  // Then form vertex labels.
  for (std::size_t i = 0; i < labels.size(); ++i) {
    auto [first, name] = labels[i];
    mathematicaCommand += std::to_string(first) + "->\"" + clean(name) + "\"";
    if (i != labels.size() - 1) {
      mathematicaCommand += ",";
    }
  }
  mathematicaCommand += "},ImageSize->Large]";

  return mathematicaCommand;
}

std::size_t LALRParser::NumParseSteps() const {
  return num_parse_steps_;
}

std::shared_ptr<LexerDFA> LALRParser::GetLexer() const {
  return lexer_;
}

void LALRParser::SetLogger(const Logger& logger) {
  logger_ = logger;
}

// ================================================
//  Action functions
// ================================================

void LALRParser::instructionNode(Node& self, const std::string& name) {
  self->designator = name;
}

void LALRParser::instructionAdd(Node& self, Node& node) {
  self->Add(node);
  node = nullptr;
}

void LALRParser::instructionAdopt(Node& self, Node& node) {
  self->children.insert(self->children.end(), node->children.begin(), node->children.end());
}

void LALRParser::instructionReplace(Node& self, Node& node) {
  self = node;
}

void LALRParser::instructionPush(Node& self, const std::string& name, Node& node) {
  // Create a new node.
  auto new_node = std::make_shared<ParseNode>(name);
  new_node->Add(node);
  self->children.push_back(new_node);
}

std::string LALRParser::entryToString(const Entry& entry) {
  std::string output;
  if (entry.IsReduce()) {
    auto rule = entry.GetRule();
    output += inverse_production_map_.at(rule.produced_nonterminal) + " ->";
    for (const auto& r : rule.rhs) {
      output += " " + idToString(r);

      //                // If r corresponds to a production symbol, print that instead.
      //                if (std::find_if(inverse_nonterminal_map_.begin(),
      //                inverse_nonterminal_map_.end(),
      //                                 [=] (auto pr) { return pr.first == r; })  !=
      //                                 inverse_nonterminal_map_.end()) {
      //                    output += " " + inverse_nonterminal_map_.at(r);
      //                }
      //                // Otherwise, this corresponds to a lexeme.
      //                else {
      //                    output += " @" + lexer_generator->lexemeName(r);
      //                }
    }
  }
  return output;
}

std::string LALRParser::idToString(int id) const {
  if (id < 0) {
    return "ERROR";
  }
  else if (id < static_cast<int>(lexer_->GetNumLexemes())) {
    auto name = lexer_->LexemeName(id);
    if (name[0] == 'R' && name[1] == 'E' && name[2] == 'S' && name[3] == ':') {
      return std::string(name.begin() + 4, name.end());
    }
    return "@" + name;
  }
  return inverse_production_map_.at(id);
}

bool LALRParser::isLexeme(int id) const {
  return id < static_cast<int>(lexer_->GetNumLexemes());
}

void LALRParser::printFatalParseError(int state) {
  using namespace lightning;
  // Record error in parse trace.
  if (auto handle = logger_.Log(Severity::Error)) {
    handle << "Lexer is at Line " << lexer_->GetLine() << ", Column " << lexer_->GetColumn() << ".";
    // Print out what valid options would have been recognized.
    int print_count = 0;
    for (auto& entry : parse_table_[state]) {
      if (!entry.IsError()) {
        handle << NewLineIndent << "  * Valid: [" << idToString(print_count) + "], Result: <"
               << entry.Write(0) + ">";
      }
      ++print_count;
    }
  }
}

void CompareParsers(const LALRParser& left, const LALRParser& right) {
  for (auto i = 0u; i < left.parse_table_.size(); ++i) {
    auto& left_row  = left.parse_table_[i];
    auto& right_row = right.parse_table_[i];
    for (auto j = 0u; j < left_row.size(); ++j) {
      if (left_row[j] != right_row[j]) {
        std::cout << "Mismatch for state " << i << ", symbol " << j << ".\n";
        std::cout << "State in left parser is:  " << left_row[j].Write(4)
                  << "\n";  // idToString(left_row[j].GetAction()) << "\n";
        std::cout << "State in right parser is: " << right_row[j].Write(4)
                  << "\n";  // idToString(right_row[j].GetAction()) << "\n";
        std::cout << std::endl;
      }
    }
  }
}

}  // namespace manta