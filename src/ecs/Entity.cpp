#include "Entity.hpp"
#include <string>
#include <iostream>

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
