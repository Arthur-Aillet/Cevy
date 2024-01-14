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
  using DurationType = std::chrono::duration<double, std::ratio<1>>;
  DurationType _duration;
  bool _paused;

  public:
  Stopwatch() : _duration(0), _paused(false) {}
  Stopwatch(float secs) : _duration(secs), _paused(false) {}
  Stopwatch(DurationType secs) : _duration(secs), _paused(false) {}

  DurationType elapsed() { return _duration; }

  double elapsed_secs() { return elapsed().count(); }

  inline Stopwatch &set_elapsed(const DurationType &delta) {
    _duration = delta;
    return *this;
  }

  inline Stopwatch &set_elapsed(double delta) {
    _duration = DurationType(delta);
    return *this;
  }

  Stopwatch &tick(const DurationType &delta) {
    if (!_paused) {
      _duration += delta;
    }
    return *this;
  }

  inline Stopwatch &pause() {
    _paused = true;
    return *this;
  }

  inline Stopwatch &unpause() {
    _paused = false;
    return *this;
  }

  inline bool paused() { return _paused; }

  inline Stopwatch &reset() {
    _duration = DurationType(0);
    return *this;
  }
};
} // namespace engine
} // namespace cevy
