/*
** Agartha-Software, 2023
** Cevy
** File description:
** Commands
*/

#include "Commands.hpp"
#include "EntityCommands.hpp"

using cevy::ecs::Commands;
using cevy::ecs::EntityCommands;

void cevy::ecs::Commands::add(std::function<void(cevy::ecs::World &w)> &&f) {
  _world_access._command_queue.push(f);
}

EntityCommands Commands::entity(const cevy::ecs::Entity& e) {
    return EntityCommands(*this, e);
}
