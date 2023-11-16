#include <criterion/criterion.h>

#include "../src/registery.hpp"
#include "../src/sparse_array.hpp"

Test(registering, constructors) {
    registery app;
}

Test(registering, register_comp) {
    registery app;
    app.register_component<int>();
    app.get_components<int>();
}

Test(registering, add_component) {
    registery app;
    app.register_component<int>();
    auto player = app.spawn_entity();
    app.add_component(player, 32);
    app.add_component(player, 32);
}
