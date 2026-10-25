#ifndef RAGNAROK_SOURCE_CONTENTS_MODELS_MODELSPATIAL_H
#define RAGNAROK_SOURCE_CONTENTS_MODELS_MODELSPATIAL_H

struct OrientedBounds {
  float halfSize[3];
  float center[3];
  float axes[3][3];

  bool Intersects(const float* bounds) const;
};

static_assert(sizeof(OrientedBounds) == 60);

#endif
