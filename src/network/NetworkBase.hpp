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

#include "../ecs/SparseVector.hpp"
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
  };

  class Connection {
    public:
    tcp::socket socket;
    udp::endpoint udp_endpoint;
    std::array<uint8_t, 512> buffer;

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
        _connections(std::move(rhs._connections)),
        _tcp_acceptor(std::move(rhs._tcp_acceptor)), _temp_tcp_co(std::move(rhs._temp_tcp_co)),
        _udp_recv(std::move(rhs._udp_recv))

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
            [this, idx, callback](asio::error_code error) {
              callback(idx);
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
      _connections.emplace(std::make_pair(idx, std::move(_temp_tcp_co)));
      tcp_accept(error, idx);
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

  private:
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

  std::array<uint8_t, 512> _udp_recv;
  protected:
  std::unordered_map<ConnectionDescriptor, Connection> _connections;

  public:
  void start_thread() {
    _nw_thread = std::thread([this]() { this->_io_context.run(); });
  }

  NetworkBase(NetworkMode mode, size_t udp_port, size_t tcp_port,
              size_t client_offset)
      : _dest_tcp_port(tcp_port), _dest_udp_port(udp_port),
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
          this->udp_receive(error, bytes, this->_udp_recv, std::find_if(_connections.begin(), _connections.end(),
            [this](auto &pair){return pair.second.udp_endpoint == _udp_endpoint;}
          )->first);
          readUDP();
        });
  }

  protected:
  virtual void udp_receive(asio::error_code error, size_t bytes, std::array<uint8_t, 512> &buffer,
                           ConnectionDescriptor cd) = 0;
  virtual void tcp_receive(asio::error_code error, size_t bytes, ConnectionDescriptor cd) = 0;

  virtual void tcp_accept(asio::error_code error, size_t idx) = 0;

  template <typename Function>
  void writeTCP(ConnectionDescriptor cd, const std::vector<uint8_t> &data, Function &&func) {
    _connections.at(cd).socket.async_send(asio::buffer(data),
                         [this, &func](asio::error_code error, size_t bytes) { func(); });
  }

  template <typename Function>
  void writeUDP(ConnectionDescriptor cd, const std::vector<uint8_t> &data, Function &&func) {
    _udp_socket.async_send_to(asio::buffer(data), _connections.at(cd).udp_endpoint,
                              [this, &func](asio::error_code error, size_t) {
                                if (error)
                                  std::cerr << "(ERROR)async_send:" << error << std::endl;
                                func();
                              });
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
                                tcp_receive(error, bytes, cd);
                                read_one_TCP(cd);
                              });
  }

  // void read_all_TCP() {
  //   for (auto &cd : _connections) {
  //     read_one_TCP(cd.first);
  //   }
  // }

};
