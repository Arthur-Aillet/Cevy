/*
** Agartha-Software, 2023
** Cevy
** File description:
** Line
*/

#pragma once

#include "Position.hpp"

/// /!\ Rotation not supported
namespace cevy::engine {
class Line {
  public:
  Position start;
  Position end;

  Line(Position start, Position end) : start(start), end(end) {}
};
} // namespace cevy::engine
