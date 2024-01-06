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

template <bool, template <class...> class, class, class Else>
struct eval_cond {
  using type = Else;
};

template <template <class...> class Z, class X, class Else>
struct eval_cond<true, Z, X, Else> {
  using type = Z<X>;
};

template <bool test, template <class...> class Z, class X, class Else>
using eval_cond_t = typename eval_cond<test, Z, X, Else>::type;

template <typename... Args>
constexpr bool all(Args... args) {
  return (... && args);
}

template <typename... Args>
constexpr bool any() {
  return (... || Args::value);
};

/// @brief contains all of the engine bits
namespace cevy {
/// @brief holds the entity components system
namespace ecs {};

/// @brief hold engine mechanics, depends on ecs
namespace engine {};
} // namespace cevy

// note: here name-spaces are being forwa-rd declared;
// they will be expanded by their relevant files;
