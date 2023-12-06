/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Schedule
*/

#pragma once
#include <functional>

#include "World.hpp"

class Schedule {
    public:
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
};
