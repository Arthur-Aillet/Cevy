/*
** Agartha-Software, 2023
** Cevy
** File description:
** Asset Manager
*/

#include "AssetManager.hpp"

void init_asset_manager(cevy::ecs::World &w) {
  w.insert_resource(cevy::engine::AssetManager());
  auto val = w.get_resource<cevy::engine::AssetManager>();
  if (val) {
    w.insert_resource(cevy::engine::Asset<cevy::engine::Model3D>(val->get()));
  }
}
