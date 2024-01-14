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

  class TcpConnexion {
    public:
    tcp::socket socket;
    std::vector<uint8_t> buffer = std::vector<uint8_t>(1024, 0);

    TcpConnexion(asio::io_context &socket) : socket(socket) {}
  };

  private:
  template <typename Component>
  std::stringstream serialization(SparseVector<Component> components) {
    std::stringstream ss;
    ss << typeid(Component).name() << ' ' << components;
    return ss;
  }

  template <typename Component>
  std::tuple<std::string, Component> unserialization(std::istringstream iss) {
    std::string typeIdName;
    Component component;
    iss >> typeIdName >> component;
    if (typeIdName != typeid(component).name()) {
      std::cerr << "Error: Invalide component type: " << typeIdName << std::endl;
    }
    return {typeIdName, component};
  }

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

  virtual void on_client_tcp_connect() { std::cout << "client: tcp connected :D" << std::endl; }

  void tcp_client_connect() {
    std::cout << "async tcp connect" << std::endl;
    _tcp_connexions.emplace(_tcp_connexions.end(), _io_context)
        ->socket.async_connect(
            tcp::endpoint(asio::ip::address::from_string(_dest_ip), _dest_tcp_port),
            [this](asio::error_code error) {
              std::cout << "triggered here, maybe with error : " << error.message() << std::endl;
              on_client_tcp_connect();
              read_one_TCP(_tcp_connexions.back());
            });
    std::cout << "set async connect" << std::endl;
  }

  void tcp_client_connect_sync() {
    try {
      _tcp_connexions.emplace(_tcp_connexions.end(), _io_context)
          ->socket.connect(tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 54321));
      std::cout << "tcp connexion successful :)" << std::endl;
    } catch (asio::system_error e) {
      std::cout << "tcp connexion fail :(" << std::endl
                << e.code() << " bonjour" << e.what() << std::endl;
    }
  }

  void tcp_accept_new_connexion() {
    _temp_tcp_co = TcpConnexion(_io_context);

    _tcp_acceptor.async_accept(_temp_tcp_co.socket, [this](asio::error_code error) {
      std::cout << "new tcp connexion accepted to the server" << std::endl;
      _tcp_connexions.push_back(std::move(_temp_tcp_co));
      read_one_TCP(_tcp_connexions.back());
      tcp_accept_new_connexion();
    });
    std::cout << "log apres" << std::endl;
  }

  void close_all_tcp() {
    std::cout << "closing tcp connexions" << std::endl;
    for (auto &i : _tcp_connexions) {
      i.socket.close();
    }
    _tcp_connexions.clear();
  }

  void close_dead_tcp() {
    std::cout << "closing dead tcp connexions" << std::endl;
    auto to_erase =
        std::remove_if(_tcp_connexions.begin(), _tcp_connexions.end(), [](TcpConnexion &co) {
          if (!co.socket.is_open())
            std::cout << "will erase closed socket" << std::endl;
          return !co.socket.is_open();
        });
    _tcp_connexions.erase(to_erase, _tcp_connexions.end());
  }

  private:
  bool quit = 0;
  asio::io_context _io_context;
  std::thread _nw_thread;

  std::string _dest_ip;
  size_t _dest_tcp_port;
  size_t _dest_udp_port;

  udp::endpoint _udp_endpoint;
  tcp::endpoint _tcp_endpoint;

  udp::socket _udp_socket;
  tcp::socket _tcp_socket;
  std::vector<TcpConnexion> _tcp_connexions;
  tcp::acceptor _tcp_acceptor;
  TcpConnexion _temp_tcp_co;

  std::array<uint8_t, 512> _udp_recv;

  public:
  void start_thread() {
    _nw_thread = std::thread([this]() { this->_io_context.run(); });
  }

  NetworkBase(NetworkMode mode, const std::string &endpoint, size_t udp_port, size_t tcp_port,
              size_t client_offset)
      : _dest_ip(endpoint), _dest_tcp_port(tcp_port), _dest_udp_port(udp_port),
        _udp_endpoint(asio::ip::udp::v4(),
                      udp_port + (client_offset * (mode == NetworkMode::Client))),
        _tcp_endpoint(asio::ip::tcp::v4(),
                      tcp_port + (client_offset * mode == NetworkMode::Client)),
        _udp_socket(_io_context), _tcp_socket(_io_context),
        _tcp_acceptor(_io_context, _tcp_endpoint), _temp_tcp_co(_io_context) {
    _udp_socket.open(asio::ip::udp::v4());
    _tcp_socket.open(asio::ip::tcp::v4());
  }

  virtual void udp_receive(asio::error_code error, size_t bytes, std::array<uint8_t, 512> &buffer) {
    if (!error) {
      std::cout << "revieved " << bytes << " bytes:" << std::endl;
      for (size_t i = 0; i < bytes; ++i) {
        std::cout << std::hex << _udp_recv[i];
      }
      std::cout << std::endl << std::endl;
      readUDP();
    }
  }

  virtual void udp_receive(asio::error_code error, size_t bytes, std::array<uint8_t, 512> &buffer,
                           udp::endpoint udp_endpoint) {
    if (!error) {
      std::cout << "revieved " << bytes << " bytes:" << std::endl;
      for (size_t i = 0; i < bytes; ++i) {
        std::cout << std::hex << _udp_recv[i];
      }
      std::cout << std::endl << std::endl;
    }
  }

  void readUDP() {
    _udp_socket.async_receive_from(
        asio::buffer(_udp_recv), _udp_endpoint, [this](asio::error_code error, size_t bytes) {
          this->udp_receive(error, bytes, this->_udp_recv, _udp_endpoint);
          readUDP();
        });
  }

  protected:
  template <typename Function>
  void writeUDP(const std::vector<uint8_t> &data, Function &&func) {
    _udp_socket.async_send_to(asio::buffer(data), _udp_endpoint,
                              [this, &func](asio::error_code error, size_t bytes) { func(); });
  }

  void read_one_TCP(TcpConnexion &co) {
    co.socket.async_read_some(asio::buffer(co.buffer),
                              [this, &co](asio::error_code error, size_t bytes) {
                                if (error.value() == 2) { // end of co
                                  co.socket.close();
                                  close_dead_tcp();
                                  return;
                                }
                                tcp_receive(error, bytes, co);
                                read_one_TCP(co);
                              });
  }

  void read_all_TCP() {
    for (auto &co : _tcp_connexions) {
      read_one_TCP(co);
    }
  }

  template <typename Function>
  void write_one_TCP(TcpConnexion &co, const std::vector<uint8_t> &data, Function &&func) {
    co.socket.async_send(asio::buffer(data),
                         [this, &func](asio::error_code error, size_t bytes) { func(); });
  }

  virtual void tcp_receive(asio::error_code error, size_t bytes, TcpConnexion &co) {
    if (!error) {
      std::cout << "revieved " << bytes << " bytes:" << std::endl;
      for (size_t i = 0; i < bytes; ++i) {
        std::cout << co.buffer[i];
      }
      std::cout << std::endl;
    }
  }
};
