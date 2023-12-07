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

class App : public World {
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
            GivenPlugin plugin;
            plugin.build(*this);
        }

    public:
        template<typename ...GivenPlugin>
        void add_plugins() {
            ((add_plugin<GivenPlugin>()),...);
        }

        void run() {
            _schedule.run(*this);
        }
};
