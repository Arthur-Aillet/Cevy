/*
** Agartha-Software, 2023
** C++evy
** File description:
** App
*/

#include "App.hpp"

void cevy::ecs::App::run() { _scheduler.run(*this); }
