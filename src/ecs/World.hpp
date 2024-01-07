/*
** Agartha-Software, 2023
** Cevy
** File description:
** World
*/

#pragma once

/**
 * @file
 * @author several
 *
 * @section DESCRIPTION
 * World. Holds the actual components and entities
 */

#include "Entity.hpp"
#include "Event.hpp"
#include "Resource.hpp"
#include "SparseVector.hpp"
#include "cevy.hpp"

#include <any>
#include <cstddef>
#include <functional>
#include <queue>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include "Query.hpp"
#include "ecs.hpp"

template <class T>
struct is_world : public std::false_type {};

template <>
struct is_world<cevy::ecs::World &> : public std::true_type {};

template <>
struct is_world<const cevy::ecs::World &> : public std::true_type {};

template <typename... T>
struct Or : std::integral_constant<bool, any<T...>()> {};

namespace cevy {
namespace ecs {
class Commands;

struct Control {
  bool abort;
};
} // namespace ecs
} // namespace cevy

template <class T>
struct is_commands : public std::false_type {};

template <>
struct is_commands<cevy::ecs::Commands> : public std::true_type {};

/**
 * Stores Entities, Components (and resources), and exposes operations
 *
 * Each entity has Components, which must first each be registered to the World
 * An Entity can only have one instance of a Component
 */
class cevy::ecs::World {
  using value_type = World;

  public:
  struct EntityWorldRef {
    World &world;
    Entity entity;

    // EntityWorldRef insert(Bundle& b);

    template <typename... Ts>
    EntityWorldRef insert(Ts... args);

    operator Entity &() { return entity; };
  };

  using erase_access = std::function<void(World &, Entity const &)>;
  using command = std::function<void(World &)>;
  using component_data = std::tuple<std::any, erase_access>;

  friend class cevy::ecs::Schedule;
  friend class cevy::ecs::Commands;
  std::queue<command> _command_queue;
  /* Bevy-compliant */
  public:
  /// @brief Id refering to a specific component
  using ComponentId = std::type_index;

  private:
  std::unordered_map<std::type_index, component_data> _components_arrays;
  SparseVector<Entity> _entities;
  cevy::ecs::ResourceManager _resource_manager;

  /* Bevy-compliant */
  public:
  /// get all entities
  SparseVector<Entity> &entities();

  /// get all entities
  const SparseVector<Entity> &entities() const;

  /// create a new empty entity
  EntityWorldRef spawn_empty();

  /// delete an entity and all its components
  bool despawn(const Entity &entity);

  /// clears all resources and entities
  void clear_all();

  /// clears all entities and their components
  void clear_entities();

  /// clears all resources registered to the world
  void clear_resources();

  /// spawn an entity with defined components
  template <typename... Components>
  EntityWorldRef spawn(Components... c) {
    return spawn_empty().insert(c...);
  }

  /// get a Component T associated with a given Entity, or Nothing if no such
  /// Component
  template <typename T>
  std::optional<ref<T>> get(Entity entity) {
    SparseVector<T> &v = std::any_cast<SparseVector<T> &>(
        std::get<0>(_components_arrays[std::type_index(typeid(T))]));
    std::optional<T> optional = v[entity];

    if (optional)
      return std::optional<ref<T>>(std::ref(optional.value()));
    else
      return std::optional<ref<T>>(std::ref(std::nullopt));
  }

  /// get a Component T associated with a given Entity, or Nothing if no such
  /// Component
  template <typename T>
  std::optional<ref<const T>> get(Entity entity) const {
    const SparseVector<T> &v = std::any_cast<SparseVector<T> &>(
        std::get<0>(_components_arrays.at(std::type_index(typeid(T)))));
    std::optional<const T> optional = v[entity];

    if (optional)
      return std::optional<ref<const T>>(std::ref(optional.value()));
    else
      return std::optional<ref<const T>>(std::ref(std::nullopt));
  }

  /// register a component to the world
  template <typename T>
  ComponentId init_component() {
    erase_access f_e = [](World &reg, Entity const &Entity) {
      auto &cmpnts = reg.get_components<T>();
      if (Entity < cmpnts.size())
        cmpnts[Entity] = std::nullopt;
    };
    std::any a = std::make_any<SparseVector<T>>();

    _components_arrays.insert({std::type_index(typeid(T)), std::make_tuple(a, f_e)});

    return std::type_index(typeid(T));
  };

  /// emplace a resource to the world by calling the
  template <typename R, typename... Params>
  void init_resource(Params &&...p) {
    _resource_manager.insert_resource(R(p...));
  }

  /// insert a resource to the world
  template <typename R>
  void insert_resource(const R &value) {
    _resource_manager.insert_resource(value);
  }

  /// remove a resource from this world
  template <typename R>
  std::optional<R> remove_resource() {
    return _resource_manager.remove_resource<R>();
  }

  /// true if the world holds this Resource
  template <typename R>
  bool contains_resource() {
    return _resource_manager.contains_resource<R>();
  }

  /// access a given Resource
  template <typename R>
  R &resource() {
    return _resource_manager.resource<R>();
  }

  /// access a given Resource
  template <typename R>
  const R &resource() const {
    return _resource_manager.resource<R>();
  }

  /// access a given Resource, or None if it not in this world
  template <typename R>
  std::optional<ref<R>> get_resource() {
    return _resource_manager.get_resource<R>();
  }
  /* TODO:
          /// access a given Resource, or None if it not in this world
          template<typename R>
          std::optional<ref<const R>> get_resource() const {
              return _resource_manager.get_resource<R>();
          }
  */

  /// send an event TODO: DO
  template <typename E>
  void send_event(const E &event) {}

  public:
  template <typename Component>
  typename SparseVector<Component>::reference_type add_component(Entity const &to,
                                                                 const Component &c) {
    auto &array = get_components<Component>();

    return array.insert_at(to, c);
  }

  template <typename Component, typename... Params>
  typename SparseVector<Component>::reference_type emplace_component(Entity const &to,
                                                                     Params &&...p) {

    auto &array = get_components<Component>();
    return array.emplace_at(to, p...);
  }

  template <typename Component>
  void remove_component(Entity const &from) {
    auto &array = get_components<Component>();
    if (from < array.size())
      array.erase(from);
  }

  template <class Component>
  SparseVector<Component> &get_components() {
    auto id = std::type_index(typeid(Component));
    auto it = _components_arrays.find(id);

    if (it != _components_arrays.end()) {
      return std::any_cast<SparseVector<Component> &>(std::get<0>(_components_arrays[id]));
    }
    throw(std::runtime_error("Cevy/Ecs: Query unregisted component!"));
  }

  template <class Component>
  SparseVector<Component> const &get_components() const {
    auto id = std::type_index(typeid(Component));
    auto it = _components_arrays.find(id);

    if (it != _components_arrays.end()) {
      return std::any_cast<SparseVector<Component> &>(std::get<0>(_components_arrays.at(id)));
    }
    throw(std::runtime_error("Cevy/Ecs: Query unregisted component!"));
  }

  template <typename W, typename std::enable_if_t<is_world<W>::value, bool> = true>
  cevy::ecs::World &get_super(size_t) {
    return *this;
  }

  template <typename Q, typename std::enable_if_t<is_query<Q>::value, bool> = true>
  Q get_super(size_t) {
    return Q::query(*this);
  }

  template <typename R, typename std::enable_if_t<is_event_reader<R>::value, bool> = true>
  R get_super(size_t) {
    if (!contains_resource<Event<typename R::value_type>>())
      throw(std::runtime_error("Cevy/Ecs: Try to use EventReader on an unregisted event!"));

    return EventReader(resource<Event<typename R::value_type>>());
  }

  template <typename W, typename std::enable_if_t<is_event_writer<W>::value, bool> = true>
  W get_super(size_t system_id) {
    if (!contains_resource<Event<typename W::value_type>>())
      throw(std::runtime_error("Cevy/Ecs: Try to use EventWriter on an unregisted event!"));

    auto &res = resource<Event<typename W::value_type>>();

    if (res.event_queue.empty())
      return EventWriter(res, system_id);

    for (int i = res.event_queue.size() - 1; i >= 0; i--)
      if (std::get<1>(res.event_queue.at(i)) == system_id)
        res.event_queue.erase(res.event_queue.begin() + i);
    return EventWriter(res, system_id);
  }

  template <typename R, typename std::enable_if_t<is_resource<R>::value, bool> = true>
  R get_super(size_t) {
    return _resource_manager.get<typename R::value>();
  }

  template <typename C, typename std::enable_if_t<is_commands<C>::value, bool> = true>
  C get_super(size_t);
};

template <typename... Ts>
cevy::ecs::World::EntityWorldRef cevy::ecs::World::EntityWorldRef::insert(Ts... args) {
  (world.add_component(entity, std::forward<Ts>(args)), ...);
  return *this;
}

template <typename... T>
cevy::ecs::Query<T...> cevy::ecs::Query<T...>::query(World &w) {
  return Query<T...>(w.entities().size(), w.get_components<remove_optional<T>>()...);
}
