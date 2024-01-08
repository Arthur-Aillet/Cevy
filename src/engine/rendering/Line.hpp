/*
** Agartha-Software, 2023
** Cevy
** File description:
** Line
*/

#pragma once

#include "Vector.hpp"

/// /!\ Rotation not supported
namespace cevy::engine {
class Line {
  public:
  Vector start;
  Vector end;

  Line(Vector start, Vector end) : start(start), end(end) {}
};
} // namespace cevy::engine
