/*
** Agartha-Software, 2023
** Cevy
** File description:
** network client
*/

#pragma once

#include "network.hpp"
#include <iostream>
#include <memory>

// class Client : public Networks
// {
//     private:
//         asio::ip::tcp::socket _socket;

//     public:
//         Client(asio::io_service &service, std::string ip = "localhost", std::string port = "8080") : _socket(service) {
//             asio::ip::tcp::resolver resolver(service);
//             asio::ip::tcp::resolver::query query(ip, port);
//             asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
//             asio::connect(_socket, endpoint_iterator);
//         }
//         ~Client() {}

//         void close(std::string str) {
//             std::cout << "Client closed: " << str << std::endl;
//             asio::error_code ec;
//             _socket.shutdown(asio::ip::tcp::socket::shutdown_send, ec);
//             if (ec)
//             {
//                 //FIXME - An error occurred.
//             }
//             _socket.close();
//         }
// };
