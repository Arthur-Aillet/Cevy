#include <criterion/criterion.h>

#include "../public/entity.hpp"
#include "../src/sparse_array.hpp"

Test(entity, convert_to_sizet) {
    entity test = entity(4);
    cr_assert(4 == test);
}

Test(sparse_array, constructers) {
    sparse_array<int> test = sparse_array<int>();
    sparse_array<int> copy(test);
    sparse_array<int> snd = std::move(test);
}
