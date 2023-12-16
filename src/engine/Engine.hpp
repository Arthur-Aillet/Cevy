/*
** Agartha-Software, 2023
** Cevy
** File description:
** GameEngine
*/

#pragma once

#include "Plugin.hpp"
#include "ecs.hpp"
#include "Schedule.hpp"

namespace cevy {
    class RenderStage : public cevy::ecs::Schedule::before<cevy::ecs::Schedule::PreUpdate> {};
    class Engine : public cevy::ecs::Plugin {
        public:
            void static build(cevy::ecs::App& app);
    };
}
