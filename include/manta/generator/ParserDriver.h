//
// Created by Nathaniel Rupprecht on 3/30/23.
//

#pragma once

#include <Lightning/Lightning.h>
#include <manta/lexer/LexerDFA.hpp>

#include "manta/utility/Formatting.h"
#include "manta/utility/IStreamContainer.hpp"
#include "manta/utility/ParserUtility.hpp"

namespace manta {

template<typename NodeBase_t, typename LexemeNode_t, typename Child_t>
class ParserDriverBase {
public:
  //! \brief Set the lexer for the parser.
  void SetLexer(std::shared_ptr<LexerDFA> lexer) { lexer_ = std::move(lexer); }

  void SetInput(utility::IStreamContainer container) { lexer_->SetContainer(container); }

  void SetLogger(lightning::Logger logger) { logger_ = logger; }

protected:
  //! \brief The standard parsing loop. This calls back to the child class's reduce functions to do the
  //!        reductions, but is otherwise a standard loop.
  std::shared_ptr<NodeBase_t> parse();

  //! \brief Convert an ID to a string. The ID may either be a lexeme, or terminal.
  std::string idToString(int id) const {
    using namespace std::string_view_literals;

    if (id < 0) {
      return "ERROR";
    }
    if (id < lexer_->GetNumLexemes()) {
      auto&& name = lexer_->LexemeName(id);
      if (std::string_view(name).substr(0, 4) == "RES:"sv) {
        return name.substr(4);
      }
      return "@" + name;
    }
    return inverse_nonterminal_map_.at(id);
  }

  NO_DISCARD bool isLexeme(int id) const { return id < lexer_->GetNumLexemes(); }

  void printFatalParseError(int state) const {
    using namespace lightning;
    // Record error in parse trace.
    if (auto handle = logger_.Log(Severity::Error)) {
      handle << "Lexer is at Line " << lexer_->GetLine() << ", Column " << lexer_->GetColumn() << ".";
      // Print out what valid options would have been recognized.
      int print_count = 0;
      for (const auto& entry : parse_table_[state]) {
        if (!entry.IsError()) {
          handle << NewLineIndent << "  * Valid: [" << idToString(print_count) + "], Result: <"
                 << entry.Write(0) + ">";
        }
        ++print_count;
      }
    }
  }

  void printNoValidTransitionsError(int state, const LexerResult& result) const {
    using namespace manta::formatting;
    using namespace lightning;

    if (auto handle = logger_.Log(Severity::Error)) {
      handle << "No valid transitions could be found for the input." << NewLineIndent
             << "Accepted lexeme(s) (for literal " << CLBG(escape(result.literal)) << ") were: ";
      for (const auto& [lexeme_id, _] : result.accepted_lexemes) {
        handle << NewLineIndent << CLBB(idToString(lexeme_id));
      }
      handle << NewLineIndent << "Token source location was " << result.source_position << ".";
    }
    printFatalParseError(state);
  }

  std::string entryToString(const Entry& entry) const {
    std::string output;
    if (entry.IsReduce()) {
      auto&& rule = entry.GetRule();
      output += inverse_nonterminal_map_.at(rule.produced_nonterminal) + " ->";
      for (const auto& r : rule.rhs) {
        output += " " + idToString(r);
      }
    }
    return output;
  }

  std::string escape(const std::string& input) const {
    std::string output;
    output.reserve(input.size());
    std::for_each(input.begin(), input.end(), [&](char c) {
      if (c == '\n') {
        output += "\\n";
      }
      else if (c == '\t') {
        output += "\\t";
      }
      else {
        output += c;
      }
    });
    return output;
  }

  void debugLogAction(const LexerResult& result) const {
    using namespace manta::formatting;
    using namespace lightning;

    if (auto handler = logger_.Log(Severity::Debug)) {
      handler << "Lexed the literal \"" << CLBG(escape(result.literal)) << "\", location "
              << result.source_position << ". Matched " << result.accepted_lexemes.size() << " lexeme(s):";
      int i = 0;
      for (auto& [id, _] : result.accepted_lexemes) {
        if (i != 0) {
          handler << ",";
        }
        handler << " " << (i % 2 == 0 ? CLB(lexer_->LexemeName(id)) : CLM(lexer_->LexemeName(id)))
                << " (id = " << id << ")";
        ++i;
      }
    }
  }

  void debugLogState(int state, const auto& working_stack_types, const auto& incoming_deque) const {
    using namespace manta::formatting;
    using namespace lightning;

    LOG_SEV_TO(logger_, Debug) << "Step " << num_parse_steps_ << ", State: " << state << ", lexer is at line "
                               << lexer_->GetLine() << ", column " << lexer_->GetColumn();

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
  }

  //! \brief A lexer.
  std::shared_ptr<LexerDFA> lexer_;

  //! \brief Which production start points to.
  int start_nonterminal_ = 0;

  //! \brief Track the number of parsing steps.
  int num_parse_steps_ = 0;

  //! \brief The parse table. It is a vector so we can add new states.
  //!
  //! The pair is [ action, state ].
  //! 0 - Error.
  //! 1 - Shift.
  //! 2 - Reduce.
  //! 3 - Accept.
  std::vector<std::vector<Entry>> parse_table_;

  //! \brief Maps production numbers to production names.
  std::map<int, std::string> inverse_nonterminal_map_;

  //! \brief Logger to monitor the parsing.
  lightning::Logger logger_;
};

template<typename NodeBase_t, typename LexemeNode_t, typename Child_t>
std::shared_ptr<NodeBase_t> ParserDriverBase<NodeBase_t, LexemeNode_t, Child_t>::parse() {
  using namespace manta::formatting;
  using namespace lightning;

  LOG_SEV_TO(logger_, Info) << "Beginning parse.";

  MANTA_REQUIRE(lexer_, "no lexer set in the parser, cannot continue");
  MANTA_REQUIRE(lexer_->IsGood(), "no lexer set in the parser, cannot continue");
  lexer_->SetRepeatEOF(true);

  // Stack of symbols that the parser has read.
  std::stack<Token> working_stack;
  // Symbols being fed into the parser.
  std::deque<Token> incoming_deque;

  // Building the AST.
  std::deque<std::shared_ptr<NodeBase_t>> incoming_parse_deque;
  std::deque<std::shared_ptr<NodeBase_t>> working_parse_deque;

  /// For Debugging.
  std::list<int> working_stack_types;

  // A vector for collecting nodes during a reduction.
  std::vector<std::shared_ptr<NodeBase_t>> collect;
  std::shared_ptr<NodeBase_t> start_node = nullptr;

  // Push starting state onto the stack.
  working_stack.emplace(start_nonterminal_, 0);
  working_parse_deque.push_front(start_node);
  working_stack_types.push_back(start_nonterminal_);  // For debugging.

  bool accept = false;
  for (num_parse_steps_ = 1; !accept; ++num_parse_steps_) {
    // Get the current state.
    int state = working_stack.top().state;

    LOG_SEV_TO(logger_, Debug) << "Starting parser step " << num_parse_steps_ << ", state is " << state
                               << ".";

    // Refill incoming_deque if necessary.
    if (incoming_deque.empty()) {
      // Check, in order of precedence (this is the order in the result), if any results
      // result in valid actions for the parser. Use this as the assumed token from the
      // lexer.
      auto result = lexer_->LexNext();
      if (!result) {
        LOG_SEV_TO(logger_, Error) << "Could not lex another symbol.";
        printFatalParseError(state);
        break;
      }
      debugLogAction(*result);

      const bool any_valid = [&] {
        for (const auto& [lexeme_id, _] : result->accepted_lexemes) {
          if (!parse_table_.at(state).at(lexeme_id).IsError()) {
            incoming_deque.emplace_back(lexeme_id, result->literal);
            incoming_parse_deque.push_back(std::make_shared<LexemeNode_t>(result->literal));
            return true;
          }
        }
        return false;
      }();

      // Check if no valid options could be found.
      if (!any_valid) {
        printNoValidTransitionsError(state, *result);
        break;
      }
    }

    MANTA_ASSERT(!incoming_deque.empty(), "incoming deque cannot be empty");
    // For Debugging: Record the step and state of the parser.
    debugLogState(state, working_stack_types, incoming_deque);

    // Get action from the parse table.
    const int incoming_symbol = incoming_deque.front().type;
    const Entry& action       = parse_table_.at(state).at(incoming_symbol);
    Token transfer            = incoming_deque.front();  // Copy.

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

      // Put (newly reduced) production Token onto the input stack.
      incoming_deque.emplace_front(production, "");

      // Take nodes that are to be reduced off the stack, and temporarily store them in
      // the collect vector.
      collect.resize(size);
      for (int i = 0; i < size; ++i) {
        collect[size - i - 1] = working_parse_deque.front();
        working_parse_deque.pop_front();
        working_stack.pop();
        working_stack_types.pop_back();  // For debugging.
      }

      // REDUCTION. This is carried out by the child classes.

      const auto reduction_id = action.GetRule().item_number;
      MANTA_ASSERT(reduction_id,
                   "reduction did not have its item number set, table entry was (state="
                       << state << ", symbol=" << incoming_symbol << ")");
      LOG_SEV_TO(logger_, Debug) << "Reducing " << collect.size() << " collected nodes using item "
                                 << *reduction_id << ".";

      // CRTP trick - cast ourselves to the child type that we know we are, and call our reduce function.
      // This creates the node via the specific, parser defined reduction function.
      auto production_node = static_cast<Child_t*>(this)->reduce(*reduction_id, collect);

      // Clear collection vector.
      collect.clear();

      // Push node onto the incoming deque.
      incoming_parse_deque.push_front(production_node);

      // Record the reduction occurring.
      LOG_SEV_TO(logger_, Debug) << "REDUCE by " << size << ". Reduce to a " << production
                                 << " via:" << NewLineIndent << entryToString(action) << ".";
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
  LOG_SEV_TO(logger_, Debug) << "Ended NON accepting. Returning empty.";
  return nullptr;
}

}  // namespace manta
