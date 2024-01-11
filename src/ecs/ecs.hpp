/*
** Agartha-Software, 2023
** Cevy
** File description:
** ecs namespace
*/

#pragma once

#include "cevy.hpp"

namespace cevy {
namespace ecs {
class App;
class Entity;
class Plugin;
class Scheduler;
class Commands;
class Command;
class EntityCommands;
class World;

template <typename Content>
class Resource;
template <class... Containers>
class Query;
} // namespace ecs
} // namespace cevy
