/*
** Agartha-Software, 2023
** Cevy
** File description:
** Rendering
*/

#pragma once

#include "Color.hpp"
#include "cevy.hpp"
#include "Query.hpp"
#include "Handle.hpp"
#include "Mesh.hpp"
#include "Diffuse.hpp"
#include "Position.hpp"
#include "Rotation.hpp"

void render_models(cevy::ecs::Query<option<cevy::engine::Position>, option<cevy::engine::Rotation>, cevy::engine::Handle<cevy::engine::Mesh>, option<cevy::engine::Handle<cevy::engine::Diffuse>>, option<cevy::engine::Color>> models);
