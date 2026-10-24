#ifndef RAGNAROK_SOURCE_CONTENTS_MODELSPATIAL_H
#define RAGNAROK_SOURCE_CONTENTS_MODELSPATIAL_H

// Model's oriented box at +0x98: half extents, center, then three local axes.
struct OrientedBounds {
  float halfSize[3];
  float center[3];
  float axes[3][3];

  bool Intersects(const float* bounds) const;
};

static_assert(sizeof(OrientedBounds) == 60);

#endif
