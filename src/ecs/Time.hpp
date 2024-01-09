/*
** Agartha-Software, 2023
** Cevy
** File description:
** time
*/

#pragma once

#include "ecs.hpp"
#include <chrono>
#include <ratio>

using namespace std::chrono;

namespace cevy::ecs {
class Time {
  private:
  time_point<high_resolution_clock> _first_update;
  time_point<high_resolution_clock> _last_update;
  duration<double, std::ratio<1>> _last_update_delta;

  public:
  duration<double, std::ratio<1>> startup() { return high_resolution_clock::now() - _first_update; }

  void update_with_instant(time_point<high_resolution_clock> &&instant) {
    _last_update_delta = instant - _last_update;
    _last_update = instant;
  }

  duration<double, std::ratio<1>> delta() { return _last_update_delta; }

  double delta_seconds() { return _last_update_delta.count(); }

  Time();
};
} // namespace cevy::ecs

void init_timer(cevy::ecs::World &w);
void update_timer(cevy::ecs::Resource<cevy::ecs::Time> time);
