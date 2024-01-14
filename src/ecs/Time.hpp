/*
** Agartha-Software, 2023
** C++evy
** File description:
** time
*/

#pragma once

#include "ecs.hpp"
#include <chrono>
#include <ratio>

namespace cevy::ecs {
class Time {
  private:
  std::chrono::time_point<std::chrono::high_resolution_clock> _first_update;
  std::chrono::time_point<std::chrono::high_resolution_clock> _last_update;
  std::chrono::duration<double, std::ratio<1>> _last_update_delta;

  public:
  std::chrono::duration<double, std::ratio<1>> startup() {
    return std::chrono::high_resolution_clock::now() - _first_update;
  }

  void update_with_instant(std::chrono::time_point<std::chrono::high_resolution_clock> &&instant) {
    _last_update_delta = instant - _last_update;
    _last_update = instant;
  }

  std::chrono::duration<double, std::ratio<1>> delta() { return _last_update_delta; }

  double delta_seconds() { return _last_update_delta.count(); }

  Time();
};
} // namespace cevy::ecs

void init_timer(cevy::ecs::World &w);
void update_timer(cevy::ecs::Resource<cevy::ecs::Time> time);
