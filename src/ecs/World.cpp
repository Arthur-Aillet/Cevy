/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** World
*/

#include "World.hpp"

using cevy::ecs::World;
using cevy::ecs::Entity;

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

Entity World::spawn_at(std::size_t idx)
{
    Entity new_e = Entity(idx);

    _entities.insert_at(idx, new_e);
    return new_e;
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
    /* implement */
}
