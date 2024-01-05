/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** Collider.cpp
*/

#include "Collider.hpp"
#include "Position.hpp"
#include "Rotation.hpp"
#include "ecs.hpp"
#include <cstdio>
#include <optional>
#include <tuple>

using namespace cevy::physic::collision;

void updateGrid(cevy::ecs::World& world) {
    for (auto& row : grid) {
        for (auto& col : row) {
            for (auto& cell : col) {
                cell.entities.clear();
            }
        }
    }

    cevy::ecs::Query<Collider, option<cevy::engine::Position>, option<cevy::engine::Rotation>> query = cevy::ecs::Query<Collider, option<cevy::engine::Position>, option<cevy::engine::Rotation>>::query(world);
    for (auto it = query.begin(); it != query.end(); ++it) {
        const auto& collider = std::get<0>(*it);
        const auto& position = std::get<1>(*it).value_or(cevy::engine::Position(0, 0,  0));
        const auto& rotation = std::get<2>(*it).value_or(cevy::engine::Rotation(0, 0, 0));
        int x = dynamic_cast<Cuboid*>(collider.getShpae())->getPosition().x / CELL_SIZE::X;
        int y = dynamic_cast<Cuboid*>(collider.getShpae())->getPosition().y / CELL_SIZE::Y;
        int z = dynamic_cast<Cuboid*>(collider.getShpae())->getPosition().z / CELL_SIZE::Z;

        grid[x][y][z].entities.push_back(std::make_tuple(collider, position, rotation));
    }
}

void collisionWithNeighboringEntities(entity& entity) {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dz = -1; dz <= 1; dz++) {
                int x = std::get<1>(entity)->x / CELL_SIZE::X + dx;
                int y = std::get<1>(entity)->y / CELL_SIZE::Y + dy;
                int z = std::get<1>(entity)->z / CELL_SIZE::Z + dz;
                if (x >= 0 && y >= 0 && z >= 0 && x <  static_cast<int>(grid.size()) && y < static_cast<int>(grid[0].size()) && z < static_cast<int>(grid[0][0].size())) {
                    for (auto& entity2 : grid[x][y][z].entities) {
                        if (entity != entity2 && std::get<0>(entity).getShpae()->calculateCollision(*std::get<0>(entity2).getShpae())) {
                            std::cout << "entity 1 in position " << x << ", " << y << ", " << z;
                            std::cout << " have a collision with the entity in position " << std::get<1>(entity2)->x << ", " << std::get<1>(entity2)->y << ", " << std::get<1>(entity2)->z << std::endl;
                        }
                    }
                }
            }
        }
    }
}

void checkCollision(cevy::ecs::World& world) {
    cevy::physic::collision::updateGrid(world);

    for (auto& row : grid) {
        for (auto& col : row) {
            for (auto& cell : col) {
                for (auto& entity : cell.entities) {
                    collisionWithNeighboringEntities(entity);
                }
            }
        }
    }
}