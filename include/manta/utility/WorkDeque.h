#pragma once

#include "manta/utility/Exceptions.h"
#include <deque>
#include <set>

namespace manta::utility {

//! \brief Object that acts as a deque of work, but you can only add new items if
//! they have never been added before.
template<typename T>
struct WorkDeque {
 public:
  bool Add(const T& item) {
    if (auto it = marked_.find(item); it == marked_.end()) {
      work_.push_back(item);
      marked_.insert(item);
      return true;
    }
    return false;
  }

  T PopNext() {
    auto temp = std::move(work_.front());
    work_.pop_front();
    return temp;
  }

  NO_DISCARD bool Empty() const {
    return work_.empty();
  }

 private:
  //! \brief Current work items.
  std::deque<T> work_;
  //! \brief All items that have ever been seen.
  std::set<T> marked_;
};


} // namespace manta::utility
