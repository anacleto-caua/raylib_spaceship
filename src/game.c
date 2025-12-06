#include "game.h"
#include "debugger/debugger.h"
#include "player.h"
#include "entity.h"
#include "raylib.h"

const char* SCREEN_TITLE = "Space Race";
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 720;

int FPS_TARGET = 265;

// private

void InitGame()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
    SetTargetFPS(FPS_TARGET);

    PlayerSpawn();
}

void Update()
{
    BeginDrawing();
    ClearBackground(PURPLE);
    BeginMode3D(GetPlayerCamera());
    
    DrawGrid(100, 1.0f); 

    UpdateAllEntities();

    EndMode3D();
    DrawAllDebugMessages();
    
    DrawFPS(0, 0);
    EndDrawing();
}

void EndGame()
{
    EndAllEntities();
    CloseWindow();
}

// main loop

int main()
{
    InitGame();

    while (!WindowShouldClose()) {
        Update();
    }

    EndGame();

    return 0;
}