#include "random.h"

#include <stdlib.h>
#include "raymath.h"

float GetRandomFloat()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float GetRandomFloatInRange(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

Vector2 GetRandomVector2InRange(float min, float max)
{
    return Vector2
    {
        GetRandomFloatInRange(min, max),
        GetRandomFloatInRange(min, max)
    };
}

Vector2 GetRandomDirection()
{
    auto angle = GetRandomFloatInRange(0, PI * 2);
    return Vector2
    {
        cos(angle),
        sin(angle)
    };
}

