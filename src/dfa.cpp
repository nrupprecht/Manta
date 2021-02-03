#include "dfa.hpp"

namespace Manta {

    bool FiniteAutomatonNode::accept(const char c) const {
        return std::any_of(transitions.begin(), transitions.end(),
                           [=](auto t) { return t.accept(c); });
    }

    void FiniteAutomatonNode::print() const {
        cout << "Transitions: ";
        // Are there any transitions?
        if (transitions.empty()) cout << "None.\n";
        else std::cout << "\n";
        // Print all transitions.
        for (const auto tr : transitions) {
            std::cout << " -> " << tr.transition_state << ":";
            if (tr.lambda()) std::cout << "lambda\n";
            else std::cout << " [" << tr.range_initial << ", " << tr.range_final << "]\n";
        }
        if (0 <= accepting_state) cout << "Accepting: " << accepting_state << endl;
    }

    void FiniteAutomaton::set_stream(std::istream &stream) {
        instream = stream;
    }

    void FiniteAutomaton::set_stream(istream_container &stream) {
        instream = stream;
    }

    //! \brief Get the next token.
    Token FiniteAutomaton::get_token() {
        if (!instream.is_good()) {
            status_flag = 2;
            return Token(-1, "");
        }

        auto& in = instream.get_istream();
        // Store the literal string
        std::string literal;
        char c;
        auto get_char = [&] () {
            if (in.eof()) c=char(0);
            else in.get(c);
        }; // Lambda function returns char(0) for eof.
        auto put_char = [&] () {
            if (in.eof());
            else in.putback(c);
        };
        // Get characters as long as we can.
        get_char();
        while (!in.eof() && will_accept(c)) {
            literal.push_back(c);
            get_char();
        }
        // The last character, that caused the FiniteAutomaton to fail, will be the first character
        // in the next lexeme.
        put_char();

        // Store what state the FiniteAutomaton ended in, then reset the state.
        unsigned final_state = state_pointer;
        state_pointer = 0; // Reset state pointer.

        // Check if we ended in an accepting state.
        if (0 <= dfa_nodes[final_state].accepting_state) {
            status_flag = 0;
            return Token(dfa_nodes[final_state].accepting_state, literal);
        }
        else { // An error has occured.
            status_flag = 1;
            return Token(-1, literal);
        }
    }

    FiniteAutomaton FiniteAutomaton::nfa_to_dfa() {
        // States are sets of nfa states.
        std::vector<std::set<int>> dfa_states;
        dfa_states.push_back(std::set<int> {0}); // Starting state just contains 0.

        // New dfa.
        auto dfa = FiniteAutomaton();

        // New states to process - stores the (DFA) state number and the set of NDFA states that makes up this state.
        std::deque<std::pair<int, std::set<int>>> working_stack;
        working_stack.emplace_back(0, dfa_states[0]); // Start with the initial state on the stack.
        dfa.add_node();
        // If the node is accepting, or can make a lambda (null) transition to an accepting state,
        // this state (the initial state) is accepting.
        dfa.dfa_nodes[0].accepting_state = accepting_state_lambda(0); // Node zero can also be accepting.

        // Process the stack until it is empty.
        while (!working_stack.empty()) {
            auto& [state_id, state_set] = working_stack.front();
            // Process the next entry in the deque.
            compute_goto(state_set, state_id, working_stack, dfa_states, dfa);
            // Pop the entry.
            working_stack.pop_front();
        }

        return dfa;
    }

    int FiniteAutomaton::size() const {
        return dfa_nodes.size();
    }

    int FiniteAutomaton::add_node(const FiniteAutomatonNode& node) {
        dfa_nodes.push_back(node);
        return static_cast<int>(dfa_nodes.size()) - 1;
    }

    int FiniteAutomaton::add_node() {
        dfa_nodes.emplace_back();
        return static_cast<int>(dfa_nodes.size()) - 1;
    }

    void FiniteAutomaton::add_transition(int index, TransitionType trn) {
        dfa_nodes[index].transitions.push_back(trn);
    }

    void FiniteAutomaton::add_transition(int source, int dest, char ci, char cf) {
        add_transition(source, TransitionType(dest, std::min(ci, cf), std::max(ci, cf)));
    }

    void FiniteAutomaton::add_transition(int source, int dest, char c) {
        add_transition(source, TransitionType(dest, c, c));
    }

    void FiniteAutomaton::add_transition(int source, int dest) {
        add_transition(source, TransitionType(dest));
    }

    void FiniteAutomaton::set_accepting(int index, int value) {
        dfa_nodes[index].accepting_state = value;
    }

    void FiniteAutomaton::print() const {
        for (int i = 0; i < dfa_nodes.size(); ++i) {
            std::cout << "Node " << i << ", ";
            dfa_nodes[i].print();
            std::cout << "\n";
        }
    }

    int FiniteAutomaton::accepts(const string& word) const {
        if (dfa_nodes.empty()) {
            return -1;
        }

        unsigned state = 0;
        for (char c : word) {
            bool did_transition = false;
            for (auto transition : dfa_nodes[state].transitions) {
                if (transition.accept(c)) {
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
        return dfa_nodes[state].accepting_state;
    }

    int FiniteAutomaton::accepts_empty() const {
        if (dfa_nodes.empty()) {
            return -2;
        }
        return dfa_nodes[0].accepting_state;
    }

    bool FiniteAutomaton::any_remaining() const {
        return instream.is_good() && !instream->eof();
    }

    int FiniteAutomaton::check_status() const {
        return status_flag;
    }

    char FiniteAutomaton::peek() const {
        if (instream.is_good()) {
            return instream->peek();
        }
        return 0;
    }

    void FiniteAutomaton::clear() {
        dfa_nodes.clear();
    }

    inline bool FiniteAutomaton::will_accept(char c) {
        auto& transitions = dfa_nodes[state_pointer].transitions;
        auto it = std::find_if(transitions.begin(), transitions.end(), [=](auto t) { return t.accept(c); });
        if (it != transitions.end()) {
            state_pointer = it->transition_state;
            return true;
        }
        return false;
    }

    inline int FiniteAutomaton::accepting_state_lambda(const unsigned index) {
        /// Determines whether the state accepts or can make a lambda (null) transition
        /// to an accepting state.

        // Check if the node itself is accepting.
        if (0 <= dfa_nodes[index].accepting_state) {
            return dfa_nodes[index].accepting_state;
        }
        // Check for lambda transitions.
        std::deque<unsigned> lambda_stack;
        std::set<unsigned> lambda_set;
        // This node is the initial entry in the lambda_stack/lambda_set.
        lambda_stack.push_back(index);
        lambda_set.insert(index);
        // Go through lambda stack.
        while (!lambda_stack.empty()) {
            unsigned node_id = lambda_stack.front();
            // Check all transitions in the node
            for (auto &transition : dfa_nodes[node_id].transitions) {
                if (transition.lambda() && !set_contains(lambda_set, transition.transition_state)) {
                    if (0 <= dfa_nodes[transition.transition_state].accepting_state) {
                        return dfa_nodes[transition.transition_state].accepting_state;
                    }
                    // Add to stack.
                    lambda_stack.push_back(transition.transition_state);
                    lambda_set.insert(transition.transition_state);
                }
            }
            lambda_stack.pop_front();
        }

        // Not an accepting state, and not lambda transitionable to an accepting state.
        return -1;
    }

    inline void FiniteAutomaton::compute_goto(
            std::set<int>& state,
            int state_id,
            std::deque<pair<int, std::set<int>>> &working_stack,
            std::vector<std::set<int>> &dfa_states,
            FiniteAutomaton& dfa) {
        // Lambda for checking whether a set is contained in dfa_states.
        auto contains = [&] (const std::set<int>& state_set_1) {
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

        for (auto& object : all_transition_sets) {
            // Unpack tuple.
            auto& [transition_set, range_initial, range_final] = object;
            // If we have derived a new set, we have to add it to the working stack
            if (!contains(transition_set)) {
                // Get an id number for the new set.
                unsigned new_state_id = dfa.size();
                // Check whether the new set is accepting. Do this by checking whether any node in the set is accepting.
                int accepting = -1;
                for (auto node_id : transition_set) {
                    int accepting_state = accepting_state_lambda(node_id);
                    if (accepting_state!=-1) {
                        // If there is already a different accepting state, that is an error.
                        if (accepting != -1 && accepting != accepting_state) {
                            std::cout << "ERROR. Overlapping accepting states: " << accepting << " and "
                                      << accepting_state << " for node " << new_state_id << endl;
                        }
                        // Set the accepting state.
                        accepting = accepting_state;
                    }
                }
                // Add a state to the dfa.
                dfa.add_node(FiniteAutomatonNode(accepting));
                // Add the transition from the current state (state_id) to the new state (new_state_id).
                dfa.add_transition(state_id,
                                   TransitionType {new_state_id, range_initial, range_final}
                );
                dfa_states.push_back(transition_set);
                // Add the new state to the working stack.
                working_stack.emplace_back(new_state_id, transition_set);
            }
            // This is not a new set, but there are new transitions to this set.
            else {
                // Add the transition between the current state (state_id) and the already existing state (first, find its id).
                unsigned id = 0;
                for (; id < dfa_states.size(); ++id) {
                    if (dfa_states[id]==transition_set) {
                        break;
                    }
                }
                // Add the transition.
                dfa.dfa_nodes[state_id].transitions.emplace_back(id, range_initial, range_final);
            }
        }
    }

    inline void FiniteAutomaton::compute_transitions(const int node_id, std::map<int, std::vector<std::pair<char, char>>>& transition_ranges, std::set<int>& lambda_set) {
        // Record states that are reachable by lambda transitions.
        std::deque<int> lambda_reachable;
        // Compute states reachable for non-lambda, and populate a deque of states reachable by lambda transitions.
        for (auto &transition : dfa_nodes[node_id].transitions) {
            int next_node = static_cast<int>(transition.transition_state);
            // Transition range.
            char transition_initial = transition.range_initial, transition_final = transition.range_final;
            // Only need to check if the node we could transition to is not already in the transition set.
            if (transition_ranges.count(next_node) == 0) {
                using vpair = std::vector<std::pair<char, char>>;
                if (transition.lambda()) {
                    if (!set_contains(lambda_set, next_node)) {
                        lambda_reachable.push_back(next_node); // Can this node be reached by a lambda?
                        lambda_set.insert(next_node);
                    }
                }
                else {
                    transition_ranges.emplace(next_node, vpair{{transition.range_initial, transition.range_final}});
                }

            }
            // If not a lambda transition, add/combine character ranges.
            else if (!transition.lambda()) {
                // There is at least one transition range to get to this node. Check if this is a different one,
                // or if it can be combined with existing transitions.
                bool combined = false;
                for (auto& range : transition_ranges[next_node]) {
                    char& range_initial = range.first, &range_final = range.second;
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

    inline void FiniteAutomaton::consolidate_ranges(vector<pair<char, char> >& vector_of_ranges) {
        // \todo Write this function.
    }

    inline void FiniteAutomaton::create_transition_sets(
            std::map<int, std::vector<pair<char, char> > >& transition_ranges,
            std::vector<std::tuple<set<int>, char, char> >& all_transition_sets) {
        // Character that starts/ends a range, whether that char starts or ends a range (false->starts range), and the state
        set<std::tuple<char, bool, int> > order_structure;
        for (const auto& range_obj : transition_ranges) {
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
        for (auto p=order_structure.begin(); p!=order_structure.end(); ++p) {
            auto [character, ends, state] = *p;
            // Add or remove state from the current states set.
            if (!ends) {
                // Add set and characters.
                if (!first_time && character-1!=last_char && !current_states.empty()) {
                    all_transition_sets.emplace_back(current_states, last_char, character - 1);
                }
                else first_time = false;
                last_char = character;
                // Find all ranges that start at this character.
                current_states.insert(std::get<2>(*p));
                auto q = p;
                ++q;
                while (q!=order_structure.end() && std::get<0>(*q)==character && std::get<1>(*q)==ends) {
                    ++p; // p and q now match.
                    current_states.insert(std::get<2>(*p));
                    ++q;
                }
                last_bool = true;
            }
            else { // Ends.
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
                while (q!=order_structure.end() && std::get<0>(*q)==character && std::get<1>(*q)==ends) {
                    ++p; // p and q now match.
                    current_states.erase(std::get<2>(*p));
                    ++q;
                }
                last_bool = false;
            }
        }
    }

}