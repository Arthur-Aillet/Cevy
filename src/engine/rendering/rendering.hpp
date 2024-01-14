/*
** Agartha-Software, 2023
** C++evy
** File description:
** Rendering
*/

#pragma once

#include "Color.hpp"
#include "Diffuse.hpp"
#include "Handle.hpp"
#include "Line.hpp"
#include "Mesh.hpp"
#include "Position.hpp"
#include "Query.hpp"
#include "Transform.hpp"
#include "cevy.hpp"

void render_models(cevy::ecs::Query<option<cevy::engine::Position>, option<cevy::engine::Transform>,
                                    cevy::engine::Handle<cevy::engine::Mesh>,
                                    option<cevy::engine::Handle<cevy::engine::Diffuse>>,
                                    option<cevy::engine::Color>>
                       models);

void render_lines(cevy::ecs::Query<option<cevy::engine::Position>, cevy::engine::Line,
                                   option<cevy::engine::Color>>
                      lines);
