/*
** Agartha-Software, 2023
** C++evy
** File description:
** Commands
*/

#include "Commands.hpp"
#include "ecs.hpp"

void cevy::ecs::Commands::add(std::function<void(cevy::ecs::World &w)> &&f) {
  _world_access._command_queue.push(f);
}

void cevy::ecs::Commands::despawn(Entity e) { _world_access.despawn(e); }
