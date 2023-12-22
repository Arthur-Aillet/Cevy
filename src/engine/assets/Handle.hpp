/*
** Agartha-Software, 2023
** Cevy
** File description:
** Handle
*/
#pragma once

#include "cevy.hpp"

namespace cevy::engine {

template <typename Type>
class Handle {
  using value = Type;

  ref<Type> _ref;


  public:
  Handle(Type &ref) : _ref(ref){};

  Type &get() { return _ref; }
};

} // namespace cevy::engine
