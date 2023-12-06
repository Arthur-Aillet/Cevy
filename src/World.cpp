/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** World
*/

#include "World.hpp"

Entity World::spawn_entity()
{
    size_t pos = _entities.first_free();
    Entity new_e = Entity(pos);

    _entities.insert_at(pos, new_e);
    return new_e;
}

Entity World::entity_from_index(std::size_t idx)
{
    Entity new_e = Entity(idx);

    _entities.insert_at(idx, new_e);
    return new_e;
}

void World::kill_entity(Entity const &e)
{
    for (auto const& [type, data] : _components_arrays) {
        std::get<1>(data)(*this, e);
    }
}
