/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** NetworkCommands
*/

#pragma once

#include "ecs.hpp"
#include "network.hpp"
#include "NetworkActions.hpp"

class cevy::NetworkCommands : protected ecs::Commands {
  public:
  NetworkCommands(NetworkActions& actions, Synchroniser& sync, ecs::World& world)
    : Commands(world), _actions(actions), _sync(sync) {};
  ~NetworkCommands();

  template<typename A>
  void action() {
    _actions.action<A>(*this);
  };

  template<typename A>
  void event() {
    _actions.event<A>(*this);
  };

  template<typename A, typename GivenArgs>
  void event_with(GivenArgs given) {
    _actions.event_with<A>(*this, given);
  };

  protected:
  NetworkActions& _actions;
  Synchroniser& _sync;
  private:
};
