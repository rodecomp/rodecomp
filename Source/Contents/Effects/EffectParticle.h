#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPARTICLE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPARTICLE_H

#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Actors/RenderObject.h"

#include <list>

struct JobNames;
class ResourceMgr;
struct RagEffectState;

struct EffectParticle : CRenderObject {
  EffectParticle(ResourceMgr& resources, JobNames& names);

  bool UpdateFireWall();
  bool UpdateStoneCurse();
  void UpdateBeginSpell();
  bool UpdateHit6();
  bool UpdateCoin();
  bool UpdateDetoxication();
  bool UpdateExit();
  bool UpdateWarp();

  bool AdvanceFrame();
  void UpdateRecords();

  /* 0138 */ EffectPrimitiveState state;
};

EffectParticle* CreateEffectParticle(
  RagEffectState& parent, const RenderObjectState& parentRender, int type, float x, float y, float z, ResourceMgr& resources, JobNames& names,
  std::list<CRenderObject*>& worldObjects
);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EffectParticle) == 0xFA18);
static_assert(offsetof(EffectParticle, state) == 0x138);
#endif

#endif
