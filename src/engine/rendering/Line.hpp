/*
** Agartha-Software, 2023
** Cevy
** File description:
** Line
*/

#pragma once

#include "Vector.hpp"

namespace cevy::engine {
class Line {
  public:
  Vector start;
  Vector end;

  Line(Vector start, Vector end) : start(start), end(end) {}
  operator Ray() { return Ray{.position = start, .direction = end}; }
};
} // namespace cevy::engine

template <>
struct cevy::serialized_size<cevy::engine::Line>
    : public std::integral_constant<size_t, serialized_size<float>::value * 4> {};

template <>
inline std::vector<uint8_t> &cevy::serialize_impl<cevy::engine::Line>(std::vector<uint8_t> &vec,
                                                                   const cevy::engine::Line &t) {
  serialize(vec, t.start);
  serialize(vec, t.end);
  return vec;
}

template <>
inline cevy::engine::Line cevy::deserialize_impl<cevy::engine::Line>(std::vector<uint8_t> &vec) {
  cevy::engine::Line t{engine::Vector(), engine::Vector()};
  t.start = deserialize<engine::Vector>(vec);
  t.end = deserialize<engine::Vector>(vec);
  return t;
}
