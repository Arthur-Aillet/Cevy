#pragma once

#include <cstddef>

class entity {
    private:
        std::size_t _id;
        explicit entity(std::size_t new_id);
    public:
        friend class registery;
        operator std::size_t&();
        operator std::size_t() const;
};
