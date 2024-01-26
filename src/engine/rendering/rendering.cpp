/*
** Agartha-Software, 2023
** C++evy
** File description:
** Rendering
*/

#include "rendering.hpp"
#include "AssetManager.hpp"
#include "Color.hpp"
#include "Diffuse.hpp"
#include "Handle.hpp"
#include "Line.hpp"
#include "Mesh.hpp"
#include "Query.hpp"
#include "Transform.hpp"
#include "World.hpp"
#include "cevy.hpp"
#include "ecs.hpp"
#include "raylib.h"
#include "raylib.hpp"
#include "raymath.h"
#include <cmath>

using namespace cevy::engine;
using namespace cevy;

void render_lines(cevy::ecs::World &w) {
  auto lines =
      ecs::Query<Line, option<cevy::engine::Transform>, option<cevy::engine::Color>>::query(w);
  for (auto [line, opt_transform, opt_color] : lines) {
    const cevy::engine::Transform &trans =
        opt_transform.value_or(cevy::engine::Transform(0., 0., 0.));
    const cevy::engine::Color &col = opt_color.value_or(cevy::engine::Color(0., 255., 60));
    Vector3 end = Vector3RotateByQuaternion(line.end - line.start, trans.rotation);
    end = Vector3Add(end, line.start + trans.position);
    DrawCylinderEx(line.start + trans.position, end, 0.1, 0.1, 4, (::Color)col);
  }
}

// static void render_model(const Model &model, engine::Transform transform, ::Color tint) {
//   // Matrix rot = QuaternionToMatrix(transform.rotation);
//   // Matrix pos = MatrixTranslate(transform.position.x, transform.position.y, transform.position.z);
//   // Matrix scale = MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z);
//   // Matrix final = MatrixMultiply(rot, MatrixMultiply(scale, pos));
//   Vector3 axis = { 0, 0, 1};
//   float angle = M_1_PI;
//   // QuaternionToAxisAngle(transform.rotation, &axis, &angle);

//   // model.transform = QuaternionToMatrix(transform.rotation);
//   // model.transform = MatrixMultiply(
//   //     model.transform, MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z));
//   DrawModelEx(model, transform.position, axis,angle, transform.scale, tint);
// }

static void render_model(const Model& model, const engine::Transform &transform, ::Color tint)
{
  Matrix rot = QuaternionToMatrix(transform.rotation);
  Matrix pos = MatrixTranslate(transform.position.x, transform.position.y, transform.position.z);
  Matrix scale = MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z);
  Matrix final = MatrixMultiply(rot, MatrixMultiply(scale, pos));
  // transform = MatrixMultiply(model.transform, transform);
  for (int i = 0; i < model.meshCount; ++i) {
    // if (model.meshMaterial[i] < model.materialCount)
      DrawMesh(model.meshes[i], model.materials[model.meshMaterial[i]], final);
    // else
      // std::cerr << "(ERROR)render_model: REQUESTING NON-EXISTENT MATERIAL";
  }
}

void render_models(cevy::ecs::World &w) {
  auto models = ecs::Query<option<engine::Transform>, Handle<engine::Mesh>, option<Handle<Diffuse>>,
                           option<engine::Color>>::query(w);

  auto assets_ = w.get_resource<AssetManager>();
  if (!assets_)
    return;
  auto &assets = assets_.value().get();

  for (auto [opt_tm, mesh, opt_diffuse, opt_color] : models) {
    const cevy::engine::Transform &tm = opt_tm.value_or(cevy::engine::Transform());
    ::Color ray_color;
    if (opt_color) {
      ray_color = opt_color.value();
    } else
      ray_color = ::WHITE;
    // if (opt_diffuse) {
    //   SetMaterialTexture(mesh->mesh.materials, MATERIAL_MAP_DIFFUSE,
    //                      opt_diffuse.value().get().texture);
    //   mesh->mesh.materialCount = 1;
    //   mesh->mesh.materials = (Material *)RL_CALLOC(mesh->mesh.materialCount, sizeof(Material));
    //   mesh->mesh.materials[0] = LoadMaterialDefault();
    // }
    render_model(mesh->mesh, tm, ray_color);
  }
}
