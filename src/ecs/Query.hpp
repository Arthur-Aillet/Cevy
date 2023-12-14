/*
** Agartha-Software, 2023
** Cevy
** File description:
** Queries
*/

#pragma once

#include <vector>
#include <cstddef>
#include <optional>
#include <utility>
#include <iostream>

#include "ecs.hpp"
#include "SparseVector.hpp"

template<class T>
struct is_query : public std::false_type {};

template<typename... T>
struct is_query<cevy::ecs::Query<T...>> : public std::true_type {};

template <class... T>
class cevy::ecs::Query {
    using Containers = std::tuple<SparseVector<T>...>;
    // template<template<typename> typename Containers>
    // using Containers = SparseVector<T>;
    public:
        class iterator {
                template<class Container>
                using iterator_t = typename Container::iterator;

                template <class Container>
                using it_reference_t = typename iterator_t<Container>::reference;

            public:
                using value_type = std::tuple<T&...>;
                using reference = value_type;
                using pointer = void;
                using difference_type = size_t;
                using iterator_category = std::bidirectional_iterator_tag;
                using iterator_tuple = std::tuple<iterator_t<SparseVector<T>>...>;

                template<typename... >
                class zipper;
                friend class zipper<T...>; // FIXME - reactivate
                iterator(iterator_tuple const &it_tuple, size_t max, size_t idx = 0) : current(it_tuple), _max(max), _idx(idx) { sync(); };
            public:
                iterator(iterator const &z) : current(z.current), _max(z._max), _idx(z._idx) {};

                iterator operator++() {
                    incr_all();
                    return *this;
                };
                iterator &operator++(int) {
                    auto old = *this;
                    incr_all();
                    return old;
                };

                value_type operator*() {return to_value();};
                value_type operator->() {return to_value();};

                friend bool operator==(iterator const &lhs, iterator const &rhs) {
                    return lhs._idx == rhs._idx;
                };
                friend bool operator!=(iterator const &lhs, iterator const &rhs) {
                    std::cerr << lhs._idx << "," << rhs._idx << std::endl;
                    return lhs._idx != rhs._idx;
                };

            private:
                static constexpr  std::index_sequence_for<T...>_seq() {
                    return std::index_sequence_for<T...>();
                };
                // template <size_t... Is>
                void incr_all( /* std::index_sequence<Is...> */ ) {
                    if (_idx == _max)
                        return;
                    do {
                        _idx++;
                        ([&] {
                            (std::get<iterator_t<SparseVector<T>>>(current)++);
                        } (), ...);
                    } while (!all_set() && _idx < _max); // NOTE - check to choose <= or <
                }

                void sync() {
                    if (_idx == _max)
                        return;
                    while (!all_set() && _idx < _max) { // NOTE - check to choose <= or <
                        _idx++;
                        ([&] {
                            (std::get<iterator_t<SparseVector<T>>>(current)++);
                        } (), ...);
                    }
                }

                // template <size_t... Is>
                bool all_set( /* std::index_sequence<Is...> */ ) {
                    if (((std::nullopt == (*std::get<iterator_t<SparseVector<T>>>(current))) && ...)) {
                        return false;
                    }
                    return true;
                }

                // template <size_t... Is>
                const value_type to_value( /* std::index_sequence<Is...> */ ) {
                    return std::tuple<T&...>{std::get<iterator_t<SparseVector<T>>>(current)->value() ...};
                }
            private:
            public:
                iterator_tuple current;
                size_t _max;
                size_t _idx;
                // auto all = std::make_index_sequence(sizeof...(T));

        };
        using iterator_tuple = typename iterator::iterator_tuple;

        static Query query(World&);

        Query(SparseVector<T> &...cs) : _size(_compute_size(cs...)), _begin(iterator(std::make_tuple(cs.begin()...), _size)), _end(iterator(std::make_tuple(cs.end()...), _size, _size)) {};

        iterator begin() { return _begin; };
        iterator end() { return _end; };

        const iterator begin() const { return _begin; };
        const iterator end() const { return _end; };

    private:
        static size_t _compute_size(SparseVector<T> &... containers) {
            return std::min({containers.size()...});
        }

        // static iterator_tuple _compute_end(Containers &... containers);
    private:
    public:
        size_t _size;
        iterator _begin;
        iterator _end;
};
