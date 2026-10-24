#ifndef RAGNAROK_SOURCE_CONTENTS_WORLDTERRAINPICKING_H
#define RAGNAROK_SOURCE_CONTENTS_WORLDTERRAINPICKING_H

struct WorldSpatialTree;
struct RenderProjection;

// Recovered terrain scan after camera ray construction.
bool PickTerrainRay(const float* ray, WorldSpatialTree& root, int& column, int& row);

// Explicit original renderer, camera and cursor inputs to CGameMode picking.
bool PickScreenTerrain(
  const RenderProjection& projection,
  int x,
  int y,
  const float* origin,
  const float* cameraMatrix,
  WorldSpatialTree& root,
  int& column,
  int& row
);

#endif
