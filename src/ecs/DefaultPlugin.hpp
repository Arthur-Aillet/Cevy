/*
** Agartha-Software, 2023
** C++evy
** File description:
** Default Plugin
*/

#pragma once

#include "Plugin.hpp"
#include "ecs.hpp"

namespace cevy::ecs {

/**
 * @brief Plugin that instanciate all the recommended building blocs for the propre construction of
 * App
 *
 * Here are what is setup by this plugin
 * - Default Stages instanciated
 * - Timer Management
 * - AppExit Event added
 * @warning Without it's instanciation or any replacement unexpected behavior might happend, treat
 * with care
 */
class DefaultPlugin : public cevy::ecs::Plugin {
  public:
  /// @brief Build the Default Plugin
  void build(cevy::ecs::App &);
};
} // namespace cevy::ecs
