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
  //std::cout << res.get().event_queue.size() << std::endl;
}

void exemple_event_system2(cevy::ecs::EventWriter<int> res) {
  static int i = 0;
  res.send(i++);
}

void exemple_event_system3(cevy::ecs::EventReader<int> res) {
  for (auto &val : res.event_access.event_queue) {
    std::cout << std::get<0>(val) << " : " << std::get<1>(val) << std::endl;
  }
}

void cevy::ecs::EventPlugin::build(cevy::ecs::App &app) {
  app.add_event<int>();
  app.add_system(exemple_event_system);
  app.add_system(exemple_event_system2);
  app.add_system(exemple_event_system3);
/*app.add_system<>();*/
}
