#ifndef RAGNAROK_SOURCE_CONTENTS_MODELS_MODELSPATIAL_H
#define RAGNAROK_SOURCE_CONTENTS_MODELS_MODELSPATIAL_H

struct OrientedBounds {
  /* 0098 */ float halfSize[3];
  /* 00A4 */ float center[3];
  /* 00B0 */ float axes[3][3];
};

static_assert(sizeof(OrientedBounds) == 60);

struct BoundsCorner {
  float values[3];

  // INLINED
  BoundsCorner() {
    values[0] = 0.0f;
    values[1] = 0.0f;
    values[2] = 0.0f;
  }

  // INLINED
  __forceinline void Set(float x, float y, float z) {
    values[0] = x;
    values[1] = y;
    values[2] = z;
  }
};

static_assert(sizeof(BoundsCorner) == 12);

bool IntersectsBounds(const OrientedBounds& box, const float* bounds);

#endif
