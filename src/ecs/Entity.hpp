/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Entity
*/

#pragma once

#include <cstddef>

#include "ecs.hpp"

class cevy::ecs::Entity {
    private:
        std::size_t _id;
        explicit Entity(std::size_t new_id);
    public:
        friend class ecs::World;
        operator std::size_t&();
        operator std::size_t() const;
};
