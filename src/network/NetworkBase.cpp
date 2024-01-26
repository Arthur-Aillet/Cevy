/*
** Agartha-Software, 2023
** Cevy
** File description:
** networkBase
*/

#include "NetworkBase.hpp"

void cevy::NetworkBase::client_connect(asio::ip::address dest) {
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

void cevy::NetworkBase::tcp_accept_new_connexion() {
  _temp_tcp_co = Connection(_io_context);
  ConnectionDescriptor idx = connection_count;

  _tcp_acceptor.async_accept(_temp_tcp_co.socket, [this, idx](asio::error_code error) {
    if (error) {
      std::cerr << "(ERROR)async_accept:" << error << std::endl;
      return;
    }
    std::cout << "new tcp connexion accepted to the server" << std::endl;
    std::cout << "address:" << _temp_tcp_co.socket.remote_endpoint().address().to_string() << ", port: " << _dest_udp_port << std::endl;
    _temp_tcp_co.udp_endpoint = asio::ip::udp::endpoint(_temp_tcp_co.socket.remote_endpoint().address(), _dest_udp_port);

    _connections.emplace(std::make_pair(idx, std::move(_temp_tcp_co)));
    tcp_accept(error, idx);
    read_one_TCP(idx);
    tcp_accept_new_connexion();
  });
  std::cout << "log apres" << std::endl;
}

void cevy::NetworkBase::close_all_tcp() {
  std::cout << "closing tcp connexions" << std::endl;
  for (auto& [idx, co] : _connections) {
    co.socket.close();
  }
  _connections.clear();
}

void cevy::NetworkBase::close_dead_tcp() {
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

void cevy::NetworkBase::start_thread() {
  if (_mode == NetworkMode::Server) {
    tcp_accept_new_connexion();
  }
  readUDP();
  _nw_thread = std::thread([this]() { this->_io_context.run(); });
}

void cevy::NetworkBase::readUDP() {
  _udp_socket.async_receive_from(
      asio::buffer(_udp_recv), _udp_endpoint,
      [this](asio::error_code error, size_t bytes) {
        // std::cout << "UDP_GET " <<  _udp_endpoint.address().to_string() << std::endl;
        auto cd_it = std::find_if(_connections.begin(), _connections.end(),
          [this](auto &pair){return pair.second.udp_endpoint.address() == _udp_endpoint.address();}
        );
        if (cd_it != _connections.end()) {
          this->udp_receive(error, bytes, _udp_recv, cd_it->first);
        }
        readUDP();
      });
}

void cevy::NetworkBase::read_one_TCP(ConnectionDescriptor cd) {
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
