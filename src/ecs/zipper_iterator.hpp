#include <vector>
#include <cstddef>
#include <optional>
#include <utility>

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

        //friend typename Containers::zipper<Containers...>; // FIXME - reactivate
        zipper_iterator(iterator_tuple const &it_tuple, size_t max) : current(it_tuple), _max(max), _idx(0) {};
    public:
        zipper_iterator(zipper_iterator const &z) : current(z.current), _max(z._max), _idx(z._idx) {};

        zipper_iterator operator++() {
            incr_all();
            return this;
        };
        zipper_iterator &operator++(int) {
            auto old = *this;
            incr_all();
            return old;
        };

        value_type operator*() {return to_value();};
        value_type operator->() {return to_value();};

        friend bool operator==(zipper_iterator const &lhs, zipper_iterator const &rhs) {
            return lhs.to_value() == rhs.to_value();
        };
        friend bool operator!=(zipper_iterator const &lhs, zipper_iterator const &rhs) {
            return lhs.to_value() != rhs.to_value();
        };

    private:
        template <size_t... Is>
        void incr_all(std::index_sequence<Is...>) {
            while (!all_set() && _idx <= _max) { // NOTE - check to choose <= or <
                _idx++;
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

        static constexpr std::index_sequence_for<Containers...> _seq() {
            return std::index_sequence_for<Containers...>();
        };

};

template <class... Containers>
class zipper {
    public:
        using iterator = zipper_iterator<Containers...>;
        using iterator_tuple = typename iterator::iterator_tuple;

        zipper(Containers &...cs) : _begin(zipper_iterator((cs.begin(), cs.size())...)), _end(zipper_iterator((cs.end(), cs.size())...)), _size(_compute_size(std::forward(cs...))) {};

        iterator begin() { return _begin; };
        iterator end() { return _end; };

    private:
        static size_t _compute_size(Containers &... containers) {

        }

        static iterator_tuple _compute_end(Containers &... containers);
    private:
        iterator_tuple _begin;
        iterator_tuple _end;
        size_t _size;
};
