#include "game_engine.hpp"
#include "camera.hpp"
#include "rlImGui.h"
#include "imgui.h"
#include <iostream>

Cevy::GameEngine::GameEngine() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(this->screenWidth, this->screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);
    rlImGuiSetup(true);
}

void Cevy::GameEngine::update(void) {
    Cevy::Camera camera;

    rlImGuiSetup(true);
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateCamera(camera, CAMERA_FREE);
        this->screenWidth = GetScreenWidth();
        this->screenHeight = GetScreenHeight();

        const char* text = "Your first window!";
        Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);
        BeginDrawing();

        ClearBackground(DARKGRAY);
        DrawText(text, this->screenWidth / 2 - text_size.x / 2, this->screenHeight / 2 - text_size.y / 2, 20, RAYWHITE);
        BeginMode3D(camera);

        DrawGrid(10, 1.0f);

        EndMode3D();
        rlImGuiBegin();
        bool open = true;
        ImGui::ShowDemoWindow(&open);
        DrawFPS(190, 300);
        rlImGuiEnd();
        EndDrawing();
    }
}
