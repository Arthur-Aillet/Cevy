#include <cstddef>

class Entity {
    private:
        std::size_t id;
    public:
        explicit Entity(std::size_t new_id);
        ~Entity();

        void display();
        operator std::size_t&();
        operator std::size_t() const;
};
