#include <criterion/criterion.h>

#include "../includes/entity.hpp"

Test(entity, convert_to_sizet) {
    Entity test = Entity(4);
    test.display();
    cr_assert(4 == test);
}
