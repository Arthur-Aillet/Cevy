/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** NetworkPlugin
*/

#include "NetworkPlugin.hpp"
#include "network/network.hpp"

using cevy::NetworkPlugin;

void NetworkPlugin::build(cevy::ecs::App& app) {
    app.init_resource<NetworkCommands>(NetworkCommands(*_action, *_sync, app));
    _sync->build(app);
    _action->build(app);
};
