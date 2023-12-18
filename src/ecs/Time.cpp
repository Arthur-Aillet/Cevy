/*
** Agartha-Software, 2023
** Cevy
** File description:
** time
*/

#include "ecs.hpp"
#include "App.hpp"
#include "Time.hpp"

using cevy::ecs::Time;

Time::Time() : _start(std::chrono::high_resolution_clock::now()) {}
