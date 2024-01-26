/*
** Agartha-Software, 2023
** Cevy
** File description:
** target
*/

#pragma once

#include "Vector.hpp"

namespace cevy::engine {
class Target : public cevy::engine::Vector {
  using cevy::engine::Vector::Vector;

  public:
  void operator=(const Vector &v) {
    x = v.x;
    y = v.y;
    z = v.z;
  }
};
} // namespace cevy::engine

template <>
struct cevy::serialized_size<cevy::engine::Target>
    : public std::integral_constant<size_t, serialized_size<engine::Vector>::value> {};

template <>
inline std::vector<uint8_t> &cevy::serialize_impl<cevy::engine::Target>(std::vector<uint8_t> &vec,
                                                                   const cevy::engine::Target &t) {
  serialize(vec, engine::Vector(t));
  return vec;
}

template <>
inline cevy::engine::Target cevy::deserialize_impl<cevy::engine::Target>(std::vector<uint8_t> &vec) {
  cevy::engine::Target t;
  t = deserialize<engine::Vector>(vec);;
  return t;
}
