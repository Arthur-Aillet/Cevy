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

#include "./SparseVector.hpp"
#include "./Entity.hpp"

#include <unordered_map>
#include <any>
#include <typeindex>
#include <iostream>
#include <tuple>
#include <functional>
#include <type_traits>

#include "ecs.hpp"
#include "Query.hpp"


template<class T>
struct is_world : public std::false_type {};

template<>
struct is_world<cevy::ecs::World&> : public std::true_type {};
template<>
struct is_world<const cevy::ecs::World&> : public std::true_type {};

template<typename... Args>
constexpr bool all(Args... args) { return (... && args); }

template<typename... Args>
constexpr bool any() { return (... || Args::value); };

template<typename... T>
struct Or : std::integral_constant<bool, any<T...>()> {};



/**
 * Stores Entities, Components (and ressources), and exposes operations
 *
 * Each entity has Components, which must first each be registered to the World
 * An Entity can only have one instance of a Component
*/
class cevy::ecs::World {
        using value_type = World;
    public:
        struct EntityWorldRef {
            World& world;
            Entity entity;


            // EntityWorldRef insert(Bundle& b);

            template<typename... Ts>
            EntityWorldRef insert(Ts... args);

            operator Entity& () { return entity; };
        };

        using erase_access = std::function<void (World &, Entity const &)>;
        using component_data = std::tuple<std::any, erase_access>;

    /* Bevy-compliant */
    public:
        /// @brief Id refering to a specific component
        using ComponentId = std::type_index;

    private:
        std::unordered_map<std::type_index, component_data> _components_arrays;
        SparseVector<Entity> _entities;

    /* Bevy-compliant */
    public:
        /// get all entities
        SparseVector<Entity>& entities();

        /// get all entities
        const SparseVector<Entity>& entities() const;

        /// create a new empty entity
        EntityWorldRef spawn_empty();

        /// delete an entity and all its components
        bool despawn(const Entity& entity);

        /// clears all resources and entities
        void clear_all();

        /// clears all entities and their components
        void clear_entities();

        /// clears all resources registered to the world
        void clear_resources();

        /// spawn an entity with defined components
        template<typename... Ts>
        EntityWorldRef spawn(Ts... a) {
            auto e = spawn_empty().insert(a...);
        }

        /// get a Component T associated with a given Entity, or Nothing if no such Component
        template<typename T>
        std::optional<ref<T>> get(Entity entity) {
            SparseVector<T>& v = std::any_cast<SparseVector<T>&>(std::get<0>(_components_arrays[std::type_index(typeid(T))]));
            std::optional<T> optional = v[entity];

            if (optional)
                return std::optional<ref<T>>(std::ref(optional.value()));
            else
                return std::optional<ref<T>>(std::ref(std::nullopt));
        }

        /// get a Component T associated with a given Entity, or Nothing if no such Component
        template<typename T>
        std::optional<ref<const T>> get(Entity entity) const {
            const SparseVector<T>& v = std::any_cast<SparseVector<T>&>(std::get<0>(_components_arrays.at(std::type_index(typeid(T)))));
            std::optional<const T> optional = v[entity];

            if (optional)
                return std::optional<ref<const T>>(std::ref(optional.value()));
            else
                return std::optional<ref<const T>>(std::ref(std::nullopt));
        }

        /// register a component to the world
        template<typename T>
        ComponentId init_component() {
            erase_access f_e = [] (World & reg, Entity const & Entity) {
                auto &cmpnts = reg.get_components<T>();
                if (Entity < cmpnts.size())
                    cmpnts[Entity] = std::nullopt;
            };
            std::any a = std::make_any<SparseVector<T>>();

            _components_arrays.insert({std::type_index(typeid(T)), std::make_tuple(a, f_e)});

            return std::type_index(typeid(T));
        };

        /// register a resource to the world; TODO: DO
        template<typename R>
        ComponentId init_resource() {

        }

        /// replace a resource to the world; TODO: DO
        template<typename R>
        void insert_resource(const R& r) {

        }

        /// rempve a resource from this world; TODO: DO
        template<typename R>
        std::optional<R> remove_resource() {

        }

        /// true if the world holds this Resource TODO: DO
        template<typename R>
        bool contains_resource() {

        }

        /// access a given Resource TODO: DO
        template<typename R>
        R& resource() {

        }

        /// access a given Resource TODO: DO
        template<typename R>
        const R& resource() const {

        }

        /// access a given Resource, or None if it not in this world TODO: DO
        template<typename R>
        std::optional<ref<R>> get_resource() {

        }

        /// access a given Resource, or None if it not in this world TODO: DO
        template<typename R>
        std::optional<ref<const R>> get_resource() const {

        }

        /// send an event TODO: DO
        template<typename E>
        void send_event(const E& event) {

        }

    public:
        Entity spawn_at(std::size_t idx);

        template <typename Component>
        typename SparseVector<Component>::reference_type add_component(Entity const &to, Component &&c) {
            auto &array = get_components<Component>();
            return array.insert_at(to, std::forward<Component>(c));
        }

        template <typename Component, typename ... Params>
        typename SparseVector<Component>::reference_type emplace_component(Entity const &to, Params &&... p) {
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
        SparseVector<Component> &register_component() {
            erase_access f_e = [] (World & reg, Entity const & Entity) {
                auto &cmpnts = reg.get_components<Component>();
                if (Entity < cmpnts.size())
                    cmpnts[Entity] = std::nullopt;
            };
            std::any a = std::make_any<SparseVector<Component>>();

            _components_arrays.insert({std::type_index(typeid(Component)), std::make_tuple(a, f_e)});
            return std::any_cast<SparseVector<Component>&>(std::get<0>(_components_arrays[std::type_index(typeid(Component))]));
        }

        template <class Component>
        SparseVector<Component> &get_components() {
            return std::any_cast<SparseVector<Component>&>(std::get<0>(_components_arrays[std::type_index(typeid(Component))]));
        }
        template <class Component>
        SparseVector<Component> const &get_components() const {
            return std::any_cast<SparseVector<Component>&>(std::get<0>(_components_arrays.at(std::type_index(typeid(Component)))));
        }

        template<typename W,
            typename std::enable_if_t<is_world<W>::value, bool> = true>
        cevy::ecs::World& get_super() {
            return *this;
        }

        template<typename Q,
            typename std::enable_if_t<is_query<Q>::value, bool> = true>
        Q get_super() {
            return Q::query(*this);
        }
};

template<typename... Ts>
cevy::ecs::World::EntityWorldRef cevy::ecs::World::EntityWorldRef::insert(Ts... args) {
    world.add_component(entity, args...);
    return *this;
}

template<typename...T>
cevy::ecs::Query<T...> cevy::ecs::Query<T...>::query(World& w) {
    return Query<T...>(w.get_components<T>()...);
}
