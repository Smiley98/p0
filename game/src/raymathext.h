#pragma once
#include "raymath.h"

RMAPI float Sign(float value)
{
    float result = value < 0.0f ? -1.0f : 1.0f;
    return result;
}

RMAPI float Vector2CrossProduct(Vector2 v1, Vector2 v2)
{
    float result = v1.x * v2.y - v1.y * v2.x;
    return result;
}

RMAPI Vector2 Vector2RotateTowards(Vector2 from, Vector2 to, float maxRadiansDelta)
{
    float angle = Vector2Angle(from, to);
    float angleDelta = fabsf(angle) > maxRadiansDelta ? maxRadiansDelta * Sign(angle) : angle;
    Vector2 result = Vector2Rotate(from, angleDelta);
    return result;
}

RMAPI Vector3 MatrixColX(Matrix m)
{
    Vector3 result = { m.m0, m.m1, m.m2 };
    return result;
}

RMAPI Vector3 MatrixColY(Matrix m)
{
    Vector3 result = { m.m4, m.m5, m.m6 };
    return result;
}

RMAPI Vector3 MatrixColZ(Matrix m)
{
    Vector3 result = { m.m8, m.m9, m.m10 };
    return result;
}
