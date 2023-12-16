/*
** Agartha-Software, 2013
** Cevy
** File description:
** Synchroniser
*/

#pragma once

#include "network.hpp"
#include "../cevy.hpp"
#include "../ecs/ecs.hpp"
#include <cstdint>
#include <cstring>
#include <vector>

class Synchroniser {
public:
    using BlockType = std::type_index;
    using ComponentType = std::type_index;

    Synchroniser();
    ~Synchroniser();

    template<typename Block, typename... Component>
    void add_sync() {
        ([&] {
            (_sync_map[typeid(Block)].push_back(typeid(Component)));
        } (), ...);
    }

    template<typename Block, typename... Component>
    void system_send(cevy::ecs::Query<Component...> q) {
        std::vector<uint8_t> block;
        ([&] {
            constexpr size_t size =  sizeof(Component);
            std::array<uint8_t, size> vec;
            std::memcpy(&vec.data(), &std::get<Component>(q), size);
            block.insert(vec.begin(), vec.end());
        } (), ...);

        //network.send(id<Block>, block);
    }

    template<typename Block, typename... Component>
    void system_recv(cevy::ecs::Query<Component...> q) {
        std::vector<uint8_t> block; // = network.recv(id<Block>);

        ([&] {
            constexpr size_t size =  sizeof(Component);
            std::array<uint8_t, size> vec(block);
            std::memcpy(&std::get<Component>(q), &vec.data(), size);
            block.erase(block.begin(), block.begin() + size);
        } (), ...);
    }

protected:
    Network _net;
    std::unordered_map<BlockType, std::vector<ComponentType>> _sync_map;

private:
};
