/*
** Agartha-Software, 2023
** Cevy
** File description:
** App
*/

#include "App.hpp"

void cevy::ecs::App::run() { _schedule.run(*this); }

void cevy::ecs::App::quit() { _schedule.quit(); }
void cevy::ecs::App::abort() { _schedule.abort(); }
