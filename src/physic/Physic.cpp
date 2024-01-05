/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** physic.cpp
*/

#include "Physic.hpp"
#include "App.hpp"
#include "DefaultPlugin.hpp"
#include "ecs.hpp"

#include "Collider.hpp"

void cevy::physic::PhysicsPlugin::build(cevy::ecs::App &app) {
  app.add_plugins(cevy::ecs::DefaultPlugin());
  app.init_component<cevy::engine::Position>();
  app.init_component<cevy::engine::Rotation>();
  app.add_system<cevy::ecs::Schedule::Update>(collision::checkCollision);
}