#include "game.h"
#include "player.h"
#include "entity.h"

const int DEBUG_FONT_SIZE = 20;

const char* SCREEN_TITLE = "Space Race";
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 720;

const Vector3 VECTOR3_FORWARD = {0, 0, 1};
const Vector3 VECTOR3_UP      = {0, 1, 0};
const Vector3 VECTOR3_RIGHT   = {1, 0, 0};

int FPS_TARGET = 265;

int main()
{
    InitGame();

    while (!WindowShouldClose()) {
        Update();
    }

    EndGame();

    return 0;
}

void InitGame()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
    SetTargetFPS(FPS_TARGET);

    SpawnPlayer();
}

void Update()
{
    BeginDrawing();
    ClearBackground(WHITE);
    BeginMode3D(camera);
    
    DrawGrid(100, 1.0f); 

    Entity* CurrentEntity;
    for(int i = 0; i < entityCount; i++)
    {
        CurrentEntity = Entities[i];
        CurrentEntity->Update();
    }        
    
    EndMode3D();

    DebugPlayerInfo(); // TODO: Make a more decent debug system

    DrawFPS(0, 0);
    EndDrawing();
}

void EndGame()
{
    Entity* CurrentEntity;
    for(int i = 0; i < entityCount; i++)
    {
        CurrentEntity = Entities[i];
        CurrentEntity->End();
    }

    CloseWindow();
}