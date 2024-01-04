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
        cevy::engine::Position _pos;

    public:
    Collider(cevy::engine::Position pos, Shape *shape) : _pos(pos), _shape(shape){};
    ~Collider(){};

};
}