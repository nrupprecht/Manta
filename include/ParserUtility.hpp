#ifndef __PARSER_CLASSES_HPP__MANTA__
#define __PARSER_CLASSES_HPP__MANTA__

#include "LexerUtility.hpp"

namespace manta {

typedef int ProductionID;
typedef int ActionID;
typedef int StateID;

//! \brief Enum for the associativity of an operator/production rule.
enum class Associativity { Left, Right, None };

inline std::string ToString(Associativity assoc) {
  switch (assoc) {
    case Associativity::Left:
      return "Left";
    case Associativity::Right:
      return "Right";
    case Associativity::None:
      return "None";
    default:
      throw std::exception();
  }
}

//! \brief Encodes the precedence and associativity of an operator/production rule.
struct ResolutionInfo {
  double precedence = 0.;
  Associativity assoc = Associativity::None;

  bool operator==(const ResolutionInfo& rhs) const {
    return precedence == rhs.precedence && assoc == rhs.assoc;
  }
  bool operator!=(const ResolutionInfo& rhs) const {
    return !(*this == rhs);
  }
};

constexpr ResolutionInfo NullResolutionInfo{};

//! \brief Encode a production rule, like A -> a X b, etc.
struct ProductionRule {
  explicit ProductionRule(ProductionID production, int label)
      : production(production), production_label(label) {}

  //! \brief Create an empty production rule.
  ProductionRule() = default;

  void add(int r);
  int& at(int i);
  int at(int i) const;
  int size() const;

  // --- Data items ---

  //! \brief The production ID that this is a rule for. I.e., the left hand side of a production rule.
  ProductionID production = -1;

  //! \brief A number for the production, i.e. this is the n-th production.
  int production_label;

  //! \brief The right hand side of the production.
  std::vector<int> rhs;

  //! \brief Instructions associated with the state.
  std::shared_ptr<class ParseNode> instructions = nullptr;

  //! \brief Resolution info - this encodes the precedence and associativity of a production. This is actualized
  //! by resolving shift/reduce conflicts such that parsing works.
  ResolutionInfo res_info;
};

//! \brief Encodes state items like A -> a * X b, etc.
//!
//! An Item is a production rule plus a bookmark.
//! A state is a set of state items.
//! \TODO: Change Item to be a pointer to a production rule, plus a bookmark.
struct Item : public ProductionRule {
  Item(ProductionID production, int label, int bookmark = 0)
      : ProductionRule(production, label), bookmark(bookmark) {}

  //! \brief Create an empty item.
  Item() = default;

  //! \brief Returns whether the bookmark is at the end position.
  void endBookmark() const;

  friend bool operator<(const Item& a, const Item& b);
  friend bool operator==(const Item& a, const Item& b);
  friend ostream& operator<<(ostream& out, const Item& item);
  friend string toString(const Item&);

  // --- Data items ---

  //! \brief The location of the bookmark.
  //!
  //! Bookmark is in the place *before* bookmark, so e.g. "A -> a * X b" has bookmark=1.
  //! A -1 means no bookmark, this is used when an item is being used to encode a pure production,
  //! not a state item.
  mutable int bookmark = 0;
};

//! \brief Define a state to be a set of Items, with some extra features for convenience.
struct State {
  State() = default;

  void insert(const Item& item);

  // Set the bookmarks in all the items in this state to be fresh.
  void zero_bookmarks();

  NO_DISCARD bool contains(const Item& item) const;

  friend ostream& operator<<(ostream& out, const State& state);

  NO_DISCARD int size() const;
  NO_DISCARD bool empty() const;

  std::set<Item>::iterator begin();
  std::set<Item>::iterator end();
  NO_DISCARD std::set<Item>::iterator begin() const;
  NO_DISCARD std::set<Item>::iterator end() const;
  std::set<Item>::iterator find(const Item& item);

  friend bool operator==(const State& s1, const State& s2);

  //! \brief True if there exists an Item in the set that is a null production, A -> null
  bool has_null_production = false;

  //! \brief The underlying set of items.
  std::set<Item> item_set;
};

//! \brief Table entry action types.
enum class Action { Error, Shift, Reduce, Accept };

inline std::string ToString(Action action) {
  switch (action) {
    case Action::Error:return "Error";
    case Action::Shift:return "Shift";
    case Action::Reduce:return "Reduce";
    case Action::Accept:return "Accept";
    default:throw std::exception();
  }
}

//! \brief Table entry.
//!
//! Actions
//! 0 - Error.
//! 1 - Shift.
//! 2 - Reduce.
//! 3 - Accept.
struct Entry {
  //! Create entry as an error
  Entry();
  //! Create entry as a shift.
  explicit Entry(int s, const ResolutionInfo& res_info = ResolutionInfo{});
  //! Create entry as a reduce.
  explicit Entry(Item r);
  //! \brief Create entry as an accept.
  explicit Entry(bool);

  bool isError() const;
  bool isShift() const;
  bool isReduce() const;
  bool isAccept() const;

  //! \brief Get the ResolutionInfo for the production rule associated with this entry.
  const ResolutionInfo& GetResInfo() const { return rule.res_info; }

  Action getAction() const { return action; }
  StateID getState() const { return state; }
  Item getRule() const { return rule; }

  string write(int length) const;

  friend ostream& operator<<(ostream&, const Entry&);

 private:
  //! \brief The action.
  Action action = Action::Error;

  //! \brief The state to transition true.
  StateID state = 0;

  //! \brief The reduce rule (if applicable).
  Item rule;
};

} // manta
#endif // __PARSER_CLASSES_HPP__MANTA__