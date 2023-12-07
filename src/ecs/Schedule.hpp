/*
** EPITECH PROJECT, 2023
** rtype
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
#include <any>

#include "ecs.hpp"


class cevy::ecs::Schedule {
    public:
        template<typename Before = std::nullopt_t, typename After = std::nullopt_t>
        class Stage {
            public:
                template<typename T>
                using before = Stage<T, std::nullopt_t>;

                template<typename T>
                using after = Stage<std::nullopt_t, T>;
        };
        template<typename T>
        using before = Stage<>::before<T>;
        template<typename T>
        using after = Stage<>::after<T>;

        class First : public Stage<> {};

        class Update : public after<First> {};
        class PreUpdate : public before<Update> {};
        class PostUpdate : public after<Update> {};

        class StateTransition : public before<Update> {};
        class RunFixedUpdateLoop : public before<Update> {};

        class Last : public after<PostUpdate> {};

    private:
        using type_tuple = std::tuple<std::type_index, std::any>;
        std::list<std::type_index> _schedule;

    public:
        void init_default_schedule() {
            insert_schedule<First>();
            insert_schedule<Update>();
            insert_schedule<PreUpdate>();
            insert_schedule<StateTransition>();
            insert_schedule<RunFixedUpdateLoop>();

            // add systems relating to the automatic parts of
            //StateTransitions and RunFixedUpdateLoop ?
        }


        template<typename T, typename T::before>
        void insert_schedule() {
            auto it = std::find(_schedule.begin(), _schedule.end(), std::type_index(typeid(T::before)));
            _schedule.insert(it, std::type_index(typeid(T)));
        }

        template<typename T, typename T::after>
        void insert_schedule() {
            auto it = std::find(_schedule.begin(), _schedule.end(), std::type_index(typeid(T::after)));
            _schedule.insert(it, std::type_index(typeid(T)));
        }

        template<typename T>
        void insert_schedule() {
            _schedule.push_back(std::type_index(typeid(T)));
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
            system_function sys = [&f] (World & reg) {
                f(reg, reg.get_components<Components>()...);
            };
            _systems.push_back(std::make_tuple(sys, std::type_index(typeid(S))));

        }

        template <typename S, class... Components, typename Function>
        void add_system(Function &&f) {
            system_function sys = [&f] (World & reg) {
                f(reg, reg.get_components<Components>()...);
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
