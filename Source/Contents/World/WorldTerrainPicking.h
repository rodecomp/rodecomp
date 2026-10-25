#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDTERRAINPICKING_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDTERRAINPICKING_H

struct WorldSpatialTree;
struct RenderProjection;

bool PickTerrainRay(const float* ray, WorldSpatialTree& root, int& column, int& row);

bool PickScreenTerrain(
  const RenderProjection& projection, int x, int y, const float* origin, const float* cameraMatrix, WorldSpatialTree& root, int& column, int& row
);

#endif
