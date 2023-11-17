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

    private:
        std::unordered_map<std::type_index, component_data> _components_arrays;
        sparse_array<entity> _entities;
    public:
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
};
