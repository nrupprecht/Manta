#ifndef __UTILITY_HPP__MANTA__
#define __UTILITY_HPP__MANTA__

#include <iostream>
using std::cout;
using std::endl;

#include <ostream>
using std::ostream;

#include <vector>
using std::vector;
using std::pair;

#include <list>
using std::list;

#include <set>
using std::set;

#include <map>
using std::map;

#include <string>
using std::string;

#include <fstream>

template<typename T> inline bool contains(const set<T>& s, T el) {
  return s.find(el)!=s.end();
}

inline bool prepends(const set<string>& s, string op) {
  // Check if the string is the first part of any string in the set.
  for (auto str : s) {
    int index = str.find(op);
    if (index==0) return true;
  }
  // It wasn't.
  return false;
}

#endif // __UTILITY_HPP__MANTA__