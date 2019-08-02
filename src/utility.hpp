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

  inline string repeat(char c, int length) {
    string str;
    for (int i=0; i<length; ++i) str += c;
    return str;
  }

  inline int max(int a, int b) {
    return a>b ? a : b;
  }

  inline string buffered(int i, int length) {
    int ai = abs(i);
    int l = i<0 ? 1 : 0;

    int tens = 10;
    for (int j=1; j<length; ++j) {
      if (ai<tens) {
        l += j;
        break;
      }
      tens *= 10;
    }
    // Put into a string.
    string str = repeat(' ', length-l);
    str += toString(i);
    // Return the string.
    return str;
  }

  inline string buffered(string str, int length) {
    return repeat(' ', max(length-str.size(), 0)) + str;
  }

}
#endif // __UTILITY_HPP__MANTA__