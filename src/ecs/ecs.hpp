/*
** Agartha-Software, 2023
** Cevy
** File description:
** ecs namespace
*/

#pragma once

#include "cevy.hpp"

namespace cevy::ecs {
    class App;
    class Entity;
    class Plugin;
    class Schedule;
    class Commands;
    class Command;
    class EntityCommands;
    class World;

    template <typename T>
    class Factory;

template <typename Content>
class Resource;
template <class... Containers>
class Query;
} // namespace cevy::ecs
