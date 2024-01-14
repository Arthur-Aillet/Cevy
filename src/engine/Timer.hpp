/*
** Agartha-Software, 2023
** Cevy
** File description:
** Timer
*/

#pragma once

#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>

#include "Stopwatch.hpp"

namespace cevy {
namespace engine {

class Timer {
  public:
  enum TimerMode { Once, Repeating };

  private:
  using DurationType = std::chrono::duration<double, std::ratio<1>>;
  DurationType _duration;
  Stopwatch _stopwatch;
  bool _finished;
  TimerMode _mode;
  size_t _times_finished_this_tick;

  public:
  Timer(double secs, TimerMode mode)
      : _duration(secs), _stopwatch(), _finished(false), _mode(mode), _times_finished_this_tick(0) {
  }
  Timer(DurationType duration, TimerMode mode)
      : _duration(), _stopwatch(), _finished(false), _mode(mode), _times_finished_this_tick(0) {}

  inline bool finished() { return _finished; }
  inline bool just_finished() { return _times_finished_this_tick > 0; }
  inline DurationType elapsed() { return _stopwatch.elapsed(); }
  inline double elapsed_secs() { return _stopwatch.elapsed_secs(); }

  inline Timer &set_elapsed(const DurationType &delta) {
    _stopwatch.set_elapsed(delta);
    return *this;
  }

  inline Timer &set_elapsed(double delta) {
    _stopwatch.set_elapsed(delta);
    return *this;
  }

  inline const DurationType &duration() { return _duration; }

  inline Timer &setDuration(float duration) {
    _duration = DurationType(duration);
    return *this;
  }

  inline Timer &setDuration(const DurationType &duration) {
    _duration = duration;
    return *this;
  }

  inline TimerMode mode() { return _mode; }

  inline Timer &setMode(TimerMode mode) {
    if (_mode == Repeating && mode == Repeating && _finished) {
      _stopwatch.reset();
      _finished = just_finished();
    }
    _mode = mode;
    return *this;
  }

  Timer &pause() {
    _stopwatch.pause();
    return *this;
  }

  Timer &unpause() {
    _stopwatch.pause();
    return *this;
  }

  bool paused() { return _stopwatch.paused(); }

  Timer &reset() {
    _stopwatch.reset();
    _finished = false;
    _times_finished_this_tick = 0;
    return *this;
  }

  Timer &tick(const DurationType &delta) {
    if (paused()) {
      _times_finished_this_tick = 0;
      if (_mode == Repeating)
        _finished = false;
      return *this;
    }

    if (_mode != Repeating && _finished) {
      _times_finished_this_tick = 0;
      return *this;
    }

    _stopwatch.tick(delta);
    _finished = elapsed() >= duration();

    if (!_finished) {
      _times_finished_this_tick = 0;
      return *this;
    }

    if (_mode != Repeating) {
      _times_finished_this_tick = 1;
      set_elapsed(DurationType(0));
      return *this;
    }

    if (_duration.count() == 0) {
      _times_finished_this_tick = SIZE_MAX;
      set_elapsed(DurationType(0));
    } else {
      _times_finished_this_tick = elapsed_secs() / _duration.count();
      set_elapsed(DurationType(std::fmod(elapsed_secs(), _duration.count())));
    }
    return *this;
  }

  inline double fraction() {
    if (_duration.count() == 0) {
      return 1;
    }
    return elapsed_secs() / _duration.count();
  }

  inline double fraction_remaining() { return 1 - fraction(); }
  inline DurationType remaining() { return duration() - elapsed(); }
  inline double remaining_secs() { return _duration.count() - elapsed_secs(); }
  inline size_t times_finished_this_tick() { return _times_finished_this_tick; }
};

} // namespace engine
} // namespace cevy
