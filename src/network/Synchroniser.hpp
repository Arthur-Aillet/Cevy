/*
** Agartha-Software, 2013
** Cevy
** File description:
** Synchroniser
*/

#pragma once

#include <any>
#include <cstdint>
#include <cstring>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <sys/types.h>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "App.hpp"
#include "CevyNetwork.hpp"
#include "Entity.hpp"
#include "Indirect.hpp"
#include "Plugin.hpp"
#include "Query.hpp"
#include "Scheduler.hpp"
#include "cevy.hpp"
#include "commands/Commands.hpp"
#include "commands/EntityCommands.hpp"
#include "ecs.hpp"
#include "network.hpp"

class cevy::Synchroniser : virtual public cevy::ecs::Plugin {
  public:
  class exception : public std::exception {
    public:
    exception(const std::string& msg) : _msg(msg) {};
    const char *what() const noexcept override {
      return _msg.c_str();
    }
    protected:
    std::string _msg;
  };

  class SummonStage : public ecs::core_stage::before<ecs::core_stage::PreUpdate> {};
  class SyncStage : public ecs::core_stage::after<SummonStage> {};
  struct SyncId {
    size_t id = unset;
    size_t type = 0;
    inline static const size_t unset = -1;
  };
  template <typename Block, typename... Component>
  class SyncBlock;
  using BlockType = std::type_index;
  using ComponentType = std::type_index;
  using Mode = CevyNetwork::NetworkMode;
  // enum class Mode {
  //   Server,
  //   Client,
  // };

  template <size_t N, typename... T>
  struct Spawnable {
    static const uint16_t value = N;
  };

  virtual void build_custom(cevy::ecs::App &app) = 0;

  Synchroniser(CevyNetwork &net) : _net(net), mode(_net.mode()) {
    _occupancy.fill(false);
  };

  Synchroniser(Synchroniser &&rhs) : Plugin(rhs), _net(rhs._net) {
    _occupancy.fill(false);
  }

  // Synchroniser(Mode mode, const std::string &host = std::string(""))
  //     : mode(mode), _net(host, 4995){};
  ~Synchroniser(){};

  template <typename Block, typename... Component>
  void add_sync(cevy::ecs::App &app) {
    _blocks[BlockType(typeid(Block))] = ++_blockCount;
    using F = SyncBlock<Block, Component...>;

    _block.emplace_back(std::make_any<F>(mode, *this, _net));
    F& block = std::any_cast<F&>(_block.back());
    std::cout << "(INFO)SyncBlock::add_sync" << std::endl;
    app.add_class_system<F, SyncStage, cevy::ecs::Query<SyncId, Component...>>(block);
    // ([&] {
    //     (_sync_map[typeid(Block)].push_back(typeid(Component)));
    // } (), ...);
  }

  template <typename S, typename... T>
  void add_spawnable(T... args) {
    std::cout << "(INFO)add_spawnable: registering at " << S::value << std::endl;
    _spawnCommands[S::value] = [args...](ecs::EntityCommands e) { e.insert(args...); };
  }

    template <typename S>
  void add_spawnable_command(std::function<void(ecs::EntityCommands)> func) {
    std::cout << "(INFO)add_spawnable: registering at " << S::value << std::endl;
    _spawnCommands[S::value] = func;
  }

  template <typename T>
  void summon(cevy::ecs::Commands &command) {
    auto e = command.spawn_empty();
    if (_spawnCommands.find(T::value) != _spawnCommands.end())
      _spawnCommands.at(T::value)(e);
    else {
      std::cerr << "(ERROR)summon: unmapped spawn command:" << T::value << std::endl;
    }
    auto id = first_free();
    _occupancy[id] = true;
    e.insert(SyncId({id, T::value}));
    if (mode == Mode::Client)
      return;
    _net.sendSummon(id, T::value);
  }

  template <typename T, typename U>
  void summon(cevy::ecs::Commands &command, CevyNetwork::ConnectionDescriptor cd) {
    auto e = command.spawn_empty();
    if (_spawnCommands.find(T::value) != _spawnCommands.end())
      _spawnCommands.at(T::value)(e);
    else {
      std::cerr << "(ERROR)summon: unmapped spawn command:" << T::value << std::endl;
    }
    auto id = first_free();
    _occupancy[id] = true;
    e.insert(SyncId({id, U::value}));
    if (mode != Mode::Server)
      return;
    auto* handler = dynamic_cast<ServerHandler*>(&_net);
    if (!handler) {
      std::cerr << "(ERROR)summon<T, U>: cast went wrong" << std::endl;
      return;
    }
    handler->sendSummon(cd, id, T::value, U::value);
  }

  void dismiss(cevy::ecs::Commands &command, SyncId syncId) {
    if (mode == Mode::Client)
      return;
    auto target = syncId.id;
    std::function<void(ecs::Query<SyncId, ecs::Entity>)> deletor =
        [target, command](ecs::Query<SyncId, ecs::Entity> q) mutable {
          for (auto [id, e] : q) {
            if (id.id == target) {
              command.entity(e).despawn();
            }
          }
        };
    command.system(deletor);
    _occupancy[target] = false;
    _net.sendDismiss(target);
  }

  protected:
  void system_summon(cevy::ecs::Commands command) {
    while (true) {
      auto x = _net.recvSummon();
      if (!x)
        break;
      auto pair = x.value();
      auto e = command.spawn_empty();
      if (_spawnCommands.find(pair.second) != _spawnCommands.end()) {
        std::cerr << "(INFO)system_summon: spawning:" << int(pair.second) << std::endl;
        _spawnCommands.at(pair.second)(e);
      } else {
        std::cerr << "(ERROR)system_summon: unmapped spawn command:" << int(pair.second) << std::endl;
      }
      e.insert(SyncId{pair.first, pair.second});
      _occupancy[pair.first] = true;
    };
    while (true) {
      auto x = _net.recvDismiss();
      if (!x)
        break;
      auto target = x.value();
      std::function<void(ecs::Query<SyncId, ecs::Entity>)> deletor =
          [target, command](ecs::Query<SyncId, ecs::Entity> q) mutable {
            for (auto [id, e] : q) {
              if (id.id == target) {
                command.entity(e).despawn();
              }
            }
          };
      command.system(deletor);
      _occupancy[target] = false;
    };
  }
  uint16_t first_free() {
    int i = -1;
    for (auto it : _occupancy) {
      ++i;
      if (!it)
        return i;
    }
    throw std::out_of_range("Trying to create more than 1023 synced entities");
  }

  CevyNetwork &_net;
  std::unordered_map<BlockType, uint8_t> _blocks;
  uint8_t _blockCount;
  std::unordered_map<uint8_t, std::function<void(ecs::EntityCommands)>> _spawnCommands;
  std::array<bool, 1024> _occupancy;
  std::vector<std::any> _block;
  // std::unordered_map<BlockType, std::vector<ComponentType>> _sync_map;
  Mode mode;

  protected:
  void build(cevy::ecs::App &app) override {
    std::cout << "(INFO)Synchroniser::build" << std::endl;

    app.add_stage<SummonStage>();
    app.add_stage<SyncStage>();
    app.init_component<SyncId>();
    std::function<void(ecs::Commands)> func = [this](cevy::ecs::Commands cmd) {
      this->system_summon(cmd);
    };
    app.add_class_system<std::function<void(ecs::Commands)>, SummonStage, cevy::ecs::Commands>(
        func);
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
    std::stringstream ss;
    (ss << ... << typeid(Component).name());
    // std::cout << "(INFO)ShipSync::system_send " << ss.str() << "[" << q.size() << "]" << std::endl;
    for (auto e : q) {
      auto sync_id = std::get<SyncId &>(e);
      // std::cout << "(INFO)SyncBlock::system_send " << sync_id.id << std::endl;
      if (sync_id.id == SyncId::unset)
        continue;
      std::vector<uint8_t> block;
      (serialize(block, std::get<Component &>(e)), ...);
      _net.sendState(id<Block>(sync_id.id), block);
    }
  }

  void system_recv(cevy::ecs::Query<SyncId, Component...> &q) const {
    std::stringstream ss;
    (ss << ... << typeid(Component).name());
    // std::cout << "(INFO)ShipSync::system_recv " << ss.str() << "[" << q.size() << "]" << std::endl;
    for (auto e : q) {
      auto sync_id = std::get<SyncId &>(e);
      // std::cout << "(INFO)SyncBlock::system_recv " << sync_id.id << std::endl;
      std::optional<std::vector<uint8_t>> block_ = _net.recvState(id<Block>(sync_id.id));
      if (!block_)
        return;
      std::vector<uint8_t> &block = block_.value();

      ((std::get<Component &>(e) = deserialize<Component>(block)), ...);
    }
  }
  Mode mode;

  public:
  void system(cevy::ecs::Query<SyncId, Component...> q) const {
    if (mode == Mode::Server)
      system_send(q);
    if (mode == Mode::Client)
      system_recv(q);
  }

  void operator()(cevy::ecs::Query<SyncId, Component...> q) const {
  // std::cout << "(INFO)SyncBlock::system()" << std::endl;
    if (mode == Mode::Server)
      system_send(q);
    if (mode == Mode::Client)
      system_recv(q);
  };
};
