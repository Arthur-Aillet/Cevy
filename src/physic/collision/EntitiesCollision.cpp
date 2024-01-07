/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** EntitiesCollision.cpp
*/

#include "App.hpp"
#include "Collider.hpp"
#include "Physic.hpp"
#include "Query.hpp"

#include <cstddef>
#include <optional>
#include <tuple>



void cevy::physic::Grid::addEntity(const size_t x,const size_t y,const size_t z, const entity& value) {
        if (x >= _cells.size()) {
            _cells.resize(x + 1);
            _cells[x].value().resize(y + 1);
            _cells[x].value()[y].value().resize(z + 1);
        }
        else if (y >= _cells[x].value().size()) {
            _cells[x].value().resize(y + 1);
            _cells[x].value()[y].value().resize(z + 1);
        }
        else if (z >= _cells[x].value()[y].value().size()) {
            _cells[x].value()[y].value().resize(z + 1);
        }
        if (_cells[x].value()[y].value()[z].value().size() == 0) {
            _cells[x].value()[y].value()[z] = std::vector<entity>();
        }
        _cells[x].value()[y].value()[z].value().push_back(value);

}

void cevy::physic::Grid::setGrid(cevy::ecs::World& world) {
    cevy::ecs::Query<cevy::physic::Collider, option<cevy::engine::Position>, option<cevy::engine::Rotation>> query =
    cevy::ecs::Query<cevy::physic::Collider, option<cevy::engine::Position>, option<cevy::engine::Rotation>>::query(world);
    
    for (auto it = query.begin(); it != query.end(); ++it) {
        const auto& collider = std::get<0>(*it);
        const auto& position = std::get<1>(*it).value_or(cevy::engine::Position(0, 0,  0));
        const auto& rotation = std::get<2>(*it).value_or(cevy::engine::Rotation(0, 0, 0));
        size_t x = dynamic_cast<cevy::physic::Cuboid*>(collider.getShape())->getPosition().x / cevy::physic::CELL_SIZE::X;
        size_t y = dynamic_cast<cevy::physic::Cuboid*>(collider.getShape())->getPosition().y / cevy::physic::CELL_SIZE::Y;
        size_t z = dynamic_cast<cevy::physic::Cuboid*>(collider.getShape())->getPosition().z / cevy::physic::CELL_SIZE::Z;
        auto tuple = std::make_tuple(&collider, &position, &rotation);
        addEntity(x, y, z, tuple);
    }
}

void cevy::physic::Grid::collisionWithNeighboringEntities(const entity& entity1) {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dz = -1; dz <= 1; dz++) {
                int x = std::get<1>(entity1)->x / cevy::physic::CELL_SIZE::X + dx;
                int y = std::get<1>(entity1)->y / cevy::physic::CELL_SIZE::Y + dy;
                int z = std::get<1>(entity1)->z / cevy::physic::CELL_SIZE::Z + dz;
                if (x >= 0 && y >= 0 && z >= 0 && x < static_cast<int>(_cells.size()) && y < static_cast<int>(_cells[0].value().size()) && z < static_cast<int>(_cells[0].value()[0].value().size())) {
                    for (auto& entity2 : _cells[x].value()[y].value()[z].value()) {
                        if (entity1 != entity2 && std::get<0>(entity1)->getShape()->calculateCollision(*std::get<0>(entity2)->getShape())) {
                            std::cout << "entity 1 in position " << x << ", " << y << ", " << z;
                            std::cout << " have a collision with the entity in position " << std::get<1>(entity2)->x << ", " << std::get<1>(entity2)->y << ", " << std::get<1>(entity2)->z << std::endl;
                        }
                    }
                }
            }
        }
    }
}
