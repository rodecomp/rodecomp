#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GROUNDSCOPE_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GROUNDSCOPE_H

#include "Contents/Effects/Core/EffectActor.h"

#include <cstddef>

extern int g_updateTick;

class CGroundScope : public EffectActor {
public:
  CGroundScope();
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) override;
  void Render(int argument) override;

  int SkillScopeType(int skillId);

  /* 0158 */ int m_masterGid;
  /* 015C */ int m_cellX;
  /* 0160 */ int m_cellY;
  /* 0164 */ unsigned long m_lastingTime;
  /* 0168 */ int m_size;
  /* 016C */ float m_angle;
  /* 0170 */ float m_angleSpeed;
  /* 0174 */ int m_skillId;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CGroundScope, m_lastingTime) == 0x164);
static_assert(offsetof(CGroundScope, m_skillId) == 0x174);
#endif

#endif
