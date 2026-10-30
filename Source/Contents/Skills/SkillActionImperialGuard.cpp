#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EZ2STREffectEx.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Utility/Math/Vector3d.h"

#include <algorithm>
#include <cstdlib>
#include <format>
#include <string>

class CSkillAction_IG_GUARDIAN_SHIELD : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_GUARDIAN_SHIELD : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_IG_HOLY_SHIELD : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_HOLY_SHIELD : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_IG_JUDGEMENT_CROSS : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_OVERSLASH : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_REBOUND_SHIELD : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_REBOUND_S : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_IG_ULTIMATE_SACRIFICE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_ULTIMATE_SACRIFICE : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_IG_IMPERIAL_CROSS : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_RADIANT_SPEAR : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_IMPERIAL_PRESSURE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_SHIELD_SHOOTING : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_ATTACK_STANCE : public CSkillAction {
public:
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_ATTACK_STANCE : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_IG_GUARD_STANCE : public CSkillAction {
public:
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_GUARD_STANCE : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

// OFFSET: 0x00787A30
// FUNCTION: fn_08590
void CSkillAction_IG_GUARDIAN_SHIELD::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guardianshield"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guardianshield_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00787CC0
// FUNCTION: fn_08591
void CEfstAction_IG_GUARDIAN_SHIELD::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guardianshield_buff_start"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guardianshield_buff_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00787F60
// FUNCTION: fn_08592
void CEfstAction_IG_GUARDIAN_SHIELD::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guardianshield_buff_loop"));
  }
}

// OFFSET: 0x007880A0
// FUNCTION: fn_08593
void CEfstAction_IG_GUARDIAN_SHIELD::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007880C0
// FUNCTION: fn_08594
EfstActionMessage CEfstAction_IG_GUARDIAN_SHIELD::Slot03() {
  return {LookupMessage(3752), 0xFFFF00};
}

// OFFSET: 0x007880F0
// FUNCTION: fn_08595
EfstActionMessage CEfstAction_IG_GUARDIAN_SHIELD::Slot04() {
  return {LookupMessage(3753), 0xFF};
}

// OFFSET: 0x00788120
// FUNCTION: fn_08596
void CSkillAction_IG_HOLY_SHIELD::Slot08(int level, CGameActor* source) {
  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_cast"),
                                       0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }

  effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_cast_bottom"),
                              0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x007883D0
// FUNCTION: fn_08597
void CSkillAction_IG_HOLY_SHIELD::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00788650
// FUNCTION: fn_08598
void CEfstAction_IG_HOLY_SHIELD::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_loop_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007888E0
// FUNCTION: fn_08599
void CEfstAction_IG_HOLY_SHIELD::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_loop"));
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_holy_shield_loop_bottom"));
  }
}

// OFFSET: 0x00788B00
// FUNCTION: fn_08600
void CEfstAction_IG_HOLY_SHIELD::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x00788B20
// FUNCTION: fn_08601
EfstActionMessage CEfstAction_IG_HOLY_SHIELD::Slot03() {
  return {LookupMessage(3756), 0xFFFF00};
}

// OFFSET: 0x00788B50
// FUNCTION: fn_08602
EfstActionMessage CEfstAction_IG_HOLY_SHIELD::Slot04() {
  return {LookupMessage(3757), 0xFF};
}

// OFFSET: 0x00788B90
// FUNCTION: fn_08604
void CSkillAction_IG_JUDGEMENT_CROSS::Slot08(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_judgement_cross_cast"),
                                       0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }

  effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_judgement_cross_cast_bottom"),
                              0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x00788E50
// FUNCTION: fn_08605
void CSkillAction_IG_JUDGEMENT_CROSS::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    const vector3d position = source->render.position;
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_judgement_cross_shot"), position.x, position.y, position.z,
                       target->render.accountId, -1, 0, 0);
  }
}

// OFFSET: 0x00788FE0
// FUNCTION: fn_08606
void CSkillAction_IG_JUDGEMENT_CROSS::Slot15(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("new_judgement_cross_bottom"), source->render.position.x,
                       source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("new_judgement_cross"), source->render.position.x,
                       source->render.position.y, source->render.position.z, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00789260
// FUNCTION: fn_08607
void CSkillAction_IG_JUDGEMENT_CROSS::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_judgement_cross_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00789400
// FUNCTION: fn_08609
void CSkillAction_IG_OVERSLASH::Slot08(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_overslash_cast"),
                                       0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x00789590
// FUNCTION: fn_08610
void CSkillAction_IG_OVERSLASH::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_overslash"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_overslash_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00789800
// FUNCTION: fn_08611
void CSkillAction_IG_OVERSLASH::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_overslash_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00789980
// FUNCTION: fn_08612
void CSkillAction_IG_REBOUND_SHIELD::Slot08(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_rebound_shield_cast"),
                                       0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }

  effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_rebound_shield_cast_bottom"),
                              0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x00789C50
// FUNCTION: fn_08613
void CSkillAction_IG_REBOUND_SHIELD::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_rebound_shield"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_rebound_shield_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00789EE0
// FUNCTION: fn_08614
EfstActionMessage CEfstAction_REBOUND_S::Slot03() {
  return {LookupMessage(3754), 0xFFFF00};
}

// OFFSET: 0x00789F10
// FUNCTION: fn_08615
EfstActionMessage CEfstAction_REBOUND_S::Slot04() {
  return {LookupMessage(3755), 0xFF};
}

// OFFSET: 0x00789F40
// FUNCTION: fn_08616
void CSkillAction_IG_ULTIMATE_SACRIFICE::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(58, level / 16);
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }
}

// OFFSET: 0x00789FA0
// FUNCTION: fn_08617
void CSkillAction_IG_ULTIMATE_SACRIFICE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ultimatesacrifice_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ultimatesacrifice"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078A220
// FUNCTION: fn_08618
void CEfstAction_IG_ULTIMATE_SACRIFICE::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ultimate_sacrifice_buff"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078A3B0
// FUNCTION: fn_08619
void CEfstAction_IG_ULTIMATE_SACRIFICE::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ultimate_sacrifice_buff"));
  }
}

// OFFSET: 0x0078A500
// FUNCTION: fn_08620
void CEfstAction_IG_ULTIMATE_SACRIFICE::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0078A530
// FUNCTION: fn_08622
void CSkillAction_IG_IMPERIAL_CROSS::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("imperial_cross"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078A6A0
// FUNCTION: fn_08623
void CSkillAction_IG_IMPERIAL_CROSS::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("imperial_cross_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078A830
// FUNCTION: fn_08625
void CSkillAction_IG_RADIANT_SPEAR::Slot08(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("radiant_spear_cast"),
                                       0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x0078A9C0
// FUNCTION: fn_08626
void CSkillAction_IG_RADIANT_SPEAR::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, mode->camera->frame.view.values);
  std::string side = "left";
  if (angle > 90.0f && angle < 270.0f) {
    side = "right";
  }

  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(std::format("radiant_spear_{}down_bottom", side)), target->render.position.x,
                     target->render.position.y, target->render.position.z, 0, -1, 0, 0);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(std::format("radiant_spear_{}down", side)), target->render.position.x,
                     target->render.position.y, target->render.position.z, 0, -1, 0, 0);
}

// OFFSET: 0x0078AC80
// FUNCTION: fn_08627
void CSkillAction_IG_RADIANT_SPEAR::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("radiant_spear_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078AE00
// FUNCTION: fn_08628
void CSkillAction_IG_IMPERIAL_PRESSURE::Slot08(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("imperal_pressure_cast"),
                                       0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x0078AF90
// FUNCTION: fn_08629
void CSkillAction_IG_IMPERIAL_PRESSURE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("imperial_pressure"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("imperial_pressure_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078B210
// FUNCTION: fn_08630
void CSkillAction_IG_IMPERIAL_PRESSURE::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("imperial_pressure_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078B3A0
// FUNCTION: fn_08632
void CSkillAction_IG_SHIELD_SHOOTING::Slot08(int level, CGameActor* source) {
  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shield_shooting_cast"),
                                       0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x0078B520
// FUNCTION: fn_08633
void CSkillAction_IG_SHIELD_SHOOTING::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  const vector3d position = source->render.position;
  g_effectMgr->Spawn(source->render.accountId, 2183, position.x, position.y, position.z, target->render.accountId, -1, 0, 0);

  const CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
  g_soundMgr->PlayWave("effect\\IG_SHIELD_SHOOTING.wav", target->render.position.x - player->render.position.x, 0.0f,
                       target->render.position.z - player->render.position.z, 250, 40, 1.0f, 0);
}

// OFFSET: 0x0078B5E0
// FUNCTION: fn_08634
void CSkillAction_IG_SHIELD_SHOOTING::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shield_shooting_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078B760
// FUNCTION: fn_08635
int CSkillAction_IG_ATTACK_STANCE::Slot02() {
  return 7;
}

// OFFSET: 0x0078B770
// FUNCTION: fn_08636
void CSkillAction_IG_ATTACK_STANCE::Slot08(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_attack_stance_cast_start"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_attack_stance_cast"),
                                       0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level - 100);
  }

  effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_attack_stance_cast_bottom"),
                              0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level - 100);
  }
}

// OFFSET: 0x0078BB40
// FUNCTION: fn_08637
void CSkillAction_IG_ATTACK_STANCE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_attack_stance"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078BCC0
// FUNCTION: fn_08638
void CEfstAction_IG_ATTACK_STANCE::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_attack_stance_loop_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078BE50
// FUNCTION: fn_08639
void CEfstAction_IG_ATTACK_STANCE::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_attack_stance_loop_bottom"));
  }
}

// OFFSET: 0x0078BFA0
// FUNCTION: fn_08640
void CEfstAction_IG_ATTACK_STANCE::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0078BFC0
// FUNCTION: fn_08641
EfstActionMessage CEfstAction_IG_ATTACK_STANCE::Slot03() {
  return {LookupMessage(3748), 0xFFFF00};
}

// OFFSET: 0x0078BFF0
// FUNCTION: fn_08642
EfstActionMessage CEfstAction_IG_ATTACK_STANCE::Slot04() {
  return {LookupMessage(3749), 0xFF};
}

// OFFSET: 0x0078C020
// FUNCTION: fn_08643
int CSkillAction_IG_GUARD_STANCE::Slot02() {
  return 7;
}

// OFFSET: 0x0078C030
// FUNCTION: fn_08644
void CSkillAction_IG_GUARD_STANCE::Slot08(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guard_stance_cast_start"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guard_stance_cast"),
                                       0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level - 100);
  }

  effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guard_stance_cast_bottom"),
                              0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level - 100);
  }
}

// OFFSET: 0x0078C400
// FUNCTION: fn_08645
void CSkillAction_IG_GUARD_STANCE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guard_stance"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078C570
// FUNCTION: fn_08646
void CEfstAction_IG_GUARD_STANCE::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guard_stance_loop_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

class CSkillAction_IG_CROSS_RAIN : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  int Slot14() override;
};

class CSkillObjectAction_IG_CROSS_RAIN : public CSkillObjectAction {
public:
  void Slot00(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_IG_GRAND_JUDGEMENT : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_GRAND_JUDGEMENT : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

// OFFSET: 0x00786AA0
// FUNCTION: fn_08579
void CSkillAction_IG_CROSS_RAIN::Slot08(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_cross_rain_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
  effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_cross_rain_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x00786D50
// FUNCTION: fn_08580
int CSkillAction_IG_CROSS_RAIN::Slot14() {
  return 9;
}

// OFFSET: 0x00786D60
// FUNCTION: fn_08581
void CSkillObjectAction_IG_CROSS_RAIN::Slot00(int argument, CRenderObject* object, CGameActor* source) {
  if (object && source) {
    g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_cross_rain_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(object->render.accountId, 2462, object->render.position.x, object->render.position.y, object->render.position.z, 0, argument, 0, 0);
  }
}

// OFFSET: 0x00786F30
// FUNCTION: fn_08583
void CSkillAction_IG_GRAND_JUDGEMENT::Slot08(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
  effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x007871F0
// FUNCTION: fn_08584
void CSkillAction_IG_GRAND_JUDGEMENT::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  const int subEffectId = CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement_sub");
  for (int index = 0; index < 8; ++index) {
    const float originX = target->render.position.x;
    float x = originX;
    const float y = target->render.position.y;
    const float originZ = target->render.position.z;
    float z = originZ;
    switch (index & 3) {
    case 1:
      x = originX + static_cast<float>(std::max(rand() % 8, 0));
      z = originZ + static_cast<float>(std::max(rand() % 8, 0));
      break;
    case 2:
      x = originX + static_cast<float>(std::max(rand() % 8, 0));
      z = originZ - static_cast<float>(std::max(rand() % 8, 0));
      break;
    case 3:
      x = originX - static_cast<float>(std::max(rand() % 8, 0));
      z = originZ + static_cast<float>(std::max(rand() % 8, 0));
      break;
    }
    CEffect* effect = g_effectMgr->Spawn(0, subEffectId, x, y, z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot16(140 * (index + 1));
    }
  }
}

// OFFSET: 0x007876E0
// FUNCTION: fn_08585
void CEfstAction_IG_GRAND_JUDGEMENT::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement_buff"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00787860
// FUNCTION: fn_08586
void CEfstAction_IG_GRAND_JUDGEMENT::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_grand_judgement_buff"));
  }
}

// OFFSET: 0x007879A0
// FUNCTION: fn_08587
void CEfstAction_IG_GRAND_JUDGEMENT::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007879D0
// FUNCTION: fn_08589
void CSkillAction_IG_GUARDIAN_SHIELD::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(58, level / 16);
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }
}

// OFFSET: 0x0078C6F0
// FUNCTION: fn_08647
void CEfstAction_IG_GUARD_STANCE::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_guard_stance_loop_bottom"));
  }
}

// OFFSET: 0x0078C830
// FUNCTION: fn_08648
void CEfstAction_IG_GUARD_STANCE::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0078C850
// FUNCTION: fn_08649
EfstActionMessage CEfstAction_IG_GUARD_STANCE::Slot03() {
  return {LookupMessage(3750), 0xFFFF00};
}

// OFFSET: 0x0078C880
// FUNCTION: fn_08650
EfstActionMessage CEfstAction_IG_GUARD_STANCE::Slot04() {
  return {LookupMessage(3751), 0xFF};
}
