/*
** Agartha-Software, 2023
** Cevy
** File description:
** network
*/

#include "./NetworkBase.hpp"

int main(int ac, char **av) {
    if (ac < 2)
        return 84;
    if (std::string(av[1]) == "server")
        NetworkBase::start_server();
    else if (std::string(av[1]) == "client")
        NetworkBase::start_client("localhost");
    else
        return 84;
    return 0;
}
