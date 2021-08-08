#include <utility>

#include "../include/ParserUtility.hpp"
// Other files
#include "../include/LALRParser.hpp"

using namespace manta;

void ProductionRule::add(int r) {
  rhs.push_back(r);
}

int &ProductionRule::at(int i) {
  return rhs.at(i);
}

int ProductionRule::at(int i) const {
  return rhs.at(i);
}

int ProductionRule::size() const {
  return rhs.size();
}

void Item::endBookmark() const {
  bookmark = rhs.size();
}

void State::insert(const Item &item) {
  // Check if the item is a null production.
  if (item.size() == 0) has_null_production = true;
  // Insert the item into the set.
  item_set.insert(item);
}

void State::zero_bookmarks() {
  for (auto &item : item_set) {
    const int *i = &item.bookmark;
    *const_cast<int*>(i) = 0;
  }
}

bool State::contains(const Item &item) const {
  bool found = false;
  for (const auto &el : item_set) {
    if (el == item) {
      found = true;
    }
  }
  return found;
  // For some reason, this was sometimes failing to work.
  // return item_set.find(item) != item_set.end();
}

ostream &operator<<(std::ostream &out, const State &state) {
  if (state.empty()) out << "{}";
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

set<Item>::iterator State::begin() {
  return item_set.begin();
}

set<Item>::iterator State::end() {
  return item_set.end();
}

set<Item>::iterator State::begin() const {
  return item_set.begin();
}

set<Item>::iterator State::end() const {
  return item_set.end();
}

set<Item>::iterator State::find(const Item &item) {
  return item_set.find(item);
}

Entry::Entry() = default;

Entry::Entry(int s, const ResolutionInfo &res_info)
    : action(Action::Shift), state(s) {
  // Store the resolution info in the rule, even though the rule is empty.
  rule.res_info = res_info;
};

Entry::Entry(Item r)
    : action(Action::Reduce), state(0), rule(std::move(r)) {};

Entry::Entry(bool)
    : action(Action::Accept) {};

bool Entry::isError() const {
  return action == Action::Error;
}

bool Entry::isShift() const {
  return action == Action::Shift;
}

bool Entry::isReduce() const {
  return action == Action::Reduce;
}

bool Entry::isAccept() const {
  return action == Action::Accept;
}

namespace manta {

bool operator==(const State &s1, const State &s2) {
  return s1.item_set == s2.item_set;
}

bool operator<(const Item &a, const Item &b) {
  return a.production < b.production || a.bookmark < b.bookmark || less_than(a.rhs, b.rhs);
}

bool operator==(const Item &a, const Item &b) {
  return a.production == b.production && a.bookmark == b.bookmark && a.rhs == b.rhs;
}

std::ostream &operator<<(ostream &out, const Item &item) {
  out << item.production << " -> ";
  for (int i = 0; i < item.size(); ++i) {
    if (i == item.bookmark) {
      out << "* ";
    }
    out << item.rhs[i] << " ";
  }
  if (item.bookmark < 0) {
    out << "[*] ";
  } else if (item.bookmark == item.size()) {
    out << "* ";
  }
  return out;
}

string toString(const Item &item) {
  std::stringstream out;
  out << item.production << " -> ";
  for (int i = 0; i < item.size(); ++i) {
    if (i == item.bookmark) {
      out << "* ";
    }
    out << item.rhs[i] << " ";
  }
  if (item.bookmark < 0) {
    out << "[*] ";
  } else if (item.bookmark == item.size()) {
    out << "* ";
  }
  return out.str();
}

std::string Entry::write(int length) const {
  string str;
  switch (action) {
    case Action::Error: {
      str = " "; // x
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
      cout << "Error. Bad Entry.\n";
      break;
    }
  }
  // Make sure the string is of the correct length.
  return buffered(str, 4);
}

std::ostream &operator<<(std::ostream &out, const Entry &entry) {
  switch (entry.getAction()) {
    case Action::Error: {
      out << "x";
      break;
    }
    case Action::Shift: {
      out << "S: " << entry.getState();
      break;
    }
    case Action::Reduce: {
      out << "R: " << entry.getRule();
      break;
    }
    case Action::Accept: {
      out << "A";
      break;
    }
    default: {
      cout << "[Bad Entry]";
      break;
    }
  }

  return out;
}

}