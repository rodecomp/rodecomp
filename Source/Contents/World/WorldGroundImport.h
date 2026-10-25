#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDGROUNDIMPORT_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDGROUNDIMPORT_H

class C3dWorldRes;
class C3dAttr;
class GroundGeometry;
class ResourceMgr;
class TextureCache;
class CRenderSystemBase;

bool ImportWorldGround(
  const C3dWorldRes& world, ResourceMgr& resources, TextureCache& textures, CRenderSystemBase* renderSystem, C3dAttr*& terrain, GroundGeometry*& ground,
  int& nearbyRange, const float* lightDirection, const float* diffuse, const float* ambient
);

#endif
