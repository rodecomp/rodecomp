#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_MANDRAATTACKEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_MANDRAATTACKEFFECT_H

#include "Contents/Actors/RenderObject.h"

#include <cstddef>

class CMandraAttackEffect : public CRenderObject {
public:
  CMandraAttackEffect();
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) override;

  /* 0138 */ float startFrame;
  /* 013C */ int padding_013C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMandraAttackEffect) == 0x140);
static_assert(offsetof(CMandraAttackEffect, startFrame) == 0x138);
#endif

#endif
