/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** Action.hpp
*/

#include "network.hpp"
#include "NetworkBase.hpp"
#include "CevyNetwork.hpp"
#include "Synchroniser.hpp"

class NetworkActions {
    public:

    enum ClientActions {
        CONNECTION,
        CHOOSE_SKINE,
        LOBBY_CONNECTION,
        MOVE,
    };

    enum ClientActionResponses {
        ACTION_SUCCESS,
        ACTION_FAILURE,
    };

    enum ActionFailureValue {
        ACTION_UNAVAILABLE,
        ACTION_DISABLED,
        ACTION_ERROR,
        ACTION_WAITING,
        ACTION_DELAYED,
        BUSY
    };
};