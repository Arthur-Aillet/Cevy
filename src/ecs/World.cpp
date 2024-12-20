/*
** Agartha-Software, 2023
** C++evy
** File description:
** World
*/

#include "World.hpp"

using cevy::ecs::Entity;
using cevy::ecs::World;

SparseVector<Entity> &World::entities() { return _entities; }

const SparseVector<Entity> &World::entities() const { return _entities; }

World::EntityWorldRef World::spawn_empty() {
  size_t pos = _entities.first_free();
  Entity new_e = Entity(pos);

  _entities.insert_at(pos, new_e);
  EntityWorldRef ref = {
      .world = *this,
      .entity = new_e,
  };
  return ref;
}

World::EntityWorldRef::operator Entity &() { return entity; };

Entity World::EntityWorldRef::id() { return entity; };

bool World::despawn(const Entity &e) {
  for (auto const &[type, data] : _components_arrays) {
    std::get<1>(data)(*this, e);
  }
  auto &w = _entities[e];
  if (!w)
    return false;
  w = std::nullopt;
  return true;
}

void World::clear_all() {
  clear_entities();
  clear_resources();
}

void World::clear_entities() {
  for (auto &e : _entities) {
    if (e)
      despawn(e.value());
  }
}

void World::clear_resources() { _resource_manager.clear_resources(); }
