#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_MSGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_MSGEFFECT_H

#include "Contents/Effects/EffectActor.h"

#include <cstddef>
#include <string>

extern int g_damageMillionUnit;
extern std::string g_damageResourceName;
extern bool g_msgEffectShowSpecial;

class CMsgEffect : public EffectActor {
public:
  CMsgEffect();
  static bool UsesMsgEffect(int type);

  ~CMsgEffect() override;
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;
  void Render(int argument) override;
  void FollowOwner();

  /* 0158 */ bool m_visible;
  /* 0159 */ unsigned char unknown_0159[0x03];
  /* 015C */ CGameObject* m_owner;
  /* 0160 */ vector3d m_drift;
  /* 016C */ vector3d unknown_016C;
  /* 0178 */ vector3d m_velocity;
  /* 0184 */ int m_value;
  /* 0188 */ int m_finished;
  /* 018C */ float unknown_018C;
  /* 0190 */ float m_baseY;
  /* 0194 */ float m_baseX;
  /* 0198 */ float m_scale;
  /* 019C */ float unknown_019C;
  /* 01A0 */ int unknown_01A0;
  /* 01A4 */ int unknown_01A4;
  /* 01A8 */ int m_type;
  /* 01AC */ int m_digit;
  /* 01B0 */ int unknown_01B0;
  /* 01B4 */ int unknown_01B4;
};

class CSummonActorEffect : public CMsgEffect {
public:
  bool OnProcess() override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMsgEffect) == 0x1B8);
static_assert(sizeof(CSummonActorEffect) == 0x1B8);
static_assert(offsetof(CMsgEffect, m_owner) == 0x15C);
static_assert(offsetof(CMsgEffect, m_velocity) == 0x178);
static_assert(offsetof(CMsgEffect, m_type) == 0x1A8);
#endif

#endif
