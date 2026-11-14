#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_MSGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_MSGEFFECT_H

#include "Contents/Effects/Core/EffectActor.h"

#include <cstddef>
#include <string>

class CGameActor;

extern bool g_msgEffectShowSpecial;

class CMsgEffect : public EffectActor {
public:
  CMsgEffect();
  static bool UsesMsgEffect(int type);

  ~CMsgEffect() override;
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) override;
  void Render(int argument) override;
  void FollowOwner();

  /* 0158 */ bool m_visible;
  /* 0159 */ unsigned char padding_0159[0x03];
  /* 015C */ CGameActor* m_owner;
  /* 0160 */ vector3d m_drift;
  /* 016C */ vector3d origin;
  /* 0178 */ vector3d m_velocity;
  /* 0184 */ int m_value;
  /* 0188 */ int m_finished;
  /* 018C */ float travelDistance;
  /* 0190 */ float m_baseY;
  /* 0194 */ float m_baseX;
  /* 0198 */ float m_scale;
  /* 019C */ float initialVelocityX;
  /* 01A0 */ int processTick;
  /* 01A4 */ int previousAnimation;
  /* 01A8 */ int m_type;
  /* 01AC */ int m_digit;
  /* 01B0 */ int motionInitialized;
  /* 01B4 */ int activeMotion;
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
