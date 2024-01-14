/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Keyboard
*/

#include "Keyboard.hpp"
#include "raylib.hpp"
#include <iostream>
#include <ostream>

using namespace cevy;
using namespace engine;

template<>
void cevy::engine::Input<Keyboard>::update_keys() {
  for (auto const &[key, val] : _just_pressed) {
    if (_pressed.find(key) != _pressed.end()) {
      _pressed.insert({key, val});
    }
  }
  _just_pressed.clear();

  int keyval = GetKeyPressed();
  while (keyval != KeyboardKey::KEY_NULL) {
    _just_pressed.insert({keyval,
                             Keyboard::State::JUST_PRESSED,
                         });
    keyval = GetKeyPressed();
  }

  _just_released.clear();
  keyval = GetKeyPressed();
  for (auto input : _pressed) {
    if (IsKeyUp(input.second)) {
      _pressed.erase(input.second);
      _just_released.insert(input);
    }
  }
};

void cevy::engine::update_keyboard(cevy::ecs::Resource<cevy::engine::Input<Keyboard>> inputs) {
  inputs.get().update_keys();
  std::cout << inputs.get()._pressed.size() << std::endl;
}
