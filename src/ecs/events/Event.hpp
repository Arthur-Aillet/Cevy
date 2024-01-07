/*
** Agartha-Software, 2023
** Cevy
** File description:
** ecs event
*/

#pragma once

#include "Plugin.hpp"
#include <tuple>
#include <vector>

namespace cevy::ecs {
class World;

template <typename T>
class EventWriter;

template <typename T>
class Event {
  private:
  friend EventWriter<T>;
  friend class World;

  using EventWriterId = size_t;

  public:
  std::vector<std::tuple<T, EventWriterId>> event_queue{};
};

template <typename T>
class EventWriter {
  private:
  friend class World;
  Event<T> &_event_access;
  size_t _idx;

  EventWriter(Event<T> &event_access, size_t idx) : _event_access(event_access), _idx(idx) {}

  public:
  using value_type = T;

  void send(T &&event) {
    _event_access.event_queue.push_back(std::make_tuple(std::move(event), _idx));
  }
};

template <typename T>
class EventReader {
  private:
  friend class World;

  EventReader(const Event<T> &event_access) : event_access(event_access) {}

  public:
  using value_type = T;

  const Event<T> &event_access; // TODO: replace by a query like iterator
};

class EventPlugin : public cevy::ecs::Plugin {
  void build(cevy::ecs::App &);
};
} // namespace cevy::ecs

template <class T>
struct is_event_writer : public std::false_type {};

template <typename T>
struct is_event_writer<cevy::ecs::EventWriter<T>> : public std::true_type {};

template <class T>
struct is_event_reader : public std::false_type {};

template <typename T>
struct is_event_reader<cevy::ecs::EventReader<T>> : public std::true_type {};
