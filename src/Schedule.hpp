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

#include "World.hpp"

template<typename T>
using ref = std::reference_wrapper<T>;


class Schedule {
    public:
        template<typename Before = std::nullopt_t, typename After = std::nullopt_t>
        class Stage {
            public:
                template<typename T>
                using before = Stage<T, std::nullopt_t>;

                template<typename T>
                using after = Stage<std::nullopt_t, T>;
            constexpr operator bool() { return true; };
        };
        template<typename T>
        using before = Stage<>::before<T>;
        template<typename T>
        using after = Stage<>::after<T>;

        class First : public Stage<> {};

        class Update : public after<First> {};
        class PreUpdate : public before<Update> {};
        class PostUpdate : public after<Update> {};
        class PreUpdate : public after<First> {};

        class StateTransition : public before<Update> {};
        class RunFixedUpdateLoop : public before<Update> {};

        class Last : public after<PostUpdate> {};

    private:
        using type_tuple = std::tuple<std::type_index, std::any>;
        std::list<std::type_index> _schedule;

        void init_default_schedule() {
            insert_schedule<First>();
            insert_schedule<Update>();
            insert_schedule<PreUpdate>();
            insert_schedule<StateTransition>();
            insert_schedule<RunFixedUpdateLoop>();

            // add systems relating to the automatic parts of
            //StateTransitions and RunFixedUpdateLoop ?
        }


        template<typename T>
        void insert_schedule() {
            if (T::before()) {
                auto it = std::find(_schedule, std::type_index(typeid(T)));
                _schedule.insert(it, std::type_index(typeid(T)));
            } else if (T::after()) {
                auto it = std::find(_schedule, std::type_index(typeid(T)));
                ++it;
                _schedule.insert(it, std::type_index(typeid(T)));
            } else {
                _schedule.push_back(std::type_index(typeid(T)));
            }

        }
        enum class STAGE {
            First,
            PreUpdate,
            StateTransition,
            RunFixedUpdateLoop,
            Update,
            PostUpdate,
            Last,
            RESET,
            ABORT,
        };

        using system_function = std::function<void (World &)>;
        using system = std::tuple<system_function, std::type_index>;
        std::vector<system> _systems;
        Schedule() = default;
        ~Schedule() = default;

        void quit() const;
        void abort();

        template <typename Stage, class... Components, typename Function>
        void add_system(Function const &f) {
            system_function sys = [&f] (World & reg) {
                f(reg, reg.get_components<Components>()...);
            };
            _systems.push_back(std::make_tuple(sys, stage));
        }

        template <typename Stage, class... Components, typename Function>
        void add_system(Function &&f) {
            system_function sys = [&f] (World & reg) {
                f(reg, reg.get_components<Components>()...);
            };
            _systems.push_back(std::make_tuple(sys, stage));
        }

        template <class... Components, typename Function>
        void add_system(Function const &f) {
            add_system<Update, Components...>(f);
        }

        template <class... Components, typename Function>
        void add_system(Function &&f) {
            add_system<Update, Components...>(f);
        }

    protected:
        mutable bool _stop = false;
        mutable bool _abort = false;
        STAGE _stageOLD = STAGE::RESET;
        std::list<std::type_index>::iterator _stage;


        void runStages(World& world);
        void runStage(World& world);
    private:

        /* Bevy-compliant */
    public:
        void run(World& world);

        // Schedule& add_systems();
};
