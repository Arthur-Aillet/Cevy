#include <criterion/criterion.h>

#include "../src/sparse_array.hpp"

Test(sparse_array, constructors) {
    sparse_array<int> test = sparse_array<int>();
    sparse_array<int> copy(test);
    sparse_array<int> snd = std::move(test);
    sparse_array<int> third;
    sparse_array<int> fourth;
    third = test;
    fourth = std::move(test);
}

Test(sparse_array, size) {
    sparse_array<int> test;
    cr_assert(test.size() == 0);
    test.resize(4);
    cr_assert(test.size() == 4);
}

Test(sparse_array, insert) {
    sparse_array<int> test;
    test.insert_at(3, 4);
    cr_assert(test[3] == 4);
    cr_assert(test[2] == std::nullopt);
    test[2] = 3;
    cr_assert(test[2] == 3);
    cr_assert(test[1] == std::nullopt);
    cr_assert(test[0] == std::nullopt);
}

Test(sparse_array, erase) {
    sparse_array<int> test;
    test.insert_at(3, 4);
    cr_assert(test[3] == 4);
    test.erase(3);
    cr_assert(test[3] == std::nullopt);
}

Test(sparse_array, get_index) {
    sparse_array<int> test;
    int var = 3;
    test.insert_at(2, var);
    cr_assert(test[2] == 3);
    cr_assert(test.get_index(var) == -1);
    cr_assert(test.get_index(*(test.begin() + 1)) == 1);
}

/*
Test(sparse_array, get) {
    sparse_array<int> test;
    test[1] = 3;
    cr_assert(test[1] == 3);
}

Test(sparse_array, begin) {
    sparse_array<int> test;
    test.begin();
    test.cbegin();
}

Test(sparse_array, end) {
    sparse_array<int> test;
    test.end();
    test.cend();
}

*/
