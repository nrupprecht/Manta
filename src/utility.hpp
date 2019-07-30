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

#include <unordered_map>
using std::unordered_map;

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include <fstream>

namespace Manta {

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

  template<typename T> inline string toString(const T x) {
    stringstream stream;
    stream << x;
    string str;
    stream >> str;
    return str;
  }

  // Less than comparison for vectors.
  template<typename T>
  inline bool less_than(const vector<T> &a, const vector<T> &b) {
    // If the sizes are the same, compare in dictionary order.
    if (a.size()==b.size()) {
      for (int i=0; i<a.size(); ++i) {
        if (a[i]==b[i]) continue;
        else return a[i]<b[i];
      }
      // The vectors are equal.
      return false;
    }
    // Otherwise, the shorter vector is "lesser."
    else return a.size()<b.size();
  }

}
#endif // __UTILITY_HPP__MANTA__