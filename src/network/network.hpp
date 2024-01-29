/*
** Agartha-Software, 2023
** rtype
** File description:
** network
*/

#pragma once

#include "../cevy.hpp"
#include <bit>
#include <cstdint>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <vector>

namespace cevy {

template <typename T>
struct serialized_size : public std::integral_constant<size_t, 0> {};


template <typename T>
constexpr std::vector<uint8_t> &serialize_impl(std::vector<uint8_t> &vec, const T &t);

/**
 * @brief
 *
 * @tparam T
 * @return constexpr std::vector<uint8_t>&
 * @warning Do NOT specialize this template
 */
template <typename T>
 std::vector<uint8_t> &serialize(std::vector<uint8_t> &vec, const T &t) {
  size_t prelen = vec.size();
  serialize_impl(vec, t);
  if (vec.size() - prelen != serialized_size<T>::value) {
    std::stringstream ss;
    ss << "Seserializing type '" << typeid(T).name() <<  "' yielded only " << vec.size() << " bytes, expecting " << serialized_size<T>::value;
    throw std::out_of_range(ss.str());
  }
  return vec;
}

template <typename T>
T deserialize_impl(std::vector<uint8_t> &);


/**
 * @brief
 *
 * @tparam T
 * @param vec
 * @return T
 * @warning Do NOT specialize this template
 */
template <typename T>
T deserialize(std::vector<uint8_t> &vec) {
  if (vec.size() < serialized_size<T>::value) {
    std::stringstream ss;
    ss << "Deserializing type '" << typeid(T).name() << "' (" << typeid(T).hash_code() << ") when only " << vec.size() << " bytes available, expecting " << serialized_size<T>::value;
    throw std::out_of_range(ss.str());
  }

  T t = deserialize_impl<T>(vec);
  return t;
};


template <template <typename...> typename T, typename... Ts>
inline constexpr std::vector<uint8_t> &serialize(std::vector<uint8_t> &vec, const T<Ts...> &t) {
  (vec.push_back(std::get<Ts>(t)), ...);
  return vec;
}

template <typename S, typename A, typename N>
class NetworkPlugin;
class Synchroniser;
class NetworkActions;
class CevyNetwork;
class NetworkBase;
class NetworkCommands;
class ClientHandler;
class ServerHandler;
using Network = CevyNetwork;
template <typename S, typename A, typename N>
class NetworkPlugin;
} // namespace cevy

template <typename... T>
struct cevy::serialized_size<std::tuple<T...>>
    : public std::integral_constant<size_t, sum(serialized_size<T>::value...)> {};


namespace cevy {
template <typename... T>
inline std::vector<uint8_t> &serialize_impl(std::vector<uint8_t> &vec, const std::tuple<T...> &t) {
  (serialize(vec, std::get<T&>(t)), ...);
  return vec;
}

template <typename... T>
inline std::tuple<T...> deserialize_impl(std::vector<uint8_t> &vec) {
  std::tuple<T...> t;
  ((std::get<T&>(t) = deserialize<T>(vec)), ...);
  return t;
}
}


template <>
struct cevy::serialized_size<uint8_t> : public std::integral_constant<size_t, sizeof(uint8_t)> {};

template <>
inline std::vector<uint8_t> &cevy::serialize_impl<uint8_t>(std::vector<uint8_t> &vec, const uint8_t &t) {
  vec.push_back(t);
  return vec;
}

template <>
inline uint8_t cevy::deserialize_impl<uint8_t>(std::vector<uint8_t> &vec) {
  uint8_t t = vec.front();
  vec.erase(vec.begin());
  return t;
}

template <>
struct cevy::serialized_size<uint16_t> : public std::integral_constant<size_t, sizeof(uint16_t)> {};

template <>
inline std::vector<uint8_t> &cevy::serialize_impl<uint16_t>(std::vector<uint8_t> &vec,
                                                       const uint16_t &t) {
  auto p = reinterpret_cast<const uint8_t *>(&t);
  vec.push_back(p[0]);
  vec.push_back(p[1]);
  return vec;
}

template <>
inline uint16_t cevy::deserialize_impl<uint16_t>(std::vector<uint8_t> &vec) {
  uint16_t t;
  auto p = reinterpret_cast<uint8_t *>(&t);
  for (size_t i = 0; i < serialized_size<uint16_t>::value; ++i) {
    p[i] = vec.front();
    vec.erase(vec.begin());
  }
  return t;
}

template <>
struct cevy::serialized_size<int> : public std::integral_constant<size_t, sizeof(int)> {};

template <>
inline std::vector<uint8_t> &cevy::serialize_impl<int>(std::vector<uint8_t> &vec, const int &t) {
  auto p = reinterpret_cast<const uint8_t *>(&t);
  vec.push_back(p[0]);
  vec.push_back(p[1]);
  vec.push_back(p[2]);
  vec.push_back(p[3]);
  return vec;
}

template <>
inline int cevy::deserialize_impl<int>(std::vector<uint8_t> &vec) {
  int t;
  auto p = reinterpret_cast<uint8_t *>(&t);
  for (size_t i = 0; i < serialized_size<uint>::value; ++i) {
    p[i] = vec.front();
    vec.erase(vec.begin());
  }
  return t;
}

template <>
struct cevy::serialized_size<size_t> : public std::integral_constant<size_t, sizeof(size_t)> {};

template <>
inline std::vector<uint8_t> &cevy::serialize_impl<size_t>(std::vector<uint8_t> &vec, const size_t &t) {
  auto p = reinterpret_cast<const uint8_t *>(&t);
  for (size_t i = 0; i < serialized_size<size_t>::value; ++i) {
    vec.push_back(p[i]);
  }
  return vec;
}

template <>
inline size_t cevy::deserialize_impl<size_t>(std::vector<uint8_t> &vec) {
  size_t t;
  auto p = reinterpret_cast<uint8_t *>(&t);
  for (size_t i = 0; i < serialized_size<size_t>::value; ++i) {
    p[i] = vec.front();
    vec.erase(vec.begin());
  }
  return t;
}

template <>
struct cevy::serialized_size<float> : public std::integral_constant<size_t, sizeof(float)> {};

template <>
inline std::vector<uint8_t> &cevy::serialize_impl<float>(std::vector<uint8_t> &vec, const float &t) {
  auto p = reinterpret_cast<const uint8_t *>(&t);
  vec.push_back(p[0]);
  vec.push_back(p[1]);
  vec.push_back(p[2]);
  vec.push_back(p[3]);
  return vec;
}

template <>
inline float cevy::deserialize_impl<float>(std::vector<uint8_t> &vec) {
  float t;
  auto p = reinterpret_cast<uint8_t *>(&t);
  for (size_t i = 0; i < serialized_size<float>::value; ++i) {
    p[i] = vec.front();
    vec.erase(vec.begin());
  }
  return t;
}
