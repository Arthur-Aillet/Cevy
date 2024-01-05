/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Collider.hpp
*/

#pragma once
#include "Vector.hpp"
#include "Position.hpp"
#include "Rotation.hpp"
#include "World.hpp"
#include "Query.hpp"

#include "Shape.hpp"
#include "ecs.hpp"
#include <tuple>

namespace cevy::physic::collision {
class Collider : public cevy::engine::Position {

    private:
        Shape* _shape;

    public:
    Collider(Shape *shape) : _shape(shape){};
    ~Collider(){};

    Shape* getShpae() const { return _shape; }

};

enum CELL_SIZE {
    X = 50,
    Y = 50,
    Z = 50,
};

using entity = std::tuple<const Collider, const option<cevy::engine::Position>, const option<cevy::engine::Rotation>>;
struct Cell {
    std::vector<std::tuple<const Collider, const option<cevy::engine::Position>, const option<cevy::engine::Rotation>>> entities;
};

std::vector<std::vector<std::vector<Cell>>> grid;

void updateGrid(ecs::World& world);
void checkCollision(ecs::World& world);

}