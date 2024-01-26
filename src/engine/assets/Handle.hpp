/*
** Agartha-Software, 2023
** C++evy
** File description:
** Handle
*/
#pragma once

#include "assets.hpp"
#include "AssetManager.hpp"
#include "cevy.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

namespace cevy::engine {

template <typename Type>
class Handle {
  friend class cevy::engine::Asset<Type>;
  using value_type = Type;

  // typename std::unordered_map<std::string, Type>::const_iterator _ref;
  ref<Type> _ref;
  std::string path;

  protected:
  Handle(typename std::unordered_map<std::string, Type>::iterator it) : _ref(it->second), path(it->first) {};
  public:

  Handle(const Handle& other) : _ref(other._ref), path(other.path) {};
  Handle(Handle&& other) : _ref(other._ref), path(other.path) {};

  Handle& operator=(const Handle other) {
    _ref = other._ref;
    return *this;
  }

  const Type &get() { return _ref; };
  const Type &operator * (){ return _ref.get(); };
  const Type *operator -> (){ return &_ref.get(); };
  // operator bool () {
  //   // return _ref.base() != nullptr;
  // }
  protected:
  // Handle() : _ref(nullptr){};
};

} // namespace cevy::engine

// struct t {
//   // typename std::vector<int>::const_iterator _ref;
//   ref<int> _ref;

//   operator bool () {
//     return _ref.get()
//   }

// }
