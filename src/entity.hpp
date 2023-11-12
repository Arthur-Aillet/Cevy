#include <cstddef>

class Entity {
    private:
        std::size_t id;
    public:
        Entity(std::size_t new_id);
        operator std::size_t() const;
        ~Entity();
};
