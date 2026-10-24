#ifndef RAGNAROK_SOURCE_CONTENTS_WORLDOCCLUSION_H
#define RAGNAROK_SOURCE_CONTENTS_WORLDOCCLUSION_H

struct WorldSpatialTree;
class GroundGeometry;
struct RenderProjection;

// Recovered traversal within CGameMode. Spatial query keeps its
// original fixed limit; actor and ground rejection limits are independent.
void MarkWorldOcclusionRay(
  const float* ray,
  float actorLimit,
  float groundLimit,
  WorldSpatialTree& root,
  GroundGeometry& ground,
  bool markGround
);

// Caller has established world/player/renderer prerequisites. Player screen Y
// is shifted by renderer +0x1C times 23, then truncated before ray construction.
void MarkScreenWorldOcclusion(
  const RenderProjection& projection,
  int screenX,
  int screenY,
  float rendererScale,
  const float* origin,
  const float* cameraMatrix,
  float playerHeight,
  float cameraHeight,
  WorldSpatialTree& root,
  GroundGeometry& ground,
  bool markGround
);

#endif
