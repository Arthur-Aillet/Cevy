#pragma once

#include <cstddef>

class Entity {
    private:
        std::size_t _id;
        explicit Entity(std::size_t new_id);
    public:
        friend class World;
        operator std::size_t&();
        operator std::size_t() const;
};
