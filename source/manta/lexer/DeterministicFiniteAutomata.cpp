#include "manta/lexer/DeterministicFiniteAutomata.hpp"

using namespace manta;

bool FiniteAutomatonNode::WillAccept(const char c) const {
  return std::any_of(transitions.begin(),
                     transitions.end(),
                     [=](const TransitionType& t) { return t.Accept(c); });
}

void FiniteAutomatonNode::ToStream(std::ostream& out) const {
  out << "Transitions: ";
  // Are there any transitions?
  if (transitions.empty()) {
    out << "None.\n";
  }
  else {
    out << "\n";
  }
  // Print all transitions.
  for (const auto tr : transitions) {
    out << " -> " << tr.transition_state << ":";
    if (tr.IsNullTransition()) {
      out << "lambda\n";
    }
    else {
      out << " [" << tr.range_initial << ", " << tr.range_final << "]\n";
    }
  }
  if (IsAccepting()) {
    out << "Accepts: ";
    for (auto [prec, state] : accepting_states) {
      out << "(S: " << state << ", P: " << prec << ")\n";
    }
  }
}

void FiniteAutomaton::SetStream(std::istream& stream) {
  ResetStatus();
  instream_ = stream;
}

void FiniteAutomaton::SetStream(IStreamContainer& stream) {
  ResetStatus();
  instream_ = stream;
}

std::optional<LexResult> FiniteAutomaton::LexNext() {
  // If repeat_eof_ is true, we repeatedly return EOF when we hit the eof.
  if (!repeat_eof_) {
    if (status_flag_ == FAStatus::AcceptedEOF) {
      return {};
    }
    if (!instream_.IsGood() || status_flag_ != FAStatus::Valid) {
      status_flag_ = FAStatus::BadStream;
      return {};
    }
  }

  // If we have reached EOF, check if we accept EOF. In so, set the status to reflect
  // this. If not, we cannot get any more tokens.
  //  if (instream_->eof()) {
  //    if (tryAccept('\0')) {
  //      unsigned final_state = state_pointer_;
  //      state_pointer_ = 0; // Reset state pointer.
  //      status_flag_ = FAStatus::AcceptedEOF; // We have reached and consumed EOF.
  //
  //      // We have to check what state EOF is.
  //      return LexResult({{0 /* EOF */, "EOF"}}, "\\0");
  //    }
  //    else {
  //      status_flag_ = FAStatus::EndedNonAccepting;
  //      return {};
  //    }
  //  }

  state_pointer_ = 0;  // Reset state pointer.

  // Store the literal string
  std::string literal;
  char c;
  bool put_back_eof = true;
  auto get_char = [&]() {
    if (instream_->eof()) {
      c = '\0';  // This character represents EOF.
    }
    else {
      instream_->get(c);
      if (instream_->eof()) {
        c = '\0';
      }
    }
  };  // Lambda function returns char(0) for eof.
  auto put_char = [&]() {
    if (instream_->eof())
      ;
    else {
      instream_->putback(c);
    }
  };
  // Get characters as long as we can.
  get_char();
  while (tryAccept(c)) {
    literal.push_back(c);

    if (instream_->eof()) {
      // We got here because EOF was accepted, therefore, don't "put it back."
      put_back_eof = false;
      break;
    }

    // Advance counters.
    if (c == '\n') {
      ++line_;
      character_ = 1;
    }
    else {
      ++character_;
    }
    get_char();
  }
  // The last character, that caused the FiniteAutomaton to fail, will be the first
  // character in the next lexeme.
  put_char();

  // Store what state the FiniteAutomaton ended in, then reset the state.
  unsigned final_state = state_pointer_;
  state_pointer_ = 0;  // Reset state pointer.

  // Check if we are able to extract any token and ended in an accepting state.
  if (!literal.empty() && dfa_nodes_[final_state].IsAccepting()) {
    status_flag_ =
        (instream_->eof() && !put_back_eof) ? FAStatus::AcceptedEOF : FAStatus::Valid;

    // The DFA does not know the lexeme names, so they will have to be filled in by the
    // Lexer.
    LexResult result({}, literal);
    for (auto& [_, lexeme_id] : dfa_nodes_[final_state].accepting_states) {
      result.accepted_lexemes.emplace_back(lexeme_id, "?");
    }
    return result;
  }
  else {  // An error has occurred.
    status_flag_ = FAStatus::EndedNonAccepting;
    return {};
  }
}

FiniteAutomaton FiniteAutomaton::NFAToDFA() {
  // States are sets of NFA states.
  std::vector<std::set<int>> dfa_states;
  dfa_states.push_back(std::set {0});  // Starting state just contains 0.

  // New dfa.
  auto dfa = FiniteAutomaton();

  // New states to process - stores the (DFA) state number and the set of NDFA states that
  // makes up this state.
  std::deque<std::pair<int, std::set<int>>> working_stack;
  // Start with the initial state on the stack.
  working_stack.emplace_back(0, dfa_states[0]);
  dfa.AddNode();
  // If the node is accepting, or can make a lambda (null) transition to an accepting
  // state, this state (the initial state) is accepting.
  dfa.dfa_nodes_[0].accepting_states =
      acceptingStateLambda(0);  // Node zero can also be accepting.

  // Process the stack until it is empty.
  while (!working_stack.empty()) {
    auto& [state_id, state_set] = working_stack.front();
    // Process the next entry in the deque.
    computeGoto(state_set, state_id, working_stack, dfa_states, dfa);
    // Pop the entry.
    working_stack.pop_front();
  }

  return dfa;
}

int FiniteAutomaton::size() const {
  return static_cast<int>(dfa_nodes_.size());
}

int FiniteAutomaton::AddNode(const FiniteAutomatonNode& node) {
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

void FiniteAutomaton::AddTransition(
    int source, int dest, char ci, char cf, bool complement) {
  auto cs = std::min(ci, cf), ce = std::max(ci, cf);
  if (complement) {
    if (cs != '\0') {
      AddTransition(source, TransitionType(dest, '\0', static_cast<char>(cs - 1)));
    }
    if (ce != std::numeric_limits<char>::max()) {
      AddTransition(
          source,
          TransitionType(
              dest, static_cast<char>(ce + 1), std::numeric_limits<char>::max()));
    }
  }
  else {
    AddTransition(source, TransitionType(dest, cs, ce));
  }
}

void FiniteAutomaton::AddTransition(int source, int dest, char c, bool complement) {
  if (complement) {
    if (c != '\0') {
      AddTransition(source, TransitionType(dest, '\0', static_cast<char>(c - 1)));
    }
    if (c != std::numeric_limits<char>::max()) {
      AddTransition(
          source,
          TransitionType(
              dest, static_cast<char>(c + 1), std::numeric_limits<char>::max()));
    }
  }
  else {
    AddTransition(source, TransitionType(dest, c, c));
  }
}

void FiniteAutomaton::AddTransition(int source, int dest) {
  AddTransition(source, TransitionType(dest));
}

void FiniteAutomaton::AddAcceptance(int index, int lexeme_id, Precedence precedence) {
  dfa_nodes_[index].accepting_states.emplace_back(precedence, lexeme_id);
}

void FiniteAutomaton::ToStream(std::ostream& out) const {
  for (int i = 0; i < dfa_nodes_.size(); ++i) {
    out << "Node " << i << ", ";
    dfa_nodes_[i].ToStream(out);
    out << "\n";
  }
}

std::vector<std::pair<Precedence, int>> FiniteAutomaton::Accepts(
    const std::string& word) const {
  if (dfa_nodes_.empty()) {
    return {};
  }

  unsigned state = 0;
  for (char c : word) {
    bool did_transition = false;
    for (auto& transition : dfa_nodes_[state].transitions) {
      if (transition.Accept(c)) {
        state = transition.transition_state;
        did_transition = true;
        break;
      }
    }
    // If we get here, it means the character did not correspond to any transition.
    if (!did_transition) {
      return {};
    }
  }
  // Return the accepting state of the final node.
  return dfa_nodes_[state].accepting_states;
}

std::vector<std::pair<Precedence, int>> FiniteAutomaton::AcceptsEmpty() const {
  if (dfa_nodes_.empty()) {
    return {};
  }
  return dfa_nodes_[0].accepting_states;
}

bool FiniteAutomaton::AnyRemaining() const {
  return status_flag_ == FAStatus::Valid;
}

void FiniteAutomaton::ResetStatus() {
  status_flag_ = FAStatus::Valid;
}

FAStatus FiniteAutomaton::CheckStatus() const {
  return status_flag_;
}

char FiniteAutomaton::Peek() const {
  if (instream_.IsGood()) {
    return static_cast<char>(instream_->peek());
  }
  return 0;
}

void FiniteAutomaton::Clear() {
  dfa_nodes_.clear();
}

void FiniteAutomaton::SetRepeatEOF(bool flag) {
  repeat_eof_ = flag;
}

inline bool FiniteAutomaton::tryAccept(char c) {
  auto& transitions = dfa_nodes_[state_pointer_].transitions;
  auto it = std::find_if(
      transitions.begin(), transitions.end(), [=](const auto& t) { return t.Accept(c); });
  if (it != transitions.end()) {
    state_pointer_ = it->transition_state;
    return true;
  }
  return false;
}

inline std::vector<std::pair<int, int>> FiniteAutomaton::acceptingStateLambda(
    const unsigned index) {
  /// Determines whether the state Accepts or can make a lambda (null) transition
  /// to an accepting state.

  std::vector<std::pair<int, int>> accepting_states;

  // Add all accepting lexemes from this node.
  accepting_states.insert(accepting_states.end(),
                          dfa_nodes_[index].accepting_states.begin(),
                          dfa_nodes_[index].accepting_states.end());

  // Check for lambda transitions. These could be of higher precedence than the accepting
  // state of the node (if any).
  std::deque<unsigned> lambda_stack;
  std::set<unsigned> lambda_set;
  // This node is the initial entry in the lambda_stack/lambda_set.
  lambda_stack.push_back(index);
  lambda_set.insert(index);
  // Go through lambda stack.
  while (!lambda_stack.empty()) {
    unsigned node_id = lambda_stack.front();
    // Check all transitions in this node, looking for
    for (const auto& transition : dfa_nodes_[node_id].transitions) {
      if (transition.IsNullTransition()
          && !SetContains(lambda_set, transition.transition_state))
      {
        auto& node = dfa_nodes_[transition.transition_state];
        // Add any accepting lexemes from this node.
        if (node.IsAccepting()) {
          accepting_states.insert(accepting_states.end(),
                                  node.accepting_states.begin(),
                                  node.accepting_states.end());
        }

        // Add to stack.
        lambda_stack.push_back(transition.transition_state);
        lambda_set.insert(transition.transition_state);
      }
    }
    lambda_stack.pop_front();
  }

  // Not an accepting state, and not lambda transitionable to an accepting state.
  std::sort(accepting_states.begin(),
            accepting_states.end(),
            std::greater<std::pair<int, int>> {});
  return accepting_states;
}

inline void FiniteAutomaton::computeGoto(
    std::set<int>& state,
    int state_id,
    std::deque<std::pair<int, std::set<int>>>& working_stack,
    std::vector<std::set<int>>& dfa_states,
    FiniteAutomaton& dfa) {
  // Lambda for checking whether a set is contained in dfa_states.
  auto contains = [&](const std::set<int>& state_set_1) {
    return std::find(dfa_states.begin(), dfa_states.end(), state_set_1)
        != dfa_states.end();
  };

  // Map from state to vector of char range that will transition to that state.
  std::map<int, std::vector<std::pair<char, char>>> transition_ranges;
  std::set<int> lambda_set;
  // Compute transition ranges.
  for (int node_id : state) {
    computeTransitions(node_id, transition_ranges, lambda_set);
  }

  // Turn transition ranges into transitions to transition sets.

  // Consolidate each range vector as much as possible.
  for (auto& range_list : transition_ranges) {
    consolidateRanges(range_list.second);
  }

  // Create transition sets. Set, initial character, final character.
  std::vector<std::tuple<std::set<int>, char, char>> all_transition_sets;
  createTransitionSets(transition_ranges, all_transition_sets);

  for (auto& object : all_transition_sets) {
    // Unpack tuple.
    auto& [transition_set, range_initial, range_final] = object;
    // If we have derived a new set, we have to add it to the working stack
    if (!contains(transition_set)) {
      // Get an id number for the new set.
      unsigned new_state_id = dfa.size();

      // Check whether the new set is accepting. Do this by checking whether any node in
      // the set is accepting.
      std::vector<std::pair<Precedence, int>> accepting_states;
      for (auto node_id : transition_set) {
        auto accepting = acceptingStateLambda(node_id);
        accepting_states.insert(
            accepting_states.end(), accepting.begin(), accepting.end());
      }
      std::sort(accepting_states.begin(),
                accepting_states.end(),
                std::greater<std::pair<int, int>> {});

      // Add a state to the dfa.
      dfa.AddNode(FiniteAutomatonNode(accepting_states));
      // Add the transition from the current state (state_id) to the new state
      // (new_state_id).
      dfa.AddTransition(state_id,
                        TransitionType {new_state_id, range_initial, range_final});
      dfa_states.push_back(transition_set);
      // Add the new state to the working stack.
      working_stack.emplace_back(new_state_id, transition_set);
    }
    // This is not a new set, but there are new transitions to this set.
    else {
      // Add the transition between the current state (state_id) and the already existing
      // state (first, find its id).
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

inline void FiniteAutomaton::computeTransitions(
    int node_id,
    std::map<int, std::vector<std::pair<char, char>>>& transition_ranges,
    std::set<int>& lambda_set) {
  // Record states that are reachable by lambda transitions.
  std::deque<int> lambda_reachable;
  // Compute states reachable for non-lambda, and populate a deque of states reachable by
  // lambda transitions.
  for (auto& transition : dfa_nodes_[node_id].transitions) {
    int next_node = static_cast<int>(transition.transition_state);
    // Transition range.
    char transition_initial = transition.range_initial,
         transition_final = transition.range_final;
    // Only need to check if the node we could transition to is not already in the
    // transition set.
    if (transition_ranges.count(next_node) == 0) {
      using vpair = std::vector<std::pair<char, char>>;
      if (transition.IsNullTransition()) {
        if (!SetContains(lambda_set, next_node)) {
          lambda_reachable.push_back(next_node);  // Can this node be reached by a lambda?
          lambda_set.insert(next_node);
        }
      }
      else {
        transition_ranges.emplace(
            next_node, vpair {{transition.range_initial, transition.range_final}});
      }
    }
    // If not a lambda transition, add/combine character ranges.
    else if (!transition.IsNullTransition()) {
      // There is at least one transition range to get to this node. Check if this is a
      // different one, or if it can be combined with existing transitions.
      bool combined = false;
      for (auto& range : transition_ranges[next_node]) {
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
    computeTransitions(next_node, transition_ranges, lambda_set);
    lambda_reachable.pop_front();
  }
}

inline void FiniteAutomaton::consolidateRanges(
    std::vector<std::pair<char, char>>& vector_of_ranges) {
  // TODO: Write this function.
}

inline void FiniteAutomaton::createTransitionSets(
    std::map<int, std::vector<std::pair<char, char>>>& transition_ranges,
    std::vector<std::tuple<std::set<int>, char, char>>& all_transition_sets) {
  // Character that starts/ends a range, whether that char starts or ends a range
  // (false->starts range), and the state
  std::set<std::tuple<char, bool, int>> order_structure;
  for (const auto& range_obj : transition_ranges) {
    int node_id = range_obj.first;
    auto& range_vector = range_obj.second;
    for (auto pr : range_vector) {
      order_structure.insert(std::make_tuple(pr.first, false, node_id));
      order_structure.insert(std::make_tuple(pr.second, true, node_id));
    }
  }

  // Create transition sets.
  std::set<int> current_states;
  bool last_bool = true, first_time = true;
  ;
  char last_char = -1;
  for (auto p = order_structure.begin(); p != order_structure.end(); ++p) {
    auto [character, ends, state] = *p;
    // Add or remove state from the current states set.
    if (!ends) {
      // Add set and characters.
      if (!first_time && character - 1 != last_char && !current_states.empty()) {
        all_transition_sets.emplace_back(current_states, last_char, character - 1);
      }
      else
        first_time = false;
      last_char = character;
      // Find all ranges that start at this character.
      current_states.insert(std::get<2>(*p));
      auto q = p;
      ++q;
      while (q != order_structure.end() && std::get<0>(*q) == character
             && std::get<1>(*q) == ends)
      {
        ++p;  // p and q now match.
        current_states.insert(std::get<2>(*p));
        ++q;
      }
      last_bool = true;
    }
    else {  // Ends.
      // Add set and characters.
      if (last_bool) {
        all_transition_sets.emplace_back(current_states, last_char, character);
      }
      else {
        all_transition_sets.emplace_back(current_states, last_char + 1, character);
      }
      last_char = character;

      // Find all ranges that end at this character.
      current_states.erase(std::get<2>(*p));
      auto q = p;
      ++q;
      while (q != order_structure.end() && std::get<0>(*q) == character
             && std::get<1>(*q) == ends)
      {
        ++p;  // p and q now match.
        current_states.erase(std::get<2>(*p));
        ++q;
      }
      last_bool = false;
    }
  }
}
