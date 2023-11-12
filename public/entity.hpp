#pragma once

class Entity {
    public:
        explicit Entity(std::size_t new_id);
        void display();

        operator std::size_t() const;
        operator std::size_t&();
};
