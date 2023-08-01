//
// Created by Nathaniel Rupprecht on 7/31/23.
//

#pragma once

#include <chrono>

namespace manta::utility {

//! \brief A utility class the measures the duration between times.
class Timer {
  using Duration =
      std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>>;

public:
  Timer() { Start(); }

  void Start() {
    start_ = getTimePoint();
    is_running_ = true;
  }

  void Stop() { end_ = getTimePoint(); }

  //! \brief Get the elapsed time, in seconds.
  double Time() const {
    using namespace std::chrono;
    if (is_running_) {
      return duration_cast<duration<double>>(getTimePoint() - start_).count();
    }
    else {
      return duration_cast<duration<double>>(end_ - start_).count();
    }
  }

private:
  Duration getTimePoint() const { return std::chrono::high_resolution_clock::now(); }

  Duration start_, end_;
  bool is_running_ = false;
};

}  // namespace manta::utility
