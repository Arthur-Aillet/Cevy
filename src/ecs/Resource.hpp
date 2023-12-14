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
class Resource {
    private:
        Content &_content;
        friend class ResourceManager;
        Resource(Content& content) : _content(content) {};
    public:
        operator Content() { return _content; }
};

class ResourceManager {
    private:
        using ressource_type = std::any;

        std::unordered_map<std::type_index, ressource_type> _ressources_map;
    public:
        template <typename Content>
        void insert_resource(Content &value) {
            std::any a = std::make_any<Content>(value);

            _ressources_map.insert({std::type_index(typeid(Content)), a});
        }

        template <typename Content>
        std::optional<Content> remove_resource() {
            //_ressources_map[std::type_index(typeid(Content))];
        }

        template <typename Content>
        Resource<Content>& get_resource() {
            return std::any_cast<Resource<Content>>(_ressources_map[std::type_index(typeid(Content))]);

        }
};
