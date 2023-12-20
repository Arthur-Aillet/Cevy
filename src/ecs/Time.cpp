/*
** Agartha-Software, 2023
** Cevy
** File description:
** time
*/

#include "Time.hpp"

using cevy::ecs::Time;

Time::Time() : _start(std::chrono::high_resolution_clock::now()) {}
