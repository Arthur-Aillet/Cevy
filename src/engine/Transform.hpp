/*
** AgarthaSoftware, 2024
** Cevy
** File description:
** Transform
*/

#pragma once

#include <raylib.h>
#include <raymath.h>
#include <string>
#include <algorithm>

#include "Pointer.hpp"
#include "Vector.hpp"
#include "cevy.hpp"

namespace cevy {
namespace engine {
class Transform;
}
} // namespace cevy

class cevy::engine::Transform {
  public:
  Transform() : _m(MatrixIdentity()){};
  Transform(Matrix &&m) : _m(m){};
  Transform(const Vector &v) : _m(MatrixTranslate(v.x, v.y, v.z)){};
  ~Transform(){};

  inline operator const Matrix &() const {
    if (!cache_valid()) {
      if (_parent)
        _cache = MatrixMultiply(_m, *_parent);
      else
        _cache = _m;
    }
    return _cache;
  }

  Transform &operator*=(const Transform &other) {
    invalidate();
    _m = MatrixMultiply(other, _m);
    return *this;
  }

  Vector operator*(const Vector &v) const {
    Vector3 w{v.x, v.y, v.z};
    w = Vector3Transform(w, *this);
    return Vector(w.x, w.y, w.z);
  }

  Transform &rotateXYZ(const Vector &v) {
    invalidate();
    *this *= MatrixRotateXYZ(v);
    return *this;
  }

  Transform &rotateAxis(const Vector &v, float angle) {
    invalidate();
    *this *= MatrixRotate(v, angle);
    return *this;
  }

  Transform &translateXYZ(const Vector &v) {
    invalidate();
    *this *= MatrixTranslate(v.x, v.y, v.z);
    return *this;
  }

  Transform &translateAxis(Vector v, float distance) {
    invalidate();
    v = v.normalize() * distance;
    *this *= MatrixTranslate(v.x, v.y, v.z);
    return *this;
  }

  Vector euler() const {
    auto q = QuaternionFromMatrix(*this);
    auto v = QuaternionToEuler(q);
    return {v.x, v.y, v.z};
  }

  Vector xyz() const {
    const Matrix &m = *this;
    return { m.m12, m.m13, m.m14 };
  }

  Vector fwd() const {
    Vector3 v{0, 0, 1};
    v = Vector3Transform(v, *this);
    return Vector(v.x, v.y, v.z);
  }

  Vector tan() const {
    Vector3 v{1, 0, 0};
    v = Vector3Transform(v, *this);
    return Vector(v.x, v.y, v.z);
  }

  Vector cotan() const {
    Vector3 v{0, 1, 0};
    v = Vector3Transform(v, *this);
    return Vector(v.x, v.y, v.z);
  }

  Transform &parent(const Transform &other) {
    _parent = pointer<Transform>(other._lock, other);
    return *this;
  }

  protected:
  template<typename F>
  Transform &map(F f) {
    _m.m0  = f(_m.m0);
    _m.m1  = f(_m.m1);
    _m.m2  = f(_m.m2);
    _m.m3  = f(_m.m3);
    _m.m4  = f(_m.m4);
    _m.m5  = f(_m.m5);
    _m.m6  = f(_m.m6);
    _m.m7  = f(_m.m7);
    _m.m8  = f(_m.m8);
    _m.m9  = f(_m.m9);
    _m.m10 = f(_m.m10);
    _m.m11 = f(_m.m11);
    _m.m12 = f(_m.m12);
    _m.m13 = f(_m.m13);
    _m.m14 = f(_m.m14);
    _m.m15 = f(_m.m15);
    return *this;
  }

  template<typename F, typename... T>
  static Transform map(F f, T... t) {
    Transform ret;
    ret._m.m0  = f(t._m.m0...);
    ret._m.m1  = f(t._m.m1...);
    ret._m.m2  = f(t._m.m2...);
    ret._m.m3  = f(t._m.m3...);
    ret._m.m4  = f(t._m.m4...);
    ret._m.m5  = f(t._m.m5...);
    ret._m.m6  = f(t._m.m6...);
    ret._m.m7  = f(t._m.m7...);
    ret._m.m8  = f(t._m.m8...);
    ret._m.m9  = f(t._m.m9...);
    ret._m.m10 = f(t._m.m10...);
    ret._m.m11 = f(t._m.m11...);
    ret._m.m12 = f(t._m.m12...);
    ret._m.m13 = f(t._m.m13...);
    ret._m.m14 = f(t._m.m14...);
    ret._m.m15 = f(t._m.m15...);
    return ret;
  }

  static Transform lerp(const Transform& a, const Transform& b, float s) {
    Transform a1 = a;
    Transform b1 = b;

    a1.map([s](float x) -> float { return x * (1 - s); });
    b1.map([s](float x) -> float { return x * s; });

    a1 = Transform::map([](auto a, auto b){return a + b;}, a1, b1);
    return a1;
  }

  float determinant() const {
    return MatrixDeterminant(_m);
  }

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
  Matrix _m;
  mutable Matrix _cache;
  mutable bool _cache_validity;

  private:
};
