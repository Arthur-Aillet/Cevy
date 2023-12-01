//std::unordered_map<std::type_index, component_data> _components_arrays;
#include <vector>

template <class... Containers>
class zipper_iterator {
    template <class Container>
    using iterator_t = typename Container::iterator;

    template <class Container>
    using it_reference_t = typename iterator_t<Container>::reference;

    public:
        // using value_type = std::tuple<Container::value_type&...>;
        // using reference = value_type;
        using pointer = void;
        using difference_type = size_t;
        using iterator_category = std::bidirectional_iterator_tag;
        using iterator_tuple = std::tuple<iterator_t<Container>...>;

        friend containers::zipper<Containers...>;
        zipper_iterator(iterator_tuple const &it_tuple, size_t max);
    public:
        zipper_iterator(zipper_iterator const &z);

        zipper_iterator operator++();
        zipper_iterator &operator++(int);

        value_type operator*();
        value_type operator->();

        friend bool operator==(zipper_iterator const &lhs, zipper_iterator const &rhs);
        friend bool operator!=(zipper_iterator const &lhs, zipper_iterator const &rhs);

    private:
        template <size_t... Is>
        void incr_all(std::index_sequence<Is...>);

        template <size_t... Is>
        bool all_set(std::index_sequence<Is...>);

        template <size_t... Is>
        value_type to_value(std::index_sequence<Is...>);
    private:
        iterator_tuple current;
        size_t _max;
        size_t _idx;

        static constexpr std::index_sequence_for<Containers...> _seq();

};