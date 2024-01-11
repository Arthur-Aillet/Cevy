/*
** Agartha-Software, 2023
** Cevy
** File description:
** Stage
*/

#pragma once

#include <optional>
#include <type_traits>

namespace cevy::ecs {
/**
 * @brief Base class for Stage used as Id for type checking
 */
struct IsStage {};

/**
 * @brief Base class for any Stage, is made to be user generated
 *
 * If the Stage-Ordering don't find a place to place the stage it will be pushed at the end of the
 * queue
 *
 * Some shorthands exists to create a stage after an other:
 * - at_start
 * - after
 * - before
 *
 * '''
 * class Startup : public at_start {};
 *
 * class PreStartup : public before<Startup> {};
 * class PostStartup : public after<Startup> {};
 *
 * @tparam Before Previous Stage
 *
 * Other Stage or std::nullopt_t (default)
 * @tparam After Next Stage
 *
 * Other Stage or std::nullopt_t (default)
 * @tparam Repeat Is it run once at app.run() or each cycle (Update vs Startup)
 *
 * std::false_type or std::true_type (default)
 */
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

/**
 * @brief Shorthand to create a stage before an other
 *
 * '''
 * class Startup : public at_start {};
 * class PreStartup : public before<Startup> {};
 * '''
 *
 * @tparam LastStage
 */
template <typename LastStage>
using before = Stage<>::before<LastStage>;

/**
 * @brief Shorthand to create a stage after an other
 *
 * '''
 * class Startup : public at_start {};
 * class PostStartup : public after<Startup> {};
 * '''
 *
 * @tparam NextStage
 */
template <typename NextStage>
using after = Stage<>::after<NextStage>;

/**
 * @brief Default stage constructor
 */
using at_start = Stage<std::nullopt_t, std::nullopt_t, std::false_type>;

class Startup : public at_start {};
class PreStartup : public before<Startup> {};
class PostStartup : public after<Startup> {};

class First : public Stage<> {};

class Update : public after<First> {};
class PreUpdate : public before<Update> {};
class PostUpdate : public after<Update> {};

/**
 * @brief Run between each state transitions
 * @bug NOT IMPLEMENTED YET
 */
class StateTransition : public before<Update> {};

/**
 * @brief Run a fixed number of time each time unit
 * @bug NOT IMPLEMENTED YET
 */
class RunFixedUpdateLoop : public before<Update> {};

class Last : public after<PostUpdate> {};
} // namespace cevy::ecs
