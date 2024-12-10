#pragma once

#include <span>

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
      return "LEFT";
    case RIGHT:
      return "RIGHT";
    case NONE:
      return "NONE";
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
  explicit ProductionRule(NonterminalID production, const std::vector<int> rhs = {})
      : produced_nonterminal(production)
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

  std::span<const int> Tail(std::size_t start) const {
    if (rhs.size() <= start) {
      return {};
    }
    return {rhs.data() + start, rhs.size() - start};
  }


  friend std::ostream& operator<<(std::ostream& out, const ProductionRule& rule) {
    out << rule.produced_nonterminal << " ->";
    for (int i = 0; i < rule.Size(); ++i) {
      out << " " << rule.rhs[i];
    }
    return out;
  }

  // --- Data items ---

  // NOTE(Nate): Check how production and production_label differ - can we consolidate?

  //! \brief The non-terminal symbol, by ID, that this is a rule for. I.e., the left hand side of a
  //!        production rule.
  NonterminalID produced_nonterminal = -1;

  //! \brief The right hand side of the production.
  std::vector<int> rhs;
};

//! \brief Encodes state items like A -> a * X b, etc.
//!
//! An Item is a production rule plus a bookmark.
//! A state is a set of state items.
// TODO: Change Item to be a pointer to a production rule, plus a bookmark.
struct Item : ProductionRule {
  explicit Item(NonterminalID production,
                unsigned item_number,
                int bookmark                = 0,
                const std::vector<int>& rhs = {})
      : ProductionRule(production, rhs)
      , item_number(item_number)
      , bookmark(bookmark) {}

  explicit Item(const ProductionRule& rule, unsigned item_number, int bookmark = 0)
      : ProductionRule(rule)
      , bookmark(bookmark)
      , item_number(item_number) {}

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

  //! \brief What the production number for this item is. Used e.g. to find the correct item reduction
  //!        function, resolution info, etc.
  ItemID item_number;

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
  void ZeroBookmarks();
  NO_DISCARD bool Contains(const Item& item) const;
  friend std::ostream& operator<<(std::ostream& out, const State& state);
  NO_DISCARD int size() const;
  NO_DISCARD bool empty() const;
  std::set<Item>::iterator begin();
  std::set<Item>::iterator end();
  NO_DISCARD std::set<Item>::iterator begin() const;
  NO_DISCARD std::set<Item>::iterator end() const;
  std::set<Item>::iterator find(const Item& item);
  friend bool operator==(const State& s1, const State& s2);

  //! \brief The underlying set of items.
  std::set<Item> item_set;
};

//! \brief A production rule along with additional information related to parsing.
struct AnnotatedProductionRule {
  AnnotatedProductionRule(const ProductionRule& rule, ItemID production_number)
      : rule(rule)
      , production_item_number(production_number) {}

  ProductionRule rule;

  //! \brief A number for the production, i.e. this is the n-th item.
  ItemID production_item_number {};

  //! \brief Instructions associated with the state.
  //!
  //! TODO: Keep these in a better, dedicated structure.
  std::shared_ptr<class ParseNode> instructions = nullptr;

  //! \brief Resolution info, this encodes the precedence and associativity of a  production.
  //!
  //! This is actualized by resolving shift/reduce conflicts such that parsing works.
  ResolutionInfo res_info;

  Item MakeReducibleForm() const { return Item(rule, production_item_number, rule.Size()); }

  Item MakeFreshItem() const { return Item(rule, production_item_number); }

  auto operator<=>(const AnnotatedProductionRule& other) const { return rule <=> other.rule; }
};

class AnnotatedProductionSet : public std::set<AnnotatedProductionRule> {
public:
  State ToState() const {
    State out;
    for (const auto& annotated_production_rule : *this) {
      out.item_set.emplace(annotated_production_rule.rule, annotated_production_rule.production_item_number);
    }
    return out;
  }
};

//! \brief Table entry action types.
enum class Action {
  ERROR  = 0,
  SHIFT  = 1,
  REDUCE = 2,
  ACCEPT = 3,
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

//! \brief Allow for Actions to be streamed directly when logging.
inline void format_logstream(const Action action, lightning::RefBundle& handler) {
  handler << ToString(action);
}

//! \brief Table entry.
//!
//! Actions
//! 0 - Error.
//! 1 - Shift.
//! 2 - Reduce.
//! 3 - Accept.
class Entry {
private:
  struct ErrorState {
    auto operator<=>(const ErrorState&) const = default;
  };
  struct ShiftState {
    //! \brief Resolution info for the shift.
    ResolutionInfo res_info;

    //! \brief The state to transition to.
    StateID state = 0;

    auto operator<=>(const ShiftState& other) const { return state <=> other.state; }
  };
  struct ReduceState {
    //! \brief The reduce rule (if applicable).
    AnnotatedProductionRule annotated_rule;

    //! \brief The state to transition to.
    // StateID state = 0;
    auto operator<=>(const ReduceState& other) const {
      return annotated_rule.rule <=> other.annotated_rule.rule;
    }
  };
  struct AcceptState {
    auto operator<=>(const AcceptState&) const = default;
  };

public:
  //! Create entry as an error
  Entry();

  //! \brief Create entry as a shift.
  explicit Entry(StateID s, const ResolutionInfo& res_info = ResolutionInfo {});

  //! \brief Create entry as a reduce.
  explicit Entry(AnnotatedProductionRule annotated_production_rule);

  //! \brief Create entry as a reduce.
  explicit Entry(ProductionRule rule, ItemID reduction_id);

  //! \brief Create entry as an accept.
  explicit Entry(bool);

  bool IsError() const;
  bool IsShift() const;
  bool IsReduce() const;
  bool IsAccept() const;

  //! \brief Get the ResolutionInfo for the production rule associated with this entry.
  const ResolutionInfo& GetResInfo() const {
    if (GetAction() == Action::SHIFT) {
      return std::get<ShiftState>(state_).res_info;
    }
    if (GetAction() == Action::REDUCE) {
      return std::get<ReduceState>(state_).annotated_rule.res_info;
    }
    MANTA_FAIL("cannot get the resolution info from an entry that is not SHIFT or REDUCE");
  }

  Action GetAction() const { return static_cast<Action>(state_.index()); }

  StateID GetState() const {
    MANTA_REQUIRE(GetAction() == Action::SHIFT, "cannot get the transition state from a non-shift entry");
    // Or reduce?
    return std::get<ShiftState>(state_).state;
  }

  const ProductionRule& GetRule() const { return GetAnnotatedRule().rule; }

  const AnnotatedProductionRule& GetAnnotatedRule() const {
    MANTA_REQUIRE(GetAction() == Action::REDUCE, "cannot get the rule from a non-reduce entry");
    return std::get<ReduceState>(state_).annotated_rule;
  }

  std::string Write(int length) const;

  auto operator<=>(const Entry& rhs) const {
    if (GetAction() != rhs.GetAction()) {
      return GetAction() <=> rhs.GetAction();
    }
    switch (GetAction()) {
      case Action::ERROR:
        return std::get<ErrorState>(state_) <=> std::get<ErrorState>(rhs.state_);
      case Action::SHIFT:
        return std::get<ShiftState>(state_) <=> std::get<ShiftState>(rhs.state_);
      case Action::REDUCE:
        return std::get<ReduceState>(state_) <=> std::get<ReduceState>(rhs.state_);
      case Action::ACCEPT:
        return std::get<AcceptState>(state_) <=> std::get<AcceptState>(rhs.state_);
      default:
        MANTA_FAIL("impossible");
    }
  }

  auto operator==(const Entry& rhs) const { return *this <=> rhs == 0; }
  auto operator!=(const Entry& rhs) const { return !(*this == rhs); }

  friend std::ostream& operator<<(std::ostream&, const Entry&);

private:
  std::variant<ErrorState, ShiftState, ReduceState, AcceptState> state_;
};

inline void format_logstream(const Entry& entry, lightning::RefBundle& handler) {
  switch (entry.GetAction()) {
    case Action::ERROR:
      handler << "ERROR";
      break;
    case Action::SHIFT:
      handler << "SHIFT " << entry.GetState();
      break;
    case Action::ACCEPT:
      handler << "ACCEPT";
      break;
    case Action::REDUCE:
      handler << "REDUCE";
      break;
  }
}

}  // namespace manta
