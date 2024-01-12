/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** NetworkCommands
*/

#pragma once

#include "network.hpp"
#include "NetworkAction.hpp"

class cevy::NetworkCommands {
  public:
  NetworkCommands(NetworkActions& actions, Synchroniser& sync)
    : _actions(actions), _sync(sync) {};
  ~NetworkCommands();

  template<typename A>
  void action() {
    _actions.action<A>();
  };

  template<typename A>
  void event() {
    _actions.action<A>();
  };

  protected:
  NetworkActions& _actions;
  Synchroniser& _sync;
  private:
};
