#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

// Simple button function
bool button(Rectangle bounds, const char* text) {
    Vector2 mousePos = GetMousePosition();
    bool hover = CheckCollisionPointRec(mousePos, bounds);
    bool clicked = hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    Color bgColor = hover ? LIGHTGRAY : GRAY;
    Color textColor = BLACK;

    DrawRectangleRec(bounds, bgColor);
    DrawRectangleLines((int)bounds.x, (int)bounds.y,
        (int)bounds.width, (int)bounds.height, DARKGRAY);

    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1);
    Vector2 textPos = {
        bounds.x + (bounds.width - textSize.x) / 2,
        bounds.y + (bounds.height - textSize.y) / 2
    };

    DrawTextEx(GetFontDefault(), text, textPos, 20, 1, textColor);

    return clicked;
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Simple GUI with Raylib - C23");
    SetTargetFPS(60);

    int counter = 0;
    bool showWindow = true;
    float sliderValue = 0.5f;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (showWindow) {
            // Window background
            Rectangle windowRect = { 50, 50, 400, 300 };
            DrawRectangleRec(windowRect, Fade(LIGHTGRAY, 0.9f));
            DrawRectangleLines(50, 50, 400, 300, DARKGRAY);

            // Title bar
            DrawRectangle(50, 50, 400, 30, DARKGRAY);
            DrawText("Hello, World!", 60, 57, 16, WHITE);

            // Close button
            if (button((Rectangle) { 420, 55, 20, 20 }, "X")) {
                showWindow = false;
            }

            // Content
            DrawText("This is a simple GUI window!", 70, 100, 20, BLACK);

            // Counter button
            if (button((Rectangle) { 70, 140, 120, 40 }, "Click me!")) {
                counter++;
            }

            DrawText(TextFormat("Counter: %d", counter), 70, 200, 20, BLACK);

            // Simple slider
            DrawText("Slider:", 70, 240, 16, BLACK);
            Rectangle sliderBg = { 140, 245, 200, 10 };
            DrawRectangleRec(sliderBg, LIGHTGRAY);

            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, sliderBg) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                sliderValue = (mousePos.x - sliderBg.x) / sliderBg.width;
                if (sliderValue < 0) sliderValue = 0;
                if (sliderValue > 1) sliderValue = 1;
            }

            float sliderHandleX = sliderBg.x + sliderValue * sliderBg.width;
            DrawRectangle((int)sliderHandleX - 5, (int)sliderBg.y - 5, 10, 20, DARKGRAY);

            DrawText(TextFormat("Value: %.2f", sliderValue), 70, 270, 16, BLACK);
        }
        else {
            DrawText("Window closed. Press SPACE to reopen.", 200, 300, 20, BLACK);
            if (IsKeyPressed(KEY_SPACE)) {
                showWindow = true;
            }
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}