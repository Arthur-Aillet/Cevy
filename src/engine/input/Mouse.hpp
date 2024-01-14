/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Mouse
*/

#pragma once

#include "Input.hpp"
#include "Resource.hpp"
#include "raylib.hpp"

namespace cevy {
namespace engine {

enum MouseInput {
  Mouse_LEFT = 0,
  Mouse_RIGHT = 1,
  Mouse_MIDDLE = 2,
  Mouse_SIDE = 3,
  Mouse_EXTRA = 4,
  Mouse_FORWARD = 5,
  Mouse_BACK = 6,
};

struct MousePosition {
  Vector2 position;
  Vector2 delta;
};

struct MouseWheel {
  double vectical;
  double horisontal;
};

struct Mouse {
  MouseInput button;
  //ButtonState state;
};

template<>
class Input<Mouse> {
  public:
  void update_keys();
  //std::map<int, Mouse> _just_pressed;
  //std::map<int, Mouse> _pressed;
  //std::map<int, Mouse> _just_released;
  MousePosition position;
  MouseWheel wheel;
};


void update_mouse(cevy::ecs::Resource<cevy::engine::Input<Mouse>> inputs);

}
}
