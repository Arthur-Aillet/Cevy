#include <criterion/criterion.h>

#include "../src/SparseVector.hpp"

Test(SparseVector, constructors) {
    SparseVector<int> test = SparseVector<int>();
    SparseVector<int> copy(test);
    SparseVector<int> snd = std::move(test);
    SparseVector<int> third;
    SparseVector<int> fourth;
    third = test;
    fourth = std::move(test);
}

Test(SparseVector, size) {
    SparseVector<int> test;
    cr_assert(test.size() == 0);
    test.resize(4);
    cr_assert(test.size() == 4);
}

Test(SparseVector, insert_at) {
    SparseVector<int> test;
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

Test(SparseVector, erase) {
    SparseVector<int> test;
    test.insert_at(3, 4);
    cr_assert(test[3] == 4);
    test.erase(3);
    cr_assert(test[3] == std::nullopt);
}

Test(SparseVector, get_index) {
    SparseVector<int> test;
    int var = 3;
    test.insert_at(2, var);
    cr_assert(test[2] == 3);
    cr_assert(test.get_index(var).has_value() == false);
    cr_assert(test.get_index(*(test.begin() + 1)).value() == 1);
}

void print_sparse(SparseVector<int> arr) {
    for (auto& elm : arr) {
        std::cout << elm.value_or(-1) << ", ";
    }
    std::cout << std::endl;
}

Test(SparseVector, insert) {
    SparseVector<int> test;
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

Test(SparseVector, assignation_op) {
    SparseVector<int> test;
    test.insert(1);
    test.insert(2);
    test.insert(3);
    test.insert(4);
    SparseVector<int> test2 = test;
    cr_assert(test[2] == 3);
    test[2] = 8;
    cr_assert(test[2] == 8);
    cr_assert(test2[2] == 3);
}

/*
Test(SparseVector, get) {
    SparseVector<int> test;
    test[1] = 3;
    cr_assert(test[1] == 3);
}

Test(SparseVector, begin) {
    SparseVector<int> test;
    test.begin();
    test.cbegin();
}

Test(SparseVector, end) {
    SparseVector<int> test;
    test.end();
    test.cend();
}

*/
