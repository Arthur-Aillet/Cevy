/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** cevy
*/

#pragma once

#include <functional>

template<typename T>
using ref = std::reference_wrapper<T>;

/// @brief contains all of the engine bits
namespace cevy {
    /// @brief holds the entity components system
    namespace ecs {};

    /// @brief hold engine mechanics, depends on ecs
    namespace engine {}; // game-engine ?
}

// note: here name-spaces are being forwa-rd declared;
// they will be expanded by their relevant files;
