#include <criterion/criterion.h>

#include "../public/entity.hpp"

Test(entity, convert_to_sizet) {
    entity test = entity(4);
    cr_assert(4 == test);
}
