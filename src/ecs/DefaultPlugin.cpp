/*
** Agartha-Software, 2023
** Cevy
** File description:
** Default Plugin
*/

#include "DefaultPlugin.hpp"
#include "App.hpp"
#include "input/Keyboard.hpp"
#include "input/Mouse.hpp"
#include "Time.hpp"
#include "ecs.hpp"

void init_default_schedules(cevy::ecs::App &app) {
  using cevy::ecs::Schedule;

  app.add_stage<Schedule::Startup>();
  app.add_stage<Schedule::PreStartup>();
  app.add_stage<Schedule::PostStartup>();

  app.add_stage<Schedule::First>();
  app.add_stage<Schedule::Update>();
  app.add_stage<Schedule::PreUpdate>();
  app.add_stage<Schedule::PostUpdate>();
  app.add_stage<Schedule::Last>();
}

void init_timer(cevy::ecs::World &w) { w.insert_resource<cevy::ecs::Time>(cevy::ecs::Time()); }

void cevy::ecs::DefaultPlugin::build(cevy::ecs::App &app) {
  init_default_schedules(app);
  app.insert_resource(cevy::ecs::Control{.abort = false});
  app.add_system<cevy::ecs::Schedule::PostStartup>(init_timer);
  app.insert_resource(Input<KeyBoard>());
  app.insert_resource(Input<Mouse>());
  app.add_system<cevy::ecs::Schedule::PreUpdate>(update_all_inputs);
}
