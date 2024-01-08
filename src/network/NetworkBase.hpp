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

// #include "../entity.hpp"
#include "../ecs/SparseVector.hpp"
#include "network.hpp"

/**NOTE - Utiliser des templates pour envoyer les données
 * prend donc n'importe qu'elle type en entré (on doit créer un prototype de la fonction pour chaque
 * type possible) sérializer l'objet avec devant son type (typeid()) cela permet de connaitre le
 * type de l'objet a déserializer recevoire l'objet serializer prendre une taille fixe de la string
 * envoyer pour retrouver le type de l'objet deserialzer l'objet dans le type trouvée
 */

class cevy::NetworkBase {
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

  static void start_server() {
    asio::io_context io_context;
    asio::ip::udp::socket udp(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 13));
    asio::ip::tcp::socket tcp(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 999));
    NetworkBase server = NetworkBase(std::move(udp), std::move(tcp));
    std::cout << "setting up read;" << std::endl;
    server.readUDP();
    std::cout << "running" << std::endl;
    while (true) {
      io_context.run();
    }
  }

  static void start_client(const std::string &host) {
    asio::io_context io_context;
    asio::ip::udp::resolver resolver(io_context);
    asio::ip::udp::endpoint receiver_endpoint =
        *resolver.resolve(asio::ip::udp::v4(), host, "daytime").begin();

    asio::ip::udp::socket udp(io_context);
    udp.open(asio::ip::udp::v4());
    asio::ip::tcp::socket tcp(io_context);
    NetworkBase client = NetworkBase(std::move(udp), std::move(tcp));
    client._udp_endpoint = receiver_endpoint;
    while (std::cin.good()) {
      std::string line;
      std::cin >> line;
      client.writeUDP(std::vector<uint8_t>(line.begin(), line.end()), []() {});
      io_context.run();
    }
  }

  protected:
  bool quit = 0;
  asio::io_context _io_context;
  std::thread _nw_thread;

  asio::ip::udp::endpoint _udp_endpoint;
  asio::ip::tcp::endpoint _tcp_endpoint;

  asio::ip::udp::socket _udp_socket;
  asio::ip::tcp::socket _tcp_socket;

  std::array<uint8_t, 512> _udp_recv;
  std::vector<uint8_t> _tcp_recv;

  NetworkBase(NetworkBase &&rhs)
      : _nw_thread(std::move(rhs._nw_thread)), _udp_endpoint(std::move(rhs._udp_endpoint)),
        _tcp_endpoint(std::move(rhs._tcp_endpoint)), _udp_socket(std::move(rhs._udp_socket)),
        _tcp_socket(std::move(rhs._tcp_socket)), _udp_recv(std::move(rhs._udp_recv)),
        _tcp_recv(std::move(rhs._tcp_recv)){};

  NetworkBase(asio::ip::udp::socket &&udp_socket, asio::ip::tcp::socket &&tcp_socket)
      : _udp_socket(std::move(udp_socket)), _tcp_socket(std::move(tcp_socket)) {}

  NetworkBase(const std::string &endpoint, size_t port)
      : _udp_endpoint(asio::ip::udp::v4(), port), _tcp_endpoint(asio::ip::tcp::v4(), port),
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

  void readUDP() {
    _udp_socket.async_receive_from(asio::buffer(_udp_recv), _udp_endpoint,
                                   [this](asio::error_code error, size_t bytes) {
                                     this->udp_receive(error, bytes, this->_udp_recv);
                                     readUDP();
                                   });
  }

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

  template <typename Component>
  void writeTCP(asio::ip::tcp::socket &socket, Component const &component) {
    std::function<void(std::error_code, std::size_t)> handler = [&socket](std::error_code error,
                                                                          std::size_t bytes) {
      if (error) {
        std::cerr << "Error: Component of the socket " << &socket << " is not sent.\n"
                  << error.message() << std::endl;
        socket.close();
      }
    };
    socket.async_write_some(asio::buffer(serialization(component), sizeof(std::stringstream)),
                            handler);
  }

  template <typename Component>
  std::tuple<std::string, Component> readTCP(asio::ip::tcp::socket &socket) {
    std::string buffer;
    std::function<void(std::error_code, std::size_t)> handler = [&](std::error_code error,
                                                                    std::size_t bytes) {
      if (!error) {
        std::istringstream iss(buffer);
        return unserialization<Component>(iss);
      } else {
        std::cerr << "Error: " << error.message() << std::endl;
        return nullptr;
      }
    };
    socket.async_read_some(asio::buffer(buffer), handler);
  }
};
