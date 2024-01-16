/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CevyNetwork
*/

#pragma once

#include <array>
#include <asio/error_code.hpp>
#include <asio/ip/address.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <list>
#include <optional>
#include <unordered_map>
#include <set>
#include <vector>
#include <mutex>

#include "App.hpp"
#include "NetworkBase.hpp"
#include "Plugin.hpp"
#include "network.hpp"

class cevy::CevyNetwork : protected cevy::NetworkBase, public ecs::Plugin {
  public:
  using NetworkBase::ConnectionDescriptor;
  using NetworkBase::NetworkMode;
  inline static const NetworkMode mode = NetworkMode::undefined;

  struct Communication {
    enum ECommunication {
      State = 1,
      StateRequest = 2,
      Event = 3,
      Action = 4,
      ActionSuccess = 5,
      ActionFailure = 6,
      HandShake = 7,
    };
  };

  struct Event {
    enum EEvent : uint16_t {
      ClientJoin = 0,
      Summon = 1,
      Dismiss = 2,
      FREE,
    };
  };

  struct ActionFailureMode {
    enum EActionFailureMode {
      Action_Success = 0,
      Action_Unavailable = 1,
      Action_Disabled = 2,
      Action_Error = 3,
      Action_Waiting = 4,
      Action_Delayed = 5,
      Busy = 6,
      Presumed = 6,
    };
  };

  struct ClientActions {
    enum EClientActions : short int {
      Connect = 1,
      Disconnect = 2,
      Lobby_Connection = 3,
      Move = 4,
      FREE,
    };
  };

  class EventIterator {

  };

  CevyNetwork(NetworkMode mode, size_t udp_port, size_t tcp_port, size_t client_offset)
      : NetworkBase(mode, udp_port, tcp_port, client_offset){};

  CevyNetwork(CevyNetwork &&rhs) : NetworkBase(rhs){};
  ~CevyNetwork(){};

  void build(ecs::App& app) override {
    start_thread();
  }

  using NetworkBase::start_thread;
  using NetworkBase::die;

  template <typename T>
  static uint8_t byte(const T &t, size_t n) {
    const T *tp = &t;
    const uint8_t *p = (uint8_t *)tp;

    return p[n];
  };

  //----------------------------------- State -----------------------------------

  virtual void sendState(uint16_t id, const std::vector<uint8_t> &block) = 0;


  void sendState(ConnectionDescriptor cd, uint16_t id, const std::vector<uint8_t> &block) {
    std::vector<uint8_t> fullblock = {uint8_t(Communication::State), byte(id, 0), byte(id, 1)};
    fullblock.insert(fullblock.end(), block.begin(), block.end());
    {
      const std::lock_guard<std::mutex> lock(_mx);
      _states_send[id] = fullblock;
    }
    writeUDP(cd, _states_send[id], [this, id]() { _states_send.erase(id); });
  }

  std::optional<std::vector<uint8_t>> recvState(uint16_t id) {
    const std::lock_guard<std::mutex> lock(_mx);
    const auto &it = _states_recv.find(id);
    if (it != _states_recv.end()) {
      auto ret = std::move(it->second);
      _states_recv.erase(id);
      ret.erase(ret.begin(), ret.begin() + 2);
      return ret;
    }
    return std::nullopt;
  }

  //----------------------------------- Event -----------------------------------

  virtual void sendEvent(uint16_t id, const std::vector<uint8_t> &block) = 0;

  void sendEvent(ConnectionDescriptor cd, uint16_t id, const std::vector<uint8_t> &block) {
    std::vector<uint8_t> fullblock = {uint8_t(Communication::Event), byte(id, 0), byte(id, 1)};
    fullblock.insert(fullblock.end(), block.begin(), block.end());
    auto it = _events_send.begin();
    {
      const std::lock_guard<std::mutex> lock(_mx);
      it = _events_send.emplace(_events_send.begin(), std::make_pair(cd, fullblock));
    }
    writeTCP(cd, it->second, [this, it]() { _events_send.erase(it); });
  }

  std::optional<std::vector<uint8_t>> recvEvent(uint16_t id) {
    const std::lock_guard<std::mutex> lock(_mx);
    const auto &it =
        std::find_if(_events_recv.begin(), _events_recv.end(), [id](auto &a) {
          return a.second[0] == byte(id, 0) && a.second[1] == byte(id, 1);
        });
    if (it != _events_recv.end()) {
      auto buff = it->second;
      buff.erase(buff.begin(), buff.begin() + 2);
      _events_recv.erase(it);
      return buff;
    }
    return std::nullopt;
  }

    std::optional<std::pair<uint16_t, std::vector<uint8_t>>> recvEvent() {
    const std::lock_guard<std::mutex> lock(_mx);
    if (!_events_recv.empty()) {
      auto buff = std::move(_events_recv.front().second);
      uint16_t id = deserialize<uint16_t>(buff);
      _events_recv.erase(_events_recv.begin());
      return std::make_pair(id, buff);
    }
    return std::nullopt;
  }

  //----------------------------------- Action -----------------------------------


  virtual void sendAction(uint16_t id, const std::vector<uint8_t> &block) = 0;

  void sendAction(ConnectionDescriptor cd, uint16_t id, const std::vector<uint8_t> &block) {
    std::vector<uint8_t> fullblock = {uint8_t(Communication::Action), byte(id, 0), byte(id, 1)};
    fullblock.insert(fullblock.end(), block.begin(), block.end());
    auto it = _actions_send.begin();
    {
      const std::lock_guard<std::mutex> lock(_mx);
      it = _actions_send.emplace(_actions_send.begin(), std::make_pair(cd, fullblock));
    }
    writeTCP(cd, it->second, [this, it] { _actions_send.erase(it); });
  }

  void sendActionSuccess(ConnectionDescriptor cd, uint16_t id, const std::vector<uint8_t> &block) {
    std::vector<uint8_t> fullblock = {uint8_t(Communication::ActionSuccess), byte(id, 0),
                                      byte(id, 1)};
    fullblock.insert(fullblock.end(), block.begin(), block.end());
    auto it = _actions_send.begin();
    {
      const std::lock_guard<std::mutex> lock(_mx);
      it = _actions_send.emplace(_actions_send.begin(), std::make_pair(cd, fullblock));
    }
    writeTCP(cd, it->second, [this, it] { _actions_send.erase(it); });
  }

  void sendActionFailure(ConnectionDescriptor cd, uint16_t id, ActionFailureMode::EActionFailureMode mode) {
    std::vector<uint8_t> fullblock = {uint8_t(Communication::ActionFailure), byte(id, 0),
                                      byte(id, 1)};
    fullblock.push_back(mode);
    auto it = _actions_send.begin();
    {
      const std::lock_guard<std::mutex> lock(_mx);
      it = _actions_send.emplace(_actions_send.begin(), std::make_pair(cd, fullblock));
    }
    writeTCP(cd, it->second, [this, it] { _actions_send.erase(it); });
  }

  std::optional<std::tuple<uint8_t, uint16_t, std::vector<uint8_t>>> recvAction() {
    if (!_actions_recv.empty()) {
      auto buff = _actions_recv.front().second;
      _actions_recv.erase(_actions_recv.begin());
      uint16_t state = deserialize<uint8_t>(buff);
      uint16_t id = deserialize<uint16_t>(buff);
      return std::make_tuple(state, id, buff);
    }
    return std::nullopt;
  }

  //----------------------------------- Summon -----------------------------------

  virtual void sendSummon(uint16_t id, uint8_t archetype) = 0;

  void sendSummon(ConnectionDescriptor cd, uint16_t id, uint8_t archetype) {
    std::vector<uint8_t> fullblock;
    serialize(fullblock, id);
    serialize(fullblock, archetype);
    sendEvent(cd, Event::Summon, fullblock);
  }

  std::optional<std::pair<uint16_t, uint8_t>> recvSummon() {
    auto buff = recvEvent(Event::Summon);
    if (!buff.has_value())
      return std::nullopt;
    uint16_t id = deserialize<uint16_t>(buff.value());
    uint8_t type = deserialize<uint8_t>(buff.value());
    return std::make_pair(id, type);
  }

  virtual void sendDismiss(uint16_t id) = 0;

  void sendDismiss(ConnectionDescriptor cd, uint16_t id) {
    std::vector<uint8_t> fullblock;
    serialize(fullblock, id);
    sendEvent(cd, Event::Dismiss, fullblock);
  }

  std::optional<uint16_t> recvDismiss() {
    auto buff = recvEvent(Event::Dismiss);
    if (!buff.has_value())
      return std::nullopt;
    uint16_t id = deserialize<uint16_t>(buff.value());
    return id;
  }

  private:
  void handle_state(ConnectionDescriptor cd, size_t bytes, std::array<uint8_t, 512> &buffer) {
    uint64_t id;
    std::memcpy(&id, &buffer[1], sizeof(id));
    std::vector<uint8_t> vec;
    vec.insert(vec.begin(), buffer.begin() + 3, buffer.begin() + bytes - 3);
    {
      const std::lock_guard<std::mutex> lock(_mx);
      _states_recv[id] = vec;
    }

    std::cout << "(INFO)handle_state: from " << cd << ", is " << id << std::endl;
  }

  void handle_action(ConnectionDescriptor cd, size_t bytes, std::array<uint8_t, 512> &buffer) {
    std::vector<uint8_t> vec;
    vec.insert(vec.begin(), buffer.begin(), buffer.begin() + bytes);
    {
      const std::lock_guard<std::mutex> lock(_mx);
      _actions_recv.push_back({cd, vec});
    }
    std::cout << "(INFO)handle_action: from " << cd << std::endl;
  }

  void handle_events(ConnectionDescriptor cd, size_t bytes, std::array<uint8_t, 512> &buffer) {
    std::vector<uint8_t> vec;
    vec.insert(vec.begin(), buffer.begin() + 1, buffer.begin() + bytes - 1);
    {
      const std::lock_guard<std::mutex> lock(_mx);
      _events_recv.push_back({cd, vec});
    }

    std::cout << "(INFO)handle_events: from " << cd << std::endl;
  }

  protected:
  void udp_receive(std::error_code error, size_t bytes, std::array<uint8_t, 512> &buffer,
                   ConnectionDescriptor cd) override {
    if (error) {
      std::cerr << "(ERROR)udp_rcv:" << error << std::endl;
      return;
    }
    if (bytes <= 0)
      return;
    if (buffer[0] == (uint8_t)Communication::State) {
      return handle_state(cd, bytes, buffer);
    }
    if (buffer[0] == (uint8_t)Communication::ActionSuccess ||
        buffer[0] == (uint8_t)Communication::ActionFailure ||
        buffer[0] == (uint8_t)Communication::Action) {
      return handle_action(cd, bytes, buffer);
    }
    if (buffer[0] == (uint8_t)Communication::Event) {
      return handle_events(cd, bytes, buffer);
    }
  }

  void tcp_receive(std::error_code error, size_t bytes, ConnectionDescriptor cd) override {
    if (error) {
      std::cerr << "(ERROR)tcp_rcv:" << error << std::endl;
      return;
    }
    if (bytes <= 0)
      return;
    auto& co = _connections.at(cd);
    if (co.buffer[0] == (uint8_t)Communication::State) {
      return handle_state(cd, bytes, co.buffer);
    }
    if (co.buffer[0] == (uint8_t)Communication::ActionSuccess ||
        co.buffer[0] == (uint8_t)Communication::ActionFailure ||
        co.buffer[0] == (uint8_t)Communication::Action) {
      return handle_action(cd, bytes, co.buffer);
    }
    if (co.buffer[0] == (uint8_t)Communication::Event) {
      return handle_events(cd, bytes, co.buffer);
    }
  }

  void tcp_accept(asio::error_code error, ConnectionDescriptor idx) override {
    if (error) {
      std::cerr << "(ERROR)tcp_accept:" << error << std::endl;
    }
    std::vector<uint8_t> block;
    serialize(block, idx);
    sendEvent(Event::ClientJoin, block);
    std::cout << "(INFO)tcp_accept" << std::endl;
  };


  private:
  // the data part contains only pure state data
  std::unordered_map<uint16_t, std::vector<uint8_t>> _states_recv;
  std::unordered_map<uint16_t, std::vector<uint8_t>> _states_send;

  using data_list = std::list<std::pair<ConnectionDescriptor, std::vector<uint8_t>>>;

  // the data part contains the action status as 1 byte
  // and the name_descriptor/id data, as a prefix of 2 bytes
  std::mutex _mx;
  data_list _actions_recv;
  data_list _actions_send;

  // the data part contains the name_descriptor/id data, as a prefix of 2 bytes
  data_list _events_recv;
  data_list _events_send;
};

// class cevy::ClientHandler : cevy::CevyNetwork {

//   std::string server_ip;
//   float server_ping;
//   // uint16_t session_id;

//   public:
//   void handleHandShake(size_t bytes, std::array<uint8_t, 512> &buffer) {
//     // std::cout << "handshake received" << std::endl;
//   }

//   // void sendHandShake(uint16_t id, uint8_t archetype) {
//   //     half h = {.h = id};
//   //     std::vector<uint8_t> fullblock = { Communication::HandShake, archetype};
//   //     writeUDP(_summon_send[id], [](){});
//   // }

//   void tcp_receive(asio::error_code error, size_t bytes, TcpConnexion &co) override {
//     if (co.buffer[0] == (uint8_t)Communication::HandShake) {
//       handleHandShake(bytes, co.buffer);
//     } else {
//       cevy::CevyNetwork::udp_receive(error, bytes, buffer, udp_endpoint); /// TODO: FIX
//     }
//   }
// };

class cevy::ServerHandler : public cevy::CevyNetwork {
  public:
    inline static const NetworkMode mode = NetworkMode::Server;
    ServerHandler(size_t udp_port, size_t tcp_port, size_t client_offset)
      : CevyNetwork(CevyNetwork::NetworkMode::Server, udp_port, tcp_port, client_offset){};

  protected:
  std::set<ConnectionDescriptor> _clients;
  std::string server_ip;
  float server_ping;
  // uint16_t session_id;

  public:
  void handleHandShake(size_t bytes, std::array<uint8_t, 512> &buffer) {
    // std::cout << "handshake received" << std::endl;
  }

  // void sendHandShake(uint16_t id, uint8_t archetype) {
  //     half h = {.h = id};
  //     std::vector<uint8_t> fullblock = { Communication::HandShake, archetype};
  //     writeUDP(_summon_send[id], [](){});
  // }

  void sendAction(uint16_t id, const std::vector<uint8_t>& block) override {
    for (auto& cd: _clients) {
      CevyNetwork::sendAction(cd, id, block);
    }
  }

  void sendState(uint16_t id, const std::vector<uint8_t> &block) override {
    for (auto& cd: _clients) {
      CevyNetwork::sendState(cd, id, block);
    }
  }

  void sendEvent(uint16_t id, const std::vector<uint8_t>& block) override {
    for (auto& cd: _clients) {
      CevyNetwork::sendEvent(cd, id, block);
    }
  }

  void sendDismiss(uint16_t id) override {
    for (auto& cd: _clients) {
      CevyNetwork::sendDismiss(cd, id);
    }
  }

  void sendSummon(uint16_t id, uint8_t archetype) override {
    for (auto& cd: _clients) {
      CevyNetwork::sendSummon(cd, id, archetype);
    }
  }

  void sendSummon(ConnectionDescriptor to, uint16_t id, uint8_t archetype, uint8_t archetype2) {
  for (auto& cd: _clients) {
      if (cd == to)
        CevyNetwork::sendSummon(cd, id, archetype);
      else
        CevyNetwork::sendSummon(cd, id, archetype2);
    }
  }

  void tcp_accept(asio::error_code error, ConnectionDescriptor idx) override {
    if (error) {
      std::cerr << "(ERROR)tcp_accept:" << error << std::endl;
    }
    _clients.insert(idx);
    std::vector<uint8_t> block;
    serialize(block, idx);
    sendEvent(Event::ClientJoin, block);
    std::cout << "(INFO)tcp_accept" << std::endl;
  };


  // void sendActionTo(ConnectionDescriptor to, uint16_t id, const std::vector<uint8_t>& block, uint16_t but_id, const std::vector<uint8_t>& but_block) {
  //   for (auto& cd: _clients) {
  //     if (cd == to)
  //       CevyNetwork::sendAction(cd, id, block);
  //     else
  //       CevyNetwork::sendAction(cd, but_id, but_block);
  //   }
  // }

  void tcp_receive(asio::error_code error, size_t bytes, ConnectionDescriptor cd) override {
    auto& co = _connections.at(cd);
    if (co.buffer[0] == (uint8_t)Communication::HandShake) {
      handleHandShake(bytes, co.buffer);
    } else {
      cevy::CevyNetwork::tcp_receive(error, bytes, cd);
    }
  }
};


class cevy::ClientHandler : public cevy::CevyNetwork {
  public:
  inline static const NetworkMode mode = NetworkMode::Client;
  ClientHandler(size_t udp_port, size_t tcp_port, size_t client_offset)
    : CevyNetwork(CevyNetwork::NetworkMode::Client, udp_port, tcp_port, client_offset){};

  protected:
  ConnectionDescriptor _server = -1;
  std::set<ConnectionDescriptor> _clients;
  std::string server_ip;
  float server_ping;
  // uint16_t session_id;

  public:
  void handleHandShake(size_t bytes, std::array<uint8_t, 512> &buffer) {
    // std::cout << "handshake received" << std::endl;
  }

  void connect(NetworkCommands& netcmd, const std::string& server_ip) {
    std::cout << "attempting to connect to " << server_ip << std::endl;
    auto callback = make_function<void, ConnectionDescriptor>([](ConnectionDescriptor cd){ std::cout << "PLAYER CONNECTION SUCCESS" << std::endl;});
    client_connect(asio::ip::address::from_string(server_ip), callback);
  }

  // void sendHandShake(uint16_t id, uint8_t archetype) {
  //     half h = {.h = id};
  //     std::vector<uint8_t> fullblock = { Communication::HandShake, archetype};
  //     writeUDP(_summon_send[id], [](){});
  // }

  void sendAction(uint16_t id, const std::vector<uint8_t>& block) override {
    if (_server != size_t(-1))
      CevyNetwork::sendAction(_server, id, block);
    else
      std::cerr << "(WARNING)sendAction before connected" << std::endl;
  }

  void sendState(uint16_t id, const std::vector<uint8_t> &block) override {
    if (_server != size_t(-1))
      CevyNetwork::sendState(_server, id, block);
    else
      std::cerr << "(WARNING)sendState before connected" << std::endl;
  }

  void sendEvent(uint16_t id, const std::vector<uint8_t>& block) override {
    if (_server != size_t(-1))
      CevyNetwork::sendEvent(_server, id, block);
    else
      std::cerr << "(WARNING)sendEvent before connected" << std::endl;
  }

  void sendDismiss(uint16_t id) override {
    if (_server != size_t(-1))
      CevyNetwork::sendDismiss(_server, id);
    else
      std::cerr << "(WARNING)sendDismiss before connected" << std::endl;
  }

  void sendSummon(uint16_t id, uint8_t archetype) override {
    if (_server != size_t(-1))
      CevyNetwork::sendSummon(_server, id, archetype);
    else
      std::cerr << "(WARNING)sendSummon before connected" << std::endl;
  }

  // void sendActionTo(ConnectionDescriptor to, uint16_t id, const std::vector<uint8_t>& block, uint16_t but_id, const std::vector<uint8_t>& but_block) {
  //   for (auto& cd: _clients) {
  //     if (cd == to)
  //       CevyNetwork::sendAction(cd, id, block);
  //     else
  //       CevyNetwork::sendAction(cd, but_id, but_block);
  //   }
  // }

  void tcp_receive(asio::error_code error, size_t bytes, ConnectionDescriptor cd) override {
    auto& co = _connections.at(cd);
    if (co.buffer[0] == (uint8_t)Communication::HandShake) {
      handleHandShake(bytes, co.buffer);
    } else {
      cevy::CevyNetwork::tcp_receive(error, bytes, cd);
    }
  }
};
