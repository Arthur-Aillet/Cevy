/*
** Agartha-Software, 2023
** Cevy
** File description:
** ecs event
*/

#pragma once

#include "Plugin.hpp"
#include "World.hpp"
#include <map>
#include <queue>
#include <tuple>
#include <any>
#include <typeindex>

namespace cevy::ecs {

class App;

template <typename T>
class EventWriter;

template <typename T>
class Event {
  private:
  friend EventWriter<T>;
  friend App;

  using EventWriterId = size_t;

  public:
  std::queue<std::tuple<T, EventWriterId>> event_queue;
};

template <typename T>
class EventWriter {
  private:
  friend class App;
  Event<T> &_event_access;
  size_t _idx;

  EventWriter(Event<T> &event_access, size_t idx) : _event_access(event_access), _idx(idx) {}

  public:
  void send(T &&event) { _event_access._event_queue.push(std::make_tuple(std::move(event), _idx)); }
};

template <typename T>
class EventReader {
  private:
  friend class App;

  EventReader(const Event<T> &event_access) : event_access(event_access) {}
  public:
  const Event<T> &event_access; // TODO: replace by a query like iterator

};

class EventManager {
  cevy::ecs::World &_world_access;
  std::map<std::type_index, std::any> _events;

  public:

  EventManager(cevy::ecs::World &world_access) : _world_access(world_access) {};

  template <typename T>
  EventWriter<T> get_event_writer() {
    return (EventWriter<T>(std::any_cast<ref<Event<T>>>(_events[typeid(T)]).get(), 0));
  }

  public:
  template <typename T>
  EventWriter<T> get_event_reader() {
    return (EventReader<T>(std::any_cast<ref<Event<T>>>(_events[typeid(T)]).get(), 0));
  }

  template <typename T>
  void add_event() {
    _world_access.insert_resource(Event<T>());
    auto &res = _world_access.get_resource<Event<T>>().value().get();
    std::any a = std::make_any<ref<Event<T>>>(res);
    _events.insert({std::type_index(typeid(T)), std::move(a)});
  }
};

class EventPlugin : public cevy::ecs::Plugin {
  void build(cevy::ecs::App &);
};
} // namespace cevy::ecs
