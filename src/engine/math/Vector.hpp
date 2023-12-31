/*
** EPITECH PROJECT, 2023
** raytracer
** File description:
** Vector
*/

#pragma once

#include "raylib.hpp"

namespace cevy::engine {
/**
 * Vector for 3D math
 * implements basic operands, as well as dot product with '*'
 */
class Vector {
  public:
  struct __attribute__((aligned(16))) {
    float x;
    float y;
    float z;
    float w = 0;
  };

  protected:
  public:
  Vector();
  Vector(float x, float y, float z);
  Vector(const Vector &) = default;

  operator Vector3() const { return ((Vector3){(float)x, (float)y, (float)z}); }

  Vector &operator=(const Vector &);

  /**
   * per component multiplication
   */
  Vector scale(const Vector &rhs) const;

  bool operator<(const Vector &rhs) const;
#if __cplusplus >= 202300
  /**
   * Compare magnitude
   */
  auto operator<=>(const Vector &rhs) const;
  bool operator<=(const Vector &rhs) const = default;
  bool operator>(const Vector &rhs) const = default;
  bool operator>=(const Vector &rhs) const = default;
#endif
  Vector &operator+=(const Vector &rhs);
  Vector &operator-=(const Vector &rhs);

  /**
   * Compare exact
   */
  bool operator==(const Vector &rhs) const;

  Vector operator+(const Vector &rhs) const;
  Vector operator-(const Vector &rhs) const;

  /**
   * Cheaply evaluate magnitude squared
   */
  float eval() const;

  /**
   * Length of vector
   */
  float magnitude() const;

  /**
   * Returns a vector of magnitude 1 or 0
   */
  Vector normalize() const;

  /**
   * ensure each component is between 0 and 1
   */
  Vector saturate() const;

  /**
   * ensure each component is between \p min and \p max
   */
  Vector clamp(float min, float max) const;

  /**
   * Scalar product
   */
  Vector operator*(float k) const;
  Vector &operator*=(float k);
  Vector &operator*=(const Vector &rhs) {
    *this = scale(rhs);
    return *this;
  };
  Vector operator/(float k) const;
  Vector &operator/=(float k);
  Vector operator/(const Vector &k) const;

  /**
   * Dot product
   */
  float operator*(const Vector &rhs) const;

  Vector cross(const Vector &rhs) const;

  void rotate(const Vector &rot);
  void rotateR(const Vector &rot);

  Vector reflect(const Vector &normal) const;

  Vector refract(const Vector &normal, float ior1, float ior2) const;
#if __cplusplus >= 202300
  static Vector random(float degree = 1, const Vector &source = Vector(0, 0, 0));
#endif
};
} // namespace cevy::engine
