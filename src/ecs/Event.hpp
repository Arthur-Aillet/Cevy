/*
** Agartha-Software, 2023
** Cevy
** File description:
** ecs event
*/

#pragma once

#include "App.hpp"

namespace cevy::ecs {
class EventPlugin : public cevy::ecs::Plugin {
  void build(cevy::ecs::App &);
};
} // namespace cevy::ecs
