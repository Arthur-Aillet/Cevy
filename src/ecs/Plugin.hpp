/*
** Agartha-Software, 2023
** Cevy
** File description:
** Plugin
*/

#pragma once

#include "ecs.hpp"

class cevy::ecs::Plugin {
    public:
        virtual void build(cevy::ecs::App&) = 0;
        Plugin() = default;
        ~Plugin() = default;
};
