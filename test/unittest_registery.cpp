#include <criterion/criterion.h>

#include "../src/registery.hpp"
#include "../src/sparse_array.hpp"

Test(registering, constructors) {
    registery test;
    test.register_component<int>();
    test.get_components<int>();
    test.spawn_entity();
    test.kill_entity(test.entity_from_index(3));
}
