#include "./entity.hpp"
#include <string>
#include <iostream>

Entity::Entity(std::size_t new_id) : id(new_id)
{
}

Entity::~Entity()
{
}

void Entity::display()
{
    std::cout << this->id << std::endl;
}

Entity::operator std::size_t&()
{
    return this->id;
}

Entity::operator std::size_t() const
{
    return this->id;
}
