/*
** Agartha-Software, 2023
** Cevy
** File description:
** networkBase
*/

#pragma once

#include <algorithm>
#include <asio.hpp>
#include <asio/buffer.hpp>
#include <asio/error_code.hpp>
#include <asio/ip/udp.hpp>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <thread>
#include <typeinfo>
#include <unistd.h>
#include <vector>
#include <list>

#include "../ecs/SparseVector.hpp"
#include "Plugin.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/address.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/system_error.hpp"
#include "network.hpp"

class cevy::NetworkBase {
  using tcp = asio::ip::tcp;
  using udp = asio::ip::udp;

  public:
  enum NetworkMode {
    Server,
    Client,
    // undefined,
  };

  class Connection {
    public:
    tcp::socket socket;
    udp::endpoint udp_endpoint;
    std::array<uint8_t, 512> buffer;
    std::vector<uint8_t> _tcp_queue;
    std::vector<uint8_t> _udp_queue;

    Connection(asio::io_context &socket) : socket(socket) {}
  };

  using ConnectionDescriptor = size_t;

  NetworkBase(const NetworkBase &rhs) = delete;

  NetworkBase(NetworkBase &rhs)
      : quit(rhs.quit), _nw_thread(std::move(rhs._nw_thread)),

        _dest_tcp_port(std::move(rhs._dest_tcp_port)),
        _dest_udp_port(std::move(rhs._dest_udp_port)),

        _udp_endpoint(std::move(rhs._udp_endpoint)), _tcp_endpoint(std::move(rhs._tcp_endpoint)),

        _udp_socket(std::move(rhs._udp_socket)), _tcp_socket(std::move(rhs._tcp_socket)),
        _tcp_acceptor(std::move(rhs._tcp_acceptor)), _temp_tcp_co(std::move(rhs._temp_tcp_co)),
        _udp_recv(std::move(rhs._udp_recv)),
        _connections(std::move(rhs._connections))

            {};

  public:
  union half {
    uint16_t h;
    struct {
      uint8_t b0;
      uint8_t b1;
    } b;
  };

  union word {
    uint16_t q;
    struct {
      half b0;
      half b1;
    } h;
    struct {
      uint8_t b0;
      uint8_t b1;
      uint8_t b2;
      uint8_t b3;
    } b;
  };

  void client_connect(asio::ip::address dest) {
    std::cout << "async tcp connect" << std::endl;
    ConnectionDescriptor idx = connection_count;
    _connections.emplace(idx, _io_context);
    _connections.at(idx)
        .socket.async_connect(
            tcp::endpoint(dest, _dest_tcp_port),
            [this, idx](asio::error_code error) {
              std::cout << "triggered here, maybe with error : " << error.message() << std::endl;
              read_one_TCP(idx);
            });
    std::cout << "set async connect" << std::endl;
  }

  template<typename F>
  void client_connect(asio::ip::address dest, F callback) {
    std::cout << "async tcp connect" << std::endl;
    ConnectionDescriptor idx = connection_count;
    _connections.emplace(idx, _io_context);
    _connections.at(idx)
        .socket.async_connect(
            tcp::endpoint(dest, _dest_tcp_port),
            [dest, this, idx, callback](asio::error_code error) {
              callback(idx);
              _connections.at(idx).udp_endpoint = std::move(asio::ip::udp::endpoint(dest, _dest_udp_port));
              std::cout << "triggered here, maybe with error : " << error.message() << std::endl;
              read_one_TCP(idx);
            });
    std::cout << "set async connect" << std::endl;
  }

  void tcp_accept_new_connexion() {
    _temp_tcp_co = Connection(_io_context);
    ConnectionDescriptor idx = connection_count;

    _tcp_acceptor.async_accept(_temp_tcp_co.socket, [this, idx](asio::error_code error) {
      if (error) {
        std::cerr << "(ERROR)async_accept:" << error << std::endl;
        return;
      }
      std::cout << "new tcp connexion accepted to the server" << std::endl;
      _temp_tcp_co.udp_endpoint = asio::ip::udp::endpoint(_temp_tcp_co.socket.remote_endpoint().address(), _dest_udp_port);

      _connections.emplace(std::make_pair(idx, std::move(_temp_tcp_co)));
      AcceptTCP(error, idx);
      read_one_TCP(idx);
      tcp_accept_new_connexion();
    });
    std::cout << "log apres" << std::endl;
  }

  void close_all_tcp() {
    std::cout << "closing tcp connexions" << std::endl;
    for (auto& [idx, co] : _connections) {
      co.socket.close();
    }
    _connections.clear();
  }

  void close_dead_tcp() {
    std::cout << "closing dead tcp connexions" << std::endl;
    std::vector<std::unordered_map<ConnectionDescriptor, Connection>::iterator> to_erase;
    for (auto it = _connections.begin(); it != _connections.end(); ++it) {
      if (!it->second.socket.is_open())
        to_erase.push_back(it);
    }
    for (auto erase : to_erase) {
      _connections.erase(erase);
    }
  }

  protected:
  NetworkMode _mode;
  bool quit = 0;
  asio::io_context _io_context;
  std::thread _nw_thread;

  size_t _dest_tcp_port;
  size_t _dest_udp_port;

  udp::endpoint _udp_endpoint;
  tcp::endpoint _tcp_endpoint;

  udp::socket _udp_socket;
  tcp::socket _tcp_socket;
  size_t connection_count = 0;
  tcp::acceptor _tcp_acceptor;
  Connection _temp_tcp_co;

  const std::vector<uint8_t> _tcp_init_header;
  const std::vector<uint8_t> _udp_init_header;

  std::vector<uint8_t> _tcp_queue;
  std::vector<uint8_t> _udp_queue;

  std::array<uint8_t, 512> _udp_recv;
  protected:
  std::unordered_map<ConnectionDescriptor, Connection> _connections;

  public:

  void send() {
    std::vector<uint8_t> tcp_header = HeaderTCP();
    std::vector<uint8_t> udp_header = HeaderUDP();
    std::copy(tcp_header.begin(), tcp_header.end(), _tcp_queue.begin());
    std::copy(udp_header.begin(), udp_header.end(), _udp_queue.begin());
    for (auto &[cd, c] : _connections) {
      send_to_if(c.socket, c._tcp_queue, tcp_header, true);
      send_to_if(c.socket, _tcp_queue, tcp_header);
      send_to_if(c.udp_endpoint, c._udp_queue, udp_header, true);
      send_to_if(c.udp_endpoint, _udp_queue, udp_header);
      c._tcp_queue = _tcp_init_header;
      c._udp_queue = _udp_init_header;
    }
  }

  inline void send_to_if(asio::ip::tcp::socket& socket, std::vector<uint8_t>& buff, const std::vector<uint8_t>& header, bool rewrite = false) {
    if (buff.size() >= header.size()) {
      if (rewrite)
        std::copy(header.begin(), header.end(), buff.begin());
      socket.send(buff);
    }
  }

  inline void send_to_if(asio::ip::udp::endpoint& endpoint, std::vector<uint8_t>& buff, const std::vector<uint8_t>& header, bool rewrite = false) {
    if (buff.size() >= header.size()) {
      if (rewrite)
        std::copy(header.begin(), header.end(), buff.begin());
      _udp_socket.send_to(buff, endpoint);
    }
  }

  void die() {
    quit = true;
    _nw_thread.join();
  }

  NetworkBase(NetworkMode mode, size_t udp_port, size_t tcp_port,
              size_t client_offset)
      : _mode(mode), _dest_tcp_port(tcp_port), _dest_udp_port(udp_port),
        _udp_endpoint(asio::ip::udp::v4(),
                      udp_port + (client_offset * (mode == NetworkMode::Client))),
        _tcp_endpoint(asio::ip::tcp::v4(),
                      tcp_port + (client_offset * mode == NetworkMode::Client)),
        _udp_socket(_io_context), _tcp_socket(_io_context),
        _tcp_acceptor(_io_context, _tcp_endpoint), _temp_tcp_co(_io_context) {
    _udp_socket.open(asio::ip::udp::v4());
    _tcp_socket.open(asio::ip::tcp::v4());
  }

  void readUDP() {
    _udp_socket.async_receive_from(
        asio::buffer(_udp_recv), _udp_endpoint,
        [this](asio::error_code error, size_t bytes) {
          this->ReceiveUDP(error, bytes, this->_udp_recv, std::find_if(_connections.begin(), _connections.end(),
            [this](auto &pair){return pair.second.udp_endpoint == _udp_endpoint;}
          )->first);
          readUDP();
        });
  }

  protected:
  virtual void ReceiveUDP(asio::error_code error, size_t bytes, std::array<uint8_t, 512> &buffer,
                           ConnectionDescriptor cd) = 0;
  virtual void ReceiveTCP(asio::error_code error, size_t bytes, ConnectionDescriptor cd) = 0;

  virtual void AcceptTCP(asio::error_code error, size_t idx) = 0;

  virtual std::vector<uint8_t> HeaderTCP() const;
  virtual std::vector<uint8_t> HeaderUDP() const;

  template <typename Function>
  void writeTCP(ConnectionDescriptor cd, const std::vector<uint8_t> &data) {
    if (_connections.find(cd) == _connections.end()) {
      return;
    }
    auto& q = _connections.at(cd)._tcp_queue;
    q.insert(q.end(), data.begin(), data.end());
  }

  template <typename Function>
  void writeUDP(ConnectionDescriptor cd, const std::vector<uint8_t> &data) {
    if (_connections.find(cd) == _connections.end()) {
      return;
    }
    auto& q = _connections.at(cd)._udp_queue;
    q.insert(q.end(), data.begin(), data.end());
  }

  template <typename Function>
  void writeTCP(const std::vector<uint8_t> &data) {
    auto& q = _tcp_queue;
    q.insert(q.end(), data.begin(), data.end());
  }

  template <typename Function>
  void writeUDP(const std::vector<uint8_t> &data) {
    auto& q = _udp_queue;
    q.insert(q.end(), data.begin(), data.end());
  }


  void read_one_TCP(ConnectionDescriptor cd) {
    auto& co = _connections.at(cd);
    co.socket.async_read_some(asio::buffer(co.buffer),
                              [this, &co, cd](asio::error_code error, size_t bytes) {
                                if (error.value() == 2) { // end of co
                                  co.socket.close();
                                  close_dead_tcp();
                                  return;
                                }
                                ReceiveTCP(error, bytes, cd);
                                read_one_TCP(cd);
                              });
  }

  // void read_all_TCP() {
  //   for (auto &cd : _connections) {
  //     read_one_TCP(cd.first);
  //   }
  // }

};
