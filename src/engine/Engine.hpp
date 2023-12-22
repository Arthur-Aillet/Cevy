/*
** Agartha-Software, 2023
** Cevy
** File description:
** GameEngine
*/

#pragma once

#include "Plugin.hpp"
#include "Schedule.hpp"
#include "ecs.hpp"

namespace cevy {
class StartupRenderStage : public cevy::ecs::Schedule::after<cevy::ecs::Schedule::PreStartup> {};
class PreStartupRenderStage : public cevy::ecs::Schedule::before<StartupRenderStage> {};
class PostStartupRenderStage : public cevy::ecs::Schedule::after<StartupRenderStage> {};

class RenderStage : public cevy::ecs::Schedule::after<cevy::ecs::Schedule::PostUpdate> {};
class PreRenderStage : public cevy::ecs::Schedule::before<RenderStage> {};
class PostRenderStage : public cevy::ecs::Schedule::after<RenderStage> {};

class Engine : public cevy::ecs::Plugin {
  public:
  void build(cevy::ecs::App &app);
};
} // namespace cevy
