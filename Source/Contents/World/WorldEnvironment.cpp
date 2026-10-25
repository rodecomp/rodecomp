#include "Contents/World/WorldEnvironment.h"
#include "Render/Renderer.h"
#include "Resource/World/WorldResource.h"

// OFFSET: 0x00AC94C0
// FUNCTION: fn_17684
void PublishWorldEnvironment(
  const C3dWorldRes& resource, int* modeBounds, WorldSpatialTree*& serialized, WorldEnvironmentLighting& lighting, unsigned char& rendererFlag,
  CRenderer& renderer
) {
  serialized = resource.spatial;
  modeBounds[0] = resource.bounds[2];
  modeBounds[1] = resource.bounds[3];
  modeBounds[2] = resource.bounds[0];
  modeBounds[3] = resource.bounds[1];

  lighting.direction[0] = resource.lightDirection[0];
  lighting.direction[1] = resource.lightDirection[1];
  lighting.direction[2] = resource.lightDirection[2];
  lighting.diffuse[0] = resource.diffuse[0];
  lighting.diffuse[1] = resource.diffuse[1];
  lighting.diffuse[2] = resource.diffuse[2];
  lighting.ambient[0] = resource.ambient[0];
  lighting.ambient[1] = resource.ambient[1];
  lighting.ambient[2] = resource.ambient[2];

  rendererFlag = resource.flag;
  renderer.SetLighting(lighting.direction, lighting.diffuse, lighting.ambient);
}
