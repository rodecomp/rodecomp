#include "Client/Mode/CGameMode.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Particles/EZ2STREffectEx.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Utility/Math/Vector3d.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstdlib>
#include <format>
#include <string>

class CSkillAction_IG_GUARDIAN_SHIELD : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_GUARDIAN_SHIELD : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_IG_HOLY_SHIELD : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_HOLY_SHIELD : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_IG_JUDGEMENT_CROSS : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_OVERSLASH : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_REBOUND_SHIELD : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_REBOUND_S : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_IG_ULTIMATE_SACRIFICE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_ULTIMATE_SACRIFICE : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_IG_IMPERIAL_CROSS : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_RADIANT_SPEAR : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_IMPERIAL_PRESSURE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_SHIELD_SHOOTING : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_ATTACK_STANCE : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_ATTACK_STANCE : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_IG_GUARD_STANCE : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_GUARD_STANCE : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

// OFFSET: 0x007879C0
// FUNCTION: fn_08588
int CSkillAction_IG_GUARDIAN_SHIELD::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00787A30
// FUNCTION: fn_08590
void CSkillAction_IG_GUARDIAN_SHIELD::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guardianshield"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guardianshield_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00787CC0
// FUNCTION: fn_08591
void CEfstAction_IG_GUARDIAN_SHIELD::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guardianshield_buff_start"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guardianshield_buff_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00787F60
// FUNCTION: fn_08592
void CEfstAction_IG_GUARDIAN_SHIELD::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guardianshield_buff_loop"));
  }
}

// OFFSET: 0x007880A0
// FUNCTION: fn_08593
void CEfstAction_IG_GUARDIAN_SHIELD::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007880C0
// FUNCTION: fn_08594
EfstActionMessage CEfstAction_IG_GUARDIAN_SHIELD::GetEfstBeginMessage() {
  return {LookupMessage(MSI_IG_REBOUND_SHIELD_OFF), 0xFFFF00};
}

// OFFSET: 0x007880F0
// FUNCTION: fn_08595
EfstActionMessage CEfstAction_IG_GUARDIAN_SHIELD::GetEfstEndMessage() {
  return {LookupMessage(MSI_IG_HOLY_SHIELD_ON), 0xFF};
}

// OFFSET: 0x00788120
// FUNCTION: fn_08596
void CSkillAction_IG_HOLY_SHIELD::OnSkillCast(int level, CGameActor* source) {
  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_cast"),
                                   vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }

  effects = g_effectMgr;
  effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_cast_bottom"),
                          vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x007883D0
// FUNCTION: fn_08597
void CSkillAction_IG_HOLY_SHIELD::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00788650
// FUNCTION: fn_08598
void CEfstAction_IG_HOLY_SHIELD::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_loop_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007888E0
// FUNCTION: fn_08599
void CEfstAction_IG_HOLY_SHIELD::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_loop"));
    effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_loop_bottom"));
  }
}

// OFFSET: 0x00788B00
// FUNCTION: fn_08600
void CEfstAction_IG_HOLY_SHIELD::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x00788B20
// FUNCTION: fn_08601
EfstActionMessage CEfstAction_IG_HOLY_SHIELD::GetEfstBeginMessage() {
  return {LookupMessage(MSI_IG_ULTIMATE_SACRIFICE_OFF), 0xFFFF00};
}

// OFFSET: 0x00788B50
// FUNCTION: fn_08602
EfstActionMessage CEfstAction_IG_HOLY_SHIELD::GetEfstEndMessage() {
  return {LookupMessage(MSI_AG_CLIMAX_ON), 0xFF};
}

// OFFSET: 0x00788B80
// FUNCTION: fn_08603
int CSkillAction_IG_JUDGEMENT_CROSS::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00788B90
// FUNCTION: fn_08604
void CSkillAction_IG_JUDGEMENT_CROSS::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_judgement_cross_cast"),
                                   vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }

  effects = g_effectMgr;
  effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_judgement_cross_cast_bottom"),
                          vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x00788E50
// FUNCTION: fn_08605
void CSkillAction_IG_JUDGEMENT_CROSS::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    const vector3d position = source->render.position;
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_judgement_cross_shot"), position,
                   target->render.accountId, -1, 0, 0);
  }
}

// OFFSET: 0x00788FE0
// FUNCTION: fn_08606
void CSkillAction_IG_JUDGEMENT_CROSS::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("new_judgement_cross_bottom"), source->render.position, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("new_judgement_cross"), source->render.position, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00789260
// FUNCTION: fn_08607
void CSkillAction_IG_JUDGEMENT_CROSS::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_judgement_cross_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007893F0
// FUNCTION: fn_08608
int CSkillAction_IG_OVERSLASH::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00789400
// FUNCTION: fn_08609
void CSkillAction_IG_OVERSLASH::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_overslash_cast"),
                                   vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x00789590
// FUNCTION: fn_08610
void CSkillAction_IG_OVERSLASH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_overslash"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_overslash_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00789800
// FUNCTION: fn_08611
void CSkillAction_IG_OVERSLASH::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_overslash_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00789980
// FUNCTION: fn_08612
void CSkillAction_IG_REBOUND_SHIELD::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_rebound_shield_cast"),
                                   vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }

  effects = g_effectMgr;
  effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_rebound_shield_cast_bottom"),
                          vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x00789C50
// FUNCTION: fn_08613
void CSkillAction_IG_REBOUND_SHIELD::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_rebound_shield"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_rebound_shield_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00789EE0
// FUNCTION: fn_08614
EfstActionMessage CEfstAction_REBOUND_S::GetEfstBeginMessage() {
  return {LookupMessage(MSI_IG_HOLY_SHIELD_OFF), 0xFFFF00};
}

// OFFSET: 0x00789F10
// FUNCTION: fn_08615
EfstActionMessage CEfstAction_REBOUND_S::GetEfstEndMessage() {
  return {LookupMessage(MSI_IG_ULTIMATE_SACRIFICE_ON), 0xFF};
}

// OFFSET: 0x00789F40
// FUNCTION: fn_08616
void CSkillAction_IG_ULTIMATE_SACRIFICE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL6, level / 16);
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }
}

// OFFSET: 0x00789FA0
// FUNCTION: fn_08617
void CSkillAction_IG_ULTIMATE_SACRIFICE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ultimatesacrifice_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ultimatesacrifice"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078A220
// FUNCTION: fn_08618
void CEfstAction_IG_ULTIMATE_SACRIFICE::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ultimate_sacrifice_buff"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078A3B0
// FUNCTION: fn_08619
void CEfstAction_IG_ULTIMATE_SACRIFICE::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ultimate_sacrifice_buff"));
  }
}

// OFFSET: 0x0078A500
// FUNCTION: fn_08620
void CEfstAction_IG_ULTIMATE_SACRIFICE::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0078A520
// FUNCTION: fn_08621
int CSkillAction_IG_IMPERIAL_CROSS::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x0078A530
// FUNCTION: fn_08622
void CSkillAction_IG_IMPERIAL_CROSS::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("imperial_cross"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078A6A0
// FUNCTION: fn_08623
void CSkillAction_IG_IMPERIAL_CROSS::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("imperial_cross_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078A820
// FUNCTION: fn_08624
int CSkillAction_IG_RADIANT_SPEAR::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x0078A830
// FUNCTION: fn_08625
void CSkillAction_IG_RADIANT_SPEAR::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("radiant_spear_cast"),
                                   vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x0078A9C0
// FUNCTION: fn_08626
void CSkillAction_IG_RADIANT_SPEAR::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, mode->camera->frame.view.values);
  const char* direction = "left";
  if (angle > 90.0f && angle < 270.0f) {
    direction = "right";
  }
  const std::string side = direction;
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(std::format("radiant_spear_{}down_bottom", side)), target->render.position, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(std::format("radiant_spear_{}down", side)), target->render.position, 0, -1, 0, 0);
}

// OFFSET: 0x0078AC80
// FUNCTION: fn_08627
void CSkillAction_IG_RADIANT_SPEAR::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("radiant_spear_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078AE00
// FUNCTION: fn_08628
void CSkillAction_IG_IMPERIAL_PRESSURE::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("imperal_pressure_cast"),
                                   vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x0078AF90
// FUNCTION: fn_08629
void CSkillAction_IG_IMPERIAL_PRESSURE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("imperial_pressure"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("imperial_pressure_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078B210
// FUNCTION: fn_08630
void CSkillAction_IG_IMPERIAL_PRESSURE::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("imperial_pressure_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078B390
// FUNCTION: fn_08631
int CSkillAction_IG_SHIELD_SHOOTING::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x0078B3A0
// FUNCTION: fn_08632
void CSkillAction_IG_SHIELD_SHOOTING::OnSkillCast(int level, CGameActor* source) {
  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shield_shooting_cast"),
                                   vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x0078B520
// FUNCTION: fn_08633
void CSkillAction_IG_SHIELD_SHOOTING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  const vector3d position = source->render.position;
  g_effectMgr->Spawn(source->render.accountId, 0x887, position, target->render.accountId, -1, 0, 0);

  const vector3d& listener = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->render.position;
  const vector3d soundPosition = {target->render.position.x - listener.x, 0.0f, target->render.position.z - listener.z};
  g_soundMgr->PlayWave("effect\\IG_SHIELD_SHOOTING.wav", soundPosition.x, soundPosition.y, soundPosition.z, 250, 40, 1.0f, 0);
}

// OFFSET: 0x0078B5E0
// FUNCTION: fn_08634
void CSkillAction_IG_SHIELD_SHOOTING::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shield_shooting_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078B760
// FUNCTION: fn_08635
int CSkillAction_IG_ATTACK_STANCE::GetSkillAttackMotion() {
  return 7;
}

// OFFSET: 0x0078B770
// FUNCTION: fn_08636
void CSkillAction_IG_ATTACK_STANCE::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_attack_stance_cast_start"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_attack_stance_cast"),
                                   vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level - 100);
  }

  effects = g_effectMgr;
  effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_attack_stance_cast_bottom"),
                          vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level - 100);
  }
}

// OFFSET: 0x0078BB40
// FUNCTION: fn_08637
void CSkillAction_IG_ATTACK_STANCE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_attack_stance"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078BCC0
// FUNCTION: fn_08638
void CEfstAction_IG_ATTACK_STANCE::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_attack_stance_loop_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078BE50
// FUNCTION: fn_08639
void CEfstAction_IG_ATTACK_STANCE::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_attack_stance_loop_bottom"));
  }
}

// OFFSET: 0x0078BFA0
// FUNCTION: fn_08640
void CEfstAction_IG_ATTACK_STANCE::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0078BFC0
// FUNCTION: fn_08641
EfstActionMessage CEfstAction_IG_ATTACK_STANCE::GetEfstBeginMessage() {
  return {LookupMessage(MSI_IG_GUARD_STANCE_OFF), 0xFFFF00};
}

// OFFSET: 0x0078BFF0
// FUNCTION: fn_08642
EfstActionMessage CEfstAction_IG_ATTACK_STANCE::GetEfstEndMessage() {
  return {LookupMessage(MSI_IG_GUARDIAN_SHIELD_ON), 0xFF};
}

// OFFSET: 0x0078C020
// FUNCTION: fn_08643
int CSkillAction_IG_GUARD_STANCE::GetSkillAttackMotion() {
  return 7;
}

// OFFSET: 0x0078C030
// FUNCTION: fn_08644
void CSkillAction_IG_GUARD_STANCE::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guard_stance_cast_start"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guard_stance_cast"),
                                   vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level - 100);
  }

  effects = g_effectMgr;
  effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guard_stance_cast_bottom"),
                          vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level - 100);
  }
}

// OFFSET: 0x0078C400
// FUNCTION: fn_08645
void CSkillAction_IG_GUARD_STANCE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guard_stance"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078C570
// FUNCTION: fn_08646
void CEfstAction_IG_GUARD_STANCE::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guard_stance_loop_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

class CSkillAction_IG_CROSS_RAIN : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  int GetSkillScopeType() override;
};

class CSkillObjectAction_IG_CROSS_RAIN : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_IG_GRAND_JUDGEMENT : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_GRAND_JUDGEMENT : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

// OFFSET: 0x00786F20
// FUNCTION: fn_08582
int CSkillAction_IG_GRAND_JUDGEMENT::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00786A90
// FUNCTION: fn_08578
int CSkillAction_IG_CROSS_RAIN::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00786AA0
// FUNCTION: fn_08579
void CSkillAction_IG_CROSS_RAIN::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_cross_rain_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
  effects = g_effectMgr;
  effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_cross_rain_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x00786D50
// FUNCTION: fn_08580
int CSkillAction_IG_CROSS_RAIN::GetSkillScopeType() {
  return 9;
}

// OFFSET: 0x00786D60
// FUNCTION: fn_08581
void CSkillObjectAction_IG_CROSS_RAIN::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  if (object && source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_cross_rain_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    g_effectMgr->Spawn(object->render.accountId, 0x99E, object->render.position, 0, argument, 0, 0);
  }
}

// OFFSET: 0x00786F30
// FUNCTION: fn_08583
void CSkillAction_IG_GRAND_JUDGEMENT::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
  effects = g_effectMgr;
  effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x007871F0
// FUNCTION: fn_08584
void CSkillAction_IG_GRAND_JUDGEMENT::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  const int subEffectId = CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement_sub");
  for (int index = 0; index < 8; ++index) {
    const float originX = target->render.position.x;
    float x = originX;
    const float y = target->render.position.y;
    const float originZ = target->render.position.z;
    float z = originZ;
    int roll;
    switch (index & 3) {
    case 1:
      roll = rand() % 8;
      x = originX + static_cast<float>(roll >= 0 ? roll : 0);
      roll = rand() % 8;
      z = originZ + static_cast<float>(roll >= 0 ? roll : 0);
      break;
    case 2:
      roll = rand() % 8;
      x = originX + static_cast<float>(roll >= 0 ? roll : 0);
      roll = rand() % 8;
      z = originZ - static_cast<float>(roll >= 0 ? roll : 0);
      break;
    case 3:
      roll = rand() % 8;
      x = originX - static_cast<float>(roll >= 0 ? roll : 0);
      roll = rand() % 8;
      z = originZ + static_cast<float>(roll >= 0 ? roll : 0);
      break;
    }
    CEffect* effect = g_effectMgr->Spawn(0, subEffectId, vector3d{x, y, z}, 0, -1, 0, 0);
    if (effect) {
      effect->SetDelay(140 * (index + 1));
    }
  }
}

// OFFSET: 0x007876E0
// FUNCTION: fn_08585
void CEfstAction_IG_GRAND_JUDGEMENT::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement_buff"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00787860
// FUNCTION: fn_08586
void CEfstAction_IG_GRAND_JUDGEMENT::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement_buff"));
  }
}

// OFFSET: 0x007879A0
// FUNCTION: fn_08587
void CEfstAction_IG_GRAND_JUDGEMENT::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007879D0
// FUNCTION: fn_08589
void CSkillAction_IG_GUARDIAN_SHIELD::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL6, level / 16);
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }
}

// OFFSET: 0x0078C6F0
// FUNCTION: fn_08647
void CEfstAction_IG_GUARD_STANCE::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guard_stance_loop_bottom"));
  }
}

// OFFSET: 0x0078C830
// FUNCTION: fn_08648
void CEfstAction_IG_GUARD_STANCE::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0078C850
// FUNCTION: fn_08649
EfstActionMessage CEfstAction_IG_GUARD_STANCE::GetEfstBeginMessage() {
  return {LookupMessage(MSI_IG_GUARDIAN_SHIELD_OFF), 0xFFFF00};
}

// OFFSET: 0x0078C880
// FUNCTION: fn_08650
EfstActionMessage CEfstAction_IG_GUARD_STANCE::GetEfstEndMessage() {
  return {LookupMessage(MSI_IG_REBOUND_SHIELD_ON), 0xFF};
}

// GLOBAL: gl_25794
static EfstActionRegistrar s_efstAction1203(1203, NewAction<CEfstAction_IG_ATTACK_STANCE, CEfstAction>);
// GLOBAL: gl_25795
static EfstActionRegistrar s_efstAction1222(1222, NewAction<CEfstAction_IG_GRAND_JUDGEMENT, CEfstAction>);
// GLOBAL: gl_25796
static EfstActionRegistrar s_efstAction1204(1204, NewAction<CEfstAction_IG_GUARDIAN_SHIELD, CEfstAction>);
// GLOBAL: gl_25797
static EfstActionRegistrar s_efstAction1202(1202, NewAction<CEfstAction_IG_GUARD_STANCE, CEfstAction>);
// GLOBAL: gl_25798
static EfstActionRegistrar s_efstAction1220(1220, NewAction<CEfstAction_IG_HOLY_SHIELD, CEfstAction>);
// GLOBAL: gl_25799
static EfstActionRegistrar s_efstAction1221(1221, NewAction<CEfstAction_IG_ULTIMATE_SACRIFICE, CEfstAction>);
// GLOBAL: gl_25800
static EfstActionRegistrar s_efstAction1217(1217, NewAction<CEfstAction_REBOUND_S, CEfstAction>);

// OFFSET: 0x0045A7E0
// FUNCTION: fn_45565
static bool s_skillObjectActionRegistered275 = [](std::function<CSkillObjectAction*()> factory) {
  CSkillActionMgr::GetInstance()->RegisterSkillObjectAction(275, factory);
  return true;
}(NewAction<CSkillObjectAction_IG_CROSS_RAIN, CSkillObjectAction>);

// GLOBAL: gl_25801
static SkillActionRegistrar s_skillAction5260(5260, NewAction<CSkillAction_IG_ATTACK_STANCE, CSkillAction>);
// GLOBAL: gl_25802
static SkillActionRegistrar s_skillAction5267(5267, NewAction<CSkillAction_IG_CROSS_RAIN, CSkillAction>);
// GLOBAL: gl_25803
static SkillActionRegistrar s_skillAction5263(5263, NewAction<CSkillAction_IG_GRAND_JUDGEMENT, CSkillAction>);
// GLOBAL: gl_25804
static SkillActionRegistrar s_skillAction5256(5256, NewAction<CSkillAction_IG_GUARDIAN_SHIELD, CSkillAction>);
// GLOBAL: gl_25805
static SkillActionRegistrar s_skillAction5255(5255, NewAction<CSkillAction_IG_GUARD_STANCE, CSkillAction>);
// GLOBAL: gl_25806
static SkillActionRegistrar s_skillAction5262(5262, NewAction<CSkillAction_IG_HOLY_SHIELD, CSkillAction>);
// GLOBAL: gl_25807
static SkillActionRegistrar s_skillAction6504(6504, NewAction<CSkillAction_IG_IMPERIAL_CROSS, CSkillAction>);
// GLOBAL: gl_25808
static SkillActionRegistrar s_skillAction6505(6505, NewAction<CSkillAction_IG_IMPERIAL_PRESSURE, CSkillAction>);
// GLOBAL: gl_25809
static SkillActionRegistrar s_skillAction5264(5264, NewAction<CSkillAction_IG_JUDGEMENT_CROSS, CSkillAction>);
// GLOBAL: gl_25810
static SkillActionRegistrar s_skillAction5266(5266, NewAction<CSkillAction_IG_OVERSLASH, CSkillAction>);
// GLOBAL: gl_25811
static SkillActionRegistrar s_skillAction6503(6503, NewAction<CSkillAction_IG_RADIANT_SPEAR, CSkillAction>);
// GLOBAL: gl_25812
static SkillActionRegistrar s_skillAction5257(5257, NewAction<CSkillAction_IG_REBOUND_SHIELD, CSkillAction>);
// GLOBAL: gl_25813
static SkillActionRegistrar s_skillAction5265(5265, NewAction<CSkillAction_IG_SHIELD_SHOOTING, CSkillAction>);
// GLOBAL: gl_25814
static SkillActionRegistrar s_skillAction5261(5261, NewAction<CSkillAction_IG_ULTIMATE_SACRIFICE, CSkillAction>);
