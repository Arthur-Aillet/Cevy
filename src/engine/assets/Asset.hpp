/*
** Agartha-Software, 2023
** C++evy
** File description:
** Asset
*/

#pragma once

#include "Diffuse.hpp"
#include "Handle.hpp"
#include "Mesh.hpp"
#include "assets.hpp"
#include "AssetManager.hpp"

namespace cevy::engine {
template <>
class Asset<Diffuse> {
  using Type = Diffuse;

  AssetManager &_ref;

  public:
  Asset<Diffuse>(AssetManager &ref) : _ref(ref){};

  Handle<Diffuse> load(const std::string &path) {
    auto find = _ref._diffuses.find(path);
    if (find != _ref._diffuses.end())
      return Handle<Diffuse>(find);
    return Handle<Diffuse>(_ref._diffuses.emplace(path, Diffuse(LoadTexture(path.c_str()))).first);
  }
};

template <>
class Asset<cevy::engine::Mesh> {
  using Type = Mesh;

  AssetManager &_ref;

  public:
  Asset<Mesh>(AssetManager &ref) : _ref(ref){};

  Handle<Mesh> load(const std::string &path) {
    auto find = _ref._meshs.find(path);
    if (find != _ref._meshs.end())
      return Handle<Mesh>(find);
    return Handle<Mesh>(_ref._meshs.emplace(path, Mesh(LoadModel(path.c_str()))).first);
  }
};

} // namespace cevy::engine
