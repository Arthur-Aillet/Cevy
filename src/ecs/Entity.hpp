/*
** Agartha-Software, 2023
** Cevy
** File description:
** Entity
*/

#pragma once

#include "ecs.hpp"

class cevy::ecs::Entity {
  private:
  std::size_t _id;
  explicit Entity(std::size_t new_id);

  public:
  friend class ecs::World;
  friend class ecs::Commands;
  template <typename... T>
  friend class ecs::Query;
  operator std::size_t &();
  operator std::size_t() const;
};
