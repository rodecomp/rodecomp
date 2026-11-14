#include "Contents/Actors/GameActor.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/Vector3d.h"

#include <algorithm>
#include <cstdlib>

class CSkillObjectAction_SKE_STAR_CANNON : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_SKE_ALL_IN_THE_SKY : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnTargetSkillCast(CGameActor* source, CGameActor* target, int delay) override;
};

class CSkillAction_SKE_ENCHANTING_SKY : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
};

class CEfstAction_SKY_ENCHANT : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SKE_SKY_SUN : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, CGameActor* hitIndex) override;
};

class CSkillAction_SKE_SKY_MOON : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, CGameActor* hitIndex) override;
};

class CSkillAction_SKE_STAR_LIGHT_KICK : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, CGameActor* hitIndex) override;
};

class CSkillAction_SKE_RISING_SUN : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
};

class CEfstAction_RISING_SUN : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_NOON_SUN : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_SUNSET_SUN : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SKE_NOON_BLAST : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_SUNSET_BLAST : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_RISING_MOON : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
};

class CEfstAction_RISING_MOON : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_MIDNIGHT_MOON : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_DAWN_MOON : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SKE_MIDNIGHT_KICK : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_DAWN_BREAK : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_TWINKLING_GALAXY : public CSkillAction {
public:
  bool KeepSkillAttackMotion(int argument) override;
  bool KeepSkillAttackSpeed() override;
  void OnSkillCast(int level, CGameActor* source) override;
};

class CSkillObjectAction_SKE_TWINKLING_GALAXY : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_SKE_STAR_BURST : public CSkillAction {
public:
  bool KeepSkillAttackMotion(int argument) override;
  void OnSkillCast(int level, CGameActor* source) override;
};

class CEfstAction_STAR_BURST : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SKE_STAR_CANNON : public CSkillAction {
public:
  bool KeepSkillAttackMotion(int argument) override;
  bool KeepSkillAttackSpeed() override;
  void OnSkillCast(int level, CGameActor* source) override;
};

// OFFSET: 0x007975A0
// FUNCTION: fn_08815
void CSkillObjectAction_SKE_STAR_CANNON::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    g_effectMgr->Spawn(object->render.accountId, EF_SKE_STAR_CANNON, object->render.position, 0, argument, 0, 0);
  }
}

// OFFSET: 0x007975F0
// FUNCTION: fn_08816
int CSkillAction_SKE_ALL_IN_THE_SKY::GetSkillAttackMotion() {
  return 53;
}

// OFFSET: 0x00797600
// FUNCTION: fn_08817
void CSkillAction_SKE_ALL_IN_THE_SKY::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL_N8, level / 16);
  }
}

// OFFSET: 0x00797630
// FUNCTION: fn_08818
void CSkillAction_SKE_ALL_IN_THE_SKY::OnTargetSkillCast(CGameActor* source, CGameActor* target, int delay) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_all_in_the_sky"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007977B0
// FUNCTION: fn_08819
void CSkillAction_SKE_ENCHANTING_SKY::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL_N2, level / 16);
  }
}

// OFFSET: 0x007977E0
// FUNCTION: fn_08820
void CEfstAction_SKY_ENCHANT::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_enchanting_sky_00"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_enchanting_sky_01"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00797A70
// FUNCTION: fn_08821
void CEfstAction_SKY_ENCHANT::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_enchanting_sky_01"));
  }
}

// OFFSET: 0x00797BB0
// FUNCTION: fn_08822
void CEfstAction_SKY_ENCHANT::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_enchanting_sky_01"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00797D30
// FUNCTION: fn_08823
int CSkillAction_SKE_SKY_SUN::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00797D40
// FUNCTION: fn_08824
void CSkillAction_SKE_SKY_SUN::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_sun"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  for (int index = 0; index < 6; ++index) {
    effects = g_effectMgr;
    CEffect* effect = effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("sky_sun_small"), target->render.position, 0, -1, 0, 0);
    if (!effect) {
      return;
    }

    const int offsetZ = 15 - std::max(0, std::rand() % 30);
    const int offsetX = 15 - std::max(0, std::rand() % 30);
    effect->position[0] = target->render.position.x + offsetX;
    effect->position[1] = target->render.position.y + 0.0f;
    effect->position[2] = target->render.position.z + offsetZ;
    effect->SetDelay(index * 32);
  }
}

// OFFSET: 0x007980C0
// FUNCTION: fn_08825
void CSkillAction_SKE_SKY_SUN::OnSkillDamage(CGameActor* source, CGameActor* target, CGameActor* hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_sun_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00798230
// FUNCTION: fn_08826
int CSkillAction_SKE_SKY_MOON::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00798240
// FUNCTION: fn_08827
void CSkillAction_SKE_SKY_MOON::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_moon_cast"),
                                             vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_moon_cast_bottom"),
                                             vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007984D0
// FUNCTION: fn_08828
void CSkillAction_SKE_SKY_MOON::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const float scale = source->attackMotion * source->render.speed * 16.0f;
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_moon"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetDelay(static_cast<int>(scale));
  }
  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_moon_bottom"),
                                       vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetDelay(static_cast<int>(scale));
  }
}

// OFFSET: 0x00798770
// FUNCTION: fn_08829
void CSkillAction_SKE_SKY_MOON::OnSkillDamage(CGameActor* source, CGameActor* target, CGameActor* hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sky_moon_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007988D0
// FUNCTION: fn_08830
void CSkillAction_SKE_STAR_LIGHT_KICK::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  source->LaunchEffect(EF_FORCE_ANIM_27, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("star_light_kick"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("star_light_kick_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x00798B90
// FUNCTION: fn_08831
void CSkillAction_SKE_STAR_LIGHT_KICK::OnSkillDamage(CGameActor* source, CGameActor* target, CGameActor* hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("star_light_kick_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00798D10
// FUNCTION: fn_08832
int CSkillAction_SKE_RISING_SUN::GetSkillAttackMotion() {
  return 52;
}

// OFFSET: 0x00798D20
// FUNCTION: fn_08833
void CEfstAction_RISING_SUN::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_00"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_01"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00798FA0
// FUNCTION: fn_08834
void CEfstAction_RISING_SUN::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_01"));
  }
}

// OFFSET: 0x007990D0
// FUNCTION: fn_08835
void CEfstAction_RISING_SUN::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_01"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00799250
// FUNCTION: fn_08836
void CEfstAction_NOON_SUN::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_02"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_03"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007994D0
// FUNCTION: fn_08837
void CEfstAction_NOON_SUN::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_03"));
  }
}

// OFFSET: 0x00799600
// FUNCTION: fn_08838
void CEfstAction_NOON_SUN::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_03"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00799780
// FUNCTION: fn_08839
void CEfstAction_SUNSET_SUN::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_04"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_05"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00799A00
// FUNCTION: fn_08840
void CEfstAction_SUNSET_SUN::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_05"));
  }
}

// OFFSET: 0x00799B30
// FUNCTION: fn_08841
void CEfstAction_SUNSET_SUN::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_sun_05"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00799CB0
// FUNCTION: fn_08842
int CSkillAction_SKE_NOON_BLAST::GetSkillAttackMotion() {
  return 52;
}

// OFFSET: 0x00799CC0
// FUNCTION: fn_08843
void CSkillAction_SKE_NOON_BLAST::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_noon_blast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00799E30
// FUNCTION: fn_08844
int CSkillAction_SKE_SUNSET_BLAST::GetSkillAttackMotion() {
  return 52;
}

// OFFSET: 0x00799E40
// FUNCTION: fn_08845
void CSkillAction_SKE_SUNSET_BLAST::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sunset_blast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  for (int index = 0; index < 6; ++index) {
    effects = g_effectMgr;
    CEffect* effect = effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("sunset_blast_small"), target->render.position, 0, -1, 0, 0);
    if (!effect) {
      return;
    }

    const int rollZ = std::rand() % 30;
    const int offsetZ = 15 - (rollZ >= 0 ? rollZ : 0);
    const int rollX = std::rand() % 30;
    const int offsetX = 15 - (rollX >= 0 ? rollX : 0);
    effect->position[0] = target->render.position.x + offsetX;
    effect->position[1] = target->render.position.y + 0.0f;
    effect->position[2] = target->render.position.z + offsetZ;
    effect->SetDelay(index * 32);
  }
}

// OFFSET: 0x0079A1C0
// FUNCTION: fn_08846
int CSkillAction_SKE_RISING_MOON::GetSkillAttackMotion() {
  return 53;
}

// OFFSET: 0x0079A1D0
// FUNCTION: fn_08847
void CEfstAction_RISING_MOON::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_00"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_01"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079A450
// FUNCTION: fn_08848
void CEfstAction_RISING_MOON::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_01"));
  }
}

// OFFSET: 0x0079A580
// FUNCTION: fn_08849
void CEfstAction_RISING_MOON::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_01"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079A700
// FUNCTION: fn_08850
void CEfstAction_MIDNIGHT_MOON::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_02"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_03"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079A980
// FUNCTION: fn_08851
void CEfstAction_MIDNIGHT_MOON::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_03"));
  }
}

// OFFSET: 0x0079AAB0
// FUNCTION: fn_08852
void CEfstAction_MIDNIGHT_MOON::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_03"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079AC30
// FUNCTION: fn_08853
void CEfstAction_DAWN_MOON::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_04"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_05"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079AEB0
// FUNCTION: fn_08854
void CEfstAction_DAWN_MOON::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_05"));
  }
}

// OFFSET: 0x0079AFE0
// FUNCTION: fn_08855
void CEfstAction_DAWN_MOON::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_rising_moon_05"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079B160
// FUNCTION: fn_08856
int CSkillAction_SKE_MIDNIGHT_KICK::GetSkillAttackMotion() {
  return 53;
}

// OFFSET: 0x0079B170
// FUNCTION: fn_08857
void CSkillAction_SKE_MIDNIGHT_KICK::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL6, level / 16);
  }
}

// OFFSET: 0x0079B1A0
// FUNCTION: fn_08858
void CSkillAction_SKE_MIDNIGHT_KICK::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_midnight_kick"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079B320
// FUNCTION: fn_08859
int CSkillAction_SKE_DAWN_BREAK::GetSkillAttackMotion() {
  return 53;
}

// OFFSET: 0x0079B330
// FUNCTION: fn_08860
void CSkillAction_SKE_DAWN_BREAK::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL6, level / 16);
  }
}

// OFFSET: 0x0079B360
// FUNCTION: fn_08861
void CSkillAction_SKE_DAWN_BREAK::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ske_dawn_break"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079B4D0
// FUNCTION: fn_08862
bool CSkillAction_SKE_TWINKLING_GALAXY::KeepSkillAttackMotion(int argument) {
  return true;
}

// OFFSET: 0x0079B4E0
// FUNCTION: fn_08863
bool CSkillAction_SKE_TWINKLING_GALAXY::KeepSkillAttackSpeed() {
  return true;
}

// OFFSET: 0x0079B4F0
// FUNCTION: fn_08864
void CSkillAction_SKE_TWINKLING_GALAXY::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BLUECASTING, level / 16);
  }
}

// OFFSET: 0x0079B520
// FUNCTION: fn_08865
void CSkillObjectAction_SKE_TWINKLING_GALAXY::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    g_effectMgr->Spawn(object->render.accountId, 0x92D, object->render.position, 0, argument, 0, 0);
  }
}

// OFFSET: 0x0079B570
// FUNCTION: fn_08866
bool CSkillAction_SKE_STAR_BURST::KeepSkillAttackMotion(int argument) {
  return true;
}

// OFFSET: 0x0079B580
// FUNCTION: fn_08867
void CSkillAction_SKE_STAR_BURST::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BLUECASTING, level / 16);
  }
}

// OFFSET: 0x0079B5B0
// FUNCTION: fn_08868
void CEfstAction_STAR_BURST::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, EF_SKE_TWINKLING_GALAXY, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079B600
// FUNCTION: fn_08869
void CEfstAction_STAR_BURST::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2348);
  }
}

// OFFSET: 0x0079B630
// FUNCTION: fn_08870
void CEfstAction_STAR_BURST::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079B650
// FUNCTION: fn_08871
bool CSkillAction_SKE_STAR_CANNON::KeepSkillAttackMotion(int argument) {
  return true;
}

// OFFSET: 0x0079B660
// FUNCTION: fn_08872
bool CSkillAction_SKE_STAR_CANNON::KeepSkillAttackSpeed() {
  return true;
}

// OFFSET: 0x0079B670
// FUNCTION: fn_08873
void CSkillAction_SKE_STAR_CANNON::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BLUECASTING, level / 16);
  }
}

// GLOBAL: gl_25870
static EfstActionRegistrar s_efstAction1390(1390, NewAction<CEfstAction_DAWN_MOON, CEfstAction>);
// GLOBAL: gl_25871
static EfstActionRegistrar s_efstAction1389(1389, NewAction<CEfstAction_MIDNIGHT_MOON, CEfstAction>);
// GLOBAL: gl_25872
static EfstActionRegistrar s_efstAction1386(1386, NewAction<CEfstAction_NOON_SUN, CEfstAction>);
// GLOBAL: gl_25873
static EfstActionRegistrar s_efstAction1388(1388, NewAction<CEfstAction_RISING_MOON, CEfstAction>);
// GLOBAL: gl_25874
static EfstActionRegistrar s_efstAction1385(1385, NewAction<CEfstAction_RISING_SUN, CEfstAction>);
// GLOBAL: gl_25875
static EfstActionRegistrar s_efstAction1392(1392, NewAction<CEfstAction_SKY_ENCHANT, CEfstAction>);
// GLOBAL: gl_25876
static EfstActionRegistrar s_efstAction1391(1391, NewAction<CEfstAction_STAR_BURST, CEfstAction>);
// GLOBAL: gl_25877
static EfstActionRegistrar s_efstAction1387(1387, NewAction<CEfstAction_SUNSET_SUN, CEfstAction>);
// GLOBAL: gl_25878
static CSkillObjectActionRegistration s_skillObjectAction287(287, NewAction<CSkillObjectAction_SKE_STAR_CANNON, CSkillObjectAction>);
// GLOBAL: gl_25879
static CSkillObjectActionRegistration s_skillObjectAction286(286, NewAction<CSkillObjectAction_SKE_TWINKLING_GALAXY, CSkillObjectAction>);
// GLOBAL: gl_25880
static SkillActionRegistrar s_skillAction5474(5474, NewAction<CSkillAction_SKE_ALL_IN_THE_SKY, CSkillAction>);
// GLOBAL: gl_25881
static SkillActionRegistrar s_skillAction5470(5470, NewAction<CSkillAction_SKE_DAWN_BREAK, CSkillAction>);
// GLOBAL: gl_25882
static SkillActionRegistrar s_skillAction5475(5475, NewAction<CSkillAction_SKE_ENCHANTING_SKY, CSkillAction>);
// GLOBAL: gl_25883
static SkillActionRegistrar s_skillAction5469(5469, NewAction<CSkillAction_SKE_MIDNIGHT_KICK, CSkillAction>);
// GLOBAL: gl_25884
static SkillActionRegistrar s_skillAction5466(5466, NewAction<CSkillAction_SKE_NOON_BLAST, CSkillAction>);
// GLOBAL: gl_25885
static SkillActionRegistrar s_skillAction5468(5468, NewAction<CSkillAction_SKE_RISING_MOON, CSkillAction>);
// GLOBAL: gl_25886
static SkillActionRegistrar s_skillAction5465(5465, NewAction<CSkillAction_SKE_RISING_SUN, CSkillAction>);
// GLOBAL: gl_25887
static SkillActionRegistrar s_skillAction5503(5503, NewAction<CSkillAction_SKE_SKY_MOON, CSkillAction>);
// GLOBAL: gl_25888
static SkillActionRegistrar s_skillAction5502(5502, NewAction<CSkillAction_SKE_SKY_SUN, CSkillAction>);
// GLOBAL: gl_25889
static SkillActionRegistrar s_skillAction5472(5472, NewAction<CSkillAction_SKE_STAR_BURST, CSkillAction>);
// GLOBAL: gl_25890
static SkillActionRegistrar s_skillAction5473(5473, NewAction<CSkillAction_SKE_STAR_CANNON, CSkillAction>);
// GLOBAL: gl_25891
static SkillActionRegistrar s_skillAction5504(5504, NewAction<CSkillAction_SKE_STAR_LIGHT_KICK, CSkillAction>);
// GLOBAL: gl_25892
static SkillActionRegistrar s_skillAction5467(5467, NewAction<CSkillAction_SKE_SUNSET_BLAST, CSkillAction>);
// GLOBAL: gl_25893
static SkillActionRegistrar s_skillAction5471(5471, NewAction<CSkillAction_SKE_TWINKLING_GALAXY, CSkillAction>);
