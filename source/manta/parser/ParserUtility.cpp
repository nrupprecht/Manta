#include "manta/utility/ParserUtility.hpp"
// Other files.
#include <utility>

#include "manta/parser/LALRParser.hpp"

namespace manta {

void ProductionRule::AddToProduction(int r) {
  rhs.push_back(r);
}

int& ProductionRule::At(int i) {
  return rhs.at(i);
}

int ProductionRule::At(int i) const {
  return rhs.at(i);
}

int ProductionRule::Size() const {
  return static_cast<int>(rhs.size());
}

bool Item::IsBookmarkAtBeginning() const {
  return bookmark == 0;
}

bool Item::IsBookmarkAtEnd() const {
  return bookmark == rhs.size();
}

Item Item::MakeReducibleForm() const {
  auto copy = *this;
  copy.bookmark = static_cast<int>(rhs.size());
  return copy;
}

std::optional<Item> Item::AdvanceDot() const {
  if (IsBookmarkAtEnd()) {
    return {};
  }
  auto copy = *this;
  ++copy.bookmark;
  return copy;
}

Item Item::WithoutInstructions() const {
  auto copy = *this;
  copy.res_info = ResolutionInfo();
  copy.instructions = nullptr;
  return copy;
}

std::optional<int> Item::GetElementFollowingBookmark() const {
  if (bookmark == rhs.size()) {
    return {};
  }
  return rhs[bookmark];
}

void State::insert(const Item& item) {
  // Check if the item is a null production.
  if (item.Size() == 0)
    has_null_production = true;
  // Insert the item into the set.
  item_set.insert(item);
}

void State::zero_bookmarks() {
  for (auto& item : item_set) {
    const int* i = &item.bookmark;
    *const_cast<int*>(i) = 0;
  }
}

bool State::contains(const Item& item) const {
  bool found = false;
  for (const auto& el : item_set) {
    if (el == item) {
      found = true;
    }
  }
  return found;
  // For some reason, this was sometimes failing to work.
  // return item_set.find(item) != item_set.end();
}

std::ostream& operator<<(std::ostream& out, const State& state) {
  if (state.empty())
    out << "{}";
  else {
    out << "{";
    auto it = state.begin();
    out << *it;
    ++it;
    for (; it != state.end(); ++it) {
      out << ", " << *it;
    }
    out << "}";
  }
  return out;
}

int State::size() const {
  return item_set.size();
}

bool State::empty() const {
  return item_set.empty();
}

std::set<Item>::iterator State::begin() {
  return item_set.begin();
}

std::set<Item>::iterator State::end() {
  return item_set.end();
}

std::set<Item>::iterator State::begin() const {
  return item_set.begin();
}

std::set<Item>::iterator State::end() const {
  return item_set.end();
}

std::set<Item>::iterator State::find(const Item& item) {
  return item_set.find(item);
}

Entry::Entry() = default;

Entry::Entry(int s, const ResolutionInfo& res_info)
    : action(Action::SHIFT)
    , state(s) {
  // Store the resolution info in the rule, even though the rule is empty.
  rule.res_info = res_info;
};

Entry::Entry(Item r)
    : action(Action::REDUCE)
    , state(0)
    , rule(std::move(r)) {};

Entry::Entry(bool)
    : action(Action::ACCEPT) {};

bool Entry::IsError() const {
  return action == Action::ERROR;
}

bool Entry::IsShift() const {
  return action == Action::SHIFT;
}

bool Entry::IsReduce() const {
  return action == Action::REDUCE;
}

bool Entry::IsAccept() const {
  return action == Action::ACCEPT;
}

bool operator==(const State& s1, const State& s2) {
  return s1.item_set == s2.item_set;
}

bool operator<(const Item& a, const Item& b) {
  return std::tie(a.produced_nonterminal, a.bookmark, a.rhs)
      < std::tie(b.produced_nonterminal, b.bookmark, b.rhs);
}

bool operator==(const Item& a, const Item& b) {
  return std::tie(a.produced_nonterminal, a.bookmark, a.rhs)
      == std::tie(b.produced_nonterminal, b.bookmark, b.rhs);
}

std::ostream& operator<<(std::ostream& out, const Item& item) {
  out << item.produced_nonterminal << " -> ";
  for (int i = 0; i < item.Size(); ++i) {
    if (i == item.bookmark) {
      out << "* ";
    }
    out << item.rhs[i] << " ";
  }
  if (item.bookmark < 0) {
    out << "[*] ";
  }
  else if (item.bookmark == item.Size()) {
    out << "* ";
  }
  return out;
}

std::string to_string(const Item& item, bool print_marker) {
  std::stringstream out;
  out << item.produced_nonterminal << " -> ";
  for (int i = 0; i < item.Size(); ++i) {
    if (i == item.bookmark && print_marker) {
      out << "* ";
    }
    out << item.rhs[i] << " ";
  }
  if (item.bookmark < 0) {
    out << "[*] ";
  }
  else if (item.bookmark == item.Size()) {
    out << "* ";
  }
  return out.str();
}

std::string Entry::Write(int length) const {
  std::string str;
  switch (action) {
    case Action::ERROR: {
      str = " "; // x
      break;
    }
    case Action::SHIFT: {
      str = "S" + std::to_string(state);
      break;
    }
    case Action::REDUCE: {
      str = "R" + std::to_string(*rule.item_number);
      break;
    }
    case Action::ACCEPT: {
      str = "A";
      break;
    }
    default: {
      LOG_SEV(Warning) << "Entry::Write: Bad Entry.";
      break;
    }
  }
  // Make sure the string is of the correct length.
  return buffered(str, length);
}

bool Entry::operator==(const Entry& rhs) const {
  return std::tie(action, state) == std::tie(rhs.action, rhs.state);
}

std::ostream& operator<<(std::ostream& out, const Entry& entry) {
  switch (entry.GetAction()) {
    case Action::ERROR: {
      out << "x";
      break;
    }
    case Action::SHIFT: {
      out << "S: " << entry.GetState();
      break;
    }
    case Action::REDUCE: {
      out << "R: " << entry.GetRule();
      break;
    }
    case Action::ACCEPT: {
      out << "A";
      break;
    }
    default: {
      LOG_SEV(Warning) << "Cannot stream entry, bad entry.";
      break;
    }
  }

  return out;
}
} // namespace manta
