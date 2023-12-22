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

namespace cevy::engine {
    using namespace cevy::ecs;

    class StartupRenderStage : public Schedule::after<Schedule::PreStartup> {};
    class PreStartupRenderStage : public Schedule::before<StartupRenderStage> {};
    class PostStartupRenderStage : public Schedule::after<StartupRenderStage> {};

    class RenderStage : public Schedule::after<Schedule::PostUpdate> {};
    class PreRenderStage : public Schedule::before<RenderStage> {};
    class PostRenderStage : public Schedule::after<RenderStage> {};

    class Engine : public Plugin {
        public:
            void build(App& app);
    };
}
