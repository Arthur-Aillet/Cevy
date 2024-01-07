/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Shape.cpp
*/

#include "Collider.hpp"

bool cevy::physic::Cuboid::calculateCollision(cevy::physic::Shape& other) {
    cevy::physic::Cuboid* cube = dynamic_cast<cevy::physic::Cuboid*>(&other);
    bool collisionX = _position.x + _dimension.x >= cube->getPosition().x &&
        cube->getPosition().x + cube->getDimension().x >= _position.x;
    bool collisionY = _position.y + _dimension.y >= cube->getPosition().y &&
        cube->getPosition().y + cube->getDimension().y >= _position.y;
    bool collisionZ = _position.z + _dimension.z >= cube->getPosition().z &&
        cube->getPosition().z + cube->getDimension().z >= _position.z;
    return collisionX && collisionY && collisionZ;
}