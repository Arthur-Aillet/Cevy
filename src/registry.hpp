#pragma once

#include "./sparse_array.hpp"
#include "./entity.hpp"

#include <unordered_map>
#include <any>
#include <typeindex>
#include <iostream>
#include <tuple>
#include <functional>


class registry {
    public:
        using erase_access = std::function<void (registry &, entity const &)>;
        using component_data = std::tuple<std::any, erase_access>;
        using system_function = std::function<void (registry &)>;

    private:
        std::vector<system_function> _systems;
        std::unordered_map<std::type_index, component_data> _components_arrays;
        sparse_array<entity> _entities;
    public:


        template<typename>
        struct is_sparse : std::false_type {};

        template<typename T, typename A>
        struct is_sparse<sparse_array<T,A> const &> : std::true_type {};

        entity spawn_entity() {
            size_t pos = _entities.first_free();
            entity new_e = entity(pos);

            _entities.insert_at(pos, new_e);
            return new_e;
        }

        entity entity_from_index(std::size_t idx) {
            entity new_e = entity(idx);

            _entities.insert_at(idx, new_e);
            return new_e;
        }

        void kill_entity(entity const &e) {
            for (auto const& [type, data] : _components_arrays) {
                std::get<1>(data)(*this, e);
            }
        }

        template <typename Component>
        typename sparse_array<Component>::reference_type add_component(entity const &to, Component &&c) {
            auto &array = get_components<Component>();
            return array.insert_at(to, std::forward<Component>(c));
        }

        template <typename Component, typename ... Params>
        typename sparse_array<Component>::reference_type emplace_component(entity const &to, Params &&... p) {
            auto &array = get_components<Component>();
            return array.emplace_at(to, p...);
        }

        template <typename Component>
        void remove_component(entity const &from) {
            auto &array = get_components<Component>();
            if (from < array.size())
                array.erase(from);
        }

        template <class Component>
        sparse_array<Component> &register_component() {
            erase_access f_e = [] (registry & reg, entity const & entity) {
                auto &cmpnts = reg.get_components<Component>();
                if (entity < cmpnts.size())
                    cmpnts[entity] = std::nullopt;
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

        // template <class Component>
        //     // ,typename = std::enable_if_t<!is_sparse<Component>::value, bool>>
        // sparse_array<Component> &get() {
        //     return std::any_cast<sparse_array<Component>&>(std::get<0>(_components_arrays[std::type_index(typeid(Component))]));
        // }
        // template <class Component>
        //     // ,typename = std::enable_if_t<!is_sparse<Component>::value, bool>>
        // sparse_array<Component> const &get() const {
        //     return std::any_cast<sparse_array<Component>&>(std::get<0>(_components_arrays.at(std::type_index(typeid(Component)))));
        // }

        template <typename Super>
            // ,typename = std::enable_if_t<is_sparse<Super>::value, bool>>
        Super &get_super() {
            std::cout << typeid(typename std::remove_reference<Super>::type::value_type::value_type).name() << std::endl;
            return std::any_cast<Super&>(std::get<0>(_components_arrays[std::type_index(typeid(typename std::remove_reference<Super>::type::value_type::value_type))]));
        }
        template <typename Super>
            // ,typename = std::enable_if_t<is_sparse<Super>::value, bool>>
        Super const &get_super() const {
            return std::any_cast<Super&>(std::get<0>(_components_arrays.at(std::type_index(typeid(typename std::remove_reference<Super>::type::value_type::value_type)))));
        }


        template<typename SparseType, std::enable_if_t<is_sparse<SparseType>::value, bool>>
        void print_arg() {
            std::cout << "v1" << std::endl;
        }

        // template<typename SparsentType, std::enable_if_t<!is_sparse<SparsentType>::value, bool>>
        // void print_arg() {
        //     std::cout << "v1" << std::endl;
        // }

        template<typename Sparse,
            typename = std::enable_if_t<is_sparse<Sparse>::value, bool>>
        void print_sparse() {
            std::cout << " --- "<< typeid(typename Sparse::value_type::value_type).name() << std::endl;
        }

        template<class R, class ...Args>
        void add_super_system(R(&&func)(registry&, Args...)) {
            system_function sys = [&func] (registry & reg) {
                func(reg, reg.get_super<Args>()...);
            };
            _systems.push_back(sys);
}

        template <class... Components, typename Function>
        void add_system(Function const &f) {
            system_function sys = [&f] (registry & reg) {
                f(reg, reg.get_components<Components>()...);
            };
            _systems.push_back(sys);
        }

        template <class... Components, typename Function>
        void add_system(Function &&f) {
            system_function sys = [&f] (registry & reg) {
                f(reg, reg.get_components<Components>()...);
            };
            _systems.push_back(sys);
        }

        void run_systems() {
            for (auto &sys : _systems) {
                sys(*this);
            }
        }
};
