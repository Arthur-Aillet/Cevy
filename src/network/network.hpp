/*
** Agartha-Software, 2023
** rtype
** File description:
** network
*/

#pragma once

#include "../cevy.hpp"
#include <cstdint>
#include <exception>
#include <tuple>
#include <type_traits>
#include <vector>
#include <bit>

namespace cevy {

template<typename T>
struct serialized_size : public std::integral_constant<size_t, 0> {};

template<typename T>
constexpr std::vector<uint8_t>& serialize(std::vector<uint8_t>&, const T&);

template<template<typename...> typename T, typename... Ts>
inline constexpr std::vector<uint8_t>& serialize(std::vector<uint8_t>& vec, const T<Ts...>& t) {
    (vec.push_back(std::get<Ts>(t)), ...);
    return vec;
}

class NetworkPlugin;
class Synchroniser;
class NetworkActions;
class CevyNetwork;
class NetworkBase;
class NetworkCommands;
class ClientHandler;
class ServerHandler;
using Network = CevyNetwork;
} // namespace cevy

template<typename... T>
struct cevy::serialized_size<std::tuple<T...>> : public std::integral_constant<size_t, sum(serialized_size<T>::value...)> {};

template<>
struct cevy::serialized_size<int> : public std::integral_constant<size_t, 4> {};

template<>
inline std::vector<uint8_t>& cevy::serialize<int>(std::vector<uint8_t>& vec, const int& t) {
    auto p = reinterpret_cast<const uint8_t*>(&t);
    vec.push_back(p[0]);
    vec.push_back(p[1]);
    vec.push_back(p[2]);
    vec.push_back(p[3]);
    return vec;
}

template<>
struct cevy::serialized_size<float> : public std::integral_constant<size_t, 4> {};

template<>
inline std::vector<uint8_t>& cevy::serialize<float>(std::vector<uint8_t>& vec, const float& t) {
    auto p = reinterpret_cast<const uint8_t*>(&t);
    vec.push_back(p[0]);
    vec.push_back(p[1]);
    vec.push_back(p[2]);
    vec.push_back(p[3]);
    return vec;
}
