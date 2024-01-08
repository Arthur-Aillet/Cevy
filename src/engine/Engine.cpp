/*
** Agartha-Software, 2023
** Cevy
** File description:
** Engine
*/

#include "Engine.hpp"
#include "App.hpp"
#include "AssetManager.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "Commands.hpp"
#include "DefaultPlugin.hpp"
#include "EntityCommands.hpp"
#include "Line.hpp"
#include "Position.hpp"
#include "Resource.hpp"
#include "Rotation.hpp"
#include "Target.hpp"
#include "World.hpp"
#include "ecs.hpp"
#include "imgui.h"
#include "raylib.hpp"

#include "rendering.hpp"
#include "rlImGui.h"

void init_window() {
#ifdef DEBUG
  SetTraceLogLevel(LOG_ALL);
#else
  SetTraceLogLevel(LOG_ERROR);
#endif
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 450, "raylib [core] example - basic window");
  SetTargetFPS(60);
  rlImGuiSetup(true);
}

void close_game(cevy::ecs::Resource<struct cevy::ecs::Control> control) {
  if (WindowShouldClose())
    control.get().abort = true;
}

void update_window(cevy::ecs::Query<cevy::engine::Camera> cams,
                   cevy::ecs::Query<option<cevy::engine::Position>, cevy::engine::Line,
                                    option<cevy::engine::Color>>
                       lines,
                   cevy::ecs::Query<option<cevy::engine::Position>, option<cevy::engine::Rotation>,
                                    cevy::engine::Handle<cevy::engine::Mesh>,
                                    option<cevy::engine::Handle<cevy::engine::Diffuse>>,
                                    option<cevy::engine::Color>>
                       models) {
  ClearBackground(WHITE);
  for (auto [cam] : cams) {
    BeginMode3D(cam);
    DrawGrid(100, 1.0f);
    render_models(models);
    render_lines(lines);
    EndMode3D();
  }
  EndDrawing();
}

void cevy::engine::Engine::build(cevy::ecs::App &app) {
  app.add_plugins(cevy::ecs::DefaultPlugin());
  app.add_stage<StartupRenderStage>();
  app.add_stage<PreStartupRenderStage>();
  app.add_stage<PostStartupRenderStage>();
  app.add_stage<RenderStage>();
  app.add_stage<PreRenderStage>();
  app.add_stage<PostRenderStage>();
  app.init_component<cevy::engine::Camera>();
  app.init_component<cevy::engine::Position>();
  app.init_component<cevy::engine::Target>();
  app.init_component<cevy::engine::Line>();
  app.init_component<cevy::engine::Rotation>();
  app.init_component<cevy::engine::Color>();
  app.add_plugins(cevy::engine::AssetManagerPlugin());
  app.add_system<cevy::engine::PreStartupRenderStage>(init_window);
  app.add_system<cevy::engine::PreRenderStage>(close_game);
  app.add_system<cevy::engine::PreRenderStage>(update_camera);
  app.add_system<cevy::engine::RenderStage>(update_window);
}

/*

void cevy::Engine::DebugWindow (void) {
    if (IsKeyPressed(KEY_F10)) {
        debugMode = (debugMode == false);
        if (debugMode)
            EnableCursor();
    }
    if(debugMode) {
        rlImGuiBegin();
        if(!ImGui::Begin("Test WIndow", &debugMode)) {
            ImGui::End();
        } else {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
        }
        rlImGuiEnd();
    } else {
        DisableCursor();
    }
}

void cevy::Engine::Fullscreen (void) {
    if (IsWindowFullscreen()) {
        int display = GetCurrentMonitor();
        this->screenWidth = GetMonitorWidth(display);
        this->screenHeight = GetMonitorHeight(display);
    } else {
        this->screenWidth = GetScreenWidth();
        this->screenHeight = GetScreenHeight();
    }
    if (!IsKeyPressed(KEY_F11))
        return;
    SetWindowSize(this->screenWidth, this->screenWidth);
        ToggleFullscreen();
}
*/
