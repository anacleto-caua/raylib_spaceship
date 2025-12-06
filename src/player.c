#include "player.h"
#include <stdio.h>
#include "debugger/debugger.h"
#include "game.h"
#include "entity.h"
#include "raylib.h"
#include "raymath.h"
#include "utils.h"

Camera3D PlayerCamera; // TODO: Find a more formal way to attach the PlayerCamera to a player
Model PlayerModel;
const Vector3 PLAYER_MODEL_LOCAL_FORWARD = (Vector3){1, 0, 0};

Vector3 PlayerPos = VEC_ZERO;
Vector3 CameraPos = VEC_ZERO;

float Throttle = 0;     // Throttle percent, how much both engines are working: 0 min to 1 max
float MaxThrottle = 1;
float ThrottleAcceleration = .1;
float ThrottleDeAcc = .05;

const float MAX_SPEED = 700000000;
float Speed = 0;
float AirResistance = .5;
float SpeedGainOnMaxThrottle = 50;

float ThrottleAsymmetry = 0;
float ThrottleAsymAcc = 0;

const float MOUSE_FLIGHT_STICK_DEADZONE = .2f;
float mouseX = 0;
float mouseY = 0;
float flightStickInputX = 0;
float flightStickInputY = 0;

// helpers

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

    // TODO: Finish implementing throttle asymetry
    if(IsKeyDown(KEY_D))
    {
        ThrottleAsymmetry += ThrottleAsymAcc * GetFrameTime();
    }
    else if(IsKeyDown(KEY_A))
    {
        ThrottleAsymmetry -= ThrottleAsymAcc * GetFrameTime();
    }
    
    Speed += Throttle * SpeedGainOnMaxThrottle * GetFrameTime();
    Speed -= AirResistance * GetFrameTime();

    // Gets the mouse input from -1 to 1
    mouseX = (((float)GetMouseX() / GetScreenWidth())  - .5) * 2;
    mouseY = (((float)GetMouseY() / GetScreenHeight()) - .5) * 2;
    
    // TODO: Since the screen isnt a square the flight stick deadzone is more of an elipse than a circle.
    // Consider just measuring the cursor distance from the screen center
    if(fabsf(mouseX) > MOUSE_FLIGHT_STICK_DEADZONE)
    {
        flightStickInputX = mouseX;
    }
    else
    {
        flightStickInputX = 0;
    }

    if(fabsf(mouseY) > MOUSE_FLIGHT_STICK_DEADZONE)
    {
        flightStickInputY = mouseY;
    }
    else 
    {
        flightStickInputY = 0;
    }
    
    
    cap(&Speed, 0.0f, MAX_SPEED);
    cap(&Throttle, 0.0f, MaxThrottle);
    cap(&ThrottleAsymmetry, -1.0f, +1.0f);
}

// public

void PlayerSpawn()
{
    AddEntity(PlayerInit, PlayerUpdate, PlayerEnd);
}

void PlayerInit()
{
    PlayerPos = (Vector3){0, 0, 0};
    CameraPos = PlayerPos;
    CameraPos.z -= 25;
    CameraPos.y += 10;

    PlayerCamera.position = CameraPos;
    PlayerCamera.target = PlayerPos;
    PlayerCamera.up = VEC_UP;
    PlayerCamera.fovy = 60.0f;
    PlayerCamera.projection = CAMERA_PERSPECTIVE;

    PlayerModel = LoadModel("assets/models/sci fi jet.glb");
    // Makes the player model look forward
    Quaternion rotationQ = QuaternionFromVector3ToVector3(PLAYER_MODEL_LOCAL_FORWARD, VEC_FORWARD);
    Matrix rotationMatrix = QuaternionToMatrix(rotationQ);
    PlayerModel.transform = MatrixIdentity(); 
    PlayerModel.transform = MatrixMultiply(rotationMatrix, PlayerModel.transform);
    PlayerModel.transform = MatrixMultiply(PlayerModel.transform, MatrixTranslate(PlayerPos.x, PlayerPos.y, PlayerPos.z));

    AddDebugMsg("Speed: ", TYPE_FLOAT, &Speed);
    AddDebugMsg("Throttle: ", TYPE_FLOAT, &Throttle);
    
    AddDebugMsg("MouseX: ", TYPE_FLOAT, &mouseX);
    AddDebugMsg("MouseY: ", TYPE_FLOAT, &mouseY);

    AddDebugMsg("FlightStickX: ", TYPE_FLOAT, &flightStickInputX);
    AddDebugMsg("FlightStickY: ", TYPE_FLOAT, &flightStickInputY);
}

void PlayerUpdate()
{
    HandleMovementInput();
    Vector3 PlayerMovement = Vector3Scale(VEC_FORWARD, Speed * GetFrameTime());

    PlayerPos = Vector3Add(PlayerPos, PlayerMovement);
    CameraPos = Vector3Add(CameraPos, PlayerMovement);
    PlayerCamera.position = CameraPos;
    PlayerCamera.target = PlayerPos;

    DrawModelWires(PlayerModel, PlayerPos, 1, PURPLE);
}

void PlayerEnd()
{
    printf("player ended :b\n");
    UnloadModel(PlayerModel);
}

Camera3D GetPlayerCamera()
{
    return PlayerCamera;
}
