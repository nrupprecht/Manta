#pragma once

#include <deque>
#include <set>

#include "manta/utility/Exceptions.h"

namespace manta::utility {

//! \brief Object that acts as a deque of work, but you can only add new items if
//!        they have never been added before.
template<typename T>
class WorkDeque {
public:
  //! \brief Try to add an object to the WorkDeque.
  bool Add(const T& item) {
    if (auto it = marked_.find(item); it == marked_.end()) {
      work_.push_back(item);
      marked_.insert(item);
      return true;
    }
    return false;
  }

  //! \brief Pop the next object off the deque and return it.
  T PopNext() {
    MANTA_REQUIRE(!work_.empty(), "cannot WorkDeque::PopNext, work deque is empty");
    auto temp = std::move(work_.front());
    work_.pop_front();
    return temp;
  }

  //! \brief Check if the work deque is empty.
  NO_DISCARD bool Empty() const { return work_.empty(); }

private:
  //! \brief Current work items.
  std::deque<T> work_;
  //! \brief All items that have ever been seen.
  std::set<T> marked_;
};


}  // namespace manta::utility
