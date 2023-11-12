#include <criterion/criterion.h>

#include "../public/entity.hpp"

Test(entity, convert_to_sizet) {
    Entity test = Entity(4);
    cr_assert(4 == test);
}
