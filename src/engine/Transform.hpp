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

namespace cevy {
namespace engine {
class Transform {
  public:
  Quaternion rotation;
  Vector position;

  Transform() : rotation(QuaternionIdentity()), position(0, 0, 0) {}
  Transform(float x, float y, float z) : rotation(QuaternionIdentity()), position(x, y, z) {}
  Transform(const Vector &vec) : rotation(QuaternionIdentity()), position(vec) {}
  Transform(const Quaternion &quat) : rotation(quat), position(0, 0, 0) {}

  std::tuple<Vector, Quaternion> get() const {
    if (!cache_valid()) {
      if (_parent) {
        auto [p_vec, p_rot] = _parent->get();
        _cache_vector = p_vec;
        _cache_quaternion = QuaternionMultiply(p_rot, _cache_quaternion);
        auto v = Vector3RotateByQuaternion(position, _cache_quaternion);
        _cache_vector += Vector(v);
      } else {
        _cache_quaternion = rotation;
        _cache_vector = Vector3RotateByQuaternion(position, _cache_quaternion);
      }
    }
    return {_cache_vector, _cache_quaternion};
  }

  Transform &operator*=(const Transform &other) {
    invalidate();
    auto [vec, rot] = other.get();
    rotation = QuaternionMultiply(rotation, rot);
    position += vec;
    return *this;
  }

  Vector operator*(const Vector &v) const {
    Vector w = v;
    auto [vec, rot] = get();
    w = Vector3RotateByQuaternion(w, rot);
    w += vec;
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
    position.y = y;
    return *this;
  }

  Transform &translateZ(float z) {
    invalidate();
    position.z = z;
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

  Vector euler() const {
    auto v = QuaternionToEuler(rotation);
    return {v.x, v.y, v.z};
  }

  Vector xyz() const { return position; }

  Vector fwd() const {
    Vector3 v{0, 0, 1};
    auto [vec, rot] = get();
    v = Vector3RotateByQuaternion(v, rot);
    return v;
  }

  Vector tan() const {
    Vector3 v{1, 0, 0};
    auto [vec, rot] = get();
    v = Vector3RotateByQuaternion(v, rot);
    return v;
  }

  Vector cotan() const {
    Vector3 v{0, 1, 0};
    auto [vec, rot] = get();
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
  mutable Quaternion _cache_quaternion;
  mutable bool _cache_validity;
};
} // namespace engine
} // namespace cevy
