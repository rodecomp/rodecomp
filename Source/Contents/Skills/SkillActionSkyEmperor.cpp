#include "Contents/Actors/GameActor.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/Vector3d.h"

#include <algorithm>
#include <cstdlib>

class CSkillObjectAction_SKE_STAR_CANNON : public CSkillObjectAction {
public:
  void Slot00(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_SKE_ALL_IN_THE_SKY : public CSkillAction {
public:
  int Slot02() override;
};

class CEfstAction_SKY_ENCHANT : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SKE_SKY_SUN : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_SKY_MOON : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_STAR_LIGHT_KICK : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_RISING_SUN : public CSkillAction {
public:
  int Slot02() override;
};

class CEfstAction_RISING_SUN : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_NOON_SUN : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_SUNSET_SUN : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SKE_NOON_BLAST : public CSkillAction {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_SUNSET_BLAST : public CSkillAction {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_RISING_MOON : public CSkillAction {
public:
  int Slot02() override;
};

class CEfstAction_RISING_MOON : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_MIDNIGHT_MOON : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_DAWN_MOON : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SKE_MIDNIGHT_KICK : public CSkillAction {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_DAWN_BREAK : public CSkillAction {
public:
  int Slot02() override;
};

// OFFSET: 0x007975A0
// FUNCTION: fn_08815
void CSkillObjectAction_SKE_STAR_CANNON::Slot00(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    g_effectMgr->Spawn(object->render.accountId, 2350, object->render.position.x, object->render.position.y, object->render.position.z, 0, argument, 0, 0);
  }
}

// OFFSET: 0x007975F0
// FUNCTION: fn_08816
int CSkillAction_SKE_ALL_IN_THE_SKY::Slot02() {
  return 53;
}

// OFFSET: 0x007977E0
// FUNCTION: fn_08820
void CEfstAction_SKY_ENCHANT::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_enchanting_sky_00"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_enchanting_sky_01"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00797A70
// FUNCTION: fn_08821
void CEfstAction_SKY_ENCHANT::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_enchanting_sky_01"));
  }
}

// OFFSET: 0x00797BB0
// FUNCTION: fn_08822
void CEfstAction_SKY_ENCHANT::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_enchanting_sky_01"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00797D40
// FUNCTION: fn_08824
void CSkillAction_SKE_SKY_SUN::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_sun"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  for (int index = 0; index < 6; ++index) {
    CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("sky_sun_small"), target->render.position.x, target->render.position.y,
                                         target->render.position.z, 0, -1, 0, 0);
    if (!effect) {
      return;
    }

    const int offsetZ = 15 - std::max(0, std::rand() % 30);
    const int offsetX = 15 - std::max(0, std::rand() % 30);
    effect->position[0] = target->render.position.x + offsetX;
    effect->position[1] = target->render.position.y + 0.0f;
    effect->position[2] = target->render.position.z + offsetZ;
    effect->Slot16(index * 32);
  }
}

// OFFSET: 0x007980C0
// FUNCTION: fn_08825
void CSkillAction_SKE_SKY_SUN::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_sun_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00798240
// FUNCTION: fn_08827
void CSkillAction_SKE_SKY_MOON::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_moon_cast"),
                                             0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_moon_cast_bottom"),
                                             0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007984D0
// FUNCTION: fn_08828
void CSkillAction_SKE_SKY_MOON::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const float scale = source->unknown_0378 * source->render.speed * 16.0f;
  if (CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_moon"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot16(static_cast<int>(scale));
  }
  if (CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_moon_bottom"),
                                           0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot16(static_cast<int>(scale));
  }
}

// OFFSET: 0x00798770
// FUNCTION: fn_08829
void CSkillAction_SKE_SKY_MOON::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_moon_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007988D0
// FUNCTION: fn_08830
void CSkillAction_SKE_STAR_LIGHT_KICK::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  source->LaunchEffect(2426, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("star_light_kick"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("star_light_kick_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00798B90
// FUNCTION: fn_08831
void CSkillAction_SKE_STAR_LIGHT_KICK::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("star_light_kick_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00798D10
// FUNCTION: fn_08832
int CSkillAction_SKE_RISING_SUN::Slot02() {
  return 52;
}

// OFFSET: 0x00798D20
// FUNCTION: fn_08833
void CEfstAction_RISING_SUN::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_00"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_01"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00798FA0
// FUNCTION: fn_08834
void CEfstAction_RISING_SUN::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_01"));
  }
}

// OFFSET: 0x007990D0
// FUNCTION: fn_08835
void CEfstAction_RISING_SUN::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_01"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00799250
// FUNCTION: fn_08836
void CEfstAction_NOON_SUN::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_02"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_03"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007994D0
// FUNCTION: fn_08837
void CEfstAction_NOON_SUN::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_03"));
  }
}

// OFFSET: 0x00799600
// FUNCTION: fn_08838
void CEfstAction_NOON_SUN::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_03"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00799780
// FUNCTION: fn_08839
void CEfstAction_SUNSET_SUN::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_04"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_05"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00799A00
// FUNCTION: fn_08840
void CEfstAction_SUNSET_SUN::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_05"));
  }
}

// OFFSET: 0x00799B30
// FUNCTION: fn_08841
void CEfstAction_SUNSET_SUN::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_05"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00799CB0
// FUNCTION: fn_08842
int CSkillAction_SKE_NOON_BLAST::Slot02() {
  return 52;
}

// OFFSET: 0x00799CC0
// FUNCTION: fn_08843
void CSkillAction_SKE_NOON_BLAST::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_noon_blast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00799E30
// FUNCTION: fn_08844
int CSkillAction_SKE_SUNSET_BLAST::Slot02() {
  return 52;
}

// OFFSET: 0x00799E40
// FUNCTION: fn_08845
void CSkillAction_SKE_SUNSET_BLAST::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sunset_blast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  for (int index = 0; index < 6; ++index) {
    CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("sunset_blast_small"), target->render.position.x,
                                         target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    if (!effect) {
      return;
    }

    const int offsetZ = 15 - std::max(0, std::rand() % 30);
    const int offsetX = 15 - std::max(0, std::rand() % 30);
    effect->position[0] = target->render.position.x + offsetX;
    effect->position[1] = target->render.position.y + 0.0f;
    effect->position[2] = target->render.position.z + offsetZ;
    effect->Slot16(index * 32);
  }
}

// OFFSET: 0x0079A1C0
// FUNCTION: fn_08846
int CSkillAction_SKE_RISING_MOON::Slot02() {
  return 53;
}

// OFFSET: 0x0079A1D0
// FUNCTION: fn_08847
void CEfstAction_RISING_MOON::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_00"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_01"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079A450
// FUNCTION: fn_08848
void CEfstAction_RISING_MOON::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_01"));
  }
}

// OFFSET: 0x0079A580
// FUNCTION: fn_08849
void CEfstAction_RISING_MOON::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_01"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079A700
// FUNCTION: fn_08850
void CEfstAction_MIDNIGHT_MOON::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_02"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_03"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079A980
// FUNCTION: fn_08851
void CEfstAction_MIDNIGHT_MOON::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_03"));
  }
}

// OFFSET: 0x0079AAB0
// FUNCTION: fn_08852
void CEfstAction_MIDNIGHT_MOON::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_03"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079AC30
// FUNCTION: fn_08853
void CEfstAction_DAWN_MOON::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_04"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_05"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079AEB0
// FUNCTION: fn_08854
void CEfstAction_DAWN_MOON::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_05"));
  }
}

// OFFSET: 0x0079AFE0
// FUNCTION: fn_08855
void CEfstAction_DAWN_MOON::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_05"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079B160
// FUNCTION: fn_08856
int CSkillAction_SKE_MIDNIGHT_KICK::Slot02() {
  return 53;
}

// OFFSET: 0x0079B1A0
// FUNCTION: fn_08858
void CSkillAction_SKE_MIDNIGHT_KICK::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_midnight_kick"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079B320
// FUNCTION: fn_08859
int CSkillAction_SKE_DAWN_BREAK::Slot02() {
  return 53;
}
