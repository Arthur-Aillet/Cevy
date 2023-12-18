/*
** Agartha-Software, 2023
** Cevy
** File description:
** Asset Manager
*/

#pragma once

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

namespace cevy {
    class AssetLoader {
        public:
            template<typename T>
            T get(const std::filesystem::path &path) {
                return std::any_cast<T>(load(path));
            }
        public:
            virtual std::any load(const std::filesystem::path &path) = 0;
            virtual std::vector<std::string> extensions(void) = 0;
    };

    class ModelLoader : public AssetLoader {
        std::any load(const std::filesystem::path &path) {
            Model model = LoadModel(path.c_str());
            return std::make_any<Model>(model);
        }
        std::vector<std::string> extensions(void) {
            return {".obj"};
        }
    };

    class AssetManager {
        private:
            template<typename T>
            using function_type = std::function<ref<std::vector<T>> (AssetManager &)>;
            using map = std::unordered_map<std::type_index, std::any>;

            std::vector<std::shared_ptr<AssetLoader>> _loaders;
            map _assets;

            template<typename T>
            void push_asset(const std::filesystem::path &path) {
                for (auto &loader : _loaders) {
                    auto ext = loader->extensions();

                    if(std::find(ext.begin(), ext.end(), path.extension()) != ext.end()) {
                        std::vector<T> &vec = get_vector<T>();

                        vec.push_back(loader->get<T>(path));
                        return;
                    }
                }
                std::cout << "NOT FOUND" << std::endl;
            }

            template<typename T>
            std::vector<T> &get_vector() {
                auto &any = _assets[std::type_index(typeid(T))];

                return std::any_cast<std::vector<T> &>(any);
            }

            template<typename T>
            void register_type() {
                using vector_type = std::vector<T>;

                vector_type empty = {};
                _assets[std::type_index(typeid(T))] = std::make_any<vector_type>(empty);
            }

        public:
            AssetManager() {
                _loaders.emplace_back(std::make_shared<ModelLoader>(ModelLoader()));
                register_type<Model>();
                push_asset<Model>("resources/church.obj");
                std::cout << "WORKED!: [" << get_vector<Model>().size() << "]" << std::endl;
            };
    };

    class AssetManagerPlugin : public ecs::Plugin {
        public:
            void build(ecs::App &app) {
                app.add_system<cevy::PostStartupRenderStage>(init_asset_manager);
            }
    };
}
