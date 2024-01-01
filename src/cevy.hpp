/*
** Agartha-Software, 2023
** Cevy
** File description:
** cevy
*/

#pragma once

#include <functional>
#include <optional>

template <typename T>
using ref = std::reference_wrapper<T>;

template <class T>
using option = std::optional<T>;

/// @brief contains all of the engine bits
namespace cevy {
/// @brief holds the entity components system
namespace ecs {};

/// @brief hold engine mechanics, depends on ecs
namespace engine {}; // namespace engine
} // namespace cevy

// note: here name-spaces are being forwa-rd declared;
// they will be expanded by their relevant files;
