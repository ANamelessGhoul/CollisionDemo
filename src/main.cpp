/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2020 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "world.h"
#include "uniformGridWorld.h"
#include <memory>

void Draw2DGrid(int screenWidth, int screenHeight, int step = 100, Color gridColor = GRAY);

int main() 
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib");

    std::unique_ptr<World> world = std::make_unique<UniformGridWorld>();

    Camera2D camera = { 0 };
    camera.zoom = 1;
    camera.rotation = 0;
    camera.target = Vector2{ 0 };
    camera.offset = Vector2{ 0 };

    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------

        world->UpdatePoints();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);
                world->DrawPoints();


            EndMode2D();

            Draw2DGrid(screenWidth, screenHeight, 200);

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void Draw2DGrid(int screenWidth, int screenHeight, int step, Color gridColor)
{
    int x = 0;
    while (x < screenWidth)
    {
        DrawLine(x, 0, x, screenHeight, gridColor);
        x += step;
    }

    int y = 0;
    while (y < screenHeight)
    {
        DrawLine(0, y, screenWidth, y, gridColor);
        y += step;
    }
}