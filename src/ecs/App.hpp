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
#include "ecs.hpp"

class cevy::ecs::App : public cevy::ecs::World {
  public:
  using World::ComponentId;
  using system = Schedule::system;

  /// TODO: refactor to has-a ?
  // World world;
  private:
  Schedule _scheduler;
  std::vector<std::unique_ptr<Plugin>> _plugins;

  template <typename GivenPlugin>
  void add_plugin(const GivenPlugin &plugin) {
    static_assert(std::is_base_of_v<Plugin, GivenPlugin>,
                  "Given plugin does not derive from Cevy Plugin class");
    static_assert(std::is_move_constructible_v<GivenPlugin>,
                  "Given plugin must be move constructible");
    _plugins.push_back(std::make_unique<GivenPlugin>(plugin));

    _plugins.back()->build(*this);
  }

  public:
  template <typename GivenPlugin, typename... Arg>
  void emplace_plugin(Arg... arg) {
    static_assert(std::is_base_of_v<Plugin, GivenPlugin>,
                  "Given plugin does not derive from Cevy Plugin class");
    static_assert(std::is_move_constructible_v<GivenPlugin>,
                  "Given plugin must be move constructible");
    _plugins.push_back(std::make_unique<GivenPlugin>(arg...));

    _plugins.back()->build(*this);
  }

  template <typename... GivenPlugin>
  void add_plugins(const GivenPlugin &...plugins) {
    ((add_plugin(std::forward<const GivenPlugin &>(plugins))), ...);
  }

  template <typename... GivenPlugin>
  void add_plugins() {
    ((emplace_plugin<GivenPlugin>()), ...);
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

  template <class S, class R, class... Args>
  void add_system(const std::function<R(Args...)> &func) {
    _scheduler.add_system<S>(func);
  }

  template <class R, class... Args>
  void add_system(R (&&func)(Args...)) {
    _scheduler.add_system(func);
  }

  template <class F, class S, class... Args>
  void add_class_system(const F &func) {
    _scheduler.add_class_system<F, S, Args...>(func);
  }

  template <typename T>
  void add_event() {
    insert_resource(Event<T>());
  }
};
