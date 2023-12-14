/*
** Agartha-Software, 2023
** Cevy
** File description:
** game_engine
*/

#include "game_engine.hpp"
#include "camera.hpp"
#include "rlImGui.h"
#include "imgui.h"

Cevy::GameEngine::GameEngine() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(this->screenWidth, this->screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);
    rlImGuiSetup(true);
}

void Cevy::GameEngine::DebugWindow (void) {
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
        UpdateCamera(camera, CAMERA_FIRST_PERSON);
        DisableCursor();
    }
}

void Cevy::GameEngine::Fullscreen (void) {
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

void Cevy::GameEngine::update(void) {
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
