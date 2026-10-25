#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDOCCLUSION_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDOCCLUSION_H

struct WorldSpatialTree;
class GroundGeometry;
struct RenderProjection;

void MarkWorldOcclusionRay(const float* ray, float actorLimit, float groundLimit, WorldSpatialTree& root, GroundGeometry& ground, bool markGround);

void MarkScreenWorldOcclusion(
  const RenderProjection& projection, int screenX, int screenY, float rendererScale, const float* origin, const float* cameraMatrix, float playerHeight,
  float cameraHeight, WorldSpatialTree& root, GroundGeometry& ground, bool markGround
);

#endif
