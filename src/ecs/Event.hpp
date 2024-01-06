/*
** Agartha-Software, 2023
** Cevy
** File description:
** ecs event
*/

#pragma once

#include "App.hpp"
#include "Event.hpp"
#include <cstddef>
#include <queue>
#include <tuple>

namespace cevy::ecs {

template <typename T>
class EventWriter;

template <typename T>
class Event {
  private:
  friend EventWriter<T>;
  friend App;

  using EventWriterId = size_t;
  std::queue<std::tuple<T, EventWriterId>> _event_queue;
};

template <typename T>
class EventWriter {
  private:
  friend class App;
  Event<T> &_event_access;
  size_t _id;

  EventWriter(Event<T> &event_access) : _event_access(event_access) {}

  public:
  void send(T &&event) { _event_access._event_queue.push(std::make_tuple(std::move(event), _id)); }
};

template <typename T>
class EventReader {
  private:
  friend class App;

  EventReader(const Event<T> &event_access) : event_access(event_access) {}
  public:
  const Event<T> &event_access; // TODO: replace by a query like iterator

};

class EventPlugin : public cevy::ecs::Plugin {
  void build(cevy::ecs::App &);
};
} // namespace cevy::ecs
