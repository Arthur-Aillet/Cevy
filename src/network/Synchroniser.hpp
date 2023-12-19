/*
** Agartha-Software, 2013
** Cevy
** File description:
** Synchroniser
*/

#pragma once

#include <cstdint>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <typeindex>

#include "CevyNetwork.hpp"
#include "../cevy.hpp"
#include "../ecs/ecs.hpp"
#include "../ecs/App.hpp"

class Synchroniser {
public:
    struct SyncId {
        size_t id;
        cevy::ecs::World::Id type;
    };
    template<typename Block, typename... Component>
    class SyncBlock;
    using BlockType = std::type_index;
    using ComponentType = std::type_index;
    enum class Mode {
        Server,
        Client,
    };

    virtual void build(cevy::ecs::World& world) = 0;

    Synchroniser(CevyNetwork& net, Mode mode) : mode(mode), _net(net) {};
    ~Synchroniser() {};

    template<typename Stage, typename Block, typename... Component>
    void add_sync(cevy::ecs::App& app) {
        _blocks[BlockType(typeid(Block))] = ++_blockCount;
        app.add_super_system<Stage>(SyncBlock<Block, Component...>(mode, *this));
        // ([&] {
        //     (_sync_map[typeid(Block)].push_back(typeid(Component)));
        // } (), ...);
    }

protected:
    CevyNetwork& _net;
    std::unordered_map<BlockType, uint8_t> _blocks;
    uint8_t _blockCount;
    // std::unordered_map<BlockType, std::vector<ComponentType>> _sync_map;
    Mode mode;

private:
};

template<typename Block, typename... Component>
class Synchroniser::SyncBlock {
    SyncBlock(Synchroniser::Mode mode, Synchroniser& sync, CevyNetwork& net) : mode(mode), _sync(sync), _net(sync._net) {};
    Synchroniser& _sync;
    CevyNetwork& _net;

    template<typename T>
    uint16_t id(uint16_t sync_id) {
        auto it = _sync._blocks.find(BlockType(typeid(T)));

        if (it != _sync._blocks.end()) {
            uint16_t block = it->second;
            block <<= 10;
            block &= (sync_id & ((1 << 10) - 1));
            return block;
        }
        return 0;
    }

    void system_send(cevy::ecs::Query<SyncId, Component...>& q) {
        for (auto& e : q) {
            auto sync_id = std::get<SyncId>(e);
            if (!sync_id)
                continue;
            std::vector<uint8_t> block;
            ([&] {
                constexpr size_t size =  sizeof(Component);
                std::array<uint8_t, size> vec;
                std::memcpy(&vec.data(), &std::get<Component>(e), size);
                block.insert(block.end(), vec.begin(), vec.end());
            } (), ...);
            _net.sendState(id<Block>(sync_id), block);
        }
    }

    void system_recv(cevy::ecs::Query<SyncId, Component...>& q) {
        for (auto& e : q) {
            auto sync_id = std::get<SyncId>(e);
            std::optional<std::vector<uint8_t>> block_ = _net.recvState(id<Block>(sync_id));
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

    void system_summon(cevy::ecs::Command command) {
        auto x = _net.recvSummon();
        for (auto it: x) {
            auto e = command.add_empty();
            e.add_component<SyncId>(it.syncID);
            for (auto component : x.componets) {
                e.add_component<???>(component);
            }
        }
    }

    void operator() (cevy::ecs::Query<SyncId, Component...> q) {
        if (mode == Mode::Server)
            system_send(q);
        if (mode == Mode::Client)
            system_recv(q);
    };

    Mode mode;
};