/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** input
*/

#include "Input.hpp"
#include "ecs.hpp"
#include <tuple>

void update_all_inputs(cevy::ecs::Query<Input<std::any>> inputs) {
  for (auto input : inputs) {
    std::get<0>(input).update_keys();
  }
}