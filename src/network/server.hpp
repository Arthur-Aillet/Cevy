#pragma once

#include "network.hpp"
#include "../sparse_array.hpp"


class Server : public Networks {
    public:
        using socket_ptr = std::shared_ptr<asio::ip::tcp::socket>;

    private:
        asio::io_service &_service;
        asio::ip::tcp::acceptor _acceptor;
        sparse_array<asio::ip::tcp::socket> _sockets;

    public:
        Server(asio::io_service &service, int port = 8080) : _service(service), _acceptor(service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}
        ~Server();

        void do_accept() {
            socket_ptr socket = std::make_shared<asio::ip::tcp::socket>(_service);
            _acceptor.async_accept(*socket, [socket](const std::error_code& error) {
                if (!error) {
                    std::cout << "New connection established form "
                    << socket->remote_endpoint().address().to_string()
                    << ":" << socket->remote_endpoint().port() << std::endl;
                } else {
                    std::cout << "Error connecting: " << error.message() << std::endl;
                }
            });
        }
};