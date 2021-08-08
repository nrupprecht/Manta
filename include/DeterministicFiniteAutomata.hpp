#ifndef __FiniteAutomaton_HPP__MANTA__
#define __FiniteAutomaton_HPP__MANTA__

#include "LexerUtility.hpp"

#include <istream>

#include <set>
#include <utility>
#include "IStreamContainer.hpp"

namespace manta {

//! \brief Helper function that checks whether a set contains a value.
template<typename T>
inline bool set_contains(const std::set<T> &container, const typename std::remove_cv<T>::type &value) {
  return std::find(container.begin(), container.end(), value) != container.end();
}

//! \brief Print out a set.
inline void print(const std::set<int> &container) {
  std::cout << "{ ";
  for (int s : container) {
    std::cout << s << " ";
  }
  std::cout << "}";
}

//! \brief A structure that represents a transition between FiniteAutomatonNodes in a FiniteAutomaton.
struct TransitionType {
  //! \brief Creates a character range transition.
  //!
  //! \param state The state this transition leads to.
  //! \param ri The initial character in the character range (inclusive).
  //! \param rf The final character in the character range (inclusive).
  TransitionType(unsigned state, char ri, char rf)
      : transition_state(state), range_initial(ri), range_final(rf) {};

  //! \brief Creates a single character transition.
  //!
  //! \param state The state this transition leads to.
  //! \param c The character this transition accepts.
  TransitionType(unsigned state, char c)
      : transition_state(state), range_initial(c), range_final(c) {};

  //! \brief Create a lambda transition.
  explicit TransitionType(unsigned ts)
      : transition_state(ts), range_initial(1), range_final(0) {};

  //! \brief Whether to accept the character or not.
  //!
  //! \param c Character to test.
  //! \return True if c is accepted by the transition.
  NO_DISCARD bool Accept(const char c) const { return (range_initial <= c && c <= range_final) || range_final < range_initial; }

  //! \brief Return whether this is a lambda (null) transition.
  //!
  //! \return True if this is a lambda transition.
  NO_DISCARD bool IsNullTransition() const { return range_final < range_initial; }

  //! \brief The state this transition leads to.
  unsigned int transition_state;

  //! \brief The initial and final characters in the range.
  //!
  //! While in NFA mode, range_final < range_initial represents a lambda transition.
  char range_initial, range_final;
};

//! \brief A structure that represents a FiniteAutomaton node.
struct FiniteAutomatonNode {
  FiniteAutomatonNode(std::vector<TransitionType> t, int accepting, int prec = 1)
      : transitions(std::move(t)), accepting_state(accepting), precedence(prec) {};

  //! \brief Creates a node of a Finite automaton that accepts a specified state, and has a specified precedence.
  //!   Higher precedence overrides lower precedence.
  explicit FiniteAutomatonNode(int accepting, int prec = 1)
      : accepting_state(accepting), precedence(prec) {};

  FiniteAutomatonNode() = default;

  //! \brief Will this character cause a transition.
  NO_DISCARD bool WillAccept(char c) const;

  //! \brief Write a representation of this node to an ostream.
  void ToStream(std::ostream &out) const;

  //! \brief Possible transition to other nodes in the FiniteAutomaton.
  std::vector<TransitionType> transitions;

  //! \brief Is this an accepting state. If not, this is -1.
  //!
  //! The FiniteAutomaton should have at most one accepting lexeme per node.
  int accepting_state = -1;

  //! \brief The precedence of a state. This is used to resolve NDFA -> DFA conflicts.
  int precedence = 1;

  //! \brief If true, this node Accepts what is *not* specified by this node.
  bool inverted = false;
};

//! \brief Status states for finite automata.
enum class FAStatus {
  // All good.
  Valid,
  // Ended in non-accepting state. I.e. reached EOF, but EOF was not a lexeme.
  EndedNonAccepting,
  // Bad stream.
  BadStream,
  // EOF (ended in accepting state after consuming EOF).
  AcceptedEOF,
};

//! \brief A class that acts as a finite automaton.
//!
//! Creates an NFA from a specification in a file, then converts the NFA to a DFA.
class FiniteAutomaton {
 public:
  //! \brief Set the istream.
  void SetStream(std::istream& stream);
  void SetStream(IStreamContainer& stream);

  //! \brief Get the next token.
  Token GetToken();

  //! \brief Return a FiniteAutomaton from this (assumed to be) NFA.
  FiniteAutomaton NFAToDFA();

  //! \brief Return the number of nodes in the FiniteAutomaton.
  NO_DISCARD int size() const;

  //! \brief Add a specific node to the FiniteAutomaton, return the node id.
  int AddNode(const FiniteAutomatonNode &node);
  //! \brief Add an empty node to the FiniteAutomaton, return the node id.
  int AddNode();

  //! \brief Add a transition object.
  void AddTransition(int index, TransitionType type);
  //! \brief Add a char range transition.
  void AddTransition(int source, int dest, char ci, char cf);
  //! \brief Add a single char transition.
  void AddTransition(int source, int dest, char c);
  //! \brief Add a lambda transition.
  void AddTransition(int source, int dest);

  //! \brief Set a FiniteAutomaton node to have an accepting value.
  void SetAccepting(int index, int value);

  //! \brief Set the precedence of a state.
  //!
  //! \param index Index of the state to modify.
  //! \param precedence Precedence for the state.
  void SetPrecedence(int index, int precedence);

  //! \brief Print a representation of the FiniteAutomaton.
  void ToStream(std::ostream &out) const;

  //! \brief Return the accepting state of a string.
  NO_DISCARD int Accepts(const std::string &word) const;

  //! \brief Returns the accepting state of node 0.
  NO_DISCARD int accepts_empty() const;

  //! \brief Return true if there are any characters left to analyze.
  NO_DISCARD bool any_remaining() const;

  //! \brief Check the internal status of the parser.
  //!
  //! 0: normal, 1: did not accept, 2: instream is null.
  NO_DISCARD FAStatus check_status() const;

  //! \brief Peek at the next char in the stream.
  NO_DISCARD char peek() const;

  //! \brief Clear all the states from the dfa.
  void clear();

  //! \brief Set the repeat eof flag.
  void SetRepeatEOF(bool flag);

  NO_DISCARD int GetLine() const { return line_; }

  NO_DISCARD int GetCharacter() const { return character_; }

 private:

  inline bool tryAccept(char c);

  //! \brief Returns whether a state is either accepting, or lambda transitionable to an accepting state.
  //! Returns the (first reachable) accepting state number if accepting, or -1 if not accepting.
  inline std::pair<int, int> acceptingStateLambda(unsigned);

  inline void compute_goto(std::set<int> &state,
                           int state_id,
                           std::deque<pair<int, set<int>>> &working_stack,
                           std::vector<std::set<int>> &dfa_states,
                           FiniteAutomaton &dfa);

  inline void compute_transitions(int node_id,
                                  std::map<int, std::vector<std::pair<char, char>>> &transition_ranges,
                                  std::set<int> &lambda_set);

  inline void consolidate_ranges(vector<pair<char, char>> &);

  inline void create_transition_sets(std::map<int, std::vector<pair<char, char>>> &transition_ranges,
                                     std::vector<std::tuple<set<int>, char, char>> &all_transition_sets);

  //! \brief Pointer to an istream. Could be a stringstream or an ifstream.
  IStreamContainer instream_;

  //! \brief The current state of the FiniteAutomaton.
  unsigned state_pointer_ = 0;

  //! \brief Keep track of what line and character (in the line) of the file we are in.
  int line_ = 1;
  int character_ = 1;

  //! \brief If true, when the EOF is consumed, whenever a new token is asked for, another EOF token is returned
  //! instead of entering a failed state and returning a bad (-1) token.
  bool repeat_eof_ = false;

  //! \brief The zero-th node is the initial node.
  std::vector<FiniteAutomatonNode> dfa_nodes_;

  //! \brief Status flag.

  FAStatus status_flag_ = FAStatus::Valid;
};

}
#endif // __FiniteAutomaton_HPP__MANTA__