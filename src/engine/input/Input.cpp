/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** input
*/

#include "Input.hpp"
#include <tuple>

void update_all_inputs(cevy::ecs::Query<cevy::ecs::Input<std::any>> inputs) {
  for (auto input : inputs) {
    std::get<0>(input).update_keys();
  }
}
