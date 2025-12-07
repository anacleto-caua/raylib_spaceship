#include "player.h"
#include "debugger/debugger.h"
#include "game.h"
#include "utils.h"
#include "entity.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

// config
const float ENGINE_POWER = 100;       // Acceleration force
const float DRAG_FACTOR = -.5;         // Air resistance (Higher = stops faster)
const float MAX_VELOCITY = 100;      // Terminal velocity clamp
const float ROTATION_SMOOTHING = 5;  // Lower = heavier plane, Higher = snappy
const float ROLL_LERP = 1;
const float CAMERA_LERP = 10;

// input configs
// TODO: Consider -1 for reverse
const float MAX_THROTTLE = 1;
const float MIN_THROTTLE = 0;
const float THROTTLE_ACCELERATION = .7;
const float THROTTLE_DE_ACCELERATION = .5;

const float PITCH_SENSI = .7;
const float ROLL_SENSI = 1;
const float YAW_SENSI = 1;

// state
Camera3D PlayerCamera; // TODO: Find a more formal way to attach the PlayerCamera to a player
Model PlayerModel;
const Vector3 PLAYER_MODEL_LOCAL_FORWARD = (Vector3){1, 0, 0};
const Vector3 PLAYER_CAMERA_OFFSET = (Vector3){ 0, 5, -25 };

Vector3 PlayerPos = VEC_ZERO;
Vector3 PlayerVelocity = VEC_ZERO;
Vector3 CameraPos = VEC_ZERO;
Matrix FixMatrix;
Quaternion PlayerRot = { 0 };

// input
float Throttle = 0;     // Throttle percent, how much both engines are working: 0 min to 1 max
Vector2 FlightStickInput = VEC2_ZERO;
float RollInput = 0;


float ft = 0; // GetFrameTime();

// helpers

void HandleMovementInput()
{
    if (IsKeyDown(KEY_W)) 
    {
        Throttle += THROTTLE_ACCELERATION * ft;
    }
    else if (IsKeyDown(KEY_S)) 
    {
        Throttle -= THROTTLE_DE_ACCELERATION * ft;
    }
    cap(&Throttle, MIN_THROTTLE, MAX_THROTTLE);

    float targetRoll = 0;
    if(IsKeyDown(KEY_D))
    {
        targetRoll = 1;
    }
    else if(IsKeyDown(KEY_A))
    {
        targetRoll = -1;
    }
    RollInput = Lerp(RollInput, targetRoll, ROLL_LERP * ft);

    // Gets the mouse input from -1 to 1
    float inputX = (((float)GetMouseX() / GetScreenWidth())  - .5) * 2;
    float inputY = (((float)GetMouseY() / GetScreenHeight()) - .5) * 2;
    FlightStickInput = (Vector2){ inputX * inputX * inputX, inputY * inputY * inputY };
}

// public

void PlayerSpawn()
{
    AddEntity(PlayerInit, PlayerUpdate, PlayerEnd);
}

void PlayerInit()
{
    PlayerPos = VEC_ZERO;
    PlayerVelocity = VEC_ZERO;
    PlayerRot = QuaternionIdentity();
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

    AddDebugMsg("Throttle: ", TYPE_FLOAT, &Throttle);
    
    AddDebugMsg("RollInput: ", TYPE_FLOAT, &RollInput);
    AddDebugMsg("FlightStick: ", TYPE_VECTOR2, &FlightStickInput);

    AddDebugMsg("PlayerPos: ", TYPE_VECTOR3, &PlayerPos);
    AddDebugMsg("PlayerVel: ", TYPE_VECTOR3, &PlayerVelocity);
}

void PlayerUpdate()
{
    ft = GetFrameTime();

    HandleMovementInput();

    // rotation
    Quaternion qPitch = QuaternionFromAxisAngle((Vector3){1,0,0}, FlightStickInput.y * PITCH_SENSI * ft);
    Quaternion qYaw   = QuaternionFromAxisAngle((Vector3){0,1,0}, FlightStickInput.x * YAW_SENSI * -ft);     //TODO: Consider a cleaner method to invert the yaw axis
    Quaternion qRoll  = QuaternionFromAxisAngle((Vector3){0,0,1}, RollInput * ROLL_SENSI * ft);
    
    // Combine them (Yaw -> Pitch -> Roll is standard order)
    Quaternion qFrame = QuaternionMultiply(qYaw, QuaternionMultiply(qPitch, qRoll));

    PlayerRot = QuaternionMultiply(PlayerRot, qFrame);
    PlayerRot = QuaternionNormalize(PlayerRot);

    Matrix matRotation = QuaternionToMatrix(PlayerRot);
    PlayerModel.transform = MatrixMultiply(FixMatrix, matRotation);

    Vector3 currentForward = Vector3RotateByQuaternion(VEC_FORWARD, PlayerRot);
    Vector3 rotatedOffset = Vector3RotateByQuaternion(PLAYER_CAMERA_OFFSET, PlayerRot);

    // physics
    Vector3 forceThrust = Vector3Scale(currentForward, Throttle * ENGINE_POWER);
    Vector3 forceDrag = Vector3Scale(PlayerVelocity, DRAG_FACTOR);
    
    // TODO: Consider gravity
    Vector3 forceGravity = Vector3Scale(VEC_UP, -9.8);

    Vector3 totalForce = Vector3Add(forceThrust, forceDrag);
    totalForce = Vector3Add(totalForce, forceGravity);
    Vector3 acceleration = totalForce;

    PlayerVelocity = Vector3Add(PlayerVelocity, Vector3Scale(acceleration, ft));
    PlayerPos = Vector3Add(PlayerPos, Vector3Scale(PlayerVelocity, ft));

    // movement
    // fix rotation
    Quaternion fixQ = QuaternionFromVector3ToVector3(PLAYER_MODEL_LOCAL_FORWARD, VEC_FORWARD);
    Matrix matFix = QuaternionToMatrix(fixQ);
    Matrix matRot = QuaternionToMatrix(PlayerRot);
    Matrix matTrans = MatrixTranslate(PlayerPos.x, PlayerPos.y, PlayerPos.z);
    // Combine: Fix -> Rotate -> Translate
    PlayerModel.transform = MatrixMultiply(matFix, MatrixMultiply(matRot, matTrans));

    // camera
    Vector3 camDesiredOffset = Vector3RotateByQuaternion(PLAYER_CAMERA_OFFSET, PlayerRot);
    Vector3 desiredCamPos = Vector3Add(PlayerPos, camDesiredOffset);
    PlayerCamera.position = Vector3Lerp(PlayerCamera.position, desiredCamPos, CAMERA_LERP * ft);
    PlayerCamera.target = PlayerPos;

    // draw
    DrawModelWires(PlayerModel,VEC_ZERO, 1, PINK);

    // debug
    // Draw Velocity Vector (Visualizing Drift)
    DrawLine3D(PlayerPos, Vector3Add(PlayerPos, PlayerVelocity), RED);
    // Draw Facing Vector
    DrawLine3D(PlayerPos, Vector3Add(PlayerPos, Vector3Scale(currentForward, 10)), GREEN);
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
