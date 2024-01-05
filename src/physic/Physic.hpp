/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** physic.hpp
*/

#pragma once

#include "Plugin.hpp"
#include "Schedule.hpp"
#include "ecs.hpp"

namespace cevy::physic {
using namespace cevy::ecs;
class PhysicsPlugin : public Plugin {
    public:
    void build(App &app);
};

}