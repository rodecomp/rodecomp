#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_EFFECTACTOR_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_EFFECTACTOR_H

#include "Contents/Actors/RenderObject.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <list>

struct CRagEffect;

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

  /* 0138 */ int launchEffectId;
  /* 013C */ int launchSkillLevel;
  /* 0140 */ std::uint32_t effectLaunched;
  /* 0144 */ std::list<CRagEffect*> effectList;
  /* 014C */ CRagEffect* firstEffect;
  /* 0150 */ CRagEffect* secondEffect;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EffectActor) == 0x158);
static_assert(offsetof(EffectActor, launchEffectId) == 0x138);
static_assert(offsetof(EffectActor, effectList) == 0x144);
static_assert(offsetof(EffectActor, secondEffect) == 0x150);
#endif

#endif
