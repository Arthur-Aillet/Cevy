/*
** AgarthaSoftware, 2024
** Cevy
** File description:
** Transform
*/

#pragma once

#include <algorithm>
#include <raylib.h>
#include <raymath.h>
#include <string>

#include "Pointer.hpp"
#include "Vector.hpp"
#include "cevy.hpp"

namespace cevy {
namespace engine {
class Transform {
  public:
  Quaternion rotation;
  Vector position;

  Transform() : rotation(QuaternionIdentity()), position({0, 0, 0}) {}
  Transform(float x, float y, float z) : rotation(QuaternionIdentity()), position({x, y, z}) {}
  Transform(const Vector &vec) : rotation(QuaternionIdentity()), position({vec.x, vec.y, vec.z}) {}
  Transform(const Quaternion &quat) : rotation(quat), position({0, 0, 0}) {}

  Transform &rotateX(float deg) {
    rotation = QuaternionMultiply(rotation, QuaternionFromEuler(deg, 0, 0));
    return *this;
  }

  Transform &rotateY(float deg) {
    rotation = QuaternionMultiply(rotation, QuaternionFromEuler(0, deg, 0));
    return *this;
  }

  Transform &rotateZ(float deg) {
    rotation = QuaternionMultiply(rotation, QuaternionFromEuler(0, 0, deg));
    return *this;
  }

  Transform &rotateXYZ(float x, float y, float z) {
    rotation = QuaternionMultiply(rotation, QuaternionFromEuler(x, y, z));
    return *this;
  }

  Transform &rotateXYZ(const Vector &vec) {
    rotation = QuaternionMultiply(rotation, QuaternionFromEuler(vec.x, vec.y, vec.z));
    return *this;
  }

  Transform &setRotationX(float deg) {
    rotation = QuaternionFromEuler(deg, 0, 0);
    return *this;
  }

  Transform &setRotationY(float deg) {
    rotation = QuaternionFromEuler(0, deg, 0);
    return *this;
  }

  Transform &setRotationZ(float deg) {
    rotation = QuaternionFromEuler(0, 0, deg);
    return *this;
  }

  Transform &setRotationXYZ(float x, float y, float z) {
    rotation = QuaternionFromEuler(x, y, z);
    return *this;
  }

  Transform &setRotationXYZ(const Vector &vec) {
    rotation = QuaternionFromEuler(vec.x, vec.y, vec.z);
    return *this;
  }

  Transform &translateX(float x) {
    position.x += x;
    return *this;
  }

  Transform &translateY(float y) {
    position.y = y;
    return *this;
  }

  Transform &translateZ(float z) {
    position.z = z;
    return *this;
  }

  Transform &translateXYZ(float x, float y, float z) {
    position += Vector(x, y, z);
    return *this;
  }

  Transform &translateXYZ(const Vector &vec) {
    position += vec;
    return *this;
  }

  Transform &setPositionX(float x) {
    position.x = x;
    return *this;
  }

  Transform &setPositionY(float y) {
    position.y = y;
    return *this;
  }

  Transform &setPositionZ(float z) {
    position.z = z;
    return *this;
  }

  Transform &setPositionXYZ(float x, float y, float z) {
    position = Vector(x, y, z);
    return *this;
  }

  Transform &setPositionXYZ(const Vector &vec) {
    position = vec;
    return *this;
  }
};
} // namespace engine
} // namespace cevy
