/*
** Agartha-Software, 2023
** Cevy
** File description:
** network
*/

#include "./network.hpp"

int main(int ac, char **av) {
    if (ac < 2)
        return 84;
    if (std::string(av[1]) == "server")
        Network::start_server();
    else if (std::string(av[1]) == "client")
        Network::start_client("localhost");
    else
        return 84;
    return 0;
}
