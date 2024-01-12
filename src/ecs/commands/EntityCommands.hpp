/*
** Agartha-Software, 2023
** C++evy
** File description:
** EntityCommands
*/

#pragma once

#include "Commands.hpp"
#include "ecs.hpp"

class cevy::ecs::EntityCommands {
  private:
  cevy::ecs::Entity _entity;
  cevy::ecs::Commands &_commands;

  friend class cevy::ecs::Commands;
  EntityCommands(cevy::ecs::Commands &commands, cevy::ecs::Entity entity)
      : _entity(entity), _commands(commands){};

  public:
  template <typename... Components>
  cevy::ecs::EntityCommands &insert(const Components &...c) {
    _commands.add(
        [c..., e = _entity](cevy::ecs::World &w) mutable { (w.add_component(e, c), ...); });
    return *this;
  }

  cevy::ecs::Entity id();

  void despawn();

  cevy::ecs::Commands &commands();
};

template <typename... Ts>
cevy::ecs::EntityCommands cevy::ecs::Commands::spawn(Ts... a) {
  return spawn_empty().insert(a...);
}
