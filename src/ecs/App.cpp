/*
** Agartha-Software, 2023
** Cevy
** File description:
** App
*/

#include "App.hpp"

void cevy::ecs::App::run() { _scheduler.run(*this); }
