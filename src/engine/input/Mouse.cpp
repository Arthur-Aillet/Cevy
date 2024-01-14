/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Mouse
*/

#include "Mouse.hpp"
void cevy::engine::Input<cevy::engine::Mouse>::update_keys() {
/*
  Vector2 wheelMove = GetMouseWheelMoveV();
  wheel.horisontal = wheelMove.x;
  wheel.vectical = wheelMove.y;
  position.delta = GetMouseDelta();
  position.position = GetMousePosition();
  _pressed.clear();
  _just_pressed.clear();
  _just_released.clear();
  for (int i = 0; i <= 6; i++) {
    if (IsMouseButtonPressed(i)) {
      _just_pressed.insert({i, (Mouse){(MouseInput)i, ButtonState::JUST_PRESSED}});
    }
    if (IsMouseButtonDown(i)) {
      _pressed.insert({i, (Mouse){(MouseInput)i, ButtonState::PRESSED}});
    }
    if (IsMouseButtonReleased(i)) {
      _just_released.insert({i, (Mouse){(MouseInput)i, ButtonState::JUST_RELEASED}});
    }
  }
*/
}

void cevy::engine::update_mouse(cevy::ecs::Resource<cevy::engine::Input<cevy::engine::Mouse>> inputs) {
  inputs.get().update_keys();
}
