//
// Created by Nathaniel Rupprecht on 3/30/23.
//

#pragma once

#include <manta/lexer/LexerDFA.hpp>
#include "manta/utility/ParserUtility.hpp"

namespace manta {


template<typename NodeBase_t, typename Parent_t>
class ParserDriverBase {
 protected:
  std::shared_ptr<NodeBase_t> parse();

  //! \brief A lexer.
  std::shared_ptr<LexerDFA> lexer_;

  //! \brief Which production start points to.
  int start_nonterminal_ = 0;

  int num_parse_steps_ = 0;

  //! \brief The parse table. It is a vector so we can add new states.
  //!
  //! The pair is [ action, state ].
  //! 0 - Error.
  //! 1 - Shift.
  //! 2 - Reduce.
  //! 3 - Accept.
  std::vector<std::vector<Entry>> parse_table_;

};


template<typename NodeBase_t, typename Parent_t>
std::shared_ptr<NodeBase_t> ParserDriverBase<NodeBase_t, Parent_t>::parse() {
  lexer_->SetRepeatEOF(true);

  /// For Debugging: Clear any old parse trace data.
//  parse_trace_.clear();

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
  working_stack_types.push_back(start_nonterminal_); // For debugging.

  bool accept = false;
  for (num_parse_steps_ = 1; !accept; ++num_parse_steps_) {
    // Get the current state.
    int state = working_stack.top().state;

    // Refill incoming_deque if necessary.
    if (incoming_deque.empty()) {
      // Check, in order of precedence (this is the order in the result), if any results result in
      // valid actions for the parser. Use this as the assumed token from the lexer.
      auto result = lexer_->LexNext();
      if (!result) {
        // Error lexing.
//        parse_trace_ += "ERROR: Could not lex another symbol.\n";
//        printFatalParseError(state);
        break;
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
        // This lexeme does not follow the previous lexemes.
//        parse_trace_ += "ERROR: No valid transitions could be found for input. Accepted lexeme(s) (for literal \"" + result->literal + "\") were: ";
//        for (auto& [lexeme_id, _] : result->accepted_lexemes) {
//          parse_trace_ += "[" + to_string(lexeme_id) + "] ";
//        }
//        parse_trace_ += "\n";
//        printFatalParseError(state);
        break;
      }

      // Record getting a new token.
      /// ===>
//      const auto& tok = incoming_deque.back();
//      auto literal = tok.literal;
//      if (literal == "\n") {
//        literal = "\\n";
//      }
//      parse_trace_ += "Getting token: [" + to_string(tok.type) + "], Literal: [" + literal + "]\n";
      /// <===
    }

    MANTA_ASSERT(!incoming_deque.empty(), "incoming deque cannot be empty");
    int incoming_symbol = incoming_deque.front().type;

    /// For Debugging: Record the step and state of the parser.
//    parse_trace_ += "Step: " + std::to_string(num_parse_steps_) + ", State: " + std::to_string(state)
//        + ", " + "lexer is at line " + std::to_string(lexer_->GetLine())
//        + ", column " + std::to_string(lexer_->GetColumn()) + "\n";

//    if (incoming_symbol < 0 || total_symbols_ <= incoming_symbol) {
//      std::cout << "ERROR - bad symbol: " << incoming_symbol << ", Literal: ["
//                << incoming_deque.front().literal << "]. Exiting.\n";
//      break;
//    }

    /// For Debugging: Print the state of the stack.
//    for (auto &ty: working_stack_types) {
//      parse_trace_ += "[" + to_string(ty) + "] ";
//    }
//    parse_trace_ += " <-->  ["; // Separate stack incoming deque.
//    parse_trace_ += to_string(incoming_deque.front().type) + "]\n";
    /// <=====

    // Get action from the parse table.
    Entry action = parse_table_[state][incoming_symbol];
    Token transfer = incoming_deque.front();

    // If shift
    if (action.IsShift()) {
      transfer.state = action.GetState(); // Set state
      incoming_deque.pop_front();    // Pop off the incoming stack...
      working_stack.push(transfer);  // and shift onto the working stack.

      // Shift ParseNode
      working_parse_deque.push_front(incoming_parse_deque.front());
      incoming_parse_deque.pop_front();

      // For debugging: Record a shift occurring.
      working_stack_types.push_back(transfer.type);
//      parse_trace_ += "Shift. State is now " + std::to_string(action.GetState()) + ".\n";
    }
    else if (action.IsReduce()) {
      int size = action.GetRule().size();
      int production = action.GetRule().production;

      // Put (newly reduced) production onto the input stack.
      incoming_deque.push_front(Token(production, ""));
      // Create a parse node.
//      auto production_node = std::make_shared<ParseNode>(inverse_production_map_.find(production)->second);

      // Take nodes that are to be reduced off the stack, and temporarily store them in the collect vector.
      collect.resize(size);
      for (int i = 0; i < size; ++i) {
        collect[size - i - 1] = working_parse_deque.front();
        working_parse_deque.pop_front();
        working_stack.pop();
        working_stack_types.pop_back(); // For debugging.
      }

      // REDUCTION. This is carried out by the child classes.

      // TODO: Get reduction ID.
      unsigned reduction_id = 0;

      auto production_node = static_cast<Parent_t>(this)->reduce(reduction_id, collect);

      // Clear collection vector.
      collect.clear();

      // Push node onto the incoming deque.
      incoming_parse_deque.push_front(production_node);

      // Record the reduction occurring.
//      parse_trace_ += "Reduce by " + std::to_string(size) + ". Reduce to a " + to_string(production)
//          + " via:\n\t" + entryToString(action) + "\n";
    }
    else if (action.IsAccept()) {
      // Set start node to be the parsed program.
      start_node = incoming_parse_deque.front();
      incoming_parse_deque.pop_front();
      // Set 'accept' to true.
      accept = true;

      // Write the acceptance to the parse trace.
//      parse_trace_ += "Accept!\n";
    }
    else if (action.IsError()) {
//      printFatalParseError(state);
      break;
    }
    // Put a newline into the parser trace.
//    parse_trace_ += "\n";
  }

  // If the parser accepted, return the AST node
  if (accept) {
    return start_node;
  }
  else {
    return nullptr;
  }
}

} // namespace manta
