/*
** AgarthaSoftware, 2024
** Cevy
** File description:
** Transform
*/

#pragma once

#include <raylib.h>
#include <raymath.h>

#include "Pointer.hpp"
#include "Vector.hpp"

#include "network/network.hpp"

namespace cevy {
namespace engine {
class Transform {
  public:
  Vector position;
  Quaternion rotation;
  Vector scale;

  Transform() :  position(0, 0, 0), rotation(QuaternionIdentity()), scale(1, 1, 1) {}
  Transform(float x, float y, float z)
      :  position(x, y, z), rotation(QuaternionIdentity()), scale(1, 1, 1) {}
  Transform(const Vector &vec) :  position(vec), rotation(QuaternionIdentity()), scale(1, 1, 1) {}
  Transform(const Quaternion &quat) : position(0, 0, 0), rotation(quat), scale(1, 1, 1) {}

  std::tuple<Vector, Quaternion, Vector> get() const {
    return {position, rotation, scale};
    if (!cache_valid()) {
      if (_parent) {
        auto [p_vec, p_rot, p_scale] = _parent->get();
        _cache_vector = p_vec;
        _cache_scale = p_scale;
        _cache_quaternion = QuaternionMultiply(p_rot, _cache_quaternion);
        auto v = Vector3RotateByQuaternion(position, _cache_quaternion);
        _cache_vector += Vector(v);
      } else {
        _cache_quaternion = rotation;
        _cache_vector = Vector3RotateByQuaternion(position, _cache_quaternion);
        _cache_scale = scale;
      }
    }
    return {_cache_vector, _cache_quaternion, _cache_scale};
  }

  Transform &operator*=(const Transform &other) {
    invalidate();
    auto [vec, rot, sca] = other.get();
    rotation = QuaternionMultiply(rotation, rot);
    position += vec;
    // scale += sca;
    return *this;
  }

  Vector operator*(const Vector &v) const {
    Vector w = v;
    auto [vec, rot, scale] = get();
    w = Vector3RotateByQuaternion(w, rot);
    w += vec;
    // w *= scale;
    return w;
  }

  Transform &rotateX(float deg) {
    invalidate();
    rotation = QuaternionMultiply(rotation, QuaternionFromEuler(deg, 0, 0));
    return *this;
  }

  Transform &rotateY(float deg) {
    invalidate();
    rotation = QuaternionMultiply(rotation, QuaternionFromEuler(0, deg, 0));
    return *this;
  }

  Transform &rotateZ(float deg) {
    invalidate();
    rotation = QuaternionMultiply(rotation, QuaternionFromEuler(0, 0, deg));
    return *this;
  }

  Transform &rotateXYZ(float x, float y, float z) {
    invalidate();
    rotation = QuaternionMultiply(rotation, QuaternionFromEuler(x, y, z));
    return *this;
  }

  Transform &rotateXYZ(const Vector &vec) {
    invalidate();
    rotation = QuaternionMultiply(rotation, QuaternionFromEuler(vec.x, vec.y, vec.z));
    return *this;
  }

  Transform &setRotationX(float deg) {
    invalidate();
    rotation = QuaternionFromEuler(deg, 0, 0);
    return *this;
  }

  Transform &setRotationY(float deg) {
    invalidate();
    rotation = QuaternionFromEuler(0, deg, 0);
    return *this;
  }

  Transform &setRotationZ(float deg) {
    invalidate();
    rotation = QuaternionFromEuler(0, 0, deg);
    return *this;
  }

  Transform &setRotationXYZ(float x, float y, float z) {
    invalidate();
    rotation = QuaternionFromEuler(x, y, z);
    return *this;
  }

  Transform &setRotationXYZ(const Vector &vec) {
    invalidate();
    rotation = QuaternionFromEuler(vec.x, vec.y, vec.z);
    return *this;
  }

  Transform &translateX(float x) {
    invalidate();
    position.x += x;
    return *this;
  }

  Transform &translateY(float y) {
    invalidate();
    position.y += y;
    return *this;
  }

  Transform &translateZ(float z) {
    invalidate();
    position.z += z;
    return *this;
  }

  Transform &translateXYZ(float x, float y, float z) {
    invalidate();
    position += Vector(x, y, z);
    return *this;
  }

  Transform &translateXYZ(const Vector &vec) {
    invalidate();
    position += vec;
    return *this;
  }

  Transform &setPositionX(float x) {
    invalidate();
    position.x = x;
    return *this;
  }

  Transform &setPositionY(float y) {
    invalidate();
    position.y = y;
    return *this;
  }

  Transform &setPositionZ(float z) {
    invalidate();
    position.z = z;
    return *this;
  }

  Transform &setPositionXYZ(float x, float y, float z) {
    invalidate();
    position = Vector(x, y, z);
    return *this;
  }

  Transform &setPositionXYZ(const Vector &vec) {
    invalidate();
    position = vec;
    return *this;
  }

  Transform &scaleX(float deg) {
    invalidate();
    scale.x *= deg;
    return *this;
  }

  Transform &scaleY(float deg) {
    invalidate();
    scale.y *= deg;
    return *this;
  }

  Transform &scaleZ(float deg) {
    invalidate();
    scale.z *= deg;
    return *this;
  }

  Transform &scaleXYZ(float x, float y, float z) {
    invalidate();
    scale *= Vector(x, y, z);
    return *this;
  }

  Transform &scaleXYZ(const Vector &vec) {
    invalidate();
    scale *= vec;
    return *this;
  }

  Transform &scaleXYZ(float deg) {
    invalidate();
    scale.x *= deg;
    scale.y *= deg;
    scale.z *= deg;
    return *this;
  }

  Transform &setScaleX(float deg) {
    invalidate();
    scale.x = deg;
    return *this;
  }

  Transform &setScaleY(float deg) {
    invalidate();
    scale.y = deg;
    return *this;
  }

  Transform &setScaleZ(float deg) {
    invalidate();
    scale.z = deg;
    return *this;
  }

  Transform &setScaleXYZ(float x, float y, float z) {
    invalidate();
    scale = Vector{x, y, z};
    return *this;
  }

  Transform &setScaleXYZ(const Vector &vec) {
    invalidate();
    scale = vec;
    return *this;
  }

  Transform &setScaleXYZ(float deg) {
    invalidate();
    scale.x = deg;
    scale.y = deg;
    scale.z = deg;
    return *this;
  }

  Vector euler() const {
    auto v = QuaternionToEuler(rotation);
    return {v.x, v.y, v.z};
  }

  Vector xyz() const { return position; }

  Vector fwd() const {
    Vector3 v{0, 0, 1};
    auto [vec, rot, sc] = get();
    v = Vector3RotateByQuaternion(v, rot);
    return v;
  }

  Vector up() const {
    Vector3 v{0, 1, 0};
    auto [vec, rot, sc] = get();
    v = Vector3RotateByQuaternion(v, rot);
    return v;
  }

  Vector right() const {
    Vector3 v{0, 1, 0};
    auto [vec, rot, _] = get();
    v = Vector3RotateByQuaternion(v, rot);
    return v;
  }

  Vector tan() const {
    Vector3 v{1, 0, 0};
    auto [vec, rot, _] = get();
    v = Vector3RotateByQuaternion(v, rot);
    return v;
  }

  Vector cotan() const {
    Vector3 v{0, 1, 0};
    auto [vec, rot, _] = get();
    v = Vector3RotateByQuaternion(v, rot);
    return v;
  }

  Transform &parent(const Transform &other) {
    invalidate();
    _parent = pointer<Transform>(other._lock, other);
    return *this;
  }

  protected:
  inline void invalidate() const { _cache_validity = false; }

  inline bool cache_valid() const {
    if (!_cache_validity) {
      return false;
    }
    if (_parent)
      return _parent->cache_valid();
    else
      return true;
  }

  pointer<Transform> _parent;
  std::shared_ptr<int> _lock = std::make_shared<int>();
  mutable Vector _cache_vector;
  mutable Vector _cache_scale;
  mutable Quaternion _cache_quaternion;
  mutable bool _cache_validity;
};
} // namespace engine
} // namespace cevy

template <>
struct cevy::serialized_size<Quaternion>
    : public std::integral_constant<size_t, 4 * serialized_size<float>::value> {};

template <>
inline std::vector<uint8_t> &cevy::serialize_impl<Quaternion>(std::vector<uint8_t> &vec,
                                                                   const Quaternion &t) {
  serialize(vec, t.x);
  serialize(vec, t.y);
  serialize(vec, t.z);
  serialize(vec, t.w);
  return vec;
}

template <>
inline Quaternion cevy::deserialize_impl<Quaternion>(std::vector<uint8_t> &vec) {
  Quaternion t;
  t.x = deserialize<float>(vec);
  t.y = deserialize<float>(vec);
  t.z = deserialize<float>(vec);
  t.w = deserialize<float>(vec);
  return t;
}

template <>
struct cevy::serialized_size<cevy::engine::Transform>
    : public std::integral_constant<size_t, serialized_size<engine::Vector>::value + serialized_size<engine::Vector>::value + serialized_size<Quaternion>::value> {};

template <>
inline std::vector<uint8_t> &cevy::serialize_impl<cevy::engine::Transform>(std::vector<uint8_t> &vec,
                                                                   const cevy::engine::Transform &t) {
  serialize(vec, t.position);
  serialize(vec, t.rotation);
  serialize(vec, t.scale);
  return vec;
}

template <>
inline cevy::engine::Transform cevy::deserialize_impl<cevy::engine::Transform>(std::vector<uint8_t> &vec) {
  cevy::engine::Transform t;
  t.position = deserialize<engine::Vector>(vec);
  t.rotation = deserialize<Quaternion>(vec);
  t.scale = deserialize<engine::Vector>(vec);
  return t;
}
