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

    cevy::ecs::Query<Collider, std::optional<cevy::engine::Position>, std::optional<cevy::engine::Rotation>> query = cevy::ecs::Query<Collider, std::optional<cevy::engine::Position>, std::optional<cevy::engine::Rotation>>::query(world);
    for (auto it = query.begin(); it != query.end(); ++it) {
        auto& collider = std::get<0>(*it);
        auto& position = std::get<1>(*it);
        auto& rotation = std::get<2>(*it);
        int x = dynamic_cast<Cuboid*>(collider.getShpae())->getPosition().x / CELL_SIZE::X;
        int y = dynamic_cast<Cuboid*>(collider.getShpae())->getPosition().y / CELL_SIZE::Y;
        int z = dynamic_cast<Cuboid*>(collider.getShpae())->getPosition().z / CELL_SIZE::Z;

        grid[x][y][z].entities.push_back(std::make_tuple(collider, position, rotation));
    }
}

// void checkCollision(cevy::ecs::World& world) {
//     cevy::physic::collision::updateGrid(world);

//     for (auto& row : grid) {
//         for (auto& col : row) {
//             for (auto& cell : col) {
//                 for (auto& entity : cell.entities) {

//                 }
//             }
//         }
//     }
// }