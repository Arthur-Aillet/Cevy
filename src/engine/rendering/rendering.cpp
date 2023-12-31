/*
** Agartha-Software, 2023
** Cevy
** File description:
** Rendering
*/

#include "rendering.hpp"
#include "Color.hpp"
#include "raylib.hpp"

using namespace cevy::engine;
using namespace cevy;

void render_models(ecs::Query<option<Position>, option<Rotation>, Handle<engine::Mesh>,
                              option<Handle<Diffuse>>, option<engine::Color>>
                       models) {
  for (auto model : models) {
    const Position &pos = std::get<0>(model).value_or(Position(0., 0., 0.));
    const Rotation &rot = std::get<1>(model).value_or(Rotation(0., 0., 0.));
    auto &handle = std::get<2>(model).get();
    auto &diffuse = std::get<3>(model);
    auto &cevy_color = std::get<4>(model);
    ::Color ray_color;
    if (cevy_color) {
      ray_color = cevy_color.value();
    } else
      ray_color = ::WHITE;
    if (diffuse) {
      SetMaterialTexture(handle.mesh.materials, MATERIAL_MAP_DIFFUSE,
                         diffuse.value().get().texture);
    }
    handle.mesh.transform =
        MatrixRotateXYZ((Vector3){DEG2RAD * rot.x, DEG2RAD * rot.y, DEG2RAD * rot.z});
    DrawModel(handle.mesh, pos, 2, ray_color);
    if (diffuse) {
      handle.mesh.materialCount = 1;
      handle.mesh.materials = (Material *)RL_CALLOC(handle.mesh.materialCount, sizeof(Material));
      handle.mesh.materials[0] = LoadMaterialDefault();
    }
  }
}
