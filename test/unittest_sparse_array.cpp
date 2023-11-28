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

Test(sparse_array, insert_at) {
    sparse_array<int> test;
    test.insert_at(3, 4);
    cr_assert(test[3] == 4);
    cr_assert(test[2] == std::nullopt);
    test[2] = 3;
    cr_assert(test[2] == 3);
    cr_assert(test[1] == std::nullopt);
    cr_assert(test[0] == std::nullopt);
    int initial = 8;
    test.insert_at(5, initial);
    test[5] = 7;
    cr_assert(test[5] == 7);
    cr_assert(initial == 8);
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
    cr_assert(test.get_index(var).has_value() == false);
    cr_assert(test.get_index(*(test.begin() + 1)).value() == 1);
}

void print_sparse(sparse_array<int> arr) {
    for (auto& elm : arr) {
        std::cout << elm.value_or(-1) << ", ";
    }
    std::cout << std::endl;
}

Test(sparse_array, insert) {
    sparse_array<int> test;
    test.insert_at(2, 3);
    test.insert_at(4, 2);
    test.insert_at(3, 5);
    cr_assert(1 == test.insert(1));
    cr_assert(4 == test.insert(4));
    int pr = test.insert(6).value_or(100);
    cr_assert(6 == pr);
    auto &fst = test.insert(7);
    cr_assert(6 == test.get_index(fst).value());
}

Test(sparse_array, assignation_op) {
    sparse_array<int> test;
    test.insert(1);
    test.insert(2);
    test.insert(3);
    test.insert(4);
    sparse_array<int> test2 = test;
    cr_assert(test[2] == 3);
    test[2] = 8;
    cr_assert(test[2] == 8);
    cr_assert(test2[2] == 3);
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
