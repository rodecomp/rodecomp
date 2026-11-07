#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTACTOR_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTACTOR_H

#include "Contents/Actors/RenderObject.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <list>

struct CRagEffect;
struct RagEffectState;

struct EffectActorState {
  EffectActorState();
  ~EffectActorState();

  /* 0138 */ int unknown_0138;
  /* 013C */ std::uint32_t unknown_013C;
  /* 0140 */ std::uint32_t unknown_0140;
  /* 0144 */ std::list<CRagEffect*> effects;
  /* 014C */ CRagEffect* first;
  /* 0150 */ CRagEffect* second;
};

struct EffectActor : CRenderObject {
  EffectActor();
  ~EffectActor() override;
  void SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;
  bool EffectVisible() const;
  bool HasId(int id) const;
  bool Contains(const RagEffectState* effect) const;
  void ResetEffectState();
  CRagEffect* LaunchEffect(int effectId, vector3d deltaPos, float rotation);
  CRagEffect* LaunchEffect(int effectId, int duration);
  CRagEffect* LaunchTaggedEffect(int effectId, std::uint32_t tag);

  /* 0138 */ EffectActorState state;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EffectActorState) == 28);
static_assert(sizeof(EffectActor) == 0x158);
static_assert(offsetof(EffectActor, state) == 0x138);
#endif

#endif
