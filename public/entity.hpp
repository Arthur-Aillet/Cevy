#pragma once

class entity {
    public:
        explicit entity(std::size_t new_id);
        void display();

        operator std::size_t() const;
        operator std::size_t&();
};
