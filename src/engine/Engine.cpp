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
#include "Position.hpp"
#include "Resource.hpp"
#include "Rotation.hpp"
#include "World.hpp"
#include "ecs.hpp"
#include "imgui.h"
#include "raylib.h"
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

void update_window(
    cevy::ecs::Query<cevy::engine::Camera, cevy::engine::Position, cevy::engine::Rotation> cams,
    cevy::ecs::Query<option<cevy::engine::Position>, option<cevy::engine::Rotation>,
                     cevy::engine::Handle<cevy::engine::Mesh>,
                     option<cevy::engine::Handle<cevy::engine::Diffuse>>,
                     option<cevy::engine::Color>>
        models) {
  Vector3 fowards = {0, 0, 0};
  for (auto cam : cams) {
    fowards = std::get<2>(cam).fowards();
    std::get<0>(cam).camera.position = std::get<1>(cam);
  }
  DrawGrid(100, 1.0f);

  ClearBackground(WHITE);
  for (auto cam : cams) {
    BeginMode3D(std::get<0>(cam));
    DrawGrid(100, 1.0f);
    render_models(models);
    EndMode3D();
  }
  EndDrawing();
}

void list_pos(cevy::ecs::Query<cevy::engine::Position> pos) {
  for (auto po : pos) {
    std::cout << std::get<0>(po).x << std::endl;
  }
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
  app.init_component<cevy::engine::Rotation>();
  app.init_component<cevy::engine::Color>();
  app.add_plugins(cevy::engine::AssetManagerPlugin());
  app.add_system<cevy::engine::PreStartupRenderStage>(init_window);
  app.add_system<cevy::engine::PreRenderStage>(close_game);
  app.add_system<cevy::engine::RenderStage>(update_window);
  app.spawn(cevy::engine::Camera(), cevy::engine::Position(10.0, 10.0, 10.0),
            cevy::engine::Rotation(0.0, 0.6, 1.0));
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

void cevy::Engine::update(void) {
    const char* text = "Your first window!";
    Vector2 text_size;

    while (!WindowShouldClose()) {
        Fullscreen();
        text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);

        BeginDrawing();

        ClearBackground(DARKGRAY);
        DrawText(text, this->screenWidth / 2 - text_size.x / 2,
this->screenHeight / 2 - text_size.y / 2, 20, RAYWHITE); BeginMode3D(camera);

        DrawGrid(100, 1.0f);

        EndMode3D();
        DebugWindow();
        EndDrawing();
    }
}
*/
