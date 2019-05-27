#ifndef __SET_HPP__MANTA__
#define __SET_HPP__MANTA__

#include "utility.hpp"

namespace Manta {

  template<typename T> class Manta_set {
  public:

    //! \brief A typedef for (simple) identification functions.
    typedef bool (*id_func) (T);

    //! \brief Add an object to the set.
    void add(T obj) {
      objects.insert(obj);
    }

    //! \brief Add an identification function to the set.
    void add(id_func f) {
      id_function_set.push_back(f);
    }

    //! \brief Check whether the set contains an object.
    bool contains(T obj) const {
      // Try to find the object in the set
      auto it = objects.find(obj);
      if (it!=objects.end()) return true;
      // Otherwise, check the functions
      if (!id_function_set.empty()) {
        for (auto f : id_function_set) 
          if (f(obj)) return true;
      }
      // Otherwise, the object is not contained in the set.
      return false;
    }

    bool empty() const {
      return objects.empty() && id_function_set.empty();
    }

  //private:
    //! \brief A set of objects
    std::set<T> objects;

    //! \brief A set of functions that can be used to identify if an element is in the set.
    vector<id_func> id_function_set;
  };

};


#endif // __SET_HPP__MANTA__