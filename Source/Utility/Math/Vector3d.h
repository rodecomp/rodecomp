#ifndef RAGNAROK_SOURCE_UTILITY_MATH_VECTOR3D_H
#define RAGNAROK_SOURCE_UTILITY_MATH_VECTOR3D_H

#include <cstddef>

struct vector3d {
  /* 0000 */ float x;
  /* 0004 */ float y;
  /* 0008 */ float z;
};

static_assert(sizeof(vector3d) == 12);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(vector3d, z) == 0x8);
#endif

#endif
