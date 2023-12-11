/*
** Agartha-Software, 2023
** Cevy
** File description:
** Schedule
*/

#pragma once
#include <functional>
#include <list>
#include <tuple>
#include <optional>
#include <algorithm>
#include <typeindex>
#include <variant>
#include <any>

#include "ecs.hpp"
#include "World.hpp"

class cevy::ecs::Schedule {
    public:
        struct IsStage {};

        template<typename Before = std::nullopt_t, typename After = std::nullopt_t, typename Repeat = std::true_type>
        class Stage : public IsStage {
            public:
                using is_repeat = Repeat;
                using previous = Before;
                using next = After;

                template<typename T>
                using before = Stage<T, std::nullopt_t, typename T::is_repeat>;

                template<typename T>
                using after = Stage<std::nullopt_t, T, typename T::is_repeat>;

        };
        template<typename T>
        using before = Stage<>::before<T>;
        template<typename T>
        using after = Stage<>::after<T>;

        using at_start = Stage<std::nullopt_t, std::nullopt_t, std::false_type>;

        class Startup : public at_start {};
        class PreStartup : public before<Startup> {};
        class PostStartup : public after<Startup> {};

        class First : public Stage<> {};

        class Update : public after<First> {};
        class PreUpdate : public before<Update> {};
        class PostUpdate : public after<Update> {};

        class StateTransition : public before<Update> {};
        class RunFixedUpdateLoop : public before<Update> {};

        class Last : public after<PostUpdate> {};

    private:
        std::list<std::type_index> _schedule;
        std::list<std::type_index> _at_start_schedule;

    public:
        void init_default_schedule() {
            insert_schedule<Startup>();
            insert_schedule<PreStartup>();
            insert_schedule<PostStartup>();

            insert_schedule<First>();
            insert_schedule<Update>();
            insert_schedule<PreUpdate>();
            insert_schedule<PostUpdate>();
            // insert_schedule<StateTransition>();
            // insert_schedule<RunFixedUpdateLoop>();
            insert_schedule<Last>();

            // add systems relating to the automatic parts of
            //StateTransitions and RunFixedUpdateLoop ?
        }

        template<typename T, typename std::enable_if_t<std::is_same_v<typename T::is_repeat, std::true_type>, bool> = true>
        void insert_schedule() {
            if constexpr (!std::is_same_v<typename T::previous, std::nullopt_t>) {
                auto it = std::find(_schedule.begin(), _schedule.end(), std::type_index(typeid(typename T::previous)));

                _schedule.insert(it, std::type_index(typeid(T)));
            } else if constexpr (!std::is_same_v<typename T::next, std::nullopt_t>) {
                auto it = std::find(_schedule.begin(), _schedule.end(), std::type_index(typeid(typename T::next)));

                ++it;
                _schedule.insert(it, std::type_index(typeid(T)));
            } else {
                _schedule.push_back(std::type_index(typeid(T)));
            }
        }

        template<typename T, typename std::enable_if_t<std::is_same_v<typename T::is_repeat, std::false_type>, bool> = true>
        void insert_schedule() {
            if constexpr (!std::is_same_v<typename T::previous, std::nullopt_t>) {
                auto it = std::find(_at_start_schedule.begin(), _at_start_schedule.end(), std::type_index(typeid(typename T::previous)));

                _at_start_schedule.insert(it, std::type_index(typeid(T)));
            } else if constexpr (!std::is_same_v<typename T::next, std::nullopt_t>) {
                auto it = std::find(_at_start_schedule.begin(), _at_start_schedule.end(), std::type_index(typeid(typename T::next)));

                ++it;
                _at_start_schedule.insert(it, std::type_index(typeid(T)));
            } else {
                _at_start_schedule.push_back(std::type_index(typeid(T)));
            }
        }

        using system_function = std::function<void (World &)>;
        using system = std::tuple<system_function, std::type_index>;
        std::vector<system> _systems;
        Schedule() {
            init_default_schedule();
        }
        ~Schedule() = default;

        void quit() const;
        void abort();

        template <typename S, class... Components, typename Function>
        void add_system(Function const &f) {
            system_function sys = [&f] (World & world) {
                f(world, world.get_components<Components>()...);
            };
            _systems.push_back(std::make_tuple(sys, std::type_index(typeid(S))));

        }

        template <typename S, class... Components, typename Function>
        void add_system(Function &&f) {
            system_function sys = [&f] (World & world) {
                f(world, world.get_components<Components>()...);
            };
            _systems.push_back(std::make_tuple(sys, std::type_index(typeid(S))));
        }

        template<class R, class ...Args>
        void add_super_system(R(&&func)(Args...)) {
            add_super_system<Update>(func);
        }

        template<class S, class R, class ...Args>
        void add_super_system(R(&&func)(Args...)) {
            static_assert(all(is_super<Args>()...), "type must be reference to super");
            system_function sys = [&func] (World & reg) {
                func(reg.get_super<Args>()...);
            };
            _systems.push_back(std::make_tuple(sys, std::type_index(typeid(S))));
        }

    protected:
        mutable bool _stop = false;
        mutable bool _abort = false;
        std::list<std::type_index>::iterator _stage;


        void runStages(World& world);
        void runStage(World& world);
    private:

        /* Bevy-compliant */
    public:
        void run(World& world);

        // Schedule& add_systems();
};
