/*
** AgarthaSoftware, 2024
** Cevy
** File description:
** NetworkAction
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <list>
#include <type_traits>
#include <typeindex>
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

    using ActionFailureMode = CevyNetwork::ActionFailureMode;
    using EActionFailureMode = ActionFailureMode::EActionFailureMode;

    enum class Presume {
        idk,
        fail,
        success,
    };
    template<size_t N = 0, Presume p = Presume::idk, typename Data = std::tuple<>>
    struct Action {
        static const uint16_t value = N;
        static const Presume presume = p;
        static const size_t serialized_size = sum(::cevy::serialized_size<Data>::value);
        using Arg = Data;
    };

    template<size_t N = 0, typename Data = std::tuple<>>
    struct Event {
        static const uint16_t value = N;
        static const size_t serialized_size = sum(::cevy::serialized_size<Data>::value);
        using Arg = Data;
    };

    virtual void build(cevy::ecs::App&) {}

    // template <typename A, typename F>
    // void add_action(F function) {
    //     _actions[A::value].first = function;
    //     _actions[A::value].seconde = nullptr;
    // };

    template<typename A,
        typename... Arg0,
        typename R1 = void, typename... Arg1,
        typename R2 = void, typename... Arg2>
    void add_action(EActionFailureMode (&&server)(Arg0...), R1 (&&client_success)(Arg1...), R2 (&&client_fail)(Arg2...)) {
            _actions[A::value] = std::make_tuple(
                [server](ecs::Commands& cmd) mutable {cmd.system(server); },
                [client_success](ecs::Commands& cmd) mutable {cmd.system(client_success); },
                [client_fail](ecs::Commands& cmd) mutable {cmd.system(client_fail); }
            );
    };

    template<typename A,
        typename... Arg0,
        typename R1 = void, typename... Arg1,
        typename R2 = void, typename... Arg2>
    void add_action(std::function<EActionFailureMode(Arg0...)> server, std::function<R1(Arg1...)> client_success = [](){}, std::function<R2(Arg2...)>  client_fail = [](){}) {
            _actions[A::value] = std::make_tuple(
                [server](ecs::Commands& cmd) mutable {cmd.system(server); },
                [client_success](ecs::Commands& cmd) mutable {cmd.system(client_success); },
                [client_fail](ecs::Commands& cmd) mutable {cmd.system(client_fail); }
            );
    }

    template<typename A,
        typename... Arg0,
        typename R1 = void, typename... Arg1,
        typename R2 = void, typename... Arg2>
    void add_action_with(std::function<EActionFailureMode(typename A::Arg, Arg0...)> server, std::function<R1(typename A::Arg, Arg1...)> client_success = [](){}, std::function<R2(typename A::Arg, Arg2...)>  client_fail = [](){}) {
        using server_ftype = std::function<EActionFailureMode(ecs::Commands&, typename A::Arg)>;
        using ftype = std::function<void(ecs::Commands&, typename A::Arg)>;
            _super_actions[A::value] = std::make_tuple(
                std::make_any<server_ftype>([server](ecs::Commands& cmd, typename A::Arg given) mutable {return cmd.system_with(server, given); }),
                std::make_any<ftype>([client_success](ecs::Commands& cmd, typename A::Arg given) mutable {cmd.system_with(client_success, given); }),
                std::make_any<ftype>([client_fail](ecs::Commands& cmd, typename A::Arg given) mutable {cmd.system_with(client_fail, given); })
            );
    }

    template<typename A,
        typename... Arg0,
        typename R1 = void, typename... Arg1,
        typename R2 = void, typename... Arg2>
    void add_action_with(EActionFailureMode (&&server)(typename A::Arg, Arg0...), R1 (&&client_success)(typename A::Arg, Arg1...) = [](){}, R2 (&& client_fail)(typename A::Arg, Arg2...) = [](){}) {
        using server_ftype = std::function<EActionFailureMode(ecs::Commands&, typename A::Arg)>;
        using ftype = std::function<void(ecs::Commands&, typename A::Arg)>;
            _super_actions[A::value] = std::make_tuple(
                std::make_any<server_ftype>([server](ecs::Commands& cmd, typename A::Arg given) mutable {cmd.system_with(server, given); }),
                std::make_any<ftype>([client_success](ecs::Commands& cmd, typename A::Arg given) mutable {cmd.system_with(client_success, given); }),
                std::make_any<ftype>([client_fail](ecs::Commands& cmd, typename A::Arg given) mutable {cmd.system_with(client_fail, given); })
            );
    }

    template <typename E, typename R, typename... Args>
    void add_event(R (&&func)(Args...)) {
        _events[E::value] = [func](ecs::Commands& cmd){cmd.system(func); };
    };

    template <typename E, typename R, typename... Args>
    void add_event_with(R (&&func)(typename E::Arg, Args...)) {
        auto lambda = [func](ecs::Commands& cmd, typename E::Arg given){cmd.system_with<typename E::Arg>(func, given); };
        _super_events[std::type_index(typeid(E))] = std::make_any<std::function<void(ecs::Commands&, typename E::Arg)>>(lambda);
    };

    template <typename E, typename R, typename... Args>
    void add_event_with(std::function<R(typename E::Arg, Args...)> func) {
        auto lambda = [func](ecs::Commands& cmd, typename E::Arg given){cmd.system_with<typename E::Arg>(func, given); };
        _super_events[std::type_index(typeid(E))] = std::make_any<std::function<void(ecs::Commands&, typename E::Arg)>>(lambda);
    };


    template<typename A>
    void action(ecs::Commands& cmd) const {
        if (_mode == Mode::Server) {
            EActionFailureMode ret = std::get<0>(_actions[A::value])(cmd);
            if (ret == ActionFailureMode::Action_Success) {
                if (A::Presume != Presume::success)
                    _net.sendActionSuccess(A::value, std::vector<uint8_t>({0}));
            } else {
                if (A::Presume != Presume::fail)
                    _net.sendActionFailure(A::value, ret);
            }
        } else {
            if (A::presume == Presume::success)
                std::get<1>(_actions[A::value])(cmd);
            else if (A::presume == Presume::fail)
                std::get<2>(_actions[A::value])(cmd);
            _net.sendAction(A::value, std::vector<uint8_t>());
        }
    }

    template<typename A>
    void action_with(ecs::Commands& cmd, typename A::Arg given) {
        auto& server = std::get<0>(_super_actions[std::type_index(typeid(A))]);
        auto& client_success = std::get<1>(_super_actions[std::type_index(typeid(A))]);
        auto& client_fail = std::get<2>(_super_actions[std::type_index(typeid(A))]);
        if (_mode == Mode::Server) {
            const auto& func = std::any_cast<std::function<EActionFailureMode(ecs::Commands&, typename A::Arg)>>(server);
            EActionFailureMode ret = func(cmd, given);
            if (ret == ActionFailureMode::Action_Success) {
                if (A::Presume != Presume::success)
                    _net.sendActionSuccess(A::value, std::vector<uint8_t>({0}));
            } else {
                if (A::Presume != Presume::fail)
                    _net.sendActionFailure(A::value, ret);
            }
        } else {
            if (A::presume == Presume::success) {
                const auto& func = std::any_cast<std::function<void(ecs::Commands&, typename A::Arg)>>(client_success);
                func(cmd, given);
            } else if (A::presume == Presume::fail) {
                const auto& func = std::any_cast<std::function<void(ecs::Commands&, typename A::Arg)>>(client_fail);
                func(cmd, given);
            }
            std::vector<uint8_t> vec;
            serialize(vec, given);
            if (vec.size() != serialized_size<typename A::Arg>::value)
                throw std::exception();
            _net.sendAction(A::value, vec);
        }
    }

    template<typename E>
    void event(ecs::Commands& cmd) {
        _events[E::value](cmd);
        std::vector<uint8_t> vec(E::serialized_size, 0);
        _net.sendEvent(E::value, vec);
    }

    template<typename E>
    void event_with(ecs::Commands& cmd, typename E::Arg given) {
        const auto& func = std::any_cast<std::function<void(ecs::Commands&, typename E::Arg)>>(_super_events[std::type_index(typeid(E))]);
        func(cmd, given);
        std::vector<uint8_t> vec;
        serialize(vec, given);
        if (vec.size() != serialized_size<typename E::Arg>::value)
            throw std::exception();
        _net.sendEvent(E::value, vec);
    }


protected:
    using system = std::function<void(ecs::Commands&)>;
    std::unordered_map<uint16_t, std::tuple<system, system, system>> _actions;
    std::unordered_map<std::type_index, std::tuple<std::any, std::any, std::any>> _super_actions;
    std::unordered_map<uint16_t, system> _events;
    std::unordered_map<std::type_index, std::any> _super_events;
    Mode _mode;
    cevy::CevyNetwork &_net;

};
