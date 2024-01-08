/*
** Agartha-Software, 2023
** Cevy
** File description:
** Entity
*/

#include "Entity.hpp"

using cevy::ecs::Entity;

Entity::Entity(std::size_t new_id) : _id(new_id) {}

Entity::operator std::size_t &() { return this->_id; }

Entity::operator std::size_t() const { return this->_id; }
