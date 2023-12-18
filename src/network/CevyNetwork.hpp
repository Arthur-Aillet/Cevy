/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CevyNetwork
*/

#pragma once

#include "NetworkBase.hpp"
#include <asio/error_code.hpp>
#include <cstdint>
#include <cstring>
#include <list>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <vector>

class CevyNetwork : protected NetworkBase {
    public:
        enum class Communication {
            State          = 1,
            StateRequest   = 2,
            Event          = 3,
            Action         = 4,
            ActionSuccess  = 5,
            ActionFailure  = 6,
        };
        enum class Event {
            Summon,
        };

        CevyNetwork();
        ~CevyNetwork();

        template<typename T>
        static uint8_t byte(const T& t, size_t n) {
            const T* tp = &t;
            const uint8_t* p = (uint8_t*)tp;

            return p[n];
        };

        void sendState(uint16_t id, const std::vector<uint8_t>& block) {
            std::vector<uint8_t> fullblock = {uint8_t(Communication::State), byte(id, 0), byte(id, 1)};
            fullblock.insert(fullblock.end(), block.begin(), block.end());
            _states_send[id] = block;
            writeUDP(_states_send[id], [this, id](){_states_send.erase(id);});
        }

        std::optional<std::vector<uint8_t>> recvState(uint16_t id) {
            const auto& it = _states_recv.find(id);
            if (it != _states_recv.end()) {
                auto ret = it->second;
                _states_recv.erase(id);
                return std::move(ret);
            }
            return std::nullopt;
        }

    private:
        void handle_state(size_t bytes, std::array<uint8_t, 512>& buffer) {
            uint64_t id;
            std::memcpy(&id, &buffer[1], sizeof(id));
            std::vector<uint8_t> vec;
            vec.insert(vec.begin(), buffer.begin() + 2, buffer.begin() + bytes - 3);
            _states_recv[id] = vec;
        }
        using error_code = asio::error_code;

        void handle_events(size_t bytes, std::array<uint8_t, 512>& buffer) {
            // if ((half){.b.b0 = buffer[0], .b.b1 = buffer[1]}.h == Event::Summon)
            uint64_t id;
            std::memcpy(&id, &buffer[1], sizeof(id));
            std::vector<uint8_t> vec;
            vec.insert(vec.begin(), buffer.begin() + 2, buffer.begin() + bytes - 3);
            _states_recv[id] = vec;
        }

    protected:

        void udp_receive(std::error_code error, size_t bytes, std::array<uint8_t, 512>& buffer) override
        {
            if (bytes < 0)
                return;
            if (buffer[0] == (uint8_t)Communication::State) {
                return handle_state(bytes, buffer);
            }
            if (buffer[0] == (uint8_t)Communication::Event) {
                return handle_events(bytes, buffer);
            }
        }

    private:
        std::unordered_map<uint16_t, std::vector<uint8_t>> _states_recv;
        std::unordered_map<uint16_t, std::vector<uint8_t>> _states_send;

};
