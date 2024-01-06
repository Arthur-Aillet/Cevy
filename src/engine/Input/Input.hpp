/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** input
*/

#pragma once

#include "raylib.h"
#include <map>

void update_all_inputs(cevy::ecs::Query<Input<Int>> inputs);

enum ButtonState {
    JUST_PRESSED = 0,
    PRESSED = 1,
    JUST_RELEASED = 2,
};

template<typename T>
class Input {
    public:
        void update_keys();
        void clear_keys() {
            just_pressed.clear();
            pressed.clear();
            just_released.clear();
        };
        std::map<int, T> just_pressed;
        std::map<int, T> pressed;
        std::map<int, T> just_released;
};
