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
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
#include <algorithm>

#include "App.hpp"
#include "Commands.hpp"
#include "Plugin.hpp"
// #include "Position.hpp"
#include "Query.hpp"
#include "network/CevyNetwork.hpp"
#include "network/network.hpp"

// template<size_t N, size_t... Ns>
// struct sum : std::integral_constant<size_t, N + sum<Ns>> {};

template <typename... Args>
constexpr size_t sum(Args... args) {
  return (0 + ... + args);
};

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

    enum class Presume {
        idk,
        fail,
        success,
    };
    template<size_t N = 0, Presume p = Presume::idk, typename... Data>
    struct Action {
        static const uint16_t value = N;
        static const Presume presume = p;
        static const size_t serialized_size = sum(::cevy::serialized_size<Data>::value...);
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
                [server](ecs::Commands& cmd) mutable {cmd.system(server); },
                [client_success](ecs::Commands& cmd) mutable {cmd.system(client_success); },
                [client_fail](ecs::Commands& cmd) mutable {cmd.system(client_fail); }
            );
    };

    template<typename A,
        typename R0 = void, typename... Arg0,
        typename R1 = void, typename... Arg1,
        typename R2 = void, typename... Arg2>
    void add_action(std::function<R0(Arg0...)> server, std::function<R1(Arg1...)> client_success = [](){}, std::function<R2(Arg2...)>  client_fail = [](){}) {
            _actions[A::value] = std::make_tuple(
                [server](ecs::Commands& cmd) mutable {cmd.system(server); },
                [client_success](ecs::Commands& cmd) mutable {cmd.system(client_success); },
                [client_fail](ecs::Commands& cmd) mutable {cmd.system(client_fail); }
            );
    }

    template <typename E, typename R, typename... Args>
    void add_event(R (&&func)(Args...)) {
        _events[E::value] = [func](ecs::Commands& cmd){cmd.system(func); };
    };

    template <typename E, typename GivenArgs, typename R, typename... Args>
    void add_event_with(R (&&func)(Args...)) {
        auto lambda = [func](ecs::Commands& cmd, GivenArgs given){cmd.system_with<GivenArgs>(func, given); };
        _super_events[E::value] = std::make_any<std::function<void(ecs::Commands&, GivenArgs)>>(lambda);
    };

    template <typename E, typename GivenArgs, typename R, typename... Args>
    void add_event_with(std::function<R(GivenArgs, Args...)> func) {
        auto lambda = [func](ecs::Commands& cmd, GivenArgs given){cmd.system_with<GivenArgs>(func, given); };
        _super_events[E::value] = std::make_any<std::function<void(ecs::Commands&, GivenArgs)>>(lambda);
    };


    template<typename A>
    void action(ecs::Commands& cmd) const {
        if (_mode == Mode::Server) {
            std::get<0>(_actions[A::value])(cmd);
        } else {
            if (A::presume == Presume::success)
                std::get<1>(_actions[A::value])(cmd);
            else if (A::presume == Presume::fail)
                std::get<2>(_actions[A::value])(cmd);
            _net.sendAction(A::value, std::vector<uint8_t>());
        }
    }

    template<typename E>
    void event(ecs::Commands& cmd) {
        _events[E::value](cmd);
        std::vector<uint8_t> vec(E::serialized_size, 0);
        _net.sendEvent(E::value, vec);
    }

    template<typename E, typename GivenArg>
    void event_with(ecs::Commands& cmd, GivenArg given) {
        const auto& func = std::any_cast<std::function<void(ecs::Commands, GivenArg)>>(_super_events[E::value]);
        func(cmd, given);
    }


protected:
    using system = std::function<void(ecs::Commands&)>;
    std::unordered_map<uint16_t, std::tuple<system, system, system>> _actions;
    std::unordered_map<uint16_t, system> _events;
    std::unordered_map<uint16_t, std::any> _super_events;
    Mode _mode;
    cevy::CevyNetwork &_net;

};

template<typename R, typename... Args>
constexpr std::function<R(Args...)> make_function(R (&&func)(Args...)) { return std::function<R(Args...)>(func); };
