#ifdef _MSC_VER
// Define these BEFORE including raylib to prevent Windows API conflicts
#define WIN32_LEAN_AND_MEAN
#define NOUSER          // Excludes USER32 API including CloseWindow

#pragma comment(lib, "raylib.lib")
#pragma comment(lib, "winmm.lib")      // For timeBeginPeriod/timeEndPeriod
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "shell32.lib")

// Use Windows subsystem for proper GUI app (no console window)
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

// Simple button function with custom font
bool button(Rectangle bounds, const char* text, Font font) {
    Vector2 mousePos = GetMousePosition();
    bool hover = CheckCollisionPointRec(mousePos, bounds);
    bool clicked = hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    Color bgColor = hover ? LIGHTGRAY : GRAY;
    Color textColor = BLACK;

    DrawRectangleRec(bounds, bgColor);
    DrawRectangleLines((int)bounds.x, (int)bounds.y,
        (int)bounds.width, (int)bounds.height, DARKGRAY);

    Vector2 textSize = MeasureTextEx(font, text, 20, 1);
    Vector2 textPos = {
        bounds.x + (bounds.width - textSize.x) / 2,
        bounds.y + (bounds.height - textSize.y) / 2
    };

    DrawTextEx(font, text, textPos, 20, 1, textColor);

    return clicked;
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    // If you want a completely invisible window (mmust do before InitWindow):
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED);

    // If you want to disable the raylib logo on startup:
    SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);

    // Configure raylib BEFORE InitWindow
    SetTraceLogLevel(LOG_NONE);          // Disable all raylib console output

    InitWindow(screenWidth, screenHeight, "Simple GUI with Raylib - ISO C");
    SetTargetFPS(60);

    // Load a better font - you can use system fonts or load TTF files
    Font font = LoadFontEx("C:/Windows/Fonts/segoeui.ttf", 20, 0, 0);  // Segoe UI
    // Alternative system fonts:
    // Font font = LoadFontEx("C:/Windows/Fonts/arial.ttf", 20, 0, 0);     // Arial
    // Font font = LoadFontEx("C:/Windows/Fonts/calibri.ttf", 20, 0, 0);   // Calibri
    // Font font = LoadFontEx("C:/Windows/Fonts/consola.ttf", 18, 0, 0);   // Consolas (monospace)

    // Fallback to default if font loading fails
    if (font.texture.id == 0) {
        font = GetFontDefault();
    }

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
            DrawTextEx(font, "Hello, World!", (Vector2) { 60, 57 }, 16, 1, WHITE);

            // Close button
            if (button((Rectangle) { 420, 55, 20, 20 }, "X", font)) {
                showWindow = false;
            }

            // Content
            DrawTextEx(font, "This is a simple GUI window!", (Vector2) { 70, 100 }, 20, 1, BLACK);

            // Counter button
            if (button((Rectangle) { 70, 140, 120, 40 }, "Click me!", font)) {
                counter++;
            }

            DrawTextEx(font, TextFormat("Counter: %d", counter), (Vector2) { 70, 200 }, 20, 1, BLACK);

            // Simple slider
            DrawTextEx(font, "Slider:", (Vector2) { 70, 240 }, 16, 1, BLACK);
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

            DrawTextEx(font, TextFormat("Value: %.2f", sliderValue), (Vector2) { 70, 270 }, 16, 1, BLACK);
        }
        else {
            DrawTextEx(font, "Window closed. Press SPACE to reopen.", (Vector2) { 200, 300 }, 20, 1, BLACK);
            if (IsKeyPressed(KEY_SPACE)) {
                showWindow = true;
            }
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    // Cleanup
    if (font.texture.id != GetFontDefault().texture.id) {
        UnloadFont(font);
    }
    CloseWindow();
    return 0;
}