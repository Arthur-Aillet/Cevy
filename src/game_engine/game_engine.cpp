#include "game_engine.hpp"
#include "camera.hpp"
#include <iostream>

GameEngine::GameEngine() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(this->screenWidth, this->screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);
}

void GameEngine::update(void) {
    //Camera3D camera = { 0 };
    //camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    //camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    //camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    //camera.fovy = 45.0f;                                // Camera field-of-view Y
    //camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    CevyCamera camera;

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
        EndDrawing();
    }
}
