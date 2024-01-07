/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** physic.cpp
*/

#include "Physic.hpp"
#include "App.hpp"
#include "DefaultPlugin.hpp"
#include "collision/Collider.hpp"
#include "ecs.hpp"

void checkCollision(cevy::ecs::World& world) {
    cevy::physic::Grid grid;
    grid.setGrid(world);

    for (std::size_t x; x < grid.getCells().size(); x++) {
        for (std::size_t y; y < grid.getCells()[x].value().size(); y++) {
            for (std::size_t z; z < grid.getCells()[x].value()[y].value().size(); z++) {
                if (grid.getCells()[x].value()[y].value()[z] != std::nullopt) {
                    for (auto& entity : grid.getCells()[x].value()[y].value()[z].value()) {
                        grid.collisionWithNeighboringEntities(entity);
                    }
                }
            }
        }
    }
}

void cevy::physic::PhysicsPlugin::build(cevy::ecs::App &app) {
  app.add_plugins(cevy::ecs::DefaultPlugin());
  app.init_component<cevy::engine::Position>();
  app.init_component<cevy::engine::Rotation>();
  app.add_system<cevy::ecs::Schedule::Update>(checkCollision);
}