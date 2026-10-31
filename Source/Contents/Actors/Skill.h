#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_SKILL_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_SKILL_H

#include "Contents/Actors/GameActor.h"

#include <cstddef>
#include <cstdint>
#include <vector>

class C3dActor;
struct CRagEffect;

class CSkill : public CGameActor {
public:
  ~CSkill() override;

  const char* GetTrapModelName();
  void ReleaseModel();
  void LaunchGroundEffects(int effectId, int unused, int duration);
  void Render(int argument) override;

  /* 0410 */ unsigned char unknown_0410[0x04];
  /* 0414 */ unsigned m_effectTimer;
  /* 0418 */ std::uint32_t skillAid;
  /* 041C */ C3dActor* m_model;
  /* 0420 */ CRagEffect* m_effect;
  /* 0424 */ int m_effectId;
  /* 0428 */ bool m_hasEffect;
  /* 0429 */ unsigned char unknown_0429[0x03];
  /* 042C */ int range;
  /* 0430 */ std::vector<std::uint64_t> m_targets;
  /* 043C */ int unknown_043C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSkill) == 0x440);
static_assert(offsetof(CSkill, skillAid) == 0x418);
static_assert(offsetof(CSkill, m_hasEffect) == 0x428);
static_assert(offsetof(CSkill, range) == 0x42C);
static_assert(offsetof(CSkill, m_targets) == 0x430);
#endif

#endif
