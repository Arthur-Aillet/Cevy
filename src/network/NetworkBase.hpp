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
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <thread>
#include <typeinfo>
#include <asio/ip/udp.hpp>
#include <vector>
#include <stdint.h>

#include "../ecs/SparseVector.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/address.hpp"
#include "asio/ip/tcp.hpp"
#include "network.hpp"

class cevy::NetworkBase
{
    using tcp = asio::ip::tcp;
    using udp = asio::ip::udp;

    public:
        enum NetworkMode {
            Server,
            Client,
        };

        class TcpConnexion
        {
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

        void tcp_accept_new_connexion(asio::io_context &io_context) {
            TcpConnexion tcp_connexion(io_context);
            tcp::acceptor tcp_acceptor(io_context, tcp::endpoint(tcp::v4(), 999));
            tcp_acceptor.async_accept(tcp_connexion.socket, [this, &io_context, &tcp_connexion](){
                std::cout << "new tcp connexion accepted to the server" << std::endl; // REVIEW - debug message
                _tcp_connexions.push_back(std::move(tcp_connexion));
                tcp_accept_new_connexion(io_context);
            });
        }

        static void start_server() {
            asio::io_context io_context;

            udp::socket udp_socket(io_context, udp::endpoint(udp::v4(), 13));

            tcp::socket tcp_socket(io_context, tcp::endpoint(tcp::v4(), 999));
            NetworkBase server = NetworkBase(std::move(udp_socket), std::move(tcp_socket));
            server.tcp_accept_new_connexion(io_context);
            std::cout << "setting up read;" << std::endl;
            server.readUDP();
            std::cout << "running" << std::endl;
            while (true) {
                io_context.run();
            }
        }

        static void start_client(const std::string& host) {
            asio::io_context io_context;
            udp::resolver resolver(io_context);
            udp::endpoint receiver_endpoint =
                *resolver.resolve(udp::v4(), host, "daytime").begin();

            udp::socket udp_socket(io_context);
            udp_socket.open(udp::v4());
            tcp::socket tcp_socket(io_context);
            tcp::endpoint tcp_endpoint(asio::ip::address::from_string("127.0.0.1"), 999); // REVIEW - ip to change
            tcp_socket.connect(tcp_endpoint); // REVIEW - actually sync, easier to test
            NetworkBase client = NetworkBase(std::move(udp_socket), std::move(tcp_socket));
            client._udp_endpoint = receiver_endpoint;
            while (std::cin.good()) {
                std::string line;
                std::cin >> line;
                client.writeUDP(std::vector<uint8_t>(line.begin(), line.end()), [](){});
                io_context.run();
            }
        }

    protected:
        bool quit = 0;
        asio::io_context _io_context;
        std::thread _nw_thread;

        udp::endpoint _udp_endpoint;
        tcp::endpoint _tcp_endpoint;

        udp::socket _udp_socket;
        tcp::socket _tcp_socket;



        std::array<uint8_t, 512> _udp_recv;
        std::vector<uint8_t> _tcp_recv;
        std::vector<TcpConnexion> _tcp_connexions;

        NetworkBase(udp::socket&& udp_socket, tcp::socket&& tcp_socket)
         : _udp_socket(std::move(udp_socket)), _tcp_socket(std::move(tcp_socket)) {}

        NetworkBase(const std::string& endpoint, size_t port)
            :   _udp_endpoint(udp::v4(), port),
                _tcp_endpoint(tcp::v4(), port),
                _udp_socket(_io_context),
                _tcp_socket(_io_context)
        {
            _udp_socket.open(udp::v4());
            _tcp_socket.open(tcp::v4());


            _nw_thread = std::thread([this](){while (!this->quit) this->_io_context.run();});
        }

        NetworkBase(NetworkMode mode, size_t port)
            :   _udp_endpoint(udp::v4(), port),
                _tcp_endpoint(tcp::v4(), port),
                _udp_socket(_io_context),
                _tcp_socket(_io_context)
        {
            _udp_socket.open(udp::v4());
            _tcp_socket.open(tcp::v4());

            if (mode == NetworkMode::Server) {
                tcp_accept_new_connexion(_io_context);
            }


            _nw_thread = std::thread([this](){while (!this->quit) this->_io_context.run();});
        }

        void readUDP() {
            _udp_socket.async_receive_from(
                asio::buffer(_udp_recv), _udp_endpoint, [this](asio::error_code error, size_t bytes){
                    this->udp_receive(error, bytes, this->_udp_recv, _udp_endpoint);
                    readUDP();
                }
            );
        }

        template<typename Function>
        void writeUDP(const std::vector<uint8_t>& data, Function&& func) {
            _udp_socket.async_send_to(asio::buffer(data), _udp_endpoint, [this](asio::error_code error, size_t bytes) {
                std::cout << "fonction à remplacer" << std::endl;
            });
        }

        virtual void udp_receive(asio::error_code error, size_t bytes, std::array<uint8_t, 512>& buffer, udp::endpoint udp_endpoint) {
            if (!error) {
                std::cout << "revieved " << bytes << " bytes:" << std::endl;
                for (size_t i = 0; i < bytes; ++i) {
                    std:: cout << std::hex << _udp_recv[i];
                }
                std::cout << std::endl << std::endl;
                readUDP();
            }
        }

        template <typename Component>
        void writeTCP(tcp::socket &socket, Component const &component) {
            std::function<void(std::error_code, std::size_t)> handler = [&socket](std::error_code error, std::size_t bytes) {
                if (error) {
                    std::cerr << "Error: Component of the socket " << &socket << " is not sent.\n"
                    << error.message() << std::endl;
                    socket.close();
                }
            };
            socket.async_write_some(asio::buffer(serialization(component), sizeof(std::stringstream)), handler);
        }

        template <typename Component>
        std::tuple<std::string, Component> readTCP(tcp::socket &socket) {
            std::string buffer;
            std::function<void(std::error_code, std::size_t)> handler = [&](std::error_code error, std::size_t bytes) {
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
