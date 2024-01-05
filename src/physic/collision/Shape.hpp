/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** Shape.hpp
*/

#include "Vector.hpp"

namespace cevy::physic::collision {
class Shape {
    public:
        virtual ~Shape(){};

        virtual bool calculateCollision(Shape& other) = 0;
};
class Cuboid : public Shape {
    private:
        cevy::engine::Vector _dimension;
        cevy::engine::Vector _position;
    public:
        Cuboid(double weight, double depth, double height) : _dimension(weight, depth, height) {};

        void setPosition(const cevy::engine::Vector& position) { _position = position; }

        const cevy::engine::Vector& getPosition() const { return _position; }

        const cevy::engine::Vector& getDimension() const { return _dimension; }

        bool calculateCollision(Shape& other) override {
            Cuboid* cube = dynamic_cast<Cuboid*>(&other);
            bool collisionX = _position.x + _dimension.x >= cube->getPosition().x &&
                cube->getPosition().x + cube->getDimension().x >= _position.x;
            bool collisionY = _position.y + _dimension.y >= cube->getPosition().y &&
                cube->getPosition().y + cube->getDimension().y >= _position.y;
            bool collisionZ = _position.z + _dimension.z >= cube->getPosition().z &&
                cube->getPosition().z + cube->getDimension().z >= _position.z;
            return collisionX && collisionY && collisionZ;
        }
};

}