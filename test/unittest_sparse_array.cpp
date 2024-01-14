#include <criterion/criterion.h>

#include "SparseVector.hpp"

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
    test.resize(3);
    cr_assert(test.size() == 3);
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
    cr_assert(test[5] == 8);
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

Test(SparseVector, iterator_operations) {
    SparseVector<int> test;
    test.insert_at(0, 1);
    test.insert_at(2, 3);
    test.insert_at(4, 5);

    auto it = test.begin();
    cr_assert(it == test.begin());
    cr_assert(*it == 1);
    ++it;
    cr_assert(*it == std::nullopt);
    it += 3;
    cr_assert(*it == 5);
    auto it2 = it - 4;
    cr_assert(it2 == test.begin());
    cr_assert(it > it2);
}
