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
#include "stats.h"
#include <memory>
#include "quadTreeWorld.h"

int main() 
{
    SetTraceLogLevel(LOG_TRACE);
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    std::unique_ptr<World> world = std::make_unique<World>();

    Camera2D camera = { 0 };
    camera.zoom = 1;
    camera.rotation = 0;
    camera.target = Vector2{ 0 };
    camera.offset = Vector2{ 0 };

    Stats& stats = Stats::GetInstance();

    double time{};
    double averageTimeMicro{};
    double averageQueries{};

    
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
                world->Draw();

            EndMode2D();
            if (GetTime() < 10.0)
            {
                time = GetTime();
                averageTimeMicro = stats.averageTime.GetValue();
                averageQueries = stats.averageQueries.GetValue();
            }

            DrawFPS(10, 10);
            DrawText(TextFormat("Time Left: %.2f", 10 - time), 10, 40, 20, LIME);
            DrawText(TextFormat("Avg Time: %.2f", averageTimeMicro), 10, 70, 20, LIME);
            DrawText(TextFormat("Avg Queries: %.2f", averageQueries), 10, 100, 20, LIME);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

