/*
** Agartha-Software, 2013
** Cevy
** File description:
** Synchroniser
*/

#pragma once

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "App.hpp"
#include "CevyNetwork.hpp"
#include "Indirect.hpp"
#include "Plugin.hpp"
#include "Query.hpp"
#include "Schedule.hpp"
#include "cevy.hpp"
#include "commands/Commands.hpp"
#include "commands/EntityCommands.hpp"
#include "network.hpp"

class cevy::Synchroniser : virtual public cevy::ecs::Plugin {
  public:
  class SummonStage : public ecs::Schedule::before<ecs::Schedule::PreUpdate> {};
  class SyncStage : public ecs::Schedule::after<SummonStage> {};
  struct SyncId {
    size_t id = unset;
    size_t type = 0;
    static const size_t unset = -1;
  };
  template <typename Block, typename... Component>
  class SyncBlock;
  using BlockType = std::type_index;
  using ComponentType = std::type_index;
  enum class Mode {
    Server,
    Client,
  };

  virtual void build_custom(cevy::ecs::App &app) = 0;

  Synchroniser(CevyNetwork& net) : _net(net) {};

  Synchroniser(Synchroniser &&rhs) : Plugin(rhs), _net(rhs._net) {}

  // Synchroniser(Mode mode, const std::string &host = std::string(""))
  //     : mode(mode), _net(host, 4995){};
  ~Synchroniser(){};

  template <typename Block, typename... Component>
  void add_sync(cevy::ecs::App &app) {
    _blocks[BlockType(typeid(Block))] = ++_blockCount;
    using F = SyncBlock<Block, Component...>;
    app.add_class_system<F, SyncStage, cevy::ecs::Query<SyncId, Component...>>(
        F(mode, *this, _net));
    // ([&] {
    //     (_sync_map[typeid(Block)].push_back(typeid(Component)));
    // } (), ...);
  }

  template <typename... T>
  void add_spawnable(uint8_t i, T... args) {
    _spawnCommands[i] = [args...](ecs::EntityCommands e) { e.insert(T(args)...); };
  }

  // void system_summon(cevy::ecs::Commands command, cevy::ecs::Query<SyncId> q) {
  //   auto x = _net.recvSummon();
  //   for (auto it : x) {
  //     auto e = command.spawn_empty();
  //     _spawnCommands[it.second](e);
  //     e.insert(SyncId{it.first, it.second});
  //     _occupancy[it.first] = true;
  //   }
  //   for (auto e : q) {
  //     if (std::get<SyncId &>(e).id == SyncId::unset) {
  //       std::get<SyncId &>(e).id = first_free();
  //       _net.sendSummon(std::get<SyncId &>(e).id, std::get<SyncId &>(e).type);
  //     }
  //   }
  // }

  protected:
  uint16_t first_free() {
    int i = -1;
    for (auto it : _occupancy) {
      ++i;
      if (!it)
        return i;
    }
    throw std::out_of_range("Trying to create more than 1023 synced entities");
  }

  CevyNetwork& _net;
  std::unordered_map<BlockType, uint8_t> _blocks;
  uint8_t _blockCount;
  std::unordered_map<uint8_t, std::function<void(ecs::EntityCommands)>> _spawnCommands;
  std::array<bool, 1024> _occupancy;
  // std::unordered_map<BlockType, std::vector<ComponentType>> _sync_map;
  Mode mode;

  protected:
  void build(cevy::ecs::App &app) override {
    // struct SyncSubSystem {
    //   Synchroniser &_sync;
    //   void operator()(cevy::ecs::Commands cmd, cevy::ecs::Query<SyncId> q) const {
    //     // _sync.system_summon(cmd, q);
    //   };
    // };
    // SyncSubSystem sys{*this};
    // app.add_class_system<SyncSubSystem, SummonStage>(std::move(sys));
    // app.add_class_system<SyncSubSystem, SummonStage, cevy::ecs::Commands, cevy::ecs::Query<SyncId>>(
        // std::move(sys));
    build_custom(app);
  }
};

template <typename Block, typename... Component>
class cevy::Synchroniser::SyncBlock {
  public:
  SyncBlock(Synchroniser::Mode mode, Synchroniser &sync, CevyNetwork &net)
      : mode(mode), _sync(sync), _net(sync._net){};

  private:
  Synchroniser &_sync;
  CevyNetwork &_net;

  template <typename T>
  uint16_t id(uint16_t sync_id) const {
    auto it = _sync._blocks.find(BlockType(typeid(T)));

    if (it != _sync._blocks.end()) {
      uint16_t block = it->second;
      block <<= 10;
      block &= (sync_id & ((1 << 10) - 1));
      return block;
    }
    return 0;
  }

  void system_send(cevy::ecs::Query<SyncId, Component...> &q) const {
    for (auto e : q) {
      auto sync_id = std::get<SyncId &>(e);
      if (sync_id.id == SyncId::unset)
        continue;
      std::vector<uint8_t> block;
      (
          [&] {
            constexpr size_t size = sizeof(Component);
            std::array<uint8_t, size> vec;
            std::memcpy(vec.data(), &std::get<Component &>(e), size);
            block.insert(block.end(), vec.begin(), vec.end());
          }(),
          ...);
      _net.sendState(id<Block>(sync_id.id), block);
    }
  }

  void system_recv(cevy::ecs::Query<SyncId, Component...> &q) const {
    for (auto e : q) {
      auto sync_id = std::get<SyncId &>(e);
      std::optional<std::vector<uint8_t>> block_ = _net.recvState(id<Block>(sync_id.id));
      if (!block_)
        return;
      std::vector<uint8_t> &block = block_.value();

      (
          [&] {
            constexpr size_t size = sizeof(Component);
            std::memcpy(&std::get<Component &>(e), block.data(), size);
            block.erase(block.begin(), block.begin() + size);
          }(),
          ...);
    }
  }
  Mode mode;

  public:
  void operator()(cevy::ecs::Query<SyncId, Component...> q) const {
    if (mode == Mode::Server)
      system_send(q);
    if (mode == Mode::Client)
      system_recv(q);
  };
};
