/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Mouse
*/

#pragma once

#include "Input.hpp"

enum MouseButton {
  MOUSE_BUTTON_LEFT = 0,
  MOUSE_BUTTON_RIGHT = 1,
  MOUSE_BUTTON_MIDDLE = 2,
  MOUSE_BUTTON_SIDE = 3,
  MOUSE_BUTTON_EXTRA = 4,
  MOUSE_BUTTON_FORWARD = 5,
  MOUSE_BUTTON_BACK = 6,
};

struct Mouse {
  MouseButton button;
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

class Input<Mouse> {
  public:
  void update_keys();
  std::map<int, Mouse> just_pressed;
  std::map<int, Mouse> pressed;
  std::map<int, Mouse> just_released;
  MousePosition position;
  MouseWheel wheel;
};
