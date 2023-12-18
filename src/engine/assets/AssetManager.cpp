/*
** Agartha-Software, 2023
** Cevy
** File description:
** Asset Manager
*/

#include "AssetManager.hpp"

void init_asset_manager(cevy::ecs::World &w) {
    w.insert_resource(cevy::AssetManager());
}
