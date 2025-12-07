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
const Vector3 PLAYER_CAMERA_OFFSET = (Vector3){ 0, 10, -25 };

Vector3 PlayerPos = VEC_ZERO;
Vector3 CameraPos = VEC_ZERO;
Matrix FixMatrix;
Quaternion PlayerRot = { 0 };

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
    CameraPos = Vector3Add(PlayerPos, PLAYER_CAMERA_OFFSET);

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
    PlayerRot = QuaternionIdentity();

    AddDebugMsg("Speed: ", TYPE_FLOAT, &Speed);
    AddDebugMsg("Throttle: ", TYPE_FLOAT, &Throttle);
    
    AddDebugMsg("FlightStick: ", TYPE_VECTOR2, &FlightStickInput);

    AddDebugMsg("PlayerPos: ", TYPE_VECTOR3, &PlayerPos);
}

void PlayerUpdate()
{
    HandleMovementInput();

    // rotation
    Quaternion qPitch = QuaternionFromAxisAngle(VEC_RIGHT, FlightStickInput.y * pitchSpeed * GetFrameTime());
    Quaternion qYaw   = QuaternionFromAxisAngle(VEC_UP, FlightStickInput.x * yawSpeed * -GetFrameTime());
    Quaternion qRoll  = QuaternionFromAxisAngle(VEC_FORWARD, 0.0f);
    
    // Combine them (Yaw -> Pitch -> Roll is standard order)
    Quaternion qFrame = QuaternionMultiply(qYaw, QuaternionMultiply(qPitch, qRoll));
    PlayerRot = QuaternionMultiply(PlayerRot, qFrame);
    PlayerRot = QuaternionNormalize(PlayerRot);
    Matrix matRotation = QuaternionToMatrix(PlayerRot);
    PlayerModel.transform = MatrixMultiply(FixMatrix, matRotation);

    Vector3 currentForward = Vector3RotateByQuaternion(VEC_FORWARD, PlayerRot);
    Vector3 rotatedOffset = Vector3RotateByQuaternion(PLAYER_CAMERA_OFFSET, PlayerRot);

    // movement
    Vector3 PlayerMovement = Vector3Scale(currentForward, Speed * GetFrameTime());

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
