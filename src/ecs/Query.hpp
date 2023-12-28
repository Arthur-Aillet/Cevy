/*
** Agartha-Software, 2023
** Cevy
** File description:
** Queries
*/

#pragma once

#include <cstddef>
#include <optional>
#include <utility>

#include "SparseVector.hpp"
#include "ecs.hpp"

template <class T> struct is_query : public std::false_type {};

template <typename... T> struct is_query<cevy::ecs::Query<T...>> : public std::true_type {};

template <typename Type>
struct is_optional : std::false_type {};

template <typename Type>
struct is_optional<std::optional<Type>> : std::true_type {};

template<bool, template<class...>class, class, class Else>
struct eval_cond {
  using type=Else;
};

template<template<class...>class Z, class X, class Else>
struct eval_cond<true,Z,X,Else> {
  using type=Z<X>;
};

template<bool test, template<class...>class Z, class X, class Else>
using eval_cond_t = typename eval_cond<test,Z,X,Else>::type;

template<class X>
using inner_optional=typename X::value_type;

template<typename Type>
using remove_optional = eval_cond_t<is_optional<Type>::value, inner_optional, Type, Type>;

template <class... T> class cevy::ecs::Query {
  using Containers = std::tuple<SparseVector<remove_optional<T>>...>;

  public:
  class iterator {
    template <class Container>
    using iterator_t = typename Container::iterator;

    template <class Container>
    using it_reference_t = typename iterator_t<Container>::reference;

    public:
    using value_type = std::tuple<T&...>;
    using reference = value_type;
    using pointer = void;
    using difference_type = size_t;
    using iterator_category = std::bidirectional_iterator_tag;
    using iterator_tuple = std::tuple<iterator_t<SparseVector<remove_optional<T>>>...>;

    iterator(iterator_tuple const &it_tuple, size_t max, size_t idx = 0)
        : current(it_tuple), _max(max), _idx(idx) {
      sync();
    };

    public:
    iterator(iterator const &z) : current(z.current), _max(z._max), _idx(z._idx){};

    iterator operator++() {
      incr_all();
      return *this;
    };
    iterator &operator++(int) {
      auto old = *this;
      incr_all();
      return old;
    };

    value_type operator*() { return to_value(); };
    value_type operator->() { return to_value(); };

    friend bool operator==(iterator const &lhs, iterator const &rhs) {
      return lhs._idx == rhs._idx;
    };
    friend bool operator!=(iterator const &lhs, iterator const &rhs) {
      return lhs._idx != rhs._idx;
    };

    private:
    void incr_all() {
      if (_idx == _max)
        return;
      std::cout << "Here! 1" << std::endl;
      while (_idx < _max && !all_set()) {// NOTE - check to choose <= or <
        _idx++;
        ((std::get<iterator_t<SparseVector<remove_optional<T>>>>(current)++), ...);
      }
    }

    void sync() {
      if (_idx == _max)
        return;
      std::cout << "Here! 2" << std::endl;
      while (_idx < _max && !all_set()) { // NOTE - check to choose <= or <
        _idx++;
        ([&] { (std::get<iterator_t<SparseVector<remove_optional<T>>>>(current)++); }(), ...);
      }
    }

    template <typename Current>
    bool is_set() {
      if constexpr (is_optional<Current>::value) {
        return true;
      } else {
        return std::nullopt != (*std::get<iterator_t<SparseVector<remove_optional<Current>>>>(current));
      }
    }

    bool all_set() {
      return (is_set<T>() && ...);
    }

    template<typename Current>
    Current& a_value() {
      if constexpr (is_optional<Current>::value) {
        return std::get<iterator_t<SparseVector<typename Current::value_type>>>(current);
      } else {
        return std::get<iterator_t<SparseVector<Current>>>(current)->value();
      }
    }

    const value_type to_value() {
      return std::tuple<T&...>{a_value<T>()...};
    }

    private:
    iterator_tuple current;
    size_t _max;
    size_t _idx;
    // auto all = std::make_index_sequence(sizeof...(T));
  };
  using iterator_tuple = typename iterator::iterator_tuple;

  static Query<T...> query(World &);

  Query(SparseVector<remove_optional<T>> &...cs)
      : _size(_compute_size(cs...)), _begin(iterator(std::make_tuple(cs.begin()...), _size)),
        _end(iterator(std::make_tuple(cs.end()...), _size, _size)){
      std::cout << "Here! 4" << std::endl;

        };

  iterator begin() { return _begin; };
  iterator end() { return _end; };

  const iterator begin() const { return _begin; };
  const iterator end() const { return _end; };

  private:
  static size_t _compute_size(SparseVector<remove_optional<T>> &...containers) {
    return std::min({containers.size()...});
  }

  // static iterator_tuple _compute_end(Containers &... containers);
  private:
  public:
  size_t _size;
  iterator _begin;
  iterator _end;
};
