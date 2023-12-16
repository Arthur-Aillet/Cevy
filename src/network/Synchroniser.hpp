/*
** Agartha-Software, 2013
** Cevy
** File description:
** Synchroniser
*/

#pragma once

#include <cstdint>
#include <cstring>
#include <vector>
#include <typeindex>

#include "network.hpp"
#include "../cevy.hpp"
#include "../ecs/ecs.hpp"
#include "../ecs/App.hpp"

template<typename Block, typename... Component>
class SyncBlock;


class Synchroniser {
public:
    using BlockType = std::type_index;
    using ComponentType = std::type_index;
    enum class Mode {
        Server,
        Client,
    };

    virtual void build(cevy::ecs::World& world) = 0;

    Synchroniser(Network& net, Mode mode) : mode(mode), _net(net) {};
    ~Synchroniser() {};

    template<typename Stage, typename Block, typename... Component>
    void add_sync(cevy::ecs::App& app) {
        app.add_super_system<Stage>(SyncBlock<Block, Component...>(mode));
        // ([&] {
        //     (_sync_map[typeid(Block)].push_back(typeid(Component)));
        // } (), ...);
    }

protected:
    Network& _net;
    // std::unordered_map<BlockType, std::vector<ComponentType>> _sync_map;
    Mode mode;

private:
};

template<typename Block, typename... Component>
class SyncBlock {
    SyncBlock(Synchroniser::Mode mode) : mode(mode) {};

    void system_send(cevy::ecs::Query<Component...>& q) {
        for (auto& e : q) {
            std::vector<uint8_t> block;
            ([&] {
                constexpr size_t size =  sizeof(Component);
                std::array<uint8_t, size> vec;
                std::memcpy(&vec.data(), &std::get<Component>(e), size);
                block.insert(vec.begin(), vec.end());
            } (), ...);
            //network.send(id<Block>(e.id()), block);
        }
    }

    void system_recv(cevy::ecs::Query<Component...>& q) {
        for (auto& e : q) {
            std::optional<std::vector<uint8_t>> block_; // = network.recv(id<Block>(e.id()));
            if (!block_)
                return;
            std::vector<uint8_t>& block = block_.value();

            ([&] {
                constexpr size_t size =  sizeof(Component);
                std::array<uint8_t, size> vec(block);
                std::memcpy(&std::get<Component>(e), &vec.data(), size);
                block.erase(block.begin(), block.begin() + size);
            } (), ...);
        }
    }

    void operator() (cevy::ecs::Query<Component...> q) {
        if (mode == Synchroniser::Mode::Server)
            system_send(q);
        if (mode == Synchroniser::Mode::Client)
            system_recv(q);
    };

    Synchroniser::Mode mode;
};
