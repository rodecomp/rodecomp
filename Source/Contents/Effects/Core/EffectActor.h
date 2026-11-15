#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_EFFECTACTOR_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_EFFECTACTOR_H

#include "Contents/Actors/RenderObject.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <list>

struct CRagEffect;

struct EffectActorState {
  EffectActorState();
  ~EffectActorState();

  /* 0138 */ int effectId;
  /* 013C */ int skillLevel;
  /* 0140 */ std::uint32_t effectLaunched;
  /* 0144 */ std::list<CRagEffect*> effects;
  /* 014C */ CRagEffect* first;
  /* 0150 */ CRagEffect* second;
};

struct EffectActor : CRenderObject {
  EffectActor();
  ~EffectActor() override;
  void SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) override;
  bool EffectVisible() const;
  bool HasId(int id) const;
  bool Contains(const CRagEffect* effect) const;
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
static_assert(offsetof(EffectActorState, skillLevel) == 0x4);
static_assert(offsetof(EffectActorState, effectLaunched) == 0x8);
#endif

#endif
