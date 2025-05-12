#pragma once
#include "raymath.h"

RMAPI Vector3 MatrixColX(Matrix m)
{
    return { m.m0, m.m1, m.m2 };
}

RMAPI Vector3 MatrixColY(Matrix m)
{
    return { m.m4, m.m5, m.m6 };
}

RMAPI Vector3 MatrixColZ(Matrix m)
{
    return { m.m8, m.m9, m.m10 };
}
