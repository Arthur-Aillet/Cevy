/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** input
*/

#pragma once

#include "Input.hpp"
#include <unordered_map>

namespace cevy {
namespace engine {

template <typename InputType>
class Input {
  public:
  void update_keys() {};
  void clear_keys() {
    _just_pressed.clear();
    _pressed.clear();
    _just_released.clear();
  };

  bool pressed(int enum_val) {
    return _pressed.count(enum_val);
  }

  bool just_pressed(int enum_val) {
    return _just_pressed.count(enum_val);
  }

  bool just_released(int enum_val) {
    return _just_released.count(enum_val);
  }

  std::unordered_map<int, typename InputType::Info> _just_pressed;
  std::unordered_map<int, typename InputType::Info> _pressed;
  std::unordered_map<int, typename InputType::Info> _just_released;
};
}
}
