#include "player.h"
#include "entity.h"
#include "game.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <string.h>

Camera3D camera; // TODO: Find a more formal way to attach the camera to a player

Vector3 PlayerPos = (Vector3){0, 0, 0};
Vector3 CameraPos = (Vector3){0, 0, 0};
float Throttle = 0;
float MaxThrottle = 500;
float ThrottleAcceleration = 10;
float ThrottleDeAcc = 5;
float AirResistance = .5;

void PlayerInit()
{
    PlayerPos = (Vector3){0, 0, 0};
    CameraPos = PlayerPos;
    CameraPos.z -= 15;
    CameraPos.y += 7;

    camera.position = CameraPos;
    camera.target = PlayerPos;
    camera.up = VECTOR3_UP;
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

}

void PlayerUpdate()
{
    HandleMovementInput();

    PlayerPos = Vector3Add(PlayerPos, Vector3Scale(VECTOR3_FORWARD, Throttle * GetFrameTime()));
    CameraPos = PlayerPos;
    CameraPos.z -= 15;
    CameraPos.y += 7;
    camera.position = CameraPos;
    camera.target = PlayerPos;

    DrawSphere(PlayerPos, 3, PURPLE);
    
}

void PlayerEnd()
{
    printf("player ended :b");
}

void SpawnPlayer()
{
    AddEntity(PlayerInit, PlayerUpdate, PlayerEnd);
}

void HandleMovementInput()
{
    if(IsKeyDown(KEY_W))
    {
        Throttle += ThrottleAcceleration * GetFrameTime();
    }
    else if(IsKeyDown(KEY_S))
    {
        Throttle -= ThrottleDeAcc * GetFrameTime();
    }

    Throttle -= AirResistance * GetFrameTime();

    if(Throttle <= 0)
    {
        Throttle = 0;
    }

    if(Throttle >= MaxThrottle)
    {
        Throttle = MaxThrottle;
    }
}

void DebugPlayerInfo()
{
    char formatedFloat[10] = "";
    char finalString[100] = "";
    snprintf(formatedFloat, sizeof(formatedFloat), "%.2f", Throttle);
    strcpy(finalString, "Throtle: ");
    strcpy(finalString, formatedFloat);

    DrawText(finalString, GetScreenWidth() - strlen(finalString) * DEBUG_FONT_SIZE, 0, DEBUG_FONT_SIZE, RED);
    // DrawText("Movement: %d", PlayerMovement);
}