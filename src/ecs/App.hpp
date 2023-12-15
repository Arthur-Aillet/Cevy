/*
** Agartha-Software, 2023
** Cevy
** File description:
** App
*/

#pragma once

#include "Plugin.hpp"
#include "Schedule.hpp"
#include "World.hpp"

#include <unordered_map>
#include <any>
#include <typeindex>
#include <iostream>
#include <tuple>
#include <functional>
#include <type_traits>

#include "ecs.hpp"

class cevy::ecs::App : public cevy::ecs::World {
    public:
        using World::ComponentId;
        using system = Schedule::system;

    private:
        Schedule _schedule;
        /// TODO: refactor to has-a ?
        // World _world;

    private:
        template<typename GivenPlugin>
        void add_plugin() {
            static_assert(
                std::is_base_of_v<Plugin, GivenPlugin>,
                "Given plugin does not derive from Cevy Plugin class"
            );
            GivenPlugin::build(*this);
        }

    public:
        template<typename ...GivenPlugin>
        void add_plugins() {
            ((add_plugin<GivenPlugin>()),...);
        }

        void run() {
            _schedule.run(*this);
        }

        void quit() const {
            _schedule.quit();
        }
        void abort() {
            _schedule.abort();
        }

        template<typename T>
        void add_stage() {
            _schedule.insert_schedule<T>();
        }

        template<class S, class R, class ...Args>
        void add_system(R(&&func)(Args...)) {
            _schedule.add_system<S>(func);
        }

        template<class R, class ...Args>
        void add_system(R(&&func)(Args...)) {
            _schedule.add_system(func);
        }

};
