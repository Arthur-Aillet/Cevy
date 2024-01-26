/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Velocity
*/

#pragma once

#include <raylib.h>
#include <raymath.h>

#include "PhysicsProps.hpp"
#include "Query.hpp"
#include "Resource.hpp"
#include "Time.hpp"
#include "Transform.hpp"
#include "Vector.hpp"

#ifdef ____cpp_lib_interpolate
using std::lerp;
#else
namespace std {
inline constexpr float lerp(float a, float b, float x) { return a + x * (b - a); }
} // namespace std
using std::lerp;
#endif

namespace cevy::engine {
class Velocity : public Vector {
  public:
  Velocity(){};
  ~Velocity(){};

  protected:
  private:
};

class TransformVelocity : public engine::Transform {
  public:
  TransformVelocity() : engine::Transform(){};
  TransformVelocity(const Transform &tm) : engine::Transform(tm){};
  ~TransformVelocity(){};

  /// delta scale
  TransformVelocity &operator*=(float s) {
    invalidate();

    rotation = QuaternionNlerp(QuaternionIdentity(), rotation, s);
    position *= s;

    return *this;
  }

  /// delta scale
  TransformVelocity operator*(float s) {
    TransformVelocity ret = *this;
    ret *= s;
    return ret;
  }

  static void
  system(ecs::Query<engine::Transform, TransformVelocity, option<cevy::engine::PhysicsProps>> q,
         ecs::Resource<cevy::ecs::Time> time) {
    float delta_t = time.get().delta_seconds();
    for (auto [tm, vel, phys] : q) {
      float decay = 0.995;
      if (phys.has_value())
        decay = 1 - phys.value().decay;
      tm *= vel * delta_t;
      vel *= powf(decay, delta_t);
    }
  }

  protected:
  private:
};
} // namespace cevy::engine

template <>
struct cevy::serialized_size<cevy::engine::TransformVelocity>
    : public std::integral_constant<size_t, serialized_size<engine::Vector>::value + serialized_size<engine::Vector>::value + serialized_size<Quaternion>::value> {};

template <>
inline std::vector<uint8_t> &cevy::serialize_impl<cevy::engine::TransformVelocity>(std::vector<uint8_t> &vec,
                                                                   const cevy::engine::TransformVelocity &t) {
  serialize(vec, t.position);
  serialize(vec, t.rotation);
  serialize(vec, t.scale);
  return vec;
}

template <>
inline cevy::engine::TransformVelocity cevy::deserialize_impl<cevy::engine::TransformVelocity>(std::vector<uint8_t> &vec) {
  cevy::engine::Transform t;
  t.position = deserialize<engine::Vector>(vec);
  t.rotation = deserialize<Quaternion>(vec);
  t.scale = deserialize<engine::Vector>(vec);
  return t;
}
