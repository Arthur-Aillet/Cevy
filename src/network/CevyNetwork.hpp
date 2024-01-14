/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CevyNetwork
*/

#pragma once

#include <array>
#include <asio/error_code.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <list>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "NetworkBase.hpp"
#include "network.hpp"

class cevy::CevyNetwork : protected cevy::NetworkBase {
  public:
  struct Communication {
  enum ECommunication {
    State = 1,
    StateRequest = 2,
    Event = 3,
    Action = 4,
    ActionSuccess = 5,
    ActionFailure = 6,
  };
  };

  struct Event {
  enum EEvent : uint16_t {
    Summon = 1,
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
  };
  };

  struct ClientActions {
  enum EClientActions : short int {
    Connection = 1,
    Choose_Skine = 2,
    Lobby_Connection = 3,
    Move = 4,
    FREE,
  };
  };

  CevyNetwork(const std::string &endpoint, size_t port) : NetworkBase(endpoint, port){};
  CevyNetwork(size_t port) : NetworkBase(port){};
  CevyNetwork(CevyNetwork &&rhs) : NetworkBase(std::move(rhs)){};
  ~CevyNetwork(){};

  template <typename T>
  static uint8_t byte(const T &t, size_t n) {
    const T *tp = &t;
    const uint8_t *p = (uint8_t *)tp;

    return p[n];
  };

//----------------------------------- State -----------------------------------

  void sendState(uint16_t id, const std::vector<uint8_t> &block) {
    std::vector<uint8_t> fullblock = {uint8_t(Communication::State), byte(id, 0), byte(id, 1)};
    fullblock.insert(fullblock.end(), block.begin(), block.end());
    _states_send[id] = fullblock;
    writeUDP(_states_send[id], [this, id]() { _states_send.erase(id); });
  }

  std::optional<std::vector<uint8_t>> recvState(uint16_t id) {
    const auto &it = _states_recv.find(id);
    if (it != _states_recv.end()) {
      auto ret = std::move(it->second);
      _states_recv.erase(id);
      ret.erase(ret.begin(), ret.begin() + 2);
      return std::move(ret);
    }
    return std::nullopt;
  }

//----------------------------------- State -----------------------------------

  void sendEvent(uint16_t id, const std::vector<uint8_t> &block) {
    std::vector<uint8_t> fullblock = {uint8_t(Communication::Event), byte(id, 0), byte(id, 1)};
    fullblock.insert(fullblock.end(), block.begin(), block.end());
    auto it = _events_send.emplace(_events_send.begin(), fullblock);
    writeUDP(*it, [this, it]() { _events_send.erase(it); });
  }

  // std::optional<std::vector<uint8_t>> recvEvent(uint16_t id) {
  //   const auto &it = std::find_if(_events_recv.begin(), _events_recv.end(),
  //     [id](std::vector<uint8_t>& a) { return a[0] == byte(id, 0) && a[1] == byte(id, 1);});
  //   if (it != _events_recv.end()) {
  //     auto ret = std::move(*it);
  //     _events_recv.erase(it);
  //     ret.erase(ret.begin(), ret.begin() + 2);
  //     return std::move(ret);
  //   }
  //   return std::nullopt;
  // }


//----------------------------------- Action -----------------------------------

  void sendAction(uint16_t id, const std::vector<uint8_t> &block) {
    std::vector<uint8_t> fullblock = { uint8_t(Communication::Action), byte(id, 0), byte(id, 1) };
    fullblock.insert(fullblock.end(), block.begin(), block.end());
    auto it = _events_send.emplace(_actions_send.begin(), fullblock);
    writeUDP(*it, [this, it] { _actions_send.erase(it); });
  }

  void sendActionSuccess(uint16_t id, const std::vector<uint8_t> &block) {
    std::vector<uint8_t> fullblock = { uint8_t(Communication::ActionSuccess), byte(id, 0), byte(id, 1) };
    fullblock.insert(fullblock.end(), block.begin(), block.end());
    auto it = _events_send.emplace(_actions_send.begin(), fullblock);
    writeUDP(*it, [this, it] { _actions_send.erase(it); });
  }

  void sendActionFailure(uint16_t id, ActionFailureMode::EActionFailureMode mode) {
    std::vector<uint8_t> fullblock = { uint8_t(Communication::ActionFailure), byte(id, 0), byte(id, 1) };
    fullblock.push_back(mode);
    auto it = _events_send.emplace(_actions_send.begin(), fullblock);
    writeUDP(*it, [this, it] { _actions_send.erase(it); });
  }

  // std::optional<std::vector<uint8_t>> recvAction(uint16_t id) {
  //   const auto &it = std::find_if(_actions_recv.begin(), _actions_recv.end(),
  //     [id](std::vector<uint8_t>& a) { return a[0] == byte(id, 0) && a[1] == byte(id, 1);});

  //   if (it != _actions_recv.end()) {
  //     auto ret = std::move(*it);
  //     _actions_recv.erase(it);
  //     ret.erase(ret.begin(), ret.begin() + 2);
  //     return std::move(ret);
  //   }
  //   return std::nullopt;
  // }

//----------------------------------- Summon -----------------------------------

  void sendSummon(uint16_t id, uint8_t archetype) {
    half h = {.h = id};
    std::vector<uint8_t> fullblock = {byte(id, 0), byte(id, 1), archetype};
    sendEvent(Event::Summon, fullblock);
  }

  // std::optional<std::vector<uint8_t>> recvState(uint16_t id) {
  //   const auto &it = _states_recv.find(id);
  //   if (it != _states_recv.end()) {
  //     auto ret = std::move(it->second);
  //     _states_recv.erase(id);
  //     ret.erase(ret.begin(), ret.begin() + 2);
  //     return std::move(ret);
  //   }
  //   return std::nullopt;
  // }

  private:
  void handle_state(size_t bytes, std::array<uint8_t, 512> &buffer) {
    uint64_t id;
    std::memcpy(&id, &buffer[1], sizeof(id));
    std::vector<uint8_t> vec;
    vec.insert(vec.begin(), buffer.begin() + 3, buffer.begin() + bytes - 3);
    _states_recv[id] = vec;
  }

  void handle_action(size_t bytes, std::array<uint8_t, 512> &buffer) {
    std::vector<uint8_t> vec;
    vec.insert(vec.begin(), buffer.begin(), buffer.begin() + bytes);
    _actions_recv.push_front(vec);
  }

  void handle_events(size_t bytes, std::array<uint8_t, 512> &buffer) {
    std::vector<uint8_t> vec;
    vec.insert(vec.begin(), buffer.begin() + 1, buffer.begin() + bytes - 1);
    _events_recv.push_front(vec);
  }

  protected:
  void udp_receive(std::error_code error, size_t bytes, std::array<uint8_t, 512> &buffer) override {
    if (bytes < 0)
      return;
    if (buffer[0] == (uint8_t)Communication::State) {
      return handle_state(bytes, buffer);
    }
    if (buffer[0] == (uint8_t)Communication::ActionSuccess ||
        buffer[0] == (uint8_t)Communication::ActionFailure ||
        buffer[0] == (uint8_t)Communication::Action) {
      return handle_action(bytes, buffer);
    }
    if (buffer[0] == (uint8_t)Communication::Event) {
      return handle_events(bytes, buffer);
    }
  }

  private:
  // the data part contains only pure state data
  std::unordered_map<uint16_t, std::vector<uint8_t>> _states_recv;
  std::unordered_map<uint16_t, std::vector<uint8_t>> _states_send;

  using data_list = std::list<std::vector<uint8_t>>;

  // the data part contains the action status as 1 byte
  // and the name_descriptor/id data, as a prefix of 2 bytes
  data_list _actions_recv;
  data_list _actions_send;

  // the data part contains the name_descriptor/id data, as a prefix of 2 bytes
  data_list _events_recv;
  data_list _events_send;
};
