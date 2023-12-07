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
        std::list<type_tuple> _schedule;

        void init_default_schedule() {

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
        using system = std::tuple<system_function, STAGE>;
        std::vector<system> _systems;
        Schedule() = default;
        ~Schedule() = default;

        void quit() const;
        void abort();

        template <class... Components, typename Function>
        void add_system(STAGE stage, Function const &f) {
            system_function sys = [&f] (World & reg) {
                f(reg, reg.get_components<Components>()...);
            };
            _systems.push_back(std::make_tuple(sys, stage));
        }

        template <class... Components, typename Function>
        void add_system(STAGE stage, Function &&f) {
            system_function sys = [&f] (World & reg) {
                f(reg, reg.get_components<Components>()...);
            };
            _systems.push_back(std::make_tuple(sys, stage));
        }

        template <class... Components, typename Function>
        void add_system(Function const &f) {
            add_system<Components...>(STAGE::Update, f);
        }

        template <class... Components, typename Function>
        void add_system(Function &&f) {
            add_system<Components...>(STAGE::Update, f);
        }

    protected:
        mutable bool _stop = false;
        STAGE _stage = STAGE::RESET;

        void runStages(World& world);
        void runStage(World& world);
    private:

        /* Bevy-compliant */
    public:
        void run(World& world);

        Schedule& add_systems();
};
