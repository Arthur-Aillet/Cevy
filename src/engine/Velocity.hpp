/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Velocity
*/

#pragma once

#include <raylib.h>
#include <raymath.h>

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
  TransformVelocity(){};
  ~TransformVelocity(){};
  /*
    /// delta scale
    TransformVelocity &operator*=(float s) {
      invalidate();

      _m.m0 = ::lerp(1, _m.m0, s);
      _m.m1 *= s;
      _m.m2 *= s;
      _m.m3 *= s;
      _m.m4 *= s;
      _m.m5 = ::lerp(1, _m.m5, s);
      _m.m6 *= s;
      _m.m7 *= s;
      _m.m8 *= s;
      _m.m9 *= s;
      _m.m10 = ::lerp(1, _m.m10, s);
      _m.m11 *= s;

      _m.m12 *= s;
      _m.m13 *= s;
      _m.m14 *= s;

      _m.m15 = ::lerp(1, _m.m15, s);

      return *this;
    }

      float nx = 0;
      float ny = 0;
      float nz = 0;
      nx += _m.m5 * _m.m5;
      nx += _m.m6 * _m.m6;
      nx += _m.m9 * _m.m9;
      nx += _m.m10 * _m.m10;

      ny += _m.m0 * _m.m0;
      ny += _m.m2 * _m.m2;
      ny += _m.m7 * _m.m7;
      ny += _m.m10 * _m.m10;

      nz += _m.m0 * _m.m0;
      nz += _m.m1 * _m.m1;
      nz += _m.m4 * _m.m4;
      nz += _m.m5 * _m.m5;

      nx = (nx / 2);
      ny = (ny / 2);
      nz = (nz / 2);

      // nx = std::pow(2, -nx);
      // ny = std::pow(2, -ny);
      // nz = std::pow(2, -nz);


      // nx = sqrt(nx);
      // ny = sqrt(ny);
      // nz = sqrt(nz);

      // nx = sqrt(nx);
      // ny = sqrt(ny);
      // nz = sqrt(nz);


      // nx = std::pow(nx, 1. / 256.);
      // ny = std::pow(ny, 1. / 256.);
      // nz = std::pow(nz, 1. / 256.);

      // nx = std::tan(nx);
      // ny = std::tan(ny);
      // nz = std::tan(nz);
      //
      // nx = std::cosh(nx);
      // ny = std::cosh(ny);
      // nz = std::cosh(nz);

      // nx = 1;
      // ny = 1;
      // nz = 1;

      _m.m5 *= nx;
      _m.m6 *= nx;
      _m.m9 *= nx;
      _m.m10 *= nx;

      _m.m0 *= ny;
      _m.m2 *= ny;
      _m.m7 *= ny;
      _m.m10 *= ny;

      _m.m0 *= nz;
      _m.m1 *= nz;
      _m.m4 *= nz;
      _m.m5 *= nz;

      return *this;
    }
    /// delta scale
    TransformVelocity operator*(float s) {
      TransformVelocity ret = *this;
      ret *= s;
      return ret;
    }

    static void system(ecs::Query<engine::Transform, TransformVelocity> q,
                       ecs::Resource<cevy::ecs::Time> time) {
      float delta_t = time.get().delta_seconds() * 60;
      float decay = 0.995;
      for (auto [tm, vel] : q) {
        tm *= vel * delta_t;
        vel *= powf(decay, delta_t);
      }
    }
    */

  protected:
  private:
};
} // namespace cevy::engine
