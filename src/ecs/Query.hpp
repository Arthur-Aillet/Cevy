/*
** Agartha-Software, 2023
** C++evy
** File description:
** Queries
*/

#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cstddef>
#include <iterator>
#include <optional>
#include <type_traits>

#include "Entity.hpp"
#include "SparseVector.hpp"
#include "cevy.hpp"
#include "ecs.hpp"

namespace cevy::ecs {

template <class T>
struct is_query : public std::false_type {};

template <typename... T>
struct is_query<cevy::ecs::Query<T...>> : public std::true_type {};

template <typename Type>
struct is_optional : std::false_type {};

template <typename Type>
struct is_optional<std::optional<Type>> : std::true_type {};

template <class X>
using inner_optional = typename X::value_type;

template <typename Type>
using remove_optional = eval_cond_t<is_optional<Type>::value, inner_optional, Type, Type>;

template <class... T>
class Query {
  using Containers = std::tuple<SparseVector<remove_optional<T>>...>;

  public:
  class iterator {
    template <class Container>
    using iterator_t = typename Container::iterator;

    template <class Container>
    using it_reference_t = typename iterator_t<Container>::reference;

    friend class Entity;

    public:
    using value_type = std::tuple<T &...>;
    using reference = value_type;
    using pointer = void;
    using difference_type = size_t;
    using iterator_category = std::forward_iterator_tag;
    using iterator_tuple = std::tuple<iterator_t<SparseVector<remove_optional<T>>>...>;

    iterator(iterator_tuple const &it_tuple, size_t max, size_t idx = 0)
        : current(it_tuple), _max(max), _idx(idx), _entity(_idx) {
      sync();
    };

    public:
    iterator(iterator const &z) : current(z.current), _max(z._max), _idx(z._idx), _entity(_idx) {};

    iterator operator++() {
      incr_all();
      return *this;
    };
    iterator operator++(int) {
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
      do {
        _idx++;
        ((std::get<iterator_t<SparseVector<remove_optional<T>>>>(current)++), ...);
      } while (_idx < _max && !all_set()); // NOTE - check to choose <= or <
    }

    void sync() {
      if (_idx == _max)
        return;
      while (_idx < _max && !all_set()) { // NOTE - check to choose <= or <
        _idx++;
        ((std::get<iterator_t<SparseVector<remove_optional<T>>>>(current)++), ...);
      }
    }

    template <typename Current>
    bool is_set() {
      if constexpr (is_optional<Current>::value) {
        return true;
      } else {
        return std::get<iterator_t<SparseVector<Current>>>(current)->has_value();
      }
    }

    bool all_set() { return (is_set<T>() && ...); }

    template <typename Current>
    Current &a_value() {
      if constexpr (std::is_same<Current, Entity>::value) {
        return _entity;
      } else if constexpr (is_optional<Current>::value) {
        return *std::get<iterator_t<SparseVector<typename Current::value_type>>>(current);
      } else {
        return std::get<iterator_t<SparseVector<Current>>>(current)->value();
      }
    }

    const value_type to_value() { return value_type{a_value<T>()...}; }

    private:
    iterator_tuple current;
    size_t _max;
    size_t _idx;
    Entity _entity;
  };
  using iterator_tuple = typename iterator::iterator_tuple;

  static Query<T...> query(World &);

  template <typename Current>
  static void resize_optional(SparseVector<remove_optional<Current>> &c, size_t n) {
    if constexpr (is_optional<Current>::value) {
      c.resize(std::max(c.size(), n + 1));
    }
  }

  Query(size_t nb_e, SparseVector<remove_optional<T>> &...cs)
      : _size(_compute_size(nb_e, cs...)), _begin(iterator(std::make_tuple(cs.begin()...), _size)),
        _end(iterator(std::make_tuple(cs.end()...), _size, _size)){};

  iterator begin() { return _begin; };
  iterator end() { return _end; };

  const iterator begin() const { return _begin; };
  const iterator end() const { return _end; };

  private:
  template <typename Current>
  static void _compute_a_size(SparseVector<Current> &container, size_t &current_size,
                              bool &is_first, size_t &idx, std::bitset<sizeof...(T)> &opts) {
    if (is_first) {
      is_first = false;
      current_size = container.size();
    } else if (!opts[idx])
      current_size = std::min(current_size, container.size());
    idx += 1;
  }

  static size_t _compute_size(size_t nb_e, SparseVector<remove_optional<T>> &...containers) {
    size_t current_size = 0;
    if ((... && is_optional<T>::value)) {
      current_size = nb_e;
    } else {
      std::bitset<sizeof...(T)> are_optional;
      size_t idx = 0;
      bool is_first = true;

      (are_optional.set(idx++, is_optional<T>::value), ...);
      idx = 0;
      (_compute_a_size(containers, current_size, is_first, idx, are_optional), ...);
    }
    (resize_optional<T>(containers, current_size), ...);
    return current_size;
  }

  public:
  size_t size() { return _size; }

  typename iterator::value_type single() { return *begin(); }

  std::optional<typename iterator::value_type> get_single() {
    if (_size <= 0) {
      return std::nullopt;
    }
    return single();
  }

  private:
  template <size_t N>
  typename iterator::value_type progress_it(iterator &it) {
    auto last = it;
    it++;
    return *last;
  }

  template <size_t N, size_t... I>
  std::array<typename iterator::value_type, N> multiple_impl(std::index_sequence<I...>) {
    auto it = begin();
    return {progress_it<I>(it)...};
  }

  public:
  template <size_t N, typename Indicies = std::make_index_sequence<N>>
  std::array<typename iterator::value_type, N> multiple() {
    return multiple_impl<N>(Indicies{});
  }

  template <size_t N>
  std::optional<std::array<typename iterator::value_type, N>> get_multiple() {
    if (_size < N) {
      return std::nullopt;
    }
    return multiple<N>();
  }

  private:
  size_t _size;
  iterator _begin;
  iterator _end;
};
} // namespace cevy::ecs
