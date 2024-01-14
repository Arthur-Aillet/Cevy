/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Keyboard
*/

#pragma once

#include "Input.hpp"
#include "Resource.hpp"
#include "raylib.h"

namespace cevy {
namespace engine {
using KeyboardInput = KeyboardKey;

class Keyboard {
  public:
  enum State {
    JUST_PRESSED = 0,
    PRESSED = 1,
    JUST_RELEASED = 2,
  };

  using Info = State;
};

KeyboardInput raylibToCevyKeyboard(int input);
void update_keyboard(cevy::ecs::Resource<cevy::engine::Input<Keyboard>> inputs);
}
}
