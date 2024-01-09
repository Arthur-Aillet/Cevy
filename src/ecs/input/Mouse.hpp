/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Mouse
*/

#pragma once

#include "Input.hpp"

enum MouseInput {
  Mouse_LEFT = 0,
  Mouse_RIGHT = 1,
  Mouse_MIDDLE = 2,
  Mouse_SIDE = 3,
  Mouse_EXTRA = 4,
  Mouse_FORWARD = 5,
  Mouse_BACK = 6,
};

struct Mouse {
  MouseInput button;
  ButtonState state;
};

struct MousePosition {
  Vector2 position;
  Vector2 delta;
};

struct MouseWheel {
  double vectical;
  double horisontal;
};

template<>
class cevy::ecs::Input<Mouse> {
  public:
  void update_keys();
  std::map<int, Mouse> just_pressed;
  std::map<int, Mouse> pressed;
  std::map<int, Mouse> just_released;
  MousePosition position;
  MouseWheel wheel;
};
