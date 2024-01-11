/*
** Agartha-Software, 2023
** Cevy
** File description:
** Schedule
*/

#include "Schedule.hpp"
#include "Event.hpp"
#include "World.hpp"

using cevy::ecs::Schedule;

void Schedule::runStage(World &world) {
  std::vector<std::reference_wrapper<system>> curr_sys;

  std::copy_if(_systems.begin(), _systems.end(), std::back_inserter(curr_sys),
               [this](const system &sys) { return std::get<1>(sys) == *_stage; });

  /* this part could be multi-threaded */
  for (auto sys : curr_sys) {
    std::get<0>(sys.get())(world);
  }

  _stage++;
}

void Schedule::runStartStages(World &world) {
  _stage = _at_start_schedule.begin();
  while (_stage != _at_start_schedule.end() && !_abort) {
    runStage(world);
  }
}

void Schedule::runStages(World &world) {
  _stage = _schedule.begin();
  while (_stage != _schedule.end() && !_abort) {
    runStage(world);
  }
}

void Schedule::run(World &world) {
  runStartStages(world);
  while (!_stop) {
    runStages(world);
    while (!world._command_queue.empty()) {
      std::function<void(World &)> func = world._command_queue.front();
      world._command_queue.pop();
      func(world);
    }
    auto close = world.get_resource<Event<AppExit>>();
    auto abort = world.get_resource<Event<AppAbort>>();
    if (close && close.value().get().event_queue.size() > 0) {
      _stop = true;
    }
    if (abort && abort.value().get().event_queue.size() > 0) {
      _stop = true;
      _abort = true;
    }
  }
}
