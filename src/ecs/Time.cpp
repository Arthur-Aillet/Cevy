/*
** Agartha-Software, 2023
** Cevy
** File description:
** time
*/

#include "Time.hpp"
#include "World.hpp"

using cevy::ecs::Time;

Time::Time() : _first_update(std::chrono::high_resolution_clock::now()) {}

void init_timer(cevy::ecs::World &w) {
  w.insert_resource<cevy::ecs::Time>(cevy::ecs::Time());
}

void update_timer(cevy::ecs::Resource<Time> time) {
  time.get().update_with_instant(high_resolution_clock::now());
}
