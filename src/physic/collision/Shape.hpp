/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** Shape.hpp
*/

#include "../../engine/Vector.hpp"

namespace cevy::physic::collision {
class Shape {
    public:
        virtual ~Shape(){};

        virtual void calculateCollision() = 0;
};

class Cuboid : public Shape, public cevy::engine::Vector {
    private:
        cevy::engine::Vector _dimension;
    public:
        Cuboid(double weight, double height, double depth) : _dimension(weight, height, depth) {};

        void calculateCollision() override {
            //TODO
        }
};

}