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

/**
 * @brief A container of app logic and data.
 *
 * Bundles together the necessary elements like World and Schedule to create an ECS-based
 * application.
 */
class cevy::ecs::App : public cevy::ecs::World {
  public:
  using World::ComponentId;
  using system = Schedule::system;
  // TODO: refactor to has-a ?
  //  World world;
  private:
  Schedule _scheduler;
  std::vector<std::shared_ptr<Plugin>> _plugins;

  private:
  /**
   * @brief Add a Plugin.
   *
   * C++evy follows Bevy’s core principle of modularity.
   * All Bevy engine features are implemented as Plugins.
   * This includes internal features like the renderer.
   * @tparam GivenPlugin
   * GivenPlugin type has to derive from the Plugin Abstract class
   * @param plugin
   */
  template <typename GivenPlugin>
  void add_plugin(const GivenPlugin &plugin) {
    static_assert(std::is_base_of_v<Plugin, GivenPlugin>,
                  "Given plugin does not derive from Cevy Plugin class");
    auto &p = _plugins.emplace_back(std::make_shared<GivenPlugin>(plugin));

    p->build(*this);
  }

  public:
  /**
   * @brief Adds one or more \link Plugin Plugins\endlink.
   *
   * C++evy follows Bevy’s core principle of modularity.
   * All Bevy engine features are implemented as Plugins.
   * This includes internal features like the renderer.
   * @tparam GivenPlugin
   * All the GivenPlugin types have to derive from the Plugin Abstract class
   * @param plugin
   */
  template <typename... GivenPlugin>
  void add_plugins(const GivenPlugin &...plugins) {
    ((add_plugin(std::forward<const GivenPlugin &>(plugins))), ...);
  }

  /**
   * @brief Starts the application, launch the \link cevy::ecs::Schedule::Stage Stages\endlink and Systems loop
   */
  void run();

  /**
   * @brief Add a stage to the pile of \link cevy::ecs::Schedule::Stage Stages\endlink.
   * Each systems are assigned to a specific stage (\link cevy::ecs::Schedule::Update Update\endlink by Default) and are run in the stage order
   * Look at \link cevy::ecs::Schedule::Stage Stage \endlink to understand how to insert a stages in a given order
   *
   * @tparam Stage
   * Must made from the \link cevy::ecs::Schedule::Stage Stage \endlink struct
   */
  template <typename Stage>
  void add_stage() {
    _scheduler.insert_schedule<Stage>();
  }

  template <class S, class R, class... Args>
  void add_system(R (&&func)(Args...)) {
    _scheduler.add_system<S>(func);
  }

  template <class R, class... Args>
  void add_system(R (&&func)(Args...)) {
    _scheduler.add_system(func);
  }

  template <typename T>
  void add_event() {
    insert_resource(Event<T>());
  }
};
