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
        virtual void build(App&) = 0;
};
