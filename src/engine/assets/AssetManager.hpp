/*
** Agartha-Software, 2023
** C++evy
** File description:
** Asset Manager
*/

#pragma once

#include <unordered_map>
#include <vector>

#include "assets.hpp"
#include "App.hpp"
#include "Diffuse.hpp"
#include "Engine.hpp"
#include "Handle.hpp"
#include "Mesh.hpp"
#include "Plugin.hpp"
#include "ecs.hpp"

void init_asset_manager(cevy::ecs::World &w);

namespace cevy::engine {
class AssetManager {
  public:
  // using map = std::unordered_map<std::type_index, std::any>;
  std::unordered_map<std::string, cevy::engine::Mesh> _meshs;
  std::unordered_map<std::string, Diffuse> _diffuses;
};

class AssetManagerPlugin : public ecs::Plugin {
  public:
  void build(ecs::App &app);
};
} // namespace cevy::engine
