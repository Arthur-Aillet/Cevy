/*
** Agartha-Software, 2023
** C++evy
** File description:
** Queries
*/

#pragma once

#include "ecs.hpp"
#include <any>
#include <optional>
#include <typeindex>
#include <unordered_map>

namespace cevy::ecs {
class ResourceManager;
}

template <typename Content>
class cevy::ecs::Resource {
  private:
  Content &_content;
  friend class cevy::ecs::ResourceManager;
  Resource(Content &content) : _content(content){};

  public:
  using value = Content;
  operator Content() const { return _content; };
  operator Content &() { return _content; };
  Content &get() { return _content; };
};

template <class T>
struct is_resource : public std::false_type {};

template <typename T>
struct is_resource<cevy::ecs::Resource<T>> : public std::true_type {};

namespace cevy::ecs {
class ResourceManager {
  private:
  using resource_type = std::any;

  std::unordered_map<std::type_index, resource_type> _resources_map;

  public:
  void clear_resources() { _resources_map.clear(); }

  template <typename Content>
  void insert_resource(const Content &value) {
    auto a = std::make_any<Content>(value);

    _resources_map[std::type_index(typeid(Content))] = a;
  }

  template <typename Content>
  std::optional<Content> remove_resource() {
    auto it = _resources_map.find(std::type_index(typeid(Content)));

    if (it != _resources_map.end()) {
      Content val = std::any_cast<Content>(_resources_map[std::type_index(typeid(Content))]);

      _resources_map.erase(it);
      return std::optional<Content>(val);
    }
    return std::nullopt;
  }

  template <typename Content>
  Content &resource() {
    return std::any_cast<Content &>(_resources_map[std::type_index(typeid(Content))]);
  }

  template <typename Content>
  const Content &resource() const {
    return std::any_cast<const Content &>(_resources_map.at(std::type_index(typeid(Content))));
  }

  template <typename Content>
  std::optional<std::reference_wrapper<Content>> get_resource() {
    auto it = _resources_map.find(std::type_index(typeid(Content)));

    if (it != _resources_map.end())
      return std::any_cast<Content &>(_resources_map[std::type_index(typeid(Content))]);
    return std::nullopt;
  }

  template <typename Content>
  cevy::ecs::Resource<Content> get() {
    return cevy::ecs::Resource(
        std::any_cast<Content &>(_resources_map[std::type_index(typeid(Content))]));
  }

  template <typename Content>
  bool contains_resource() {
    return _resources_map.find(std::type_index(typeid(Content))) != _resources_map.end();
  }
};
} // namespace cevy::ecs
