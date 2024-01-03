/*
** Agartha-Software, 2023
** Cevy
** File description:
** Asset Manager
*/

#pragma once

#include "App.hpp"
#include "Diffuse.hpp"
#include "Engine.hpp"
#include "Handle.hpp"
#include "Mesh.hpp"
#include "Plugin.hpp"
#include "ecs.hpp"

#include <vector>

#include <stdlib.h>

void init_asset_manager(cevy::ecs::World &w);

namespace cevy::engine {
class AssetManager {
  public:
  // using map = std::unordered_map<std::type_index, std::any>;
  std::vector<cevy::engine::Mesh> _meshs;
  std::vector<Diffuse> _diffuses;
};

class AssetManagerPlugin : public ecs::Plugin {
  public:
  void build(ecs::App &app);
};
} // namespace cevy::engine
