#pragma once

#include <cstddef>

class entity {
    private:
        std::size_t _id;
    public:
        explicit entity(std::size_t new_id);

        void display();
        operator std::size_t&();
        operator std::size_t() const;
};
