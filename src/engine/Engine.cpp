/*
** Agartha-Software, 2023
** Cevy
** File description:
** Engine
*/

#include "Engine.hpp"
#include "World.hpp"
#include "ecs.hpp"
#include "raylib.h"
#include "rlImGui.h"
#include "Schedule.hpp"
#include "App.hpp"
#include "Camera.hpp"
#include "Position.hpp"
#include "Commands.hpp"
#include "EntityCommands.hpp"
#include "imgui.h"

void init_window() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "raylib [core] example - basic window");
    SetTargetFPS(60);
    rlImGuiSetup(true);
}

void update_window(cevy::ecs::Query<cevy::Camera> cams, cevy::ecs::Commands cmd) {
    cmd.spawn_empty().insert(cevy::Position());
    for (auto cam : cams) {
        UpdateCamera(std::get<0>(cam), CAMERA_FIRST_PERSON);
    }
    BeginDrawing();

    ClearBackground(WHITE);
    for (auto cam : cams) {
        BeginMode3D(std::get<0>(cam));

        DrawGrid(100, 1.0f);

        EndMode3D();
    }
    EndDrawing();
}

void list_pos(cevy::ecs::Query<cevy::Position> pos) {
    for (auto po : pos) {
        std::cout << std::get<0>(po).x << std::endl;
    }
}

void cevy::Engine::build(cevy::ecs::App& app) {
    app.add_stage<RenderStage>();
    app.add_system<cevy::ecs::Schedule::PreStartup>(init_window);
    app.add_system<cevy::RenderStage>(update_window);
    app.add_system<cevy::RenderStage>(list_pos);
    app.init_component<cevy::Camera>();
    app.init_component<cevy::Position>();
    app.spawn(cevy::Camera());
}


/*
cevy::Engine::Engine() {
}

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
        DrawText(text, this->screenWidth / 2 - text_size.x / 2, this->screenHeight / 2 - text_size.y / 2, 20, RAYWHITE);
        BeginMode3D(camera);

        DrawGrid(100, 1.0f);

        EndMode3D();
        DebugWindow();
        EndDrawing();
    }
}
*/
