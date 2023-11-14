#pragma once

#include "./sparse_array.hpp"

#include <unordered_map>
#include <any>
#include <typeindex>
#include <iostream>

class registery {
    private:
        std::unordered_map<std::type_index, std::any> _components_arrays;
    public:
        template <class Component>
        sparse_array<Component> &register_component() {
            _components_arrays.insert({std::type_index(typeid(Component)), std::make_any<sparse_array<Component>>()});
            return std::any_cast<sparse_array<Component>&>(_components_arrays[std::type_index(typeid(Component))]);
        }
        template <class Component>
        sparse_array<Component> &get_components() {
            return std::any_cast<sparse_array<Component>&>(_components_arrays[std::type_index(typeid(Component))]);
        }
        template <class Component>
        sparse_array<Component> const &get_components() const {
            return std::any_cast<sparse_array<Component>&>(_components_arrays.at(std::type_index(typeid(Component))));
        }
};
