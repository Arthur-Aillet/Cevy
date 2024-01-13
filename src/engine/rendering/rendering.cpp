/*
** Agartha-Software, 2023
** C++evy
** File description:
** Rendering
*/

#include "rendering.hpp"
#include "Color.hpp"
#include "Line.hpp"
#include "Transform.hpp"
#include "cevy.hpp"
#include "raylib.hpp"
#include <raylib.h>
#include <raymath.h>

using namespace cevy::engine;
using namespace cevy;

void render_lines(ecs::Query<Line, option<cevy::engine::Transform>, option<cevy::engine::Color>> lines) {
  for (auto [line, opt_transform, opt_color] : lines) {
    const cevy::engine::Transform &trans = opt_transform.value_or(cevy::engine::Transform(0., 0., 0.));
    const cevy::engine::Color &col = opt_color.value_or(cevy::engine::Color(0., 255., 60));
    Vector3 end = Vector3RotateByQuaternion(line.end - line.start, trans.rotation);
    end = Vector3Add(end, line.start + trans.position);
    DrawCylinderEx(line.start + trans.position,  end, 0.1, 0.1, 4, (::Color)col);
  }
}

static void render_model(const Model &model, engine::Transform transform) {
  Matrix rot = QuaternionToMatrix(transform.rotation);
  Matrix pos = MatrixTranslate(transform.position.x, transform.position.y, transform.position.z);
  Matrix final = MatrixMultiply(rot, pos);
  for (int i = 0; i < model.meshCount; ++i) {
    DrawMesh(model.meshes[i], model.materials[model.meshMaterial[i]], final);
  }
}

void render_models(ecs::Query<option<engine::Transform>, Handle<engine::Mesh>,
                              option<Handle<Diffuse>>, option<engine::Color>>
                       models) {
  for (auto [opt_tm, mesh, opt_diffuse, opt_color] : models) {
    const cevy::engine::Transform &tm = opt_tm.value_or(cevy::engine::Transform());
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
    render_model(handle.mesh, tm);
    if (opt_diffuse) {
      handle.mesh.materialCount = 1;
      handle.mesh.materials = (Material *)RL_CALLOC(handle.mesh.materialCount, sizeof(Material));
      handle.mesh.materials[0] = LoadMaterialDefault();
    }
  }
}
