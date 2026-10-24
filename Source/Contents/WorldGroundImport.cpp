#include "Client/ErrorDialog.h"
#include "Contents/GroundGeometry.h"
#include "Contents/WorldGroundImport.h"
#include "Resource/GroundResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TerrainResource.h"
#include "Resource/WorldResource.h"

#include <bit>
#include <emmintrin.h>

bool ImportWorldGround(
  const C3dWorldRes& world,
  ResourceMgr& resources,
  TextureCache& textures,
  CRenderSystemBase* renderSystem,
  C3dAttr*& terrain,
  GroundGeometry*& ground,
  int& nearbyRange,
  const float* lightDirection,
  const float* diffuse,
  const float* ambient
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
  nearbyRange = std::bit_cast<int>(static_cast<unsigned>(nearbyRange) * static_cast<unsigned>(terrain->cellSize));

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
  ground->Build(*resource, textures, lightDirection, diffuse, ambient);
  const int pitch = _mm_cvttss_si32(_mm_set_ss(world.waveSpeed));
  const int phase = _mm_cvttss_si32(_mm_set_ss(world.wavePitch));
  ground->water.Set(world.waterLevel, world.waterType, world.waterAnimation, pitch, phase, world.waveHeight);
  ground->water.Import(*resource, textures);
  resources.Remove(resource);
  return true;
}
