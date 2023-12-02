#include "entity.hpp"
#include <string>
#include <iostream>

entity::entity(std::size_t new_id) : _id(new_id)
{
}

entity::operator std::size_t&()
{
    return this->_id;
}

entity::operator std::size_t() const
{
    return this->_id;
}
