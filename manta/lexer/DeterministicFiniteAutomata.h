#pragma once

#include <istream>
#include <set>
#include <utility>

#include "manta/utility/LexerUtility.h"
#include "manta/utility/IStreamContainer.h"

namespace manta {

using Precedence = int;

//! \brief Helper function that checks whether a set contains a value.
template<typename T>
bool SetContains(const std::set<T>& container,
                        const typename std::remove_cv<T>::type& value) {
  return std::find(container.begin(), container.end(), value) != container.end();
}

//! \brief Print out a set.
inline void Print(const std::set<int>& container) {
  std::cout << "{ ";
  for (int s : container) {
    std::cout << s << " ";
  }
  std::cout << "}";
}

//! \brief A structure that represents a transition between FiniteAutomatonNodes in a
//! FiniteAutomaton.
struct TransitionType {
  //! \brief Creates a character range transition.
  //!
  //! \param state The state this transition leads to.
  //! \param ri The initial character in the character range (inclusive).
  //! \param rf The final character in the character range (inclusive).
  TransitionType(unsigned state, char ri, char rf)
      : transition_state(state)
      , range_initial(ri)
      , range_final(rf) {};

  //! \brief Creates a single character transition.
  //!
  //! \param state The state this transition leads to.
  //! \param c The character this transition accepts.
  TransitionType(unsigned state, char c)
      : transition_state(state)
      , range_initial(c)
      , range_final(c) {};

  //! \brief Create a lambda transition.
  explicit TransitionType(unsigned ts)
      : transition_state(ts)
      , range_initial(1)
      , range_final(0) {};

  //! \brief Whether to accept the character or not.
  //!
  //! \param c Character to test.
  //! \return True if c is accepted by the transition.
  NO_DISCARD bool Accept(const char c) const {
    return (range_initial <= c && c <= range_final)  // Normal transition
        || range_final < range_initial;  // Lambda transition.
  }

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
  FiniteAutomatonNode(std::vector<TransitionType> t, int accepting, Precedence prec = 1)
      : transitions(std::move(t))
      , accepting_states(1, std::pair(prec, accepting)) {};

  //! \brief Creates a node of a Finite automaton that accepts a specified state, and has
  //! a specified precedence.
  //!   Higher precedence overrides lower precedence.
  explicit FiniteAutomatonNode(std::vector<std::pair<Precedence, int>> accepting)
      : accepting_states(std::move(accepting)) {};

  FiniteAutomatonNode() = default;

  //! \brief Will this character cause a transition.
  NO_DISCARD bool WillAccept(char c) const;

  //! \brief Write a representation of this node to an ostream.
  void ToStream(std::ostream& out) const;

  //! \brief Check whether the state is accepting of at least one lexeme.
  NO_DISCARD bool IsAccepting() const { return !accepting_states.empty(); }

  //! \brief Possible transition to other nodes in the FiniteAutomaton.
  std::vector<TransitionType> transitions;

  //! \brief The accepting states of this node, and their precedence (first entry). If not
  //! accepting at all, this is empty.
  std::vector<std::pair<Precedence, int>> accepting_states;

  //! \brief If true, this node Accepts what is *not* specified by this node.
  bool inverted = false;
};

//! \brief Status states for finite automata.
enum class FAStatus
{
  // All good.
  Valid,
  // Ended in non-accepting state. I.e. reached EOF, but EOF was not a lexeme.
  EndedNonAccepting,
  // Bad stream.
  BadStream,
  // EOF (ended in accepting state after consuming EOF).
  AcceptedEOF,
};

inline std::string to_string(FAStatus status) {
  switch (status) {
    case FAStatus::Valid:
      return "FAStatus::Value";
    case FAStatus::EndedNonAccepting:
      return "FAStatus::EndedNonAccepting";
    case FAStatus::BadStream:
      return "FAStatus::BadStream";
    case FAStatus::AcceptedEOF:
      return "FAStatus::AcceptedEOF";
    default:
      MANTA_FAIL("unrecognized FAStatus");
  }
}

//! \brief A class that acts as a finite automaton.
//!
//! Creates an NFA from a specification in a file, then converts the NFA to a DFA.
class FiniteAutomaton {
public:
  //! \brief Set the istream.
  void SetStream(std::istream& stream);
  void SetContainer(utility::IStreamContainer& container);

  //! \brief Lex the next part of the input.
  std::optional<LexerResult> LexNext();

  //! \brief Return a FiniteAutomaton from this (assumed to be) NFA.
  FiniteAutomaton NFAToDFA();

  //! \brief Return the number of nodes in the FiniteAutomaton.
  NO_DISCARD int size() const;

  //! \brief Add a specific node to the FiniteAutomaton, return the node id.
  int AddNode(const FiniteAutomatonNode& node);
  //! \brief Add an empty node to the FiniteAutomaton, return the node id.
  int AddNode();

  //! \brief Add a transition object.
  void AddTransition(int index, TransitionType type);

  //! \brief Add a char range transition.
  void AddTransition(int source, int dest, char ci, char cf, bool complement = false);

  //! \brief Add a single char transition.
  void AddTransition(int source, int dest, char c, bool complement = false);

  //! \brief Add a lambda transition.
  void AddTransition(int source, int dest);

  //! \brief Set a lexeme that a state accepts, and the acceptance precedence.
  void AddAcceptance(int index, int lexeme_id, Precedence precedence);

  //! \brief Print a representation of the FiniteAutomaton.
  void ToStream(std::ostream& out) const;

  //! \brief Return the accepting precedence(s) and state(s) of a string.
  NO_DISCARD std::vector<std::pair<Precedence, int>> Accepts(
      const std::string& word) const;

  //! \brief Returns the accepting precedence(s) and state(s) of node 0.
  NO_DISCARD std::vector<std::pair<Precedence, int>> AcceptsEmpty() const;

  //! \brief Return true if there are any characters left to analyze.
  NO_DISCARD bool AnyRemaining() const;

  //! \brief Reset the internal status of the FA.
  void ResetStatus();

  //! \brief Check the internal status of the FA.
  //!
  //! 0: normal, 1: did not accept, 2: instream is null.
  NO_DISCARD FAStatus CheckStatus() const;

  //! \brief Peek at the next char in the stream.
  NO_DISCARD char Peek() const;

  //! \brief Clear all the states from the dfa.
  void Clear();

  //! \brief Set the repeat eof flag.
  void SetRepeatEOF(bool flag);

  //! \brief Get the line number that the DFA is on.
  NO_DISCARD int GetLine() const { return source_position_.line; }

  //! \brief Get the column in the line that the DFA is on.
  NO_DISCARD int GetCharacter() const { return source_position_.column; }

  NO_DISCARD auto& GetSourcePosition() const { return source_position_; }

  //! \brief Check whether the DFA's istreamcontainer is good.
  NO_DISCARD bool IsGood() const { return instream_.IsGood(); }

private:
  //! \brief If the character can be accepted, advance the state pointer and returning
  //! true. Otherwise, return false, and do not change state.
  //!
  //! Does not advance the line or character counters.
  //!
  //! \param c The character to try to accept.
  //! \return Whether the DFA will advance when fed the character.
  bool tryAccept(char c);

  //! \brief Returns whether a state is either accepting, or lambda transitionable to an
  //! accepting state. Returns the (first reachable) accepting state number if accepting,
  //! or -1 if not accepting.
  std::vector<std::pair<int, int>> acceptingStateLambda(unsigned);

  void computeGoto(std::set<int>& state,
                   int state_id,
                   std::deque<std::pair<int, std::set<int>>>& working_stack,
                   std::vector<std::set<int>>& dfa_states,
                   FiniteAutomaton& dfa);

  void computeTransitions(
      int node_id,
      std::map<int, std::vector<std::pair<char, char>>>& transition_ranges,
      std::set<int>& lambda_set);

  void consolidateRanges(std::vector<std::pair<char, char>>&);

  void createTransitionSets(
      std::map<int, std::vector<std::pair<char, char>>>& transition_ranges,
      std::vector<std::tuple<std::set<int>, char, char>>& all_transition_sets);

  //! \brief Pointer to an istream. Could be a stringstream or an ifstream.
  utility::IStreamContainer instream_;

  //! \brief The current state of the FiniteAutomaton.
  unsigned state_pointer_ = 0;

  //! \brief Keep track of what line and character (in the line) of the file we are in.
  SourcePosition source_position_{1, 1};

  //! \brief If true, when the EOF is consumed, whenever a new token is asked for, another
  //! EOF token is returned instead of entering a failed state and returning a bad (-1)
  //! token.
  bool repeat_eof_ = false;

  //! \brief The zero-th node is the initial node.
  std::vector<FiniteAutomatonNode> dfa_nodes_ {};

  //! \brief Status flag.
  FAStatus status_flag_ = FAStatus::Valid;
};

}  // namespace manta
