#include "../include/DeterministicFiniteAutomata.hpp"

using namespace manta;

bool FiniteAutomatonNode::WillAccept(const char c) const {
  return std::any_of(transitions.begin(), transitions.end(),
                     [=](const TransitionType &t) { return t.Accept(c); });
}

void FiniteAutomatonNode::ToStream(std::ostream &out) const {
  out << "Transitions: ";
  // Are there any transitions?
  if (transitions.empty()) {
    out << "None.\n";
  } else {
    out << "\n";
  }
  // Print all transitions.
  for (const auto tr : transitions) {
    out << " -> " << tr.transition_state << ":";
    if (tr.IsNullTransition()) {
      out << "lambda\n";
    } else {
      out << " [" << tr.range_initial << ", " << tr.range_final << "]\n";
    }
  }
  if (0 <= accepting_state) {
    out << "Accepting: " << accepting_state << "\n";
  }
}

void FiniteAutomaton::SetStream(std::istream &stream) {
  instream_ = stream;
}

void FiniteAutomaton::SetStream(IStreamContainer &stream) {
  instream_ = stream;
}

//! \brief Get the next token.
Token FiniteAutomaton::GetToken() {
  // If repeat_eof_ is true, we repeatedly return EOF when we hit the eof.
  if (!repeat_eof_) {
    if (status_flag_ == FAStatus::AcceptedEOF) {
      return Token(-1, "");
    }
    if (!instream_.IsGood() || status_flag_ != FAStatus::Valid) {
      status_flag_ = FAStatus::BadStream;
      return Token(-1, "");
    }
  }

  // If we have reached EOF, check if we accept EOF. In so, set the status to reflect this.
  // If not, we cannot get any more tokens.
  if (instream_->eof()) {
    if (tryAccept('\0')) {
      unsigned final_state = state_pointer_;
      state_pointer_ = 0; // Reset state pointer.
      status_flag_ = FAStatus::AcceptedEOF; // We have reached and consumed EOF.
      return Token(dfa_nodes_[final_state].accepting_state, "\\0");
    } else {
      status_flag_ = FAStatus::EndedNonAccepting;
      return Token();
    }
  }

  // Store the literal string
  std::string literal;
  char c;
  auto get_char = [&]() {
    if (instream_->eof()) {
      c = char(0);
    } else {
      instream_->get(c);
    }
  }; // Lambda function returns char(0) for eof.
  auto put_char = [&]() {
    if (instream_->eof());
    else {
      instream_->putback(c);
    }
  };
  // Get characters as long as we can.
  get_char();
  while (!instream_->eof() && tryAccept(c)) {
    literal.push_back(c);
    // Advance counters.
    if (c == '\n') {
      ++line_;
      character_ = 1;
    } else {
      ++character_;
    }
    get_char();
  }
  // The last character, that caused the FiniteAutomaton to fail, will be the first character
  // in the next lexeme.
  put_char();

  // Store what state the FiniteAutomaton ended in, then reset the state.
  unsigned final_state = state_pointer_;
  state_pointer_ = 0; // Reset state pointer.

  // Check if we able to extract any token and ended in an accepting state.
  if (!literal.empty() && 0 <= dfa_nodes_[final_state].accepting_state) {
    status_flag_ = FAStatus::Valid;
    return Token(dfa_nodes_[final_state].accepting_state, literal);
  } else { // An error has occurred.
    status_flag_ = FAStatus::EndedNonAccepting;
    return Token(-1, literal);
  }
}

FiniteAutomaton FiniteAutomaton::NFAToDFA() {
  // States are sets of nfa states.
  std::vector<std::set<int>> dfa_states;
  dfa_states.push_back(std::set{0}); // Starting state just contains 0.

  // New dfa.
  auto dfa = FiniteAutomaton();

  // New states to process - stores the (DFA) state number and the set of NDFA states that makes up this state.
  std::deque<std::pair<int, std::set<int>>> working_stack;
  // Start with the initial state on the stack.
  working_stack.emplace_back(0, dfa_states[0]);
  dfa.AddNode();
  // If the node is accepting, or can make a lambda (null) transition to an accepting state,
  // this state (the initial state) is accepting.
  auto[state, precedence] = acceptingStateLambda(0); // Node zero can also be accepting.
  dfa.dfa_nodes_[0].accepting_state = state;
  dfa.dfa_nodes_[0].precedence = precedence;

  // Process the stack until it is empty.
  while (!working_stack.empty()) {
    auto&[state_id, state_set] = working_stack.front();
    // Process the next entry in the deque.
    compute_goto(state_set, state_id, working_stack, dfa_states, dfa);
    // Pop the entry.
    working_stack.pop_front();
  }

  return dfa;
}

int FiniteAutomaton::size() const {
  return dfa_nodes_.size();
}

int FiniteAutomaton::AddNode(const FiniteAutomatonNode &node) {
  dfa_nodes_.push_back(node);
  return static_cast<int>(dfa_nodes_.size()) - 1;
}

int FiniteAutomaton::AddNode() {
  dfa_nodes_.emplace_back();
  return static_cast<int>(dfa_nodes_.size()) - 1;
}

void FiniteAutomaton::AddTransition(int index, TransitionType type) {
  dfa_nodes_[index].transitions.push_back(type);
}

void FiniteAutomaton::AddTransition(int source, int dest, char ci, char cf) {
  AddTransition(source, TransitionType(dest, std::min(ci, cf), std::max(ci, cf)));
}

void FiniteAutomaton::AddTransition(int source, int dest, char c) {
  AddTransition(source, TransitionType(dest, c, c));
}

void FiniteAutomaton::AddTransition(int source, int dest) {
  AddTransition(source, TransitionType(dest));
}

void FiniteAutomaton::SetAccepting(int index, int value) {
  dfa_nodes_[index].accepting_state = value;
}

void FiniteAutomaton::SetPrecedence(int index, int precedence) {
  dfa_nodes_[index].precedence = precedence;
}

void FiniteAutomaton::ToStream(std::ostream &out) const {
  for (int i = 0; i < dfa_nodes_.size(); ++i) {
    out << "Node " << i << ", ";
    dfa_nodes_[i].ToStream(out);
    out << "\n";
  }
}

int FiniteAutomaton::Accepts(const string &word) const {
  if (dfa_nodes_.empty()) {
    return -1;
  }

  unsigned state = 0;
  for (char c : word) {
    bool did_transition = false;
    for (auto transition : dfa_nodes_[state].transitions) {
      if (transition.Accept(c)) {
        state = transition.transition_state;
        did_transition = true;
        break;
      }

    }
    // If we get here, it means the character did not correspond to any transition.
    if (!did_transition) {
      return -1;
    }
  }
  // Return the accepting state of the final node.
  return dfa_nodes_[state].accepting_state;
}

int FiniteAutomaton::accepts_empty() const {
  if (dfa_nodes_.empty()) {
    return -2;
  }
  return dfa_nodes_[0].accepting_state;
}

bool FiniteAutomaton::any_remaining() const {
  return status_flag_ == FAStatus::Valid;
}

FAStatus FiniteAutomaton::check_status() const {
  return status_flag_;
}

char FiniteAutomaton::peek() const {
  if (instream_.IsGood()) {
    return instream_->peek();
  }
  return 0;
}

void FiniteAutomaton::clear() {
  dfa_nodes_.clear();
}

void FiniteAutomaton::SetRepeatEOF(bool flag) {
  repeat_eof_ = flag;
}

inline bool FiniteAutomaton::tryAccept(char c) {
  auto &transitions = dfa_nodes_[state_pointer_].transitions;
  auto it = std::find_if(transitions.begin(), transitions.end(),
                         [=](const auto &t) { return t.Accept(c); });
  if (it != transitions.end()) {
    state_pointer_ = it->transition_state;
    return true;
  }
  return false;
}

inline std::pair<int, int> FiniteAutomaton::acceptingStateLambda(const unsigned index) {
  /// Determines whether the state Accepts or can make a lambda (null) transition
  /// to an accepting sstate.

  int accepting_state = -1, precedence = 0;

  // Check if the node itself is accepting.
  if (0 <= dfa_nodes_[index].accepting_state) {
    accepting_state = dfa_nodes_[index].accepting_state;
    precedence = dfa_nodes_[index].precedence;
  }

  // Check for lambda transitions. These could be of higher precedence than the accepting state
  // of the node (if any).
  std::deque<unsigned> lambda_stack;
  std::set<unsigned> lambda_set;
  // This node is the initial entry in the lambda_stack/lambda_set.
  lambda_stack.push_back(index);
  lambda_set.insert(index);
  // Go through lambda stack.
  while (!lambda_stack.empty()) {
    unsigned node_id = lambda_stack.front();
    // Check all transitions in this node, looking for
    for (const auto &transition : dfa_nodes_[node_id].transitions) {
      if (transition.IsNullTransition()
          && !set_contains(lambda_set, transition.transition_state)) {
        auto &node = dfa_nodes_[transition.transition_state];
        if (0 <= node.accepting_state && precedence < node.precedence) {
          accepting_state = node.accepting_state;
          precedence = node.precedence;
        }
        // Add to stack.
        lambda_stack.push_back(transition.transition_state);
        lambda_set.insert(transition.transition_state);
      }
    }
    lambda_stack.pop_front();
  }

  // Not an accepting state, and not lambda transitionable to an accepting state.
  return std::make_pair(accepting_state, precedence);
}

inline void FiniteAutomaton::compute_goto(
    std::set<int> &state,
    int state_id,
    std::deque<pair<int, std::set<int>>> &working_stack,
    std::vector<std::set<int>> &dfa_states,
    FiniteAutomaton &dfa) {
  // Lambda for checking whether a set is contained in dfa_states.
  auto contains = [&](const std::set<int> &state_set_1) {
    return std::find(dfa_states.begin(), dfa_states.end(), state_set_1) != dfa_states.end();
  };

  // Map from state to vector of char range that will transition to that state.
  std::map<int, std::vector<std::pair<char, char>>> transition_ranges;
  std::set<int> lambda_set;
  // Compute transition ranges.
  for (int node_id : state) {
    compute_transitions(node_id, transition_ranges, lambda_set);
  }

  // Turn transition ranges into transitions to transition sets.

  // Consolidate each range vector as much as possible.
  for (auto &range_list : transition_ranges) {
    consolidate_ranges(range_list.second);
  }

  // Create transition sets. Set, initial character, final character.
  std::vector<std::tuple<set<int>, char, char>> all_transition_sets;
  create_transition_sets(transition_ranges, all_transition_sets);

  for (auto &object : all_transition_sets) {
    // Unpack tuple.
    auto&[transition_set, range_initial, range_final] = object;
    // If we have derived a new set, we have to add it to the working stack
    if (!contains(transition_set)) {
      // Get an id number for the new set.
      unsigned new_state_id = dfa.size();
      // Check whether the new set is accepting. Do this by checking whether any node in the set is accepting.
      int accepting = -1, best_precedence = 0;
      for (auto node_id : transition_set) {
        auto[accepting_state, precedence] = acceptingStateLambda(node_id);
        if (accepting_state != -1) {
          // If there is already a different accepting state, that is an error.
          if (best_precedence < precedence) {
            accepting = accepting_state;
            best_precedence = precedence;
          }
        }
      }
      // Add a state to the dfa.
      dfa.AddNode(FiniteAutomatonNode(accepting, best_precedence));
      // Add the transition from the current state (state_id) to the new state (new_state_id).
      dfa.AddTransition(state_id, TransitionType{new_state_id, range_initial, range_final});
      dfa_states.push_back(transition_set);
      // Add the new state to the working stack.
      working_stack.emplace_back(new_state_id, transition_set);
    }
      // This is not a new set, but there are new transitions to this set.
    else {
      // Add the transition between the current state (state_id) and the already existing state (first, find its id).
      unsigned id = 0;
      for (; id < dfa_states.size(); ++id) {
        if (dfa_states[id] == transition_set) {
          break;
        }
      }
      // Add the transition.
      dfa.dfa_nodes_[state_id].transitions.emplace_back(id, range_initial, range_final);
    }
  }
}

inline void FiniteAutomaton::compute_transitions(const int node_id,
                                                 std::map<int, std::vector<std::pair<char, char>>> &transition_ranges,
                                                 std::set<int> &lambda_set) {
  // Record states that are reachable by lambda transitions.
  std::deque<int> lambda_reachable;
  // Compute states reachable for non-lambda, and populate a deque of states reachable by lambda transitions.
  for (auto &transition : dfa_nodes_[node_id].transitions) {
    int next_node = static_cast<int>(transition.transition_state);
    // Transition range.
    char transition_initial = transition.range_initial, transition_final = transition.range_final;
    // Only need to check if the node we could transition to is not already in the transition set.
    if (transition_ranges.count(next_node) == 0) {
      using vpair = std::vector<std::pair<char, char>>;
      if (transition.IsNullTransition()) {
        if (!set_contains(lambda_set, next_node)) {
          lambda_reachable.push_back(next_node); // Can this node be reached by a lambda?
          lambda_set.insert(next_node);
        }
      } else {
        transition_ranges.emplace(next_node, vpair{{transition.range_initial, transition.range_final}});
      }

    }
      // If not a lambda transition, add/combine character ranges.
    else if (!transition.IsNullTransition()) {
      // There is at least one transition range to get to this node. Check if this is a different one,
      // or if it can be combined with existing transitions.
      bool combined = false;
      for (auto &range : transition_ranges[next_node]) {
        char &range_initial = range.first, &range_final = range.second;
        // Adjust range min.
        if (transition_initial < range_initial && range_initial <= transition_final + 1) {
          range_initial = std::min(range_initial, transition_initial);
          combined = true;
        }

        // Adjust range max.
        if (transition_initial - 1 <= range_final && range_final < transition_final) {
          range_final = std::max(range_final, transition_final);
          combined = true;
        }
      }
      if (!combined) {
        transition_ranges[next_node].emplace_back(transition_initial, transition_final);
      }
    }
  }

  // Compute transitions for lambda reachable states.
  while (!lambda_reachable.empty()) {
    int next_node = lambda_reachable.front();
    compute_transitions(next_node, transition_ranges, lambda_set);
    lambda_reachable.pop_front();
  }
}

inline void FiniteAutomaton::consolidate_ranges(vector<pair<char, char> > &vector_of_ranges) {
  // TODO: Write this function.
}

inline void FiniteAutomaton::create_transition_sets(
    std::map<int, std::vector<pair<char, char> > > &transition_ranges,
    std::vector<std::tuple<set<int>, char, char> > &all_transition_sets) {
  // Character that starts/ends a range, whether that char starts or ends a range (false->starts range), and the state
  set<std::tuple<char, bool, int> > order_structure;
  for (const auto &range_obj : transition_ranges) {
    int node_id = range_obj.first;
    auto &range_vector = range_obj.second;
    for (auto pr : range_vector) {
      order_structure.insert(std::make_tuple(pr.first, false, node_id));
      order_structure.insert(std::make_tuple(pr.second, true, node_id));
    }
  }

  // Create transition sets.
  set<int> current_states;
  bool last_bool = true, first_time = true;;
  char last_char = -1;
  for (auto p = order_structure.begin(); p != order_structure.end(); ++p) {
    auto[character, ends, state] = *p;
    // Add or remove state from the current states set.
    if (!ends) {
      // Add set and characters.
      if (!first_time && character - 1 != last_char && !current_states.empty()) {
        all_transition_sets.emplace_back(current_states, last_char, character - 1);
      } else first_time = false;
      last_char = character;
      // Find all ranges that start at this character.
      current_states.insert(std::get<2>(*p));
      auto q = p;
      ++q;
      while (q != order_structure.end() && std::get<0>(*q) == character && std::get<1>(*q) == ends) {
        ++p; // p and q now match.
        current_states.insert(std::get<2>(*p));
        ++q;
      }
      last_bool = true;
    } else { // Ends.
      // Add set and characters.
      if (last_bool) {
        all_transition_sets.emplace_back(current_states, last_char, character);
      } else {
        all_transition_sets.emplace_back(current_states, last_char + 1, character);
      }
      last_char = character;

      // Find all ranges that end at this character.
      current_states.erase(std::get<2>(*p));
      auto q = p;
      ++q;
      while (q != order_structure.end() && std::get<0>(*q) == character && std::get<1>(*q) == ends) {
        ++p; // p and q now match.
        current_states.erase(std::get<2>(*p));
        ++q;
      }
      last_bool = false;
    }
  }
}
