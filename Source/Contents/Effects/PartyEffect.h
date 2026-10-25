#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTYEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTYEFFECT_H

#include <list>

struct RagEffectState;
struct RenderObjectState;
struct EffectParticle;
struct JobNames;
struct TerrainHeight;
class ResourceMgr;
class TextureCache;
class CRenderObject;

bool UpdatePartyEffect(
  RagEffectState& effect, RenderObjectState& render, ResourceMgr& resources, JobNames& names, std::list<CRenderObject*>& worldObjects, TextureCache& textures,
  const TerrainHeight& ground, int updateTick
);

#endif
