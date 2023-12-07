/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Entity
*/

#include <string>
#include <iostream>
#include "Entity.hpp"

using cevy::ecs::Entity;

Entity::Entity(std::size_t new_id) : _id(new_id)
{
}

Entity::operator std::size_t&()
{
    return this->_id;
}

Entity::operator std::size_t() const
{
    return this->_id;
}
