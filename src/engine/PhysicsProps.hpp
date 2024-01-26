/*
** EPITECH PROJECT, 2024
** C++evy
** File description:
** PhysicsProps
*/

#pragma once

#include <cstddef>

#include "network/network.hpp"

namespace cevy {
namespace engine {
class PhysicsProps {
  public:
  float mass = 1;
  float decay = 0.005;
  PhysicsProps(){};
  ~PhysicsProps(){};

  PhysicsProps &setDecay(float val) {
    decay = val;
    return *this;
  }

  protected:
  private:
};
} // namespace engine
} // namespace cevy

template <>
struct cevy::serialized_size<cevy::engine::PhysicsProps>
    : public std::integral_constant<size_t, serialized_size<float>::value * 2> {};

template <>
inline std::vector<uint8_t> &cevy::serialize_impl<cevy::engine::PhysicsProps>(std::vector<uint8_t> &vec,
                                                                   const cevy::engine::PhysicsProps &t) {
  serialize(vec, t.mass);
  serialize(vec, t.decay);
  return vec;
}

template <>
inline cevy::engine::PhysicsProps cevy::deserialize_impl<cevy::engine::PhysicsProps>(std::vector<uint8_t> &vec) {
  cevy::engine::PhysicsProps t;
  t.mass = deserialize<float>(vec);
  t.decay = deserialize<float>(vec);
  return t;
}
