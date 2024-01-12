/*
** AgarthaSoftware, 2024
** Cevy
** File description:
** NetworkAction
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <unordered_map>
#include <utility>

#include "App.hpp"
#include "Commands.hpp"
#include "Plugin.hpp"
#include "Position.hpp"
#include "Query.hpp"
#include "network/CevyNetwork.hpp"

// template <typename A, typename F>
class cevy::NetworkActions : public ecs::Plugin {
public:
    class ShipActions;

    enum class Mode {
        Server,
        Client,
    };

    NetworkActions(CevyNetwork& net) : _net(net) {};
    NetworkActions(const NetworkActions &) = delete;
    NetworkActions &operator=(const NetworkActions &) = delete;
    NetworkActions &operator=(NetworkActions &&) = delete;
    ~NetworkActions();

    template<size_t N = 0>
    struct Action {
        enum class Presume {
            idk,
            fail,
            success,
        };
        static const uint16_t value = N;
        static const Presume presume = Presume::idk;
    };

    virtual void build(cevy::ecs::App& app) override {

    };

    // template <typename A, typename F>
    // void add_action(F function) {
    //     _actions[A::value].first = function;
    //     _actions[A::value].seconde = nullptr;
    // };

    template<typename A,
        typename R0 = void, typename... Arg0,
        typename R1 = void, typename... Arg1,
        typename R2 = void, typename... Arg2>
    void add_action(R0 (&&server)(Arg0...), R1 (&&client_success)(Arg1...), R2 (&&client_fail)(Arg2...)) {
            _actions[A::value] = std::make_tuple(
                [server](ecs::Commands cmd) mutable {cmd.system(server); },
                [client_success](ecs::Commands cmd) mutable {cmd.system(client_success); },
                [client_fail](ecs::Commands cmd) mutable {cmd.system(client_fail); }
            );
    };

        template<typename A,
        typename R0 = void, typename... Arg0,
        typename R1 = void, typename... Arg1,
        typename R2 = void, typename... Arg2>
    void add_action(std::function<R0(Arg0...)> server, std::function<R1(Arg1...)> client_success = [](){}, std::function<R2(Arg2...)>  client_fail = [](){}) {
            _actions[A::value] = std::make_tuple(
                [server](ecs::Commands cmd) mutable {cmd.system(server); },
                [client_success](ecs::Commands cmd) mutable {cmd.system(client_success); },
                [client_fail](ecs::Commands cmd) mutable {cmd.system(client_fail); }
            );
    }

    template <typename E, typename F>
    void add_event(F func) {
        _events[E::value] = [func](ecs::Commands cmd){cmd.system(func); };
    };


protected:
    using system = std::function<void(ecs::Commands)>;
    std::unordered_map<uint16_t, std::tuple<system, system, system>> _actions;
    std::unordered_map<uint16_t, system> _events;
    // Mode _mode;
    cevy::CevyNetwork &_net;

};

template<typename R, typename... Args>
constexpr std::function<R(Args...)> make_function(R (&&func)(Args...)) { return std::function<R(Args...)>(func); };

class ShipActions : cevy::NetworkActions {
public:
    ShipActions(cevy::CevyNetwork &net) : NetworkActions(net) {};

    enum Act {
        shoot,
        flyUp,
        flyDown,
    };
    struct Shoot : Action<Act::shoot> {};
    struct FlyUp : Action<Act::flyUp> {
        Presume presume = Presume::success;
    };

    void build(cevy::ecs::App& app) override {
        add_action<Shoot>(make_function(shootAction));
    }

    static void shootAction(cevy::ecs::Query<cevy::engine::Position> q) {};
private:
};
