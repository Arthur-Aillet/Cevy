/*
** Agartha-Software, 2023
** Cevy
** File description:
** ecs namespace
*/

#pragma once

#include "../cevy.hpp"

namespace cevy::ecs {
    class App;
    class Entity;
    class Plugin;
    class Schedule;
    class World;

    template <class... Containers>
    class Query;
}
