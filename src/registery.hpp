#pragma once

#include "./sparse_array.hpp"
#include "./entity.hpp"

#include <unordered_map>
#include <any>
#include <typeindex>
#include <iostream>
#include <tuple>
#include <functional>


class registery {
    public:
        using erase_access = std::function<void (registery &, entity const &)>;
        using component_data = std::tuple<std::any, erase_access>;

    private:
        std::unordered_map<std::type_index, component_data> _components_arrays;
        sparse_array<entity> _entities;
    public:
        template <class Component>
        sparse_array<Component> &register_component() {
            erase_access f = [] (registery & reg, entity const & entity) {
                reg.get_components<Component>()[entity] = std::nullopt;
            };
            std::any a = std::make_any<sparse_array<Component>>();

            _components_arrays.insert({std::type_index(typeid(Component)), std::make_tuple(a, f)});
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
};
