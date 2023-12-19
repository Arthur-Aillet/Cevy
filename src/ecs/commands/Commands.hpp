/*
** Agartha-Software, 2023
** Cevy
** File description:
** Commands
*/

#pragma once

#include "World.hpp"
#include "Command.hpp"
#include <typeindex>

namespace cevy {
    namespace ecs {
        class EntityCommands;
    }
}

class cevy::ecs::Commands {
    private:
        friend class cevy::ecs::World;

        cevy::ecs::World& _world_access;
        Commands(cevy::ecs::World& world_access) : _world_access(world_access) {};
    public:
        template<typename GivenCommand, typename std::enable_if_t<std::is_base_of_v<Command, GivenCommand>, bool> = true>
        void add(const GivenCommand &a) {
            auto l = [a] (cevy::ecs::World &w) {
                a.apply(w);
            };
            _world_access._command_queue.push(l);
        }

        void add(std::function<void (cevy::ecs::World &w)>&& f);

        template<typename R>
        void insert_resource(const R& value) {
            add([value] (cevy::ecs::World &w) {
                w.insert_resource(value);
            });
        }

        cevy::ecs::EntityCommands spawn_empty();

        template<typename... Ts>
        cevy::ecs::EntityCommands spawn(Ts... a);

        template<typename... Ts>
        cevy::ecs::EntityCommands spawn_archetype() {
            add([] (cevy::ecs::World &w) {
                w.spawn_archetype<T>();
            });
        };

        cevy::ecs::EntityCommands spawn_archetype(std::type_index type) {
            add([] (cevy::ecs::World &w) {
                w.spawn_archetype(type);
            });
        };
};

template<typename C,
    typename std::enable_if_t<is_commands<C>::value, bool>>
C cevy::ecs::World::get_super() {
    return cevy::ecs::Commands(*this);
}
