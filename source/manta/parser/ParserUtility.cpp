

#include "manta/utility/ParserUtility.hpp"
// Other files
#include <utility>

#include "manta/parser/LALRParser.hpp"

using namespace manta;

void ProductionRule::add(int r) {
  rhs.push_back(r);
}

int& ProductionRule::at(int i) {
  return rhs.at(i);
}

int ProductionRule::at(int i) const {
  return rhs.at(i);
}

int ProductionRule::size() const {
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
  if (item.size() == 0)
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
    : action(Action::Shift)
    , state(s) {
  // Store the resolution info in the rule, even though the rule is empty.
  rule.res_info = res_info;
};

Entry::Entry(Item r)
    : action(Action::Reduce)
    , state(0)
    , rule(std::move(r)) {};

Entry::Entry(bool)
    : action(Action::Accept) {};

bool Entry::IsError() const {
  return action == Action::Error;
}

bool Entry::IsShift() const {
  return action == Action::Shift;
}

bool Entry::IsReduce() const {
  return action == Action::Reduce;
}

bool Entry::IsAccept() const {
  return action == Action::Accept;
}

namespace manta {

bool operator==(const State& s1, const State& s2) {
  return s1.item_set == s2.item_set;
}

bool operator<(const Item& a, const Item& b) {
  return std::tie(a.production, a.bookmark, a.rhs)
      < std::tie(b.production, b.bookmark, b.rhs);
}

bool operator==(const Item& a, const Item& b) {
  return std::tie(a.production, a.bookmark, a.rhs)
      == std::tie(b.production, b.bookmark, b.rhs);
}

std::ostream& operator<<(std::ostream& out, const Item& item) {
  out << item.production << " -> ";
  for (int i = 0; i < item.size(); ++i) {
    if (i == item.bookmark) {
      out << "* ";
    }
    out << item.rhs[i] << " ";
  }
  if (item.bookmark < 0) {
    out << "[*] ";
  }
  else if (item.bookmark == item.size()) {
    out << "* ";
  }
  return out;
}

std::string to_string(const Item& item, bool print_marker) {
  std::stringstream out;
  out << item.production << " -> ";
  for (int i = 0; i < item.size(); ++i) {
    if (i == item.bookmark && print_marker) {
      out << "* ";
    }
    out << item.rhs[i] << " ";
  }
  if (item.bookmark < 0) {
    out << "[*] ";
  }
  else if (item.bookmark == item.size()) {
    out << "* ";
  }
  return out.str();
}

std::string Entry::Write(int length) const {
  std::string str;
  switch (action) {
    case Action::Error: {
      str = " ";  // x
      break;
    }
    case Action::Shift: {
      str = "S" + std::to_string(state);
      break;
    }
    case Action::Reduce: {
      str = "R" + std::to_string(rule.production_label);
      break;
    }
    case Action::Accept: {
      str = "A";
      break;
    }
    default: {
      std::cout << "Error. Bad Entry.\n";
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
    case Action::Error: {
      out << "x";
      break;
    }
    case Action::Shift: {
      out << "S: " << entry.GetState();
      break;
    }
    case Action::Reduce: {
      out << "R: " << entry.GetRule();
      break;
    }
    case Action::Accept: {
      out << "A";
      break;
    }
    default: {
      std::cout << "[Bad Entry]";
      break;
    }
  }

  return out;
}

}  // namespace manta
