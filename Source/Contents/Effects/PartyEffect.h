#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTYEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTYEFFECT_H

#include <list>

struct CRagEffect;
struct CEffectPrim;
struct TerrainHeight;
class ResourceMgr;
class TextureCache;
class CRenderObject;

bool UpdatePartyEffect(
  CRagEffect& self, ResourceMgr& resources, std::list<CRenderObject*>& worldObjects, const TerrainHeight& ground,
  int updateTick
);

#endif
