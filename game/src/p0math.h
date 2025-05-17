#pragma once
#include "raymathext.h"

RMAPI Vector2 p0Vector2Direction(float radians)
{
    Vector2 v = Vector2Rotate(Vector2UnitY, radians);
    return v;
}

RMAPI float p0Vector2Angle(Vector2 direction)
{
    float angle = Vector2Angle(Vector2UnitY, direction);
    return angle;
}

RMAPI Vector3 p0Vector3Direction(float radians)
{
    Vector2 v = Vector2Rotate(Vector2UnitY, radians);
    return { v.x, v.y, 0.0f };
}

RMAPI float p0Vector3Angle(Vector3 direction)
{
    float angle = Vector2Angle(Vector2UnitY, { direction.x, direction.y });
    return angle;
}

RMAPI Matrix p0MatrixLookRotation(Vector3 direction)
{
    return MatrixLookRotation(Vector3UnitZ, direction);
}
