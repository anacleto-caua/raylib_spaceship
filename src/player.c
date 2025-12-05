#include "player.h"
#include "debug/debugger.h"
#include "entity.h"
#include "game.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

Camera3D PlayerCamera; // TODO: Find a more formal way to attach the PlayerCamera to a player
Model PlayerModel;

Vector3 PlayerPos = (Vector3){0, 0, 0};
Vector3 CameraPos = (Vector3){0, 0, 0};
float Throttle = 0;
float MaxThrottle = 500;
float ThrottleAcceleration = 10;
float ThrottleDeAcc = 5;
float AirResistance = .5;

float ThrottleAsymmetry = 0; // Goes from -1 to 1; -1 -> All Throttle left no right | 1 All right no left -> 0, equivalent    
float ThrottleAsymAcc = 0;

const Vector3 PLAYER_MODEL_LOCAL_FORWARD = (Vector3){1, 0, 0};

void PlayerInit()
{
    PlayerPos = (Vector3){0, 0, 0};
    CameraPos = PlayerPos;
    CameraPos.z -= 25;
    CameraPos.y += 10;

    PlayerCamera.position = CameraPos;
    PlayerCamera.target = PlayerPos;
    PlayerCamera.up = VECTOR3_UP;
    PlayerCamera.fovy = 60.0f;
    PlayerCamera.projection = CAMERA_PERSPECTIVE;

    PlayerModel = LoadModel("assets/models/sci fi jet.glb");
    // Makes the player model look forward
    Quaternion rotationQ = QuaternionFromVector3ToVector3(PLAYER_MODEL_LOCAL_FORWARD, VECTOR3_FORWARD);
    Matrix rotationMatrix = QuaternionToMatrix(rotationQ);
    PlayerModel.transform = MatrixIdentity(); 
    PlayerModel.transform = MatrixMultiply(rotationMatrix, PlayerModel.transform);
    PlayerModel.transform = MatrixMultiply(PlayerModel.transform, MatrixTranslate(PlayerPos.x, PlayerPos.y, PlayerPos.z));

    AddDebugMsg("Throtle: ", TYPE_FLOAT, &Throttle);
}

void PlayerUpdate()
{
    HandleMovementInput();
    Vector3 PlayerMovement = Vector3Scale(VECTOR3_FORWARD, Throttle * GetFrameTime());

    PlayerPos = Vector3Add(PlayerPos, PlayerMovement);
    CameraPos = Vector3Add(CameraPos, PlayerMovement);
    PlayerCamera.position = CameraPos;
    PlayerCamera.target = PlayerPos;

    DrawModelWiresEx(PlayerModel, PlayerPos, VECTOR3_FORWARD, 0.0f, VECTOR3_ONE, PURPLE);
}

void PlayerEnd()
{
    printf("player ended :b\n");
    UnloadModel(PlayerModel);
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

    if(IsKeyDown(KEY_D))
    {
        ThrottleAsymmetry += ThrottleAsymAcc;
        // Throttle += ThrottleAcceleration * GetFrameTime();
    }
    else if(IsKeyDown(KEY_A))
    {
        ThrottleAsymmetry -= ThrottleAsymAcc;
        // Throttle -= ThrottleAcceleration * GetFrameTime();
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