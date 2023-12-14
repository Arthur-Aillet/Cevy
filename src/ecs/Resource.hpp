/*
** Agartha-Software, 2023
** Cevy
** File description:
** Queries
*/

#include <any>
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

        std::unordered_map<std::type_index, ressource_type> _ressources_arrays;
    public:
        template <typename Content>
        void insert_resource(Content value) {
            std::any a = std::make_any<Content>(value);

            _ressources_arrays.insert({std::type_index(typeid(Content)), a});
        }

        template <typename Content>
        Resource<Content>& get_resource() {
            return std::any_cast<Resource<Content>>(_ressources_arrays[std::type_index(typeid(Content))]);

        }
};
