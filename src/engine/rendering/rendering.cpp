/*
** Agartha-Software, 2023
** Cevy
** File description:
** Rendering
*/

#include "rendering.hpp"
#include "Color.hpp"
#include "Line.hpp"
#include "Rotation.hpp"
#include "cevy.hpp"
#include "raylib.hpp"

using namespace cevy::engine;
using namespace cevy;

void render_lines(
    ecs::Query<option<Position>, Line, option<cevy::engine::Color>> lines) {
  for (auto [opt_pos, lines, opt_color] : lines) {
    const Position &pos = opt_pos.value_or(Position(0., 0., 0.));
    const cevy::engine::Color &col = opt_color.value_or(cevy::engine::Color(0., 255., 60));

    DrawCylinderEx(lines.start + pos, lines.end + pos, 0.1, 0.1, 4, (::Color)col);
  }
}

void render_models(ecs::Query<option<Position>, option<Rotation>, Handle<engine::Mesh>,
                              option<Handle<Diffuse>>, option<engine::Color>>
                       models) {
  for (auto [opt_pos, opt_rot, mesh, opt_diffuse, opt_color] : models) {
    const Position &pos = opt_pos.value_or(Position(0., 0., 0.));
    const Rotation &rot = opt_rot.value_or(Rotation(0., 0., 0.));
    auto &handle = mesh.get();
    ::Color ray_color;
    if (opt_color) {
      ray_color = opt_color.value();
    } else
      ray_color = ::WHITE;
    if (opt_diffuse) {
      SetMaterialTexture(handle.mesh.materials, MATERIAL_MAP_DIFFUSE,
                         opt_diffuse.value().get().texture);
    }
    handle.mesh.transform =
        MatrixRotateXYZ((Vector3){DEG2RAD * rot.x, DEG2RAD * rot.y, DEG2RAD * rot.z});
    DrawModel(handle.mesh, pos, 2, ray_color);
    if (opt_diffuse) {
      handle.mesh.materialCount = 1;
      handle.mesh.materials = (Material *)RL_CALLOC(handle.mesh.materialCount, sizeof(Material));
      handle.mesh.materials[0] = LoadMaterialDefault();
    }
  }
}
