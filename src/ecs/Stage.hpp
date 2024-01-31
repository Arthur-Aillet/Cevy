/*
** Agartha-Software, 2023
** C++evy
** File description:
** Stage
*/

#pragma once

#include "cevy.hpp"
#include <optional>
#include <type_traits>

namespace cevy::ecs {
/**
 * @brief Hold the stage utilities and base stages
 */
namespace core_stage {
/**
 * @brief Base class for Stage used as Id for type checking
 */
struct IsStage {};

struct Setup {};
struct Loop {};
struct Unwind {};


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
          typename Type = Loop>
class Stage : public IsStage {
  public:
  // using is_repeat = Repeat;
  using type = Type;
  using previous = Before;
  using next = After;

  template <typename T>
  using before = Stage<T, std::nullopt_t, typename T::type>;

  template <typename T>
  using after = Stage<std::nullopt_t, T, typename T::type>;
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
using at_start = Stage<std::nullopt_t, std::nullopt_t, Setup>;
using at_end = Stage<std::nullopt_t, std::nullopt_t, Unwind>;

class Startup : public at_start {};
class PreStartup : public before<Startup> {};
class PostStartup : public after<Startup> {};
class ShutDown : public at_end {};
class PreShutDown : public before<ShutDown> {};

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
} // namespace core_stage
} // namespace cevy::ecs
