/*
** Agartha-Software, 2023
** Cevy
** File description:
** Asset Manager
*/

#pragma once

#include "AssetManager.hpp"
#include "ecs.hpp"
#include "Plugin.hpp"
#include "App.hpp"
#include "Engine.hpp"
#include "raylib.h"

#include <any>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <algorithm>

void init_asset_manager(cevy::ecs::World &w);

namespace cevy::engine {
    struct Model3D {
        Model model;
        Texture2D diffuse;
    };

    class AssetManager {
        public:
            //using map = std::unordered_map<std::type_index, std::any>;
            std::vector<Model3D> models;
    };

    template <typename Type>
    class Asset {};

    struct Settings3D {
        std::filesystem::path model;
        std::filesystem::path diffuse;
    };

    template <typename Type>
    class Handle {
        private:
            friend class Asset<Type>;

            using value = Type;


            ref<Type> _ref;

            Handle(Type &ref) : _ref(ref) {};
        public:
            int val = 3;
            Type &get() {
                return _ref;
            }
    };

    template <>
    class Asset<Model3D> {
        using Type = Model3D;

        AssetManager &_ref;

        public:
            Asset<Model3D>(AssetManager &ref) : _ref(ref) {};

            Handle<Model3D> load(const Settings3D &set) {
                Model3D model {
                    .model = LoadModel(set.model.c_str()),
                    .diffuse = LoadTexture(set.diffuse.c_str())
                };
                model.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = model.diffuse;

                _ref.models.push_back(model);
                return Handle<Model3D>(_ref.models[_ref.models.size() - 1]);
            }
    };
}

namespace cevy::engine {
    class AssetManagerPlugin : public ecs::Plugin {
        public:
            void build(ecs::App &app) {
                app.add_system<PostStartupRenderStage>(init_asset_manager);
                app.init_component<Handle<Model3D>>();
            }
    };
}
