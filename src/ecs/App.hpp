/*
** Agartha-Software, 2023
** Cevy
** File description:
** App
*/

#pragma once

#include <memory>

#include "Plugin.hpp"
#include "Schedule.hpp"
#include "World.hpp"
#include "Event.hpp"
#include "ecs.hpp"

class cevy::ecs::App : public cevy::ecs::World {
  public:
  using World::ComponentId;
  using system = Schedule::system;
  //TODO: refactor to has-a ?
  // World world;
  private:
  Schedule _scheduler;
  EventManager _event_manager;
  std::vector<std::shared_ptr<Plugin>> _plugins;

  public:
  App() : _event_manager(EventManager(*this)) {};

  private:
  template <typename GivenPlugin>
  void add_plugin(const GivenPlugin &plugin) {
    static_assert(std::is_base_of_v<Plugin, GivenPlugin>,
                  "Given plugin does not derive from Cevy Plugin class");
    auto &p = _plugins.emplace_back(std::make_shared<GivenPlugin>(plugin));

    p->build(*this);
  }

  public:
  template <typename... GivenPlugin>
  void add_plugins(const GivenPlugin &...plugins) {
    ((add_plugin(std::forward<const GivenPlugin &>(plugins))), ...);
  }

  void run();
  void quit();
  void abort();

  template <typename T>
  void add_stage() {
    _scheduler.insert_schedule<T>();
  }

  template <class S, class R, class... Args>
  void add_system(R (&&func)(Args...)) {
    _scheduler.add_system<S>(func);
  }

  template <class R, class... Args>
  void add_system(R (&&func)(Args...)) {
    _scheduler.add_system(func);
  }

  template <typename Event>
  void add_event() {
    _event_manager.add_event<Event>();
  }
};
