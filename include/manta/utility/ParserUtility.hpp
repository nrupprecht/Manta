#pragma once

#include "manta/lexer/LexerUtility.hpp"

namespace manta {

using NonterminalID = int;
using ItemID        = int;
using StateID       = int;

//! \brief Enum for the associativity of an operator/production rule.
enum class Associativity {
  LEFT,
  RIGHT,
  NONE
};

inline std::string to_string(Associativity assoc) {
  switch (assoc) {
    using enum Associativity;
    case LEFT:
      return "Left";
    case RIGHT:
      return "Right";
    case NONE:
      return "None";
    default:
      MANTA_FAIL("unrecognized Associativity");
  }
}

//! \brief Encodes the precedence and associativity of an operator/production rule.
struct ResolutionInfo {
  double precedence   = 0.;
  Associativity assoc = Associativity::NONE;

  bool operator==(const ResolutionInfo& rhs) const {
    return precedence == rhs.precedence && assoc == rhs.assoc;
  }

  bool operator!=(const ResolutionInfo& rhs) const { return !(*this == rhs); }
};

constexpr ResolutionInfo NullResolutionInfo {};

//! \brief Encode a production rule, like A -> a X b, etc.
struct ProductionRule {
  explicit ProductionRule(NonterminalID production, int label, const std::vector<int> rhs = {})
      : produced_nonterminal(production)
      , production_item_number(label)
      , rhs(rhs) {}

  //! \brief Create an empty production rule.
  ProductionRule() = default;

  //! \brief Add a terminal or non-terminal, by ID, to the right hand side of the production rule.
  //!
  //! Thinking of the RHS as a vector of symbols, this adds a non-terminal to the end of the vector.
  void AddToProduction(int r);

  //! \brief Get the i-th element of the RHS of the production.
  NO_DISCARD int& At(int i);

  NO_DISCARD int At(int i) const;

  //! \brief Get the number of elements in the RHS of the production.
  NO_DISCARD int Size() const;

  auto operator<=>(const ProductionRule& rule) const {
    return std::tie(produced_nonterminal, rhs) <=> std::tie(rule.produced_nonterminal, rule.rhs);
  }

  // --- Data items ---

  // NOTE(Nate): Check how production and production_label differ - can we consolidate?

  //! \brief The non-terminal symbol, by ID, that this is a rule for. I.e., the left hand side of a
  //! production rule.
  NonterminalID produced_nonterminal = -1;

  //! \brief A number for the production, i.e. this is the n-th item.
  ItemID production_item_number {};

  //! \brief The right hand side of the production.
  std::vector<int> rhs;

  //! \brief Instructions associated with the state.
  //!
  //! TODO: Keep these in a better, dedicated structure.
  std::shared_ptr<class ParseNode> instructions = nullptr;

  //! \brief Resolution info, this encodes the precedence and associativity of a  production.
  //!
  //! This is actualized by resolving shift/reduce conflicts such that parsing works.
  ResolutionInfo res_info;
};

//! \brief Encodes state items like A -> a * X b, etc.
//!
//! An Item is a production rule plus a bookmark.
//! A state is a set of state items.
// TODO: Change Item to be a pointer to a production rule, plus a bookmark.
struct Item : public ProductionRule {
  Item(NonterminalID production,
       int label,
       int bookmark                        = 0,
       const std::vector<int>& rhs         = {},
       std::optional<unsigned> item_number = {})
      : ProductionRule(production, label, rhs)
      , item_number(item_number)
      , bookmark(bookmark) {}

  //! \brief Create an empty item.
  Item() = default;

  //! \brief Returns whether the bookmark is at the beginning (zero position).
  bool IsBookmarkAtBeginning() const;

  //! \brief Returns whether the bookmark is at the end position.
  bool IsBookmarkAtEnd() const;

  //! \brief Create a new Item where the bookmark is at the end, the "reducible form" of
  //! this item.
  Item MakeReducibleForm() const;

  //! \brief Create a new Item where the bookmark has advanced by one. If the bookmark is at the end already,
  //!        returns nullopt.
  std::optional<Item> AdvanceDot() const;

  //! \brief Make a new identical Item without any instructions or resolution info
  Item WithoutInstructions() const;

  //! \brief If the bookmark is at the end, returns {}, otherwise, returns the terminal or nonterminal
  //!        immediately following the bookmark.
  std::optional<int> GetElementFollowingBookmark() const;

  friend bool operator<(const Item& a, const Item& b);
  friend bool operator==(const Item& a, const Item& b);
  friend std::ostream& operator<<(std::ostream& out, const Item& item);
  friend std::string to_string(const Item&, bool);

  // =====================================================================================
  //  Data
  // =====================================================================================

  //! \brief What the item number for this item is. Used e.g. to find the correct item reduction function.
  std::optional<ItemID> item_number {};

  //! \brief The location of the bookmark.
  //!
  //! Bookmark is in the place *before* bookmark, so e.g. "A -> a * X b" has bookmark=1.
  //! A -1 means no bookmark, this is used when an item is being used to encode a pure
  //! production, not a state item.
  mutable int bookmark = 0;
};

bool operator<(const Item& a, const Item& b);
bool operator==(const Item& a, const Item& b);
std::ostream& operator<<(std::ostream& out, const Item& item);
std::string to_string(const Item& item, bool print_marker = true);

//! \brief Define a state to be a set of Items, with some extra features for convenience.
struct State {
  State() = default;

  void insert(const Item& item);

  // Set the bookmarks in all the items in this state to be fresh.
  void zero_bookmarks();
  NO_DISCARD bool contains(const Item& item) const;
  friend std::ostream& operator<<(std::ostream& out, const State& state);
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
enum class Action {
  ERROR,
  SHIFT,
  REDUCE,
  ACCEPT
};

//! \brief Write an action as a string.
inline std::string ToString(Action action) {
  switch (action) {
    case Action::ERROR:
      return "ERROR";
    case Action::SHIFT:
      return "SHIFT";
    case Action::REDUCE:
      return "REDUCE";
    case Action::ACCEPT:
      return "ACCEPT";
    default:
      MANTA_FAIL("unrecognized Action");
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
  explicit Entry(int s, const ResolutionInfo& res_info = ResolutionInfo {});

  //! Create entry as a reduce.
  explicit Entry(Item r);

  //! \brief Create entry as an accept.
  explicit Entry(bool);

  bool IsError() const;
  bool IsShift() const;
  bool IsReduce() const;
  bool IsAccept() const;

  //! \brief Get the ResolutionInfo for the production rule associated with this entry.
  const ResolutionInfo& GetResInfo() const { return rule.res_info; }
  Action GetAction() const { return action; }
  StateID GetState() const { return state; }
  const Item& GetRule() const { return rule; }
  std::string Write(int length) const;
  friend std::ostream& operator<<(std::ostream&, const Entry&);
  bool operator==(const Entry& rhs) const;

private:
  //! \brief The action.
  Action action = Action::ERROR;

  //! \brief The state to transition to.
  StateID state = 0;

  //! \brief The reduce rule (if applicable).
  Item rule;
};

}  // namespace manta
