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
Matrix FixMatrix;

float Throttle = 0;     // Throttle percent, how much both engines are working: 0 min to 1 max
float MaxThrottle = 1;
float ThrottleAcceleration = .7;
float ThrottleDeAcc = .5;

const float MAX_SPEED = 700000000;
float Speed = 0;
float AirResistance = .5;
float SpeedGainOnMaxThrottle = 50;

float ThrottleAsymmetry = 0;
float ThrottleAsymAcc = 0;

const float FLIGHT_STICK_DEADZONE_RADIUS = .2f;
Vector2 FlightStickInput = VEC2_ZERO;
float pitchSpeed = 1;
float rollSpeed = 1;
float yawSpeed = 1;
float maneuverability = 1000;

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
    float inputX = (((float)GetMouseX() / GetScreenWidth())  - .5) * 2;
    float inputY = (((float)GetMouseY() / GetScreenHeight()) - .5) * 2;
    FlightStickInput = (Vector2){ inputX, inputY };
    
    if(Vector2Distance(FlightStickInput, VEC2_ZERO) < FLIGHT_STICK_DEADZONE_RADIUS * 2)
    {
        FlightStickInput = VEC2_ZERO;
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
    PlayerPos = VEC_ZERO;
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
    FixMatrix = MatrixIdentity(); 
    FixMatrix = MatrixMultiply(rotationMatrix, FixMatrix);
    FixMatrix = MatrixMultiply(FixMatrix, MatrixTranslate(PlayerPos.x, PlayerPos.y, PlayerPos.z));
    
    PlayerModel.transform = FixMatrix;

    AddDebugMsg("Speed: ", TYPE_FLOAT, &Speed);
    AddDebugMsg("Throttle: ", TYPE_FLOAT, &Throttle);
    
    AddDebugMsg("FlightStick: ", TYPE_VECTOR2, &FlightStickInput);

    AddDebugMsg("PlayerPos: ", TYPE_VECTOR3, &PlayerPos);
}

void PlayerUpdate()
{
    HandleMovementInput();

    // rotation
    Vector3 rot =
        Vector3Scale(
            (Vector3)
            {
                FlightStickInput.y * pitchSpeed,
                FlightStickInput.x * yawSpeed,
                0 
            },
            GetFrameTime() * maneuverability
        );
    
    Matrix matrixSpin = MatrixRotateXYZ((Vector3){
        rot.x * DEG2RAD,
        rot.y * DEG2RAD,
        rot.z * DEG2RAD 
    });

    Matrix TranformMatrix = MatrixMultiply(FixMatrix, matrixSpin);
    PlayerModel.transform = TranformMatrix;

    // movement
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
