#pragma once

#include "raylib.h"

extern Camera3D camera;

extern Vector3 PlayerPos;
extern Vector3 CameraPos;
extern Vector3 PlayerMovement;
extern float playeSpeed;
extern float Throttle;
extern float MaxThrottle;
extern float ThrottleAcceleration;

void PlayerInit();

void PlayerUpdate();

void PlayerEnd();

void SpawnPlayer();


void HandleMovementInput();
void DebugPlayerInfo();