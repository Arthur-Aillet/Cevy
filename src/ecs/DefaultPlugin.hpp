/*
** Agartha-Software, 2023
** Cevy
** File description:
** Default Plugin
*/

#pragma once

#include "ecs.hpp"
#include "Plugin.hpp"

namespace cevy::ecs {
    class DefaultPlugin : public cevy::ecs::Plugin {
        public:
            void build(cevy::ecs::App&);
    };
}
