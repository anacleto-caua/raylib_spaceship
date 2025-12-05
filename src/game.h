#pragma once

#include "raylib.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const char* SCREEN_TITLE;

extern const Vector3 VECTOR3_ZERO;
extern const Vector3 VECTOR3_ONE;

extern const Vector3 VECTOR3_FORWARD;
extern const Vector3 VECTOR3_UP;
extern const Vector3 VECTOR3_RIGHT;

extern int FPS_TARGET;

void InitGame();
void Update();
void EndGame();