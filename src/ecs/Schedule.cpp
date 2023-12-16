/*
** Agartha-Software, 2023
** Cevy
** File description:
** Schedule
*/

#include "Schedule.hpp"

using cevy::ecs::Schedule;


void Schedule::runStage(World &world)
{
    std::vector<std::reference_wrapper<system>> curr_sys;

    std::copy_if(
        _systems.begin(),
        _systems.end(),
        std::back_inserter(curr_sys),
        [this](const system& sys) { return std::get<1>(sys) == *_stage;});

    /* this part could be multi-threaded */
    for (auto sys : curr_sys) {
        std::get<0>(sys.get())(world);
    }

    _stage++;
}

void Schedule::runStartStages(World& world) {
    _stage = _at_start_schedule.begin();
    do {
        runStage(world);
    }
    while (_stage != _at_start_schedule.end() && !_abort);
}

void Schedule::runStages(World& world) {
    _stage = _schedule.begin();
    do {
        runStage(world);
    }
    while (_stage != _schedule.end() && !_abort);
}

void Schedule::run(World &world) {
    runStartStages(world);
    while (!_stop) {
        runStages(world);
        while(!world._command_queue.empty()) {
            std::function<void(World &)> func = world._command_queue.front();
            world._command_queue.pop();
            func(world);
        }
    }
}

void Schedule::quit() const {
    _stop = true;
}

void Schedule::abort() {
    _abort = true;
    _stop = true;
}
