#pragma once

#include "asio.hpp"
#include <cstdint>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <typeinfo>

#include "../entity.hpp"
#include "../sparse_array.hpp"

/**NOTE - Utiliser des templates pour envoyer les données
 * prend donc n'importe qu'elle type en entré (on doit créer un prototype de la fonction pour chaque type possible)
 * sérializer l'objet avec devant son type (typeid())
 * cela permet de connaitre le type de l'objet a déserializer
 * recevoire l'objet serializer
 * prendre une taille fixe de la string envoyer pour retrouver le type de l'objet
 * deserialzer l'objet dans le type trouvée
*/

class Networks
{
    public:
        enum C {
            State,
            StateRequest,
            StateChange,
            Action,
            ActionSuccess,
            ActionFailure,
        };

    private:
        template <typename Component>
        std::stringstream serialization(sparse_array<Component> components) {
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

    protected:
        template <typename Component>
        void sendComponent(asio::ip::tcp::socket &socket, Component const &component) {
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
        std::tuple<std::string, Component> getComponent(asio::ip::tcp::socket &socket) {
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
