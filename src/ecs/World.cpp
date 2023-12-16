/*
** Agartha-Software, 2023
** Cevy
** File description:
** World
*/

#include "World.hpp"

using cevy::ecs::World;
using cevy::ecs::Entity;

SparseVector<Entity>& World::entities() {
    return _entities;
}

const SparseVector<Entity>& World::entities() const {
    return _entities;
}


World::EntityWorldRef World::spawn_empty()
{
    size_t pos = _entities.first_free();
    Entity new_e = Entity(pos);

    _entities.insert_at(pos, new_e);
    EntityWorldRef ref = {
        .world = *this,
        .entity = new_e,
    };
    return ref;
}

bool  World::despawn(Entity const &e)
{
    for (auto const& [type, data] : _components_arrays) {
        std::get<1>(data)(*this, e);
    }
    auto& w = _entities[e];
    if (!w)
        return false;
    w = std::nullopt;
    return true;
}

void World::clear_all()
{
    clear_entities();
    clear_resources();
}

void World::clear_entities()
{
    for (auto& e : _entities) {
        if (e)
            despawn(e.value());
    }
}

void World::clear_resources()
{
    _resource_manager.clear_ressources();
}

cevy::ecs::EntityCommands cevy::ecs::Commands::spawn_empty() {
    size_t pos = _world_access._entities.first_free(); // Should be check when multithreading
    Entity new_e = Entity(pos);

    _world_access._entities.insert_at(pos, new_e);
    return (cevy::ecs::EntityCommands(*this, new_e));
}
