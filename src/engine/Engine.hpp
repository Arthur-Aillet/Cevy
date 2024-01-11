/*
** Agartha-Software, 2023
** Cevy
** File description:
** GameEngine
*/

#pragma once

#include "Plugin.hpp"
#include "Stage.hpp"
#include "ecs.hpp"

namespace cevy::engine {
using namespace cevy::ecs;

class StartupRenderStage : public after<PreStartup> {};
class PreStartupRenderStage : public before<StartupRenderStage> {};
class PostStartupRenderStage : public after<StartupRenderStage> {};

class RenderStage : public after<PostUpdate> {};
class PreRenderStage : public before<RenderStage> {};
class PostRenderStage : public after<RenderStage> {};

class Engine : public Plugin {
  public:
  void build(App &app);
};
} // namespace cevy::engine
