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

#include "Vector.hpp"
#include "cevy.hpp"
#include "Pointer.hpp"

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
