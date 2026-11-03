#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDOCCLUSION_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDOCCLUSION_H

struct WorldSpatialTree;
class GroundGeometry;

void MarkWorldOcclusionRay(const float* ray, float actorLimit, float groundLimit, WorldSpatialTree& root, GroundGeometry& ground, bool markGround);

void MarkScreenWorldOcclusion(
  int screenX, int screenY, float rendererScale, const float* origin, const float* cameraMatrix, float playerHeight,
  float cameraHeight, WorldSpatialTree& root, GroundGeometry& ground, bool markGround
);

#endif
