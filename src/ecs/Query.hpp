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

template<typename Type>
using remove_optional = std::conditional_t<is_optional<Type>::value, typename Type::value_type, Type>;

template <class... T> class cevy::ecs::Query {
  using Containers = std::tuple<SparseVector<remove_optional<T>>...>;
  // template<template<typename> typename Containers>
  // using Containers = SparseVector<T>;
  public:
  class iterator {
    template <class Container> using iterator_t = typename Container::iterator;

    template <class Container> using it_reference_t = typename iterator_t<Container>::reference;

    public:
    using value_type = std::tuple<remove_optional<T> &...>;
    using reference = value_type;
    using pointer = void;
    using difference_type = size_t;
    using iterator_category = std::bidirectional_iterator_tag;
    using iterator_tuple = std::tuple<iterator_t<SparseVector<remove_optional<T>>>...>;

    template <typename...> class zipper;
    friend class zipper<T...>; // FIXME - reactivate
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
    static constexpr std::index_sequence_for<T...> _seq() {
      std::cout << "Here! 5" << std::endl;
      return std::index_sequence_for<T...>();
    };
    // template <size_t... Is>
    void incr_all(/* std::index_sequence<Is...> */) {
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

    // template <size_t... Is>
    bool all_set(/* std::index_sequence<Is...> */) {
      // TODO: Static Assert
      std::cout << "Here! 3" << std::endl;
      return ((std::nullopt != (*std::get<iterator_t<SparseVector<remove_optional<T>>>>(current))) && ...);
    }

    // template <size_t... Is>
    const value_type to_value(/* std::index_sequence<Is...> */) {
      return std::tuple<remove_optional<T> &...>{std::get<iterator_t<SparseVector<remove_optional<T>>>>(current)->value()...};
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
