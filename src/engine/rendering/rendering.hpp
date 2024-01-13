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
#include "Query.hpp"
#include "Transform.hpp"
#include "cevy.hpp"
#include "ecs.hpp"

void render_models(cevy::ecs::World &world);

void render_lines(cevy::ecs::World &w);
