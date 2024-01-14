/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** physics.hpp
*/

#pragma once

#include "Plugin.hpp"
#include "Schedule.hpp"
#include "ecs.hpp"

namespace cevy::physics {
using namespace cevy::ecs;
class PhysicsPlugin : public Plugin {
  public:
  void build(App &app);
};
} // namespace cevy::physics
