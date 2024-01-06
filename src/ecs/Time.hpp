/*
** Agartha-Software, 2023
** Cevy
** File description:
** time
*/

#pragma once

#include <chrono>

using namespace std::chrono;

namespace cevy::ecs {
class Time {
  private:
  time_point<system_clock, nanoseconds> _start;

  public:
  Time();
};
} // namespace cevy::ecs
