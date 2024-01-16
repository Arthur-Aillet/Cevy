/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** NetworkPlugin
*/

#pragma once

#include <memory>

#include "App.hpp"
#include "NetworkCommands.hpp"
#include "Plugin.hpp"
#include "Stage.hpp"
#include "Synchroniser.hpp"
#include "cevy.hpp"
#include "network.hpp"
#include "network/network.hpp"

template <typename S, typename A, typename N = cevy::CevyNetwork>
class cevy::NetworkPlugin : public ecs::Plugin {
  public:
  ~NetworkPlugin(){};

  // NetworkPlugin(CevyNetwork::NetworkMode mode, const std::string &endpoint, size_t udp_port, size_t tcp_port,
  //             size_t client_offset)
  //     : _net(mode, endpoint, udp_port, tcp_port, client_offset), _sync(_net), _action(_net){};

  NetworkPlugin(N &&net) : _net(std::move(net)), _sync(_net), _action(_net){};

  template<typename... Args>
  NetworkPlugin(Args... args) : _net(args...), _sync(_net), _action(_net){};


  NetworkPlugin(NetworkPlugin &&rhs) : _net(std::move(rhs._net)), _sync(std::move(rhs._sync)), _action(std::move(rhs._net)) {};


  // template<typename S, typename A>
  // NetworkPlugin(S&& s, A&& a) {
  //     _sync = std::make_unique<S>(std::move(s));
  //     _action = std::make_unique<A>(std::move(a));
  // }

  void build(cevy::ecs::App &app) override {
    std::cout << "(INFO)NetworkPlugin::build" << std::endl;
    _net.build(app);
    app.init_resource<cevy::NetworkCommands>(cevy::NetworkCommands(_net, _action, _sync, app));
    _sync.build(app);
    _action.build(app);
    app.add_systems<ecs::core_stage::ShutDown>(make_function<void>([this](){ _net.die();}));
  };

  protected:
  N _net;
  S _sync;
  A _action;

  private:
};
