#pragma once

const float pi = 3.1415927f;

inline float rad2deg(float radians) {
    return radians * 180.0f / pi;
}

inline float deg2rad(float degree) {
    return degree * pi / 180.f;
}