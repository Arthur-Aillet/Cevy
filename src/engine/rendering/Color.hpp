/*
** Agartha-Software, 2023
** C++evy
** File description:
** Color
*/

#pragma once

#include "raylib.hpp"
#include "network/network.hpp"

namespace cevy::engine {
class Color {
  public:
  unsigned char r; // Color red value
  unsigned char g; // Color green value
  unsigned char b; // Color blue value
  unsigned char a; // Color alpha value

  Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
  operator ::Color();
  operator const ::Color() const;
};
} // namespace cevy::engine

template <>
struct cevy::serialized_size<cevy::engine::Color>
    : public std::integral_constant<size_t, serialized_size<float>::value * 4> {};

template <>
inline std::vector<uint8_t> &cevy::serialize<cevy::engine::Color>(std::vector<uint8_t> &vec,
                                                                   const cevy::engine::Color &t) {
  serialize(vec, t.r);
  serialize(vec, t.g);
  serialize(vec, t.b);
  serialize(vec, t.a);
  return vec;
}

template <>
inline cevy::engine::Color cevy::deserialize<cevy::engine::Color>(std::vector<uint8_t> &vec) {
  cevy::engine::Color t{0, 0, 0, 0};
  t.r = deserialize<float>(vec);
  t.g = deserialize<float>(vec);
  t.b = deserialize<float>(vec);
  t.a = deserialize<float>(vec);
  return t;
}
