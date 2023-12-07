/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Plugin
*/

#pragma once

#include "ecs.hpp"

class cevy::ecs::Plugin {
    public:
        virtual void build(App&) = 0;
};
