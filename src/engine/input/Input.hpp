/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** input
*/

#pragma once

#pragma GCC system_header

#include "ecs.hpp"
#include "Query.hpp"
#include "raylib.hpp"
#include <any>
#include <map>

void update_all_inputs(cevy::ecs::Query<cevy::ecs::Input<std::any>> inputs);

enum ButtonState {
  JUST_PRESSED = 0,
  PRESSED = 1,
  JUST_RELEASED = 2,
};

template <typename InputType>
class cevy::ecs::Input {
  public:
  void update_keys() {};
  void clear_keys() {
    just_pressed.clear();
    pressed.clear();
    just_released.clear();
  };
  std::map<int, InputType> just_pressed;
  std::map<int, InputType> pressed;
  std::map<int, InputType> just_released;
};
