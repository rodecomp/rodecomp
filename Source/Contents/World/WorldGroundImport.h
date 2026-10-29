#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDGROUNDIMPORT_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDGROUNDIMPORT_H

#include "Client/Application/ErrorDialog.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Resource/World/GroundResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/World/TerrainResource.h"
#include "Resource/World/WorldResource.h"
#include "Utility/Math/ScalarConversion.h"

// INLINED
inline bool ImportWorldGround(
  const C3dWorldRes& world, ResourceMgr& resources, CRenderSystemBase* renderSystem, C3dAttr*& terrain, GroundGeometry*& ground,
  int& nearbyRange, const float* lightDirection, const float* diffuse, const float* ambient
) {
  if (world.terrainName.empty()) {
    ShowErrorDialog(renderSystem, "attr ", 0);
    return false;
  }

  terrain = static_cast<C3dAttr*>(resources.Load(world.terrainName.c_str()));

  if (!terrain) {
    ShowErrorDialog(renderSystem, "attr ", 0);
    return false;
  }

  terrain->cellSize = 5;
  nearbyRange *= terrain->cellSize;

  if (world.groundName.empty()) {
    ShowErrorDialog(renderSystem, "gnd ", 0);
    return false;
  }

  auto* resource = static_cast<CGndRes*>(resources.Load(world.groundName.c_str()));

  if (!resource) {
    ShowErrorDialog(renderSystem, "ground", 0);
    return false;
  }

  if (!resource->loaded) {
    ShowErrorDialog(renderSystem, "", 0);
    return false;
  }

  ground = new GroundGeometry;
  ground->Build(*resource, lightDirection, diffuse, ambient);
  const int pitch = TruncateToInt(world.waveSpeed);
  const int phase = TruncateToInt(world.wavePitch);
  ground->water.Set(world.waterLevel, world.waterType, world.waterAnimation, pitch, phase, world.waveHeight);
  ground->water.Import(*resource);
  resources.Remove(resource);

  return true;
}

#endif
