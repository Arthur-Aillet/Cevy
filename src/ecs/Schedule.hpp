/*
** Agartha-Software, 2023
** Cevy
** File description:
** Schedule
*/

#pragma once
#include <algorithm>
#include <functional>
#include <list>
#include <optional>
#include <tuple>
#include <type_traits>
#include <typeindex>

#include "Event.hpp"
#include "World.hpp"
#include "ecs.hpp"

namespace cevy::ecs {

typedef struct AppExit {
} AppExit;

typedef struct AppAbort {
} AppAbort;

class Schedule {
  public:
  struct IsStage {};

  template <typename Before = std::nullopt_t, typename After = std::nullopt_t,
            typename Repeat = std::true_type>
  class Stage : public IsStage {
    public:
    using is_repeat = Repeat;
    using previous = Before;
    using next = After;

    template <typename T>
    using before = Stage<T, std::nullopt_t, typename T::is_repeat>;

    template <typename T>
    using after = Stage<std::nullopt_t, T, typename T::is_repeat>;
  };

  template <typename T>
  using before = Stage<>::before<T>;
  template <typename T>
  using after = Stage<>::after<T>;

  using at_start = Stage<std::nullopt_t, std::nullopt_t, std::false_type>;

  class Startup : public at_start {};
  class PreStartup : public before<Startup> {};
  class PostStartup : public after<Startup> {};

  class First : public Stage<> {};

  class Update : public after<First> {};
  class PreUpdate : public before<Update> {};
  class PostUpdate : public after<Update> {};

  class StateTransition : public before<Update> {};
  class RunFixedUpdateLoop : public before<Update> {};

  class Last : public after<PostUpdate> {};

  using SystemId = size_t;

  private:
  std::list<std::type_index> _schedule;
  std::list<std::type_index> _at_start_schedule;
  SystemId last_id = 0;

  public:
  template <typename S, typename std::enable_if_t<
                            std::is_same_v<typename S::is_repeat, std::true_type>, bool> = true>
  bool schedule_defined() {
    auto it = std::find(_schedule.begin(), _schedule.end(), std::type_index(typeid(S)));

    return (it != _schedule.end());
  }

  template <typename S, typename std::enable_if_t<
                            std::is_same_v<typename S::is_repeat, std::false_type>, bool> = true>
  bool schedule_defined() {
    auto it =
        std::find(_at_start_schedule.begin(), _at_start_schedule.end(), std::type_index(typeid(S)));

    return (it != _at_start_schedule.end());
  }

  template <typename T, typename std::enable_if_t<
                            std::is_same_v<typename T::is_repeat, std::true_type>, bool> = true>
  void insert_schedule() {
    if constexpr (!std::is_same_v<typename T::previous, std::nullopt_t>) {
      auto it = std::find(_schedule.begin(), _schedule.end(),
                          std::type_index(typeid(typename T::previous)));

      _schedule.insert(it, std::type_index(typeid(T)));
    } else if constexpr (!std::is_same_v<typename T::next, std::nullopt_t>) {
      auto it =
          std::find(_schedule.begin(), _schedule.end(), std::type_index(typeid(typename T::next)));

      ++it;
      _schedule.insert(it, std::type_index(typeid(T)));
    } else {
      _schedule.push_back(std::type_index(typeid(T)));
    }
  }

  template <typename T, typename std::enable_if_t<
                            std::is_same_v<typename T::is_repeat, std::false_type>, bool> = true>
  void insert_schedule() {
    if constexpr (!std::is_same_v<typename T::previous, std::nullopt_t>) {
      auto it = std::find(_at_start_schedule.begin(), _at_start_schedule.end(),
                          std::type_index(typeid(typename T::previous)));

      _at_start_schedule.insert(it, std::type_index(typeid(T)));
    } else if constexpr (!std::is_same_v<typename T::next, std::nullopt_t>) {
      auto it = std::find(_at_start_schedule.begin(), _at_start_schedule.end(),
                          std::type_index(typeid(typename T::next)));

      ++it;
      _at_start_schedule.insert(it, std::type_index(typeid(T)));
    } else {
      _at_start_schedule.push_back(std::type_index(typeid(T)));
    }
  }

  using system_function = std::function<void(World &)>;
  using system = std::tuple<system_function, std::type_index>;
  std::vector<system> _systems;
  Schedule() : _stage(_at_start_schedule.begin()){};
  ~Schedule() = default;

  void quit() const;
  void abort();

  template <class R, class... Args>
  void add_system(R (&&func)(Args...)) {
    add_system<Update>(func);
  }

  template <class S, class R, class... Args>
  void add_system(R (&&func)(Args...)) {
    static_assert(
        all(Or<is_query<Args>, is_world<Args>, is_resource<Args>, is_commands<Args>,
               is_event_reader<Args>, is_event_writer<Args>>()...),
        "type must be reference to query, world, commands, event reader, event writer or resource");
#ifdef DEBUG
    if (!schedule_defined<S>()) {
      std::cerr << "WARNING/Cevy: Stage not yet added to ecs pipeline" << std::endl;
    }
#endif

    system_function sys = [id = last_id, &func](World &reg) mutable {
      func(reg.get_super<Args>(id)...);
    };
    last_id += 1;
    _systems.push_back(std::make_tuple(sys, std::type_index(typeid(S))));
  }

  protected:
  mutable bool _stop = false;
  mutable bool _abort = false;
  std::list<std::type_index>::iterator _stage;

  void runStartStages(World &world);
  void runStages(World &world);
  void runStage(World &world);

  private:
  /* Bevy-compliant */
  public:
  void run(World &world);
};
} // namespace cevy::ecs
