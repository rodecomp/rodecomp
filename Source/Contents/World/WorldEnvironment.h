#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDENVIRONMENT_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDENVIRONMENT_H

class C3dWorldRes;
struct WorldSpatialTree;
class CRenderer;

struct WorldEnvironmentLighting {
  float direction[3];
  float diffuse[3];
  float ambient[3];
};

void PublishWorldEnvironment(
  const C3dWorldRes& resource, int* modeBounds, WorldSpatialTree*& serialized, WorldEnvironmentLighting& lighting, unsigned char& rendererFlag,
  CRenderer& renderer
);

#endif
