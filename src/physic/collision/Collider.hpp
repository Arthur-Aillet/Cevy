/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Collider.hpp
*/

#pragma once
#include "Vector.hpp"
#include "Position.hpp"

#include "Shape.hpp"

namespace cevy::physic::collision {

class Collider : public cevy::engine::Position {

    private:
        Shape* _shape;

    public:
    Collider(Shape *shape) : _shape(shape){};
    ~Collider(){};

};
}