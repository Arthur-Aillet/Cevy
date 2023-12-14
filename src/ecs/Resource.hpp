/*
** Agartha-Software, 2023
** Cevy
** File description:
** Queries
*/

#include <any>
#include <optional>
#include <typeindex>
#include <unordered_map>

template <typename Content>
class Resource;

template<class T>
struct is_resource : public std::false_type {};

template<typename T>
struct is_resource<Resource<T>> : public std::true_type {};

template <typename Content>
class Resource {
    private:
        Content &_content;
        friend class ResourceManager;
        Resource(Content& content) : _content(content) {};
    public:
        using value = Content;
        operator Content () const { return _content; }
        operator Content&() { return _content; }
};

class ResourceManager {
    private:
        using ressource_type = std::any;

        std::unordered_map<std::type_index, ressource_type> _ressources_map;
    public:
        template <typename Content>
        void insert_resource(Content &value) {
            std::any a = std::make_any<Content>(value);

            _ressources_map[std::type_index(typeid(Content))] = a;
        }

        template <typename Content>
        std::optional<Content> remove_resource() {
            auto it = _ressources_map.find(std::type_index(typeid(Content)));

            if (it != _ressources_map.end()) {
                Content val = std::any_cast<Content>(_ressources_map[std::type_index(typeid(Content))]);
                _ressources_map.erase(it);
                return std::optional<Content>(val);
            }
            return std::nullopt;
        }

        template <typename Content>
        Resource<Content> get() {
            return Resource(std::any_cast<Content&>(_ressources_map[std::type_index(typeid(Content))]));
        }
};
