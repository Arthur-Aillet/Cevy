//std::unordered_map<std::type_index, component_data> _components_arrays;
#include <vector>
#include <cstddef>
#include <optional>
#include <utility>

// template <class... Containers>
// class zipper;

template <class... Containers>
class zipper_iterator {
        template <class Container>
        using iterator_t = decltype(Container::begin());

        template <class Container>
        using it_reference_t = typename iterator_t<Container>::reference;

    public:
        using value_type = std::tuple<typename Containers::value_type&...>;
        using reference = value_type;
        using pointer = void;
        using difference_type = size_t;
        using iterator_category = std::bidirectional_iterator_tag;
        using iterator_tuple = std::tuple<iterator_t<Containers>...>;

        // friend typename Containers::zipper<Containers...>; // FIXME - reactivate
        zipper_iterator(iterator_tuple const &it_tuple, size_t max) : iterator_tuple(it_tuple), _max(max) {};
    public:
        zipper_iterator(zipper_iterator const &z);

        zipper_iterator operator++() {incr_all();};
        zipper_iterator &operator++(int);

        value_type operator*() {return to_value();};
        value_type operator->();

        friend bool operator==(zipper_iterator const &lhs, zipper_iterator const &rhs);
        friend bool operator!=(zipper_iterator const &lhs, zipper_iterator const &rhs);

    private:
        template <size_t... Is>
        void incr_all(std::index_sequence<Is...>) {
            while (!all_set()) {
                ([&] {
                    (std::get<Is>(current)++);
                } (), ...);
            }
        }

        template <size_t... Is>
        bool all_set(std::index_sequence<Is...>) {
            if (((std::nullopt != (*std::get<Is>(current))) && ...)) {
                return true;
            }
            return false;
        }

        template <size_t... Is>
        value_type to_value(std::index_sequence<Is...>) {
            return {(*std::get<Is>(current))...};
        }
    private:
        iterator_tuple current;
        size_t _max;
        size_t _idx;

        static constexpr std::index_sequence_for<Containers...> _seq();

};

template <class... Containers>
class zipper {
    public:
        using iterator = zipper_iterator<Containers...>;
        using iterator_tuple = typename iterator::iterator_tuple;

        zipper(Containers &...cs);

        iterator begin();
        iterator end();

    private:
        static size_t _compute_size(Containers &... containers);

        static iterator_tuple _compute_end(Containers &... containers);
    private:
        iterator_tuple _begin;
        iterator_tuple _end;
        size_t _size;
};