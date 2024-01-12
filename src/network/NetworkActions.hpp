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
#include "Plugin.hpp"
#include "network/CevyNetwork.hpp"

template <typename A, typename F>
class NetworkActions : virtual public cevy::ecs::Plugin {
public:
    class ShipActions;

    NetworkActions(const NetworkActions &) = delete;
    NetworkActions &operator=(const NetworkActions &) = delete;
    NetworkActions &operator=(NetworkActions &&) = delete;
    NetworkActions(NetworkActions &&rhs) : _net(std::move(rhs._net)) {}
    NetworkActions(const std::string &host = std::string(""))
        : _net(host, 4995){};
    ~NetworkActions();

    template<size_t N = 0>
    struct Action {
        enum class Presume {
            idk,
            fail,
            success,
        };
        static const uint8_t value = N;
        static const Presume presume = Presume::idk;

        enum class Act {
            Shoot,
            FlyUp,
            FlyDown,
        };
    };

    virtual void build(cevy::ecs::App& app) = 0;

    void add_action(F function) {
        _actions[A::value].first = function;
        _actions[A::value].seconde = nullptr;
    };

    void add_action(F client_success, F client_fail) {
            _actions[A::value].first = client_fail;
            _actions[A::value].seconde = client_success;
    };


    void add_event(A action, F function) {
        _events[A::value] = function;
    };

protected:
    std::unordered_map<size_t, std::pair<const std::function<void()>, const std::function<void()>>> _actions;
    std::unordered_map<size_t, const std::function<void()>> _events;
    cevy::CevyNetwork _net;

};

class cevy::NetworkActions::ShipActions {
public:
    ShipActions(NetworkActions &netAct, CevyNetwork &net)
        : _netAct(netAct), _net(net) {};
    struct Shoot : Action<A::Act::Shoot> {};
    struct FlyUp : Action<A::Act::FlyUp> {
        Presume presume = Presume::success;
    };

    void build(cevy::ecs::App& app) override {
        add_action<Shoot>(shootAction);
    }

    static void shootAction(cevy::ecs::App& app) {};

private:
    CevyNetwork &_net;
    NetworkActions &_netAct;
};
