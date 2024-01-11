/*
** Agartha-Software, 2023
** Cevy
** File description:
** Scheduler
*/

#pragma once

#include <optional>
#include <type_traits>

namespace cevy::ecs {
struct IsStage {};

template <typename Before = std::nullopt_t, typename After = std::nullopt_t,
          typename Repeat = std::true_type>
class Stage : public IsStage {
  public:
  using is_repeat = Repeat;
  using previous = Before;
  using next = After;

  template <typename T>
  using before = Stage<T, std::nullopt_t, typename T::is_repeat>;

  template <typename T>
  using after = Stage<std::nullopt_t, T, typename T::is_repeat>;
};

template <typename T>
using before = Stage<>::before<T>;
template <typename T>
using after = Stage<>::after<T>;

using at_start = Stage<std::nullopt_t, std::nullopt_t, std::false_type>;

class Startup : public at_start {};
class PreStartup : public before<Startup> {};
class PostStartup : public after<Startup> {};

class First : public Stage<> {};

class Update : public after<First> {};
class PreUpdate : public before<Update> {};
class PostUpdate : public after<Update> {};

class StateTransition : public before<Update> {};
class RunFixedUpdateLoop : public before<Update> {};

class Last : public after<PostUpdate> {};
} // namespace cevy::ecs
