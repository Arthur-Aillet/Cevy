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
#include <type_traits>
#include <vector>
#include <bit>

namespace cevy {

template<typename T>
struct serialized_size : public std::integral_constant<size_t, 0> {};

template<typename T>
constexpr std::vector<uint8_t>& serialize(std::vector<uint8_t>&, const T&);

class NetworkPlugin;
class Synchroniser;
class NetworkActions;
class CevyNetwork;
class NetworkBase;
using Network = CevyNetwork;
class NetworkActions;
class NetworkCommands;
} // namespace cevy


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
