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
#include <mutex>
#include <sstream>
#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <thread>
#include <typeinfo>
#include <unistd.h>
#include <vector>

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

    Connection(asio::io_context &socket) : socket(socket) {}
  };

  using ConnectionDescriptor = size_t;

  NetworkBase(const NetworkBase &rhs) = delete;

  NetworkBase(NetworkBase &rhs)
      : quit(rhs.quit), _nw_thread(std::move(rhs._nw_thread)),

        _tcp_port(std::move(rhs._tcp_port)),
        _udp_port(std::move(rhs._udp_port)),

        _dest_tcp_port(std::move(rhs._dest_tcp_port)),
        _dest_udp_port(std::move(rhs._dest_udp_port)),

        // _udp_endpoint(std::move(rhs._udp_endpoint)),
        _tcp_endpoint(std::move(rhs._tcp_endpoint)),

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

  void client_connect(asio::ip::address dest);

  template<typename F>
  void client_connect(asio::ip::address dest, F callback) {
    std::cout << "async tcp connect" << std::endl;
    ConnectionDescriptor idx = ++connection_count;
    _connections.emplace(idx, _io_context);
    _connections.at(idx)
        .socket.async_connect(
            tcp::endpoint(dest, _dest_tcp_port),
            [dest, this, idx, callback](asio::error_code error) {
              callback(idx);
              std::cout << "address:" << dest.to_string() << ", port: " << _dest_udp_port << std::endl;
              _connections.at(idx).udp_endpoint = std::move(asio::ip::udp::endpoint(dest, _dest_udp_port));
              std::cout << "triggered here, maybe with error : " << error.message() << std::endl;
              read_one_TCP(idx);
            });
    std::cout << "set async connect" << std::endl;
  }

  void tcp_accept_new_connexion();

  void close_all_tcp();

  void close_dead_tcp();

  protected:
  NetworkMode _mode;
  private:
  bool quit = 0;
  asio::io_context _io_context;
  std::thread _nw_thread;
  std::mutex _mx;

  size_t _tcp_port;
  size_t _udp_port;
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

  void start_thread();

  void die() {
    quit = true;
    _nw_thread.join();
  }

  NetworkBase(NetworkMode mode, size_t udp_port, size_t tcp_port,
              size_t client_offset)
      : _mode(mode),
        _tcp_port(tcp_port + (client_offset * (mode == NetworkMode::Client))),
        _udp_port(udp_port + (client_offset * (mode == NetworkMode::Client))),
        _dest_tcp_port(tcp_port + (client_offset * (mode == NetworkMode::Server))),
        _dest_udp_port(udp_port + (client_offset * (mode == NetworkMode::Server))),
        _udp_endpoint(asio::ip::udp::v4(), _udp_port),
        _tcp_endpoint(asio::ip::tcp::v4(), _tcp_port),
        _udp_socket(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), _udp_port)),
        _tcp_socket(_io_context),
        _tcp_acceptor(_io_context), _temp_tcp_co(_io_context) {
    if (_mode == NetworkMode::Server) {
      _tcp_acceptor = asio::ip::tcp::acceptor(_io_context, _tcp_endpoint);
    } else {
      _tcp_socket = asio::ip::tcp::socket(_io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), _tcp_port));
    }
    // _udp_socket.open(asio::ip::udp::v4());
    // _tcp_socket.open(asio::ip::tcp::v4());
  }

  void readUDP();

  protected:
  virtual void udp_receive(asio::error_code error, size_t bytes, std::array<uint8_t, 512> &buffer,
                           ConnectionDescriptor cd) = 0;
  virtual void tcp_receive(asio::error_code error, size_t bytes, ConnectionDescriptor cd) = 0;

  virtual void tcp_accept(asio::error_code error, size_t idx) = 0;

  template <typename Function>
  void writeTCP(ConnectionDescriptor cd, const std::vector<uint8_t> &data, Function &&func) {
    std::lock_guard lock(_mx);
    if (_connections.find(cd) == _connections.end()) {
      return;
    }
    _connections.at(cd).socket.async_send(asio::buffer(data),
                         [this, func](asio::error_code error, size_t) {
                            if (error)
                              std::cerr << "(ERROR)writeTCP:" << error << std::endl;
                            func();
                          });
  }

  template <typename Function>
  void writeUDP(ConnectionDescriptor cd, const std::vector<uint8_t> &data, Function &&func) {
    std::lock_guard lock(_mx);
    if (_connections.find(cd) == _connections.end()) {
      return;
    }
    _udp_socket.async_send_to(asio::buffer(data), _connections.at(cd).udp_endpoint,
                              [this, func](asio::error_code error, size_t) {
                                if (error)
                                  std::cerr << "(ERROR)writeUDP:" << error << std::endl;
                                func();
                              });
  }

  void read_one_TCP(ConnectionDescriptor cd);

  // void read_all_TCP() {
  //   for (auto &cd : _connections) {
  //     read_one_TCP(cd.first);
  //   }
  // }

};
