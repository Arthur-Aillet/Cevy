/*
** Agartha-Software, 2023
** Cevy
** File description:
** networkBase
*/

#pragma once

#include <asio.hpp>
#include <asio/buffer.hpp>
#include <asio/error_code.hpp>
#include <asio/ip/udp.hpp>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <sys/types.h>
#include <thread>
#include <typeinfo>
#include <vector>

#include "../ecs/SparseVector.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/address.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/read.hpp"
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
    std::string buffer;

    TcpConnexion(asio::io_context &socket) : socket(socket) {}
    // ~TcpConnexion() {};
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

  virtual void on_client_tcp_connect() { std::cout << "client: tcp connected" << std::endl; }

  void tcp_client_connect() {
    std::cout << "async tcp connect" << std::endl;
    _tcp_connexions.emplace(_tcp_connexions.end(), _io_context)
        ->socket.async_connect(tcp::endpoint(asio::ip::address::from_string("127.0.0.1"),
                                             54321), // REVIEW - ip to change
                               [this](asio::error_code error) {
              std::cout << "triggered here, maybe with error : " << error.message() << std::endl;
                                 on_client_tcp_connect();
                                 read_one_TCP(*(_tcp_connexions.end()));
                               });
  }

  void tcp_accept_new_connexion() {
    TcpConnexion tcp_connexion(_io_context);
    tcp::acceptor tcp_acceptor(_io_context, _tcp_endpoint);
    tcp_acceptor.async_accept(tcp_connexion.socket, [this, &tcp_connexion](asio::error_code error) {
      std::cout << "new tcp connexion accepted to the server"
                << std::endl; // REVIEW - debug message
      _tcp_connexions.push_back(std::move(tcp_connexion));
      read_one_TCP(*(_tcp_connexions.end()));
      tcp_accept_new_connexion();
    });
  }

  void close_all_tcp() {
    std::cout << "closing tcp connexions" << std::endl; // REVIEW - debug print
    for (auto &i : _tcp_connexions) {
      i.socket.close();
    }
    _tcp_connexions.erase(_tcp_connexions.begin(), _tcp_connexions.end());
  }

  static void start_server(tcp::endpoint _tcp_endpoint) {
    asio::io_context io_context; // FIXME - recreation only for test purposes, use _io_context from
                                 // the class instead

    udp::socket udp_socket(io_context, udp::endpoint(udp::v4(), 12345));

    tcp::socket tcp_socket(io_context, _tcp_endpoint);
    NetworkBase server = NetworkBase(std::move(udp_socket), std::move(tcp_socket));
    std::cout << "setting up acceptor;" << std::endl;
    server.tcp_accept_new_connexion();
    std::cout << "setting up udp read;" << std::endl;
    server.readUDP();
    std::cout << "running" << std::endl;
    while (true) {
      io_context.run();
    }
  }

  static void start_client(const std::string &host) {
    asio::io_context io_context; // FIXME - recreation only for test purposes, use _io_context from
                                 // the class instead
    udp::resolver resolver(io_context);
    udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), host, "daytime").begin();

    udp::socket udp_socket(io_context);
    udp_socket.open(udp::v4());
    tcp::socket tcp_socket(io_context);
    tcp::endpoint tcp_endpoint(asio::ip::address::from_string("127.0.0.1"),
                               54321); // REVIEW - ip to change
    try {
      tcp_socket.connect(tcp_endpoint); // REVIEW - actually sync, easier to test
      std::cout << "tcp connexion successful :)" << std::endl;
    } catch (asio::system_error e) {
      std::cout << "tcp connexion fail :(" << std::endl << e.code() << " bonjour" << e.what() << std::endl;
    }
    NetworkBase client = NetworkBase(std::move(udp_socket), std::move(tcp_socket));
    client._udp_endpoint = receiver_endpoint;
    client._tcp_endpoint = tcp_endpoint;
    client.tcp_client_connect();
    while (std::cin.good()) {
      std::string line;
      std::cin >> line;
      client.writeUDP(std::vector<uint8_t>(line.begin(), line.end()), []() {});
      io_context.run();
    }
  }

  public: // FIXME - public to facilitate tests
  bool quit = 0;
  asio::io_context _io_context;
  std::thread _nw_thread;

  udp::endpoint _udp_endpoint;
  tcp::endpoint _tcp_endpoint;

  udp::socket _udp_socket;
  tcp::socket _tcp_socket;
  std::vector<TcpConnexion> _tcp_connexions;

  std::array<uint8_t, 512> _udp_recv;
  std::vector<uint8_t> _tcp_recv;

  public:
  NetworkBase(NetworkBase &&rhs)
      : _nw_thread(std::move(rhs._nw_thread)), _udp_endpoint(std::move(rhs._udp_endpoint)),
        _tcp_endpoint(std::move(rhs._tcp_endpoint)), _udp_socket(std::move(rhs._udp_socket)),
        _tcp_socket(std::move(rhs._tcp_socket)), _udp_recv(std::move(rhs._udp_recv)),
        _tcp_recv(std::move(rhs._tcp_recv)){};

  NetworkBase(asio::ip::udp::socket &&udp_socket, asio::ip::tcp::socket &&tcp_socket)
      : _udp_socket(std::move(udp_socket)), _tcp_socket(std::move(tcp_socket)) {}

  NetworkBase(const std::string &endpoint, size_t port)
      : _udp_endpoint(asio::ip::udp::v4(), port), _tcp_endpoint(asio::ip::tcp::v4(), 54321), // FIXME - tcp port
        _udp_socket(_io_context), _tcp_socket(_io_context) {
    _udp_socket.open(asio::ip::udp::v4());
    _tcp_socket.open(asio::ip::tcp::v4());

    _nw_thread = std::thread([this]() {
      while (!this->quit)
        this->_io_context.run();
    });
  }

  NetworkBase(size_t port)
      : _udp_endpoint(asio::ip::udp::v4(), port), _tcp_endpoint(asio::ip::tcp::v4(), port),
        _udp_socket(_io_context), _tcp_socket(_io_context) {
    _udp_socket.open(asio::ip::udp::v4());
    _tcp_socket.open(asio::ip::tcp::v4());

    _nw_thread = std::thread([this]() {
      while (!this->quit)
        this->_io_context.run();
    });
  }

  NetworkBase(NetworkMode mode, size_t port)
      : _udp_endpoint(udp::v4(), port), _tcp_endpoint(tcp::v4(), port), _udp_socket(_io_context),
        _tcp_socket(_io_context) {
    _udp_socket.open(udp::v4());
    _tcp_socket.open(tcp::v4());

    if (mode == NetworkMode::Server) {
      tcp_accept_new_connexion();
    }
  }

  protected:
  template <typename Function>
  void writeUDP(const std::vector<uint8_t> &data, Function &&func) {
    _udp_socket.async_send_to(asio::buffer(data), _udp_endpoint,
                              [this](asio::error_code error, size_t bytes) {
                                std::cout << "fonction à remplacer" << std::endl;
                              });
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

  void readUDP() {
    _udp_socket.async_receive_from(
        asio::buffer(_udp_recv), _udp_endpoint, [this](asio::error_code error, size_t bytes) {
          this->udp_receive(error, bytes, this->_udp_recv, _udp_endpoint);
          readUDP();
        });
  }

  void read_one_TCP(TcpConnexion &co) {
    asio::async_read(co.socket, asio::buffer(co.buffer),
                     [this, &co](asio::error_code error, size_t bytes) {
                       tcp_receive(error, bytes, co);
                       read_one_TCP(co);
                     });
  }

  void readTCP() { // REVIEW - useful ?
    for (auto &co : _tcp_connexions) {
      read_one_TCP(co);
    }
  }

  virtual void tcp_receive(asio::error_code error, size_t bytes, TcpConnexion &co) {
    if (!error) {
      std::cout << "revieved " << bytes << " bytes:" << std::endl;
      std::cout << co.buffer << std::endl;
      // for (size_t i = 0; i < bytes; ++i) {
      //     std:: cout << std::hex << _udp_recv[i];
      // }
      std::cout << std::endl << std::endl;
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
};

// class TcpClient : cevy::NetworkBase {
//   const std::string server_ip;
//   const int server_port;
//   tcp::socket socket;

//   TcpClient(std::string server_ip, int server_port) : cevy::NetworkBase(), server_ip(server_ip),
//   server_port(server_port), socket(_io_context) {};

// };