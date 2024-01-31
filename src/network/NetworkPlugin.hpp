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


  NetworkPlugin(NetworkPlugin &&rhs) : _net(std::move(rhs._net)), _sync(std::move(rhs._sync)), _action(std::move(rhs._net)) {
      _action._sync = _sync;
  };



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
    app.add_systems<ecs::core_stage::PostUpdate>(make_function<void, ecs::Resource<NetworkCommands>>([this](auto cmd) -> void { activity_system(cmd.get());}));
  };

  protected:

  using ActionFailureMode = CevyNetwork::ActionFailureMode;
  using EActionFailureMode = ActionFailureMode::EActionFailureMode;
  using Actor = NetworkActions::Actor;


  void activity_system(NetworkCommands& cmd) {
    while (true) {
      auto event = _net.recvEvent();
      if (!event)
        break;
      auto& [id, vec] = event.value();
      vec.resize(std::max(vec.size(), _action._remote_event_arg_size[id]));
      event_remote(cmd, id, vec);
    }
    while (true) {
      auto action = _net.recvAction();
      if (!action)
        break;
      auto& [actor, state, id, vec] = action.value();
      EActionFailureMode fail = EActionFailureMode(state);
      vec.resize(std::max(vec.size(), _action._remote_act_arg_size[id]));
      if (fail == EActionFailureMode::Action_Yet) {
        action_remote(cmd, actor, id, vec);
      } else if (fail == EActionFailureMode::Action_Success) {
        actionSuccess_remote(cmd, id, vec);
      } else {
        actionFailure_remote(cmd, fail, id, vec);
      }
    }
  }

  protected:
  void event_remote(NetworkCommands& cmd, uint16_t id, std::vector<uint8_t> &vec) {
    // std::cerr << "(INFO)event_remote: treating" << id << std::endl;
    if (id == CevyNetwork::Event::Summon) {
      uint8_t archetype = deserialize<uint8_t>(vec);
      _sync.summon(cmd, archetype);
    }
    if (_action._events.find(id) != _action._events.end()) {
      _action._events.find(id)->second(cmd);
    } else if (_action._remote_events.find(id) != _action._remote_events.end()) {
      _action._remote_events.find(id)->second(cmd, vec);
    } else {
      std::cerr << "(WARNING)event_remote: unmapped" << std::endl;
    }
  }

  void action_remote(NetworkCommands& cmd, Actor actor, uint16_t id, std::vector<uint8_t> &vec) {
    EActionFailureMode state;
    // std::cerr << "(INFO)action_remote: treating" << id << std::endl;
    if (_action._actions.find(id) != _action._actions.end()) {
      state = std::get<0>(_action._actions.find(id)->second)(cmd, actor);
    } else if (_action._remote_actions.find(id) != _action._remote_actions.end()) {
      state = std::get<0>(_action._remote_actions.find(id)->second)(cmd, actor, vec);
    } else {
      std::cerr << "(WARNING)action_remote: unmapped" << std::endl;
      return;
    }
    if (state == ActionFailureMode::Action_Success) {
      if (_action._remote_act_presume[id] != NetworkActions::Presume::success)
        _net.sendActionSuccess(actor, id);
    } else {
      if (_action._remote_act_presume[id] != NetworkActions::Presume::fail)
        _net.sendActionFailure(actor, id, state);
    }

  }

  void actionSuccess_remote(NetworkCommands& cmd, uint16_t id, std::vector<uint8_t> &vec) {
    // std::cerr << "(INFO)actionSuccess_remote: treating" << id << std::endl;
    if (_action._actions.find(id) != _action._actions.end()) {
      std::get<1>(_action._actions.find(id)->second)(cmd);
    } else if (_action._remote_actions.find(id) != _action._remote_actions.end()) {
      std::get<1>(_action._remote_actions.find(id)->second)(cmd, vec);
    } else {
      std::cerr << "(WARNING)actionSuccess_remote: unmapped" << std::endl;
    }
  }

  void actionFailure_remote(NetworkCommands& cmd, EActionFailureMode fail, uint16_t id, std::vector<uint8_t> &vec) {
    // std::cerr << "(INFO)actionFailure_remote: treating" << id << std::endl;
    if (_action._actions.find(id) != _action._actions.end()) {
      std::get<2>(_action._actions.find(id)->second)(cmd, fail);
    } else if (_action._remote_actions.find(id) != _action._remote_actions.end()) {
      std::get<2>(_action._remote_actions.find(id)->second)(cmd, fail, vec);
    } else {
      std::cerr << "(WARNING)actionFailure_remote: unmapped" << std::endl;
    }
  }


  protected:
  N _net;
  S _sync;
  A _action;

  private:
};
