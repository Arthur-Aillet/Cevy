#include <criterion/criterion.h>

#include "../includes/test.hpp"

Test(misc, passing) {
    Entity test = Entity(3);
    cr_assert(3 == test);
}
