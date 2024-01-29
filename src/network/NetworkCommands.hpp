/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** NetworkCommands
*/

#pragma once

#include "NetworkActions.hpp"
#include "Synchroniser.hpp"
#include "ecs.hpp"
#include "network.hpp"
#include "network/NetworkBase.hpp"
#include <type_traits>

/**
 * @brief Commands helper for facilitating network related actions
 *
 * binds to a given \link cevy::NetworkActions NetworkActions \endlink
 * Accessed from a system as a
 * \link cevy::ecs::Resource Resource<NetworkCommands> \endlink
 *
 */
class cevy::NetworkCommands : protected ecs::Commands {
  friend class NetworkActions;
  template<typename S, typename A, typename N>
  friend class NetworkPlugin;
  public:
  NetworkCommands(CevyNetwork& net, NetworkActions &actions, Synchroniser &sync, ecs::World &world)
      : Commands(world), _net(net), _actions(actions), _sync(sync){};
  ~NetworkCommands(){};

  /**
   * @brief Call an action
   *
   * when run client-side, this will send the action to the server, and preform
   * the presumed system if specified
   *
   * when run server-side, this will perform the server system following which
   * the action is responded to with a failure or success
   *
   * @tparam A Action to call
   */
  template <typename A>
  void action() {
    _actions.action<A>(*this);
  };

  /**
   * @brief Call an action with a given argument
   *
   * the argument must correspond to the one specified by the Action
   *
   * when run client-side, this will send the action to the server, and preform
   * the presumed system if specified
   *
   * when run server-side, this will perform the server system following which
   * the action is responded to with a failure or success
   *
   * the argument will be send, so it must be defined for
   * \link cevy::serialize_impl serialize \endlink
   *
   * @tparam A Action to call
   * @tparam A::Arg Argument required
   * @param given argument to give
   */
  template <typename A>
  void action_with(typename A::Arg given) {
    _actions.action_with<A>(*this, given);
  };

  /**
   * @brief send an event
   *
   * Event will be sent, and the system will be run on the server and all clients
   *
   * @tparam E Event to send
   */
  template <typename E>
  void event() {
    // if (_net.mode() == NetworkBase::Server)
      _actions.event<E>(*this);
  };

  /**
   * @brief Send an event with a given argument
   *
   * the argument must correspond to the one specified by the Event
   *
   * Event will be sent, and the system will be run on the server and all clients
   *
   * the argument will be send, so it must be defined for
   * \link serialize_impl serialize_impl \endlink
   *
   * @tparam E Event to send
   * @tparam E::Arg Argument required
   * @param given argument to give
   */
  template <typename E>
  void event_with(typename E::Arg given) {
    if (_net.mode() == CevyNetwork::NetworkMode::Server)
      _actions.event_with<E>(*this, given);
  };

  template <typename T>
  void summon() {
    if (_net.mode() == CevyNetwork::NetworkMode::Server)
      _sync.summon<T>(*this);
  };

  template <typename T, typename... Components>
  void summon(Components... components) {
    if (_net.mode() == CevyNetwork::NetworkMode::Server)
      _sync.summon<T>(*this, components...);
  };

  template <typename T, typename U>
  void summon(CevyNetwork::ConnectionDescriptor cd) {
    if (_net.mode() == CevyNetwork::NetworkMode::Server)
      _sync.summon<T, U>(*this, cd);
  };

  void dismiss(Synchroniser::SyncId id) {
    if (_net.mode() == CevyNetwork::NetworkMode::Server)
      _sync.dismiss(*this, id);
  }

  void connect(const std::string& str) {
    auto* handler = dynamic_cast<ClientHandler*>(&_net);
    if (!handler) {
      std::cerr << "(ERROR)connect: cast went wrong" << std::endl;
      std::exit(0);
    }
    handler->connect(*this, str);
  }

  void die() const {
    _net.die();
  }

  protected:
  CevyNetwork &_net;
  NetworkActions &_actions;
  Synchroniser &_sync;

  private:
};
