/*
** Agartha-Software, 2023
** Cevy
** File description:
** App
*/

#include "App.hpp"

void cevy::ecs::App::run() { _scheduler.run(*this); }

void cevy::ecs::App::quit() { _scheduler.quit(); }
void cevy::ecs::App::abort() { _scheduler.abort(); }
