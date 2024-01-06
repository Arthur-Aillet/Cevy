/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Keyboard
*/

#include "Keyboard.hpp"


void Input<KeyBoard>::update_keys() {
    int keyval = GetKeyPressed();
    for (auto const& [key, val] : just_pressed) {
        if (pressed.find(key) != pressed.end()) {
            pressed.insert({key, val});
        }
    }
    just_pressed.clear();
    while (keyval != KeyboardKey::KEY_NULL) {
        just_pressed.insert({
            raylibToCevyKeyboard(keyval),
            (KeyBoard){
                keyval, // to change to scancode
                raylibToCevyKeyboard(keyval),
                ButtonState::JUST_PRESSED,
            }
        });
        keyval = GetKeyPressed();
    }
    just_released.clear();
    keyval = GetKeyPressed();
    for (auto input : pressed) {
        if (IsKeyUp(input.second.scancode)) {
            pressed.erase(input.second.keycode);
            just_released.insert(input);
        }
    }
};