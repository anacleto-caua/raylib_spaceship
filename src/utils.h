#pragma once

// private

static inline void _cap_int(int* value, int min, int max) {
    if(*value > max) *value = max;
    else if(*value < min) *value = min;
}

static inline void _cap_float(float* value, float min, float max) {
    if(*value > max) *value = max;
    else if(*value < min) *value = min;
}

// public helper

#define cap(x, min, max) _Generic((x), \
    int*:   _cap_int, \
    float*: _cap_float \
)(x, min, max)