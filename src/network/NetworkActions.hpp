/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** NetworkActions.hpp
*/

#pragma once

#include "App.hpp"
#include "CevyNetwork.hpp"
#include "Plugin.hpp"
#include "Query.hpp"
#include "Schedule.hpp"
#include "network/network.hpp"

class cevy::NetworkActions : virtual public cevy::ecs::Plugin {
    public:
    enum class Mode {
        Server,
        Client,
    };

    NetworkActions(Mode mode, const std::string &host = std::string(""))
        : _mode(mode), _net(host, 4995) {};
    ~NetworkActions() {};

    protected:
    void build(cevy::ecs::App &app) override {

    }

    protected:
    CevyNetwork _net;
    Mode _mode;


};