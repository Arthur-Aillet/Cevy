/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Collider.hpp
*/

#pragma once
#include "Vector.hpp"
#include "SparseVector.hpp"
#include "Position.hpp"
#include "Rotation.hpp"
#include "World.hpp"

#include "Shape.hpp"
#include "ecs.hpp"
#include <tuple>
#include <vector>

namespace cevy::physic {

enum CELL_SIZE {
    X = 50,
    Y = 50,
    Z = 50,
};
class Collider {
    private:
        Shape* _shape;

    public:


    Collider(Shape *shape) : _shape(shape){};
    ~Collider(){};

    Shape* getShape() const { return _shape; }
};

using entity = std::tuple<const Collider*, const cevy::engine::Position*, const cevy::engine::Rotation*>;
class Grid {
    private:
    SparseVector<SparseVector<SparseVector<std::vector<entity>>>> _cells;

    public:
    Grid() {};
    ~Grid() {};

    SparseVector<SparseVector<SparseVector<std::vector<entity>>>> getCells() const { return _cells; };
    void addEntity(const size_t x, const size_t y, const size_t z, const entity& value);
    void setGrid(cevy::ecs::World& world);
    void collisionWithNeighboringEntities(const entity& entity1);
};
}