/*
** Agartha-Software, 2023
** C++evy
** File description:
** StopWatch
*/

#pragma once

#include <chrono>

namespace cevy {
namespace engine {
class Stopwatch {
  private:
  std::chrono::duration<double, std::ratio<1>> _duration;
  bool _paused;

  public:
  Stopwatch() : _duration(0), _paused(false) {}
  Stopwatch(float secs) : _duration(secs), _paused(false) {}

  std::chrono::duration<double, std::ratio<1>> elapsed() {
    return _duration;
  }

  double elapsed_secs() {
    return elapsed().count();
  }

  Stopwatch &set_elapsed(const std::chrono::duration<double, std::ratio<1>> &delta) {
    _duration = delta;
    return *this;
  }

  Stopwatch &tick(const std::chrono::duration<double, std::ratio<1>> &delta) {
    if (!_paused) {
        _duration += delta;
    }
    return *this;
  }

  Stopwatch &pause() {
    _paused = true;
    return *this;
  }

  Stopwatch &unpause() {
    _paused = false;
    return *this;
  }

  bool paused() {
    return _paused;
  }

  Stopwatch &reset() {
    _duration = std::chrono::duration<double, std::ratio<1>>(0);
    return *this;
  }
};
}
}
