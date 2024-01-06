/*
** Agartha-Software, 2023
** Cevy
** File description:
** ecs event
*/

#include "Event.hpp"
#include "App.hpp"
#include "ecs.hpp"

void exemple_event_system(cevy::ecs::Resource<cevy::ecs::Event<int>> res) {
  std::cout << res.get().event_queue.size() << std::endl;
}

void cevy::ecs::EventPlugin::build(cevy::ecs::App &app) {
  app.add_event<int>();
  app.add_system(exemple_event_system);
/*app.add_system<>();*/
}
