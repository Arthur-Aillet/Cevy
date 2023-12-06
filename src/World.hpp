/*
** EPITECH PROJECT, 2023
** rtype
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

#include "./sparse_array.hpp"
#include "./Entity.hpp"

#include <unordered_map>
#include <any>
#include <typeindex>
#include <iostream>
#include <tuple>
#include <functional>
#include <type_traits>

template<typename T>
using ref = std::reference_wrapper<T>;


/**
template<class T>
struct is_super : public std::false_type {};

class World;
template<>
struct is_super<World&> : std::true_type {};
template<>
struct is_super<const World&> : std::true_type {};


template<typename T, typename A>
struct is_super<const sparse_array<T, A>&> : std::true_type {};

template<typename T, typename A>
struct is_super<sparse_array<T, A>&> : std::true_type {};

template<typename... Args>
constexpr bool all(Args... args) { return (... && args); }
*/

struct EntityWorldRef {
    World& world;
    Entity entity;


    // EntityWorldRef insert(Bundle& b);

    template<typename... Ts>
    EntityWorldRef insert(Ts... args) {
        world.add_component(enitity, args...);
    }
};

/**
 * Stores Entities, Components (and ressources), and exposes operations
 *
 * Each entity has Components, which must first each be registered to the World
 * An Entity can only have one instance of a Component
*/
class World {
    public:
        using erase_access = std::function<void (World &, Entity const &)>;
        using component_data = std::tuple<std::any, erase_access>;

    /* Bevy-compliant */
    public:
        /// @brief Id refering to a specific component
        using ComponentId = std::type_index;

    private:
        std::unordered_map<std::type_index, component_data> _components_arrays;
        sparse_array<Entity> _entities;

    /* Bevy-compliant */
    public:
        /// get all entities
        sparse_array<Entity>& entities();

        /// get all entities
        const sparse_array<Entity>& entities() const;

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
            sparse_array<T>& v = std::any_cast<sparse_array<T>&>(std::get<0>(_components_arrays[std::type_index(typeid(T))]));
            std::optional<T> optional = v[entity];

            if (optional)
                return std::optional<ref<T>>(ref(optional.value()));
            else
                return std::optional<ref<T>>(ref(std::nullopt()));
        }

        /// get a Component T associated with a given Entity, or Nothing if no such Component
        template<typename T>
        std::optional<ref<const T>> get(Entity entity) const {
            sparse_array<T>& v = std::any_cast<sparse_array<T>&>(std::get<0>(_components_arrays[std::type_index(typeid(T))]));
            std::optional<const T> optional = v[entity];

            if (optional)
                return std::optional<ref<const T>>(ref(optional.value()));
            else
                return std::optional<ref<const T>>(ref(std::nullopt()));
        }

        /// register a component to the world
        template<typename T>
        ComponentId init_component() {
            erase_access f_e = [] (World & reg, Entity const & Entity) {
                auto &cmpnts = reg.get_components<T>();
                if (Entity < cmpnts.size())
                    cmpnts[Entity] = std::nullopt;
            };
            std::any a = std::make_any<sparse_array<T>>();

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
        typename sparse_array<Component>::reference_type add_component(Entity const &to, Component &&c) {
            auto &array = get_components<Component>();
            return array.insert_at(to, std::forward<Component>(c));
        }

        template <typename Component, typename ... Params>
        typename sparse_array<Component>::reference_type emplace_component(Entity const &to, Params &&... p) {
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
        sparse_array<Component> &register_component() {
            erase_access f_e = [] (World & reg, Entity const & Entity) {
                auto &cmpnts = reg.get_components<Component>();
                if (Entity < cmpnts.size())
                    cmpnts[Entity] = std::nullopt;
            };
            std::any a = std::make_any<sparse_array<Component>>();

            _components_arrays.insert({std::type_index(typeid(Component)), std::make_tuple(a, f_e)});
            return std::any_cast<sparse_array<Component>&>(std::get<0>(_components_arrays[std::type_index(typeid(Component))]));
        }

        template <class Component>
        sparse_array<Component> &get_components() {
            return std::any_cast<sparse_array<Component>&>(std::get<0>(_components_arrays[std::type_index(typeid(Component))]));
        }
        template <class Component>
        sparse_array<Component> const &get_components() const {
            return std::any_cast<sparse_array<Component>&>(std::get<0>(_components_arrays.at(std::type_index(typeid(Component)))));
        }

        // template <typename Super>
        // const Super get_super() {
        //     if (typeid(World&) == typeid(Super)) {
        //         auto p = std::any_cast<std::remove_const_t<std::remove_reference_t<Super>>*>(this);
        //         if (p) {
        //             return *p;
        //         } else {
        //             throw std::bad_any_cast();
        //         }
        //     } else {
        //         return std::any_cast<Super>(std::get<0>(_components_arrays[std::type_index(typeid(typename std::remove_reference<Super>::type::value_type::value_type))]));
        //     }
        // }

        // template<class R, class ...Args>
        // void add_super_system(R(&&func)(Args...)) {
        //     add_super_system<R, Args...>(STAGE::Update, func);
        // }

        // template<class R, class ...Args>
        // void add_super_system(STAGE stage, R(&&func)(Args...)) {
        //     static_assert(all(is_super<Args>()...), "type must be reference to super");
        //     system_function sys = [&func] (World & reg) {
        //         func(reg.get_super<Args>()...);
        //     };
        //     _systems.push_back(std::make_tuple(sys, stage));
        // }
};
