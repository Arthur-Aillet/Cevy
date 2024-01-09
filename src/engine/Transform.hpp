/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Transform
*/

#pragma once

#include "Vector.hpp"
#include <raylib.h>
#include <raymath.h>

namespace cevy {
namespace engine {
    class Transform;
}
}

class cevy::engine::Transform : public Matrix {
public:
    Transform() : Matrix(MatrixIdentity()) {};
    Transform(Matrix&& m) : Matrix(m) {};
    Transform(const Vector& v) : Matrix(MatrixTranslate(v.x, v.y, v.z)) {};
    ~Transform() {};
    Transform& operator*=(const Transform& other) {
        *this = MatrixMultiply(other, *this);
        return *this;
    }

    Vector operator*(const Vector& v) {
        Vector3 w{v.x, v.y, v.z};
        w = Vector3Transform(w, *this);
        return Vector(w.x, w.y, w.z);
    }

    Transform& rotateXYZ(const Vector& v) {
        *this *= MatrixRotateXYZ(v);
        return *this;
    }

    Transform& rotateAxis(const Vector& v, float angle) {
        *this *= MatrixRotate(v, angle);
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

protected:
private:
};
