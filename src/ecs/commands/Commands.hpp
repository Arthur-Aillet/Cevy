/*
** Agartha-Software, 2023
** C++evy
** File description:
** Commands
*/

#pragma once

#include "Command.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include <queue>
#include <type_traits>

namespace cevy {
namespace ecs {
class EntityCommands;
}
} // namespace cevy

class cevy::ecs::Commands {
  protected:
  friend class cevy::ecs::World;

  cevy::ecs::World &_world_access;
  using command = std::function<void(World &)>;
  std::queue<command> _command_queue;
  Commands(cevy::ecs::World &world_access) : _world_access(world_access){};

  public:
  ~Commands() { merge_queues(_world_access._command_queue, _command_queue);};
  template <typename GivenCommand,
            typename std::enable_if_t<std::is_base_of_v<Command, GivenCommand>, bool> = true>
  void add(const GivenCommand &a) {
    auto l = [a](cevy::ecs::World &w) { a.apply(w); };
    _command_queue.push(l);
  }

  void add(std::function<void(cevy::ecs::World &w)> &&f);

  static void merge_queues(std::queue<command> &to, std::queue<command> &from) {
        while (from.size()) {
      to.push(from.front());
      from.pop();
    }
  }

  void merge(Commands& other) {
    merge_queues(_command_queue, other._command_queue);
  }

  void clear() {
    while (_command_queue.size())
      _command_queue.pop();
  }

  template <typename R>
  void insert_resource(const R &value) {
    add([value](cevy::ecs::World &w) { w.insert_resource(value); });
  }

  cevy::ecs::EntityCommands spawn_empty();

  void despawn(Entity e);

  template <typename... Ts>
  cevy::ecs::EntityCommands spawn(Ts... a);

  EntityCommands entity(const Entity &e);

  template <class R, class... Args>
  R system(R (&&func)(Args...)) {
    return _world_access.run_system<R, Args...>(func);
  }

  // template < class... Args>
  // void system(void (&&func)(Args...)) {
  //     _world_access.run_system<Args...>(func);
  // }

  template <class GivenArgs, class R, class... Args>
  R system_with(R (&&func)(GivenArgs, Args...), GivenArgs given) {
    return _world_access.run_system_with<GivenArgs, R, Args...>(func, given);
  }

  template <typename GivenArgs, class R, class... Args>
  R system_with(std::function<R(GivenArgs, Args...)> func, GivenArgs given) {
    return _world_access.run_system_with<GivenArgs, R, Args...>(func, given);
  }

  // template <class GivenArgs, class... Args>
  // void system_with(void (&&func)(GivenArgs, Args...), GivenArgs given) {
  //   _world_access.run_system_with<GivenArgs, Args...>(func, given);
  // }

  template <class R, class... Args>
  R system(std::function<R(Args...)> func) {
    return _world_access.run_system<R, Args...>(func);
  }

  // template <class... Args>
  // void system(std::function<void(Args...)> func) {
  //   _world_access.run_system<Args...>(func);
  // }

  template <class R, class... Args, typename... GivenArgs>
  R system_with_tuple(std::function<R(Args...)> func, std::tuple<GivenArgs...> given) {
    return _world_access.run_system_with_tuple<R, Args...>(func, given);
  }


  // template <class GivenArgs, class... Args>
  // void system_with(std::function<void(GivenArgs, Args...)> func, GivenArgs given) {
  //   _world_access.run_system_with<GivenArgs, Args...>(func, given);
  // }
};

template <typename C, typename std::enable_if_t<is_commands<C>::value, bool>>
C cevy::ecs::World::get_super(size_t _last_id) {
  return cevy::ecs::Commands(*this);
}
