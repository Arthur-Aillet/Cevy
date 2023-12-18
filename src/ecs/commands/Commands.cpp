/*
** Agartha-Software, 2023
** Cevy
** File description:
** Commands
*/

#include "Commands.hpp"

void cevy::ecs::Commands::add(std::function<void (cevy::ecs::World &w)>&& f) {
    _world_access._command_queue.push(f);
}
