#pragma once

#include <iostream>
#include <ostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>

namespace manta {

template <typename T>
inline bool contains(const std::set<T> &s, T &&el) {
  return s.find(el) != s.end();
}

inline bool prepends(const std::set<std::string> &s, const std::string &op) {
  // Check if the string is the first part of any string in the set.
  for (const auto &str: s) {
    int index = str.find(op);
    if (index == 0) {
      return true;
    }
  }
  // It wasn't.
  return false;
}

inline std::string repeat(char c, int length) {
  std::string str;
  str.reserve(length);
  for (int i = 0; i < length; ++i) {
    str += c;
  }
  return str;
}

inline std::string buffered(int i, int length) {
  int ai = abs(i);
  int l = i < 0 ? 1 : 0;

  int tens = 10;
  for (int j = 1; j < length; ++j) {
    if (ai < tens) {
      l += j;
      break;
    }
    tens *= 10;
  }
  // Put into a string.
  std::string str = repeat(' ', length - l);
  str += std::to_string(i);
  // Return the string.
  return str;
}

inline std::string buffered(const std::string &str, int length) {
  return repeat(' ', std::max(length - static_cast<int>(str.size()), 0)) + str;
}

inline std::string clean(const std::string &str) {
  std::string output;
  for (char c: str) {
    if (c == '\n') output += "\\n";
    else if (c == '\t') output += "\\t";
    else if (c == '\r') output += "\\r";
    else if (c == '\0') output += "\\0";
    else output += c;
  }
  return output;
}

} // namespace manta
