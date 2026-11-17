#ifndef RAGNAROK_SOURCE_CONTENTS_MODELS_MODELSPATIAL_H
#define RAGNAROK_SOURCE_CONTENTS_MODELS_MODELSPATIAL_H

struct OrientedBounds {
  /* 0098 */ float halfSize[3];
  /* 00A4 */ float center[3];
  /* 00B0 */ float axes[3][3];
};

static_assert(sizeof(OrientedBounds) == 60);

bool IntersectsBounds(const OrientedBounds& box, const float* bounds);

#endif
