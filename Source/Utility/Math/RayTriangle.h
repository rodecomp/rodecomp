#ifndef RAGNAROK_SOURCE_UTILITY_MATH_RAYTRIANGLE_H
#define RAGNAROK_SOURCE_UTILITY_MATH_RAYTRIANGLE_H

bool IntersectRayTriangle(const float* ray, const float* first, const float* second, const float* third, float& u, float& v, float& distance);

#endif
