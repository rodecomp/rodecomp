#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_GROUNDSCOPE_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_GROUNDSCOPE_H

#include "Contents/Effects/EffectActor.h"

#include <cstddef>

extern int g_updateTick;

class CGroundScope : public EffectActor {
public:
  CGroundScope();
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;
  void Render(int argument) override;

  int SkillScopeType(int skillId);

  /* 0158 */ int unknown_0158;
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
