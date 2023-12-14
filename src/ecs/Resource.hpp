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
        using resource_type = std::any;

        std::unordered_map<std::type_index, resource_type> _resources_map;
    public:
        template <typename Content>
        void insert_resource(Content &value) {
            std::any a = std::make_any<Content>(value);

            _resources_map[std::type_index(typeid(Content))] = a;
        }

        template <typename Content>
        std::optional<Content> remove_resource() {
            auto it = _resources_map.find(std::type_index(typeid(Content)));

            if (it != _resources_map.end()) {
                Content val = std::any_cast<Content>(_resources_map[std::type_index(typeid(Content))]);
                _resources_map.erase(it);
                return std::optional<Content>(val);
            }
            return std::nullopt;
        }

        template <typename Content>
        Content& resource() {
            return std::any_cast<Content&>(_resources_map[std::type_index(typeid(Content))]);
        }

        template <typename Content>
        const Content& resource() {
            return std::any_cast<const Content&>(_resources_map[std::type_index(typeid(Content))]);
        }

        template <typename Content>
        std::optional<std::reference_wrapper<Content>> get_resource() {
            auto it = _resources_map.find(std::type_index(typeid(Content)));

            if (it != _resources_map.end()) {
                return std::any_cast<Content&>(_resources_map[std::type_index(typeid(Content))]);
            }
            return std::nullopt;
        }

        template <typename Content>
        std::optional<std::reference_wrapper<const Content>>& get_resource() {
            auto it = _resources_map.find(std::type_index(typeid(Content)));

            if (it != _resources_map.end()) {
                return std::any_cast<const Content&>(_resources_map[std::type_index(typeid(Content))]);
            }
            return std::nullopt;
        }

        template <typename Content>
        Resource<Content> get() {
            return Resource(std::any_cast<Content&>(_resources_map[std::type_index(typeid(Content))]));
        }

        template<typename Content>
        bool contains_resource() {
            return _resources_map.find(std::type_index(typeid(Content))) != _resources_map.end();
        }
};
