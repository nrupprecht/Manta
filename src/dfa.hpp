#ifndef __FiniteAutomaton_HPP__MANTA__
#define __FiniteAutomaton_HPP__MANTA__

#include "lexer-Manta.hpp"

#include <istream>

#include <set>
using std::set;

using std::make_pair;
using std::tuple;
using std::get;
using std::make_tuple;

namespace Manta {

  //! \brief Helper function that checks whether a set contains a value.
  template<typename T> inline bool set_contains(set<T> &container, T &value) {
    return std::find(container.begin(), container.end(), value)!=container.end();
  }

  //! \brief Print out a set.
  inline void print(set<int> &container) {
    cout << "{ ";
    for (int s : container) cout << s << " ";
    cout << "}";
  }

  //! \brief A structure that represents a transition between FiniteAutomatonNodes in a FiniteAutomaton.
  struct TransitionType {
    //! \brief Create a transition, specifying all its data.
    TransitionType(unsigned ts, char ri, char rf) : transition_state(ts), range_initial(ri), range_final(rf) {};
    //! \brief Single character transition.
    TransitionType(unsigned ts, char c) : transition_state(ts), range_initial(c), range_final(c) {};
    //! \brief Create a lambda transition.
    TransitionType(unsigned ts) : transition_state(ts), range_initial(1), range_final(0) {};

    //! \brief The state this transition leads to.
    unsigned transition_state;
    //! \brief The initial and final characters in the range.
    //!
    //! While in NFA mode, range_final < range_initial represents a lambda transition.
    char range_initial, range_final;

    //! \brief Whether to accept the character or not.
    bool accept(const char c) const { return (range_initial<=c && c<=range_final) || range_final<range_initial; }
    //! \brief Return whether this is a lambda (null) transition.
    bool lambda() const { return range_final<range_initial; }
  };

  //! \brief A structure that represents a FiniteAutomaton node.
  struct FiniteAutomatonNode {
    FiniteAutomatonNode (vector<TransitionType> t, int accepting) : transitions(t), accepting_state(accepting) {};
    FiniteAutomatonNode(int accepting) : accepting_state(accepting) {};
    FiniteAutomatonNode() {};

    //! \brief Will this character cause a transition.
    bool accept(const char c) const;

    //! \brief Print a representation of this node.
    void print() const;

    //! \brief Possible transition to other nodes in the FiniteAutomaton.
    vector<TransitionType> transitions;

    //! \brief Is this an accepting state. If not, this is -1.
    //!
    //! The FiniteAutomaton should have at most one accepting lexeme per node.
    int accepting_state = -1;
  };

  //! \brief A class that acts as a finite automaton.
  //!
  //! Creates an NFA from a specification in a file, then converts the NFA to a DFA.
  class FiniteAutomaton {
  public:
    //! \brief Set the istream.
    void set_stream(std::istream&);

    //! \brief Get the next token.
    Token get_token();

    //! \brief Return a FiniteAutomaton from this (assumed to be) NFA.
    FiniteAutomaton nfa_to_dfa();

    //! \brief Return the number of nodes in the FiniteAutomaton.
    int size() const;

    //! \brief Add a specific node to the FiniteAutomaton, return the node id.
    int add_node(FiniteAutomatonNode);
    //! \brief Add an empty node to the FiniteAutomaton, return the node id.
    int add_node();

    //! \brief Add a transition object.
    void add_transition(int, TransitionType);
    //! \brief Add a char range transition.
    void add_transition(int, int, char, char);
    //! \brief Add a single char transition.
    void add_transition(int, int, char);
    //! \brief Add a lambda transition.
    void add_transition(int, int);

    //! \brief Set a FiniteAutomaton node to have an accepting value.
    void set_accepting(int, int);

    //! \brief Print a representation of the FiniteAutomaton.
    void print() const;

    //! \brief Return the accepting state of a string.
    int accepts(const string&) const;

    //! \brief Returns the accepting state of node 0.
    int accepts_empty() const;

    //! \brief Return true if there are any characters left to analyze.
    bool any_remaining() const;

    //! \brief Check the internal status of the parser.
    //!
    //! 0: normal, 1: did not accept, 2: instream is null.
    int check_status() const;

    //! \brief Peek at the next char in the stream.
    char peek() const;

    //! \brief Clear all the states from the dfa.
    void clear();

  private:

    inline bool will_accept(char c);

    //! \brief Returns whether a state is either accepting, or lambda transitionable to an accepting state.
    //! Returns the (first reachable) accepting state number if accepting, or -1 if not accepting.
    inline int accepting_state_lambda(const unsigned);

    inline void compute_goto(set<int>&, int, std::deque<pair<int, set<int> > >&, vector<set<int> >&, FiniteAutomaton&);

    inline void compute_transitions(const int, map<int, vector<pair<char, char> > >&, set<int>&);

    inline void consolidate_ranges(vector<pair<char, char> >&);

    inline void create_transition_sets(map<int, vector<pair<char, char> > >&, vector<tuple<set<int>, char, char> >&);

    //! \brief Pointer to an istream. Could be a stringstream or an ifstream.
    std::istream *in = nullptr;

    //! \brief The current state of the FiniteAutomaton.
    unsigned state_pointer = 0;

    //! \brief The zero-th node is the initial node.
    vector<FiniteAutomatonNode> dfa_nodes;

    //! \brief Status flag.
    int status_flag = 0;
  };

}
#endif // __FiniteAutomaton_HPP__MANTA__