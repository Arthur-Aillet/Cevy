/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Mouse
*/

#include "Mouse.hpp"

void cevy::ecs::Input<Mouse>::update_keys() {
  Vector2 wheelMove = GetMouseWheelMoveV();
  wheel.horisontal = wheelMove.x;
  wheel.vectical = wheelMove.y;
  position.delta = GetMouseDelta();
  position.position = GetMousePosition();
  pressed.clear();
  just_pressed.clear();
  just_released.clear();
  for (int i = 0; i <= 6; i++) {
    if (IsMouseButtonPressed(i)) {
      just_pressed.insert({i, (Mouse){(MouseInput)i, ButtonState::JUST_PRESSED}});
    }
    if (IsMouseButtonDown(i)) {
      pressed.insert({i, (Mouse){(MouseInput)i, ButtonState::PRESSED}});
    }
    if (IsMouseButtonReleased(i)) {
      just_released.insert({i, (Mouse){(MouseInput)i, ButtonState::JUST_RELEASED}});
    }
  }
}
