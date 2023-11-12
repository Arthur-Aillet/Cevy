#include "./entity.hpp"
#include <string>

Entity::Entity(std::size_t new_id) : id(new_id)
{
}

Entity::~Entity()
{
}


Entity::operator std::size_t() const
{
    return this->id;
}
