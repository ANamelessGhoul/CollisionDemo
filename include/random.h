#ifndef RANDOM_H
#define RANDOM_H

#include "raylib.h"


float GetRandomFloat();

float GetRandomFloatInRange(float min, float max);

Vector2 GetRandomVector2InRange(float min, float max);

Vector2 GetRandomDirection();

#endif // RANDOM_H
