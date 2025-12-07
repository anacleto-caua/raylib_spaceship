#pragma once

// Raylib is Right-Handed, Y-Up (OpenGL Convention)
#define VEC_ZERO    (Vector3){ 0.0f, 0.0f, 0.0f }
#define VEC_ONE     (Vector3){ 1.0f, 1.0f, 1.0f }

// Directions
#define VEC_RIGHT   (Vector3){ 1.0f, 0.0f, 0.0f }
#define VEC_UP      (Vector3){ 0.0f, 1.0f, 0.0f }
#define VEC_FORWARD (Vector3){ 0.0f, 0.0f, 1.0f }

#define VEC2_ZERO    (Vector2){ 0.0f, 0.0f }
#define VEC2_ONE     (Vector2){ 1.0f, 1.0f }

#define VEC2_RIGHT   (Vector2){ 1.0f, 0.0f }
#define VEC2_UP      (Vector2){ 0.0f, 1.0f }