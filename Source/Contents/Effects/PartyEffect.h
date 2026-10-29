#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTYEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTYEFFECT_H

#include <list>

struct AttachedEffect;
struct CEffectPrim;
struct TerrainHeight;
class ResourceMgr;
class TextureCache;
class CRenderObject;

bool UpdatePartyEffect(
  AttachedEffect& self, ResourceMgr& resources, std::list<CRenderObject*>& worldObjects, TextureCache& textures, const TerrainHeight& ground,
  int updateTick
);

#endif
