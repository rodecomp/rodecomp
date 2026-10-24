#ifndef RAGNAROK_SOURCE_CONTENTS_WORLDENVIRONMENT_H
#define RAGNAROK_SOURCE_CONTENTS_WORLDENVIRONMENT_H

class C3dWorldRes;
struct WorldSpatialTree;
class CRenderer;

// Shared import-light globals, distinct from the
// resource loader's diffuse/ambient globals.
struct WorldEnvironmentLighting {
  float direction[3];
  float diffuse[3];
  float ambient[3];
};

// Borrowed mode +0x58..+0x67 bounds, CWorld +0xD4 serialized tree and renderer
// +0x48 byte. World resource ownership remains with ResourceMgr.
void PublishWorldEnvironment(
  const C3dWorldRes& resource,
  int* modeBounds,
  WorldSpatialTree*& serialized,
  WorldEnvironmentLighting& lighting,
  unsigned char& rendererFlag,
  CRenderer& renderer
);

#endif
