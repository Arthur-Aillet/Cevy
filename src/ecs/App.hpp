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
 *
 * @warning For now App derive from the class World instead of containing a World, this is up to
 * change
 *
 */
class cevy::ecs::App : public cevy::ecs::World {
  public:
  using World::ComponentId;
  using system = Schedule::system;
  // TODO: refactor to has-a ? Change App description
  //  World world;
  private:
  /**
   * @brief Schedule represent the manager of Systems and \link cevy::ecs::Schedule::Stage
   * Stages\endlink.
   *
   */
  Schedule _scheduler;
  /**
   * @brief Store plugins
   *
   * Allows plugins to hold values, this let plugin.build() to not be a dynamic function and for
   * plugin to take parameters
   */
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
   * @brief Starts the application, launch the \link cevy::ecs::Schedule::Stage Stages\endlink and
   * Systems loop
   */
  void run();

  /**
   * @brief Add a stage to the pile of \link cevy::ecs::Schedule::Stage Stages\endlink.
   * Each systems are assigned to a specific stage (\link cevy::ecs::Schedule::Update Update\endlink
   * by Default) and are run in the stage order Look at \link cevy::ecs::Schedule::Stage Stage
   * \endlink to understand how to insert a stages in a given order
   *
   * @tparam Stage
   * Have to be made from the \link cevy::ecs::Schedule::Stage Stage \endlink struct.
   */
  template <typename Stage>
  void add_stage() {
    _scheduler.insert_schedule<Stage>();
  }

  /**
   * @brief Move one or multiple systems in this app’s \link cevy::ecs::Schedule::Update
   * Update\endlink Schedule.
   *
   * @tparam System Could be any thing that have the Operator `()` implemented
   * @param system
   */
  template <class... System>
  void add_systems(System &&...system) {
    (_scheduler.add_system(system), ...);
  }

  /**
   * @brief Move one or multiple systems to the given schedule in this app’s \link
   * cevy::ecs::Schedule Schedules\endlink.
   *
   * @tparam System Could be any thing that have the Operator `()` implemented
   * @param system
   */
  template <class Stage, class... System>
  void add_systems(System &&...system) {
    (_scheduler.add_system<Stage>(system), ...);
  }

  /**
   * @brief Setup the application to manage events of type T
   *
   * This is done by adding a Resource of type @link Events Events<T>@endlink
   * @tparam T Type serving as event, All are supported
   * @warning C++evy implementation differs from `Bevy` one's where
   * we don't define a event_update_system into First,
   * the event queue cleaning is done when the event reach it's EventWriter again,
   * supporting multiple EventWriter
   * See Events for defining events.
   */
  template <typename T>
  void add_event() {
    insert_resource(Event<T>());
  }
};
