#pragma once

#include <cstddef>
#include <string>
#include <array>
#include <typeinfo>

// template<size_t N>
// constexpr size_t const_hash(const std::array<char, N> s) {
//     static_assert(N < 64);
//     size_t hash = 0;
//     for (auto c : s)
//         hash = hash * 256 + int(c);
//     return hash;
// }

template<size_t N>
constexpr size_t const_hash(const char s[N]) {
    static_assert(N < 64);
    size_t hash = 0;
    for (int i = 0; i < N; ++i)
        hash = hash * 256 + int(s[i]);
    return hash;
}

// template<size_t N>
constexpr size_t const_hash2(const char* s) {
    size_t hash = 0;
    for (auto i = s; *i ; ++i) {
        hash = hash * 256 + *i;
        // if (s - i > 64)
            // assert(false, "string must be shorter than 64 bytes");
    }
    return hash;
}

#define ENUM_FROM(x) (inline static const size_t x = const_hash(std::array(#x)))

#define hash_name(x) (const_hash(#x))


// template<typename... Args>
struct Enum {
public:
    size_t dbg() {return value;};
    Enum(size_t v) : value(v) {};
    template<typename T>
    static Enum ctor() { T a; Enum e = Enum(const_hash2(typeid(T).name())); return e;};
    template<typename T>
    Enum() : value(const_hash2(typeid(T).name())) {};
protected:
    size_t value = 666;
    // template<T = Args...>
    // ENUM_FROM(T)...;

};
