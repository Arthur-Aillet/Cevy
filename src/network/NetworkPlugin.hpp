/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** NetworkPlugin
*/

#pragma once

#include <memory>

#include "NetworkCommands.hpp"
#include "Plugin.hpp"
#include "App.hpp"
#include "Synchroniser.hpp"
#include "network.hpp"

class cevy::NetworkPlugin : ecs::Plugin {
public:
    ~NetworkPlugin();

    template<typename S, typename A>
    static void construct(S&& s, A&& a) {
        NetworkPlugin this_;
        this_._sync = std::make_unique<S>(std::move(s));
        this_._action = std::make_unique<A>(std::move(a));

    }

    template<typename S, typename A>
    NetworkPlugin(S&& s, A&& a) {
        _sync = std::make_unique<S>(std::move(s));
        _action = std::make_unique<A>(std::move(a));
    }

    void build(ecs::App& app) override {
        app.init_resource<NetworkCommands>(NetworkCommands(*_action, *_sync));
        _sync->build(app);
        _action->build(app);
    };

protected:
    NetworkPlugin();
    std::unique_ptr<Synchroniser> _sync;
    std::unique_ptr<NetworkActions> _action;
private:
};
