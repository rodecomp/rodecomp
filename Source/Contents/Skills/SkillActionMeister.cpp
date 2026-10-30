#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/EZ2STREffectEx.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/Vector3d.h"

class CSkillAction_MT_SPARK_BLASTER : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_MT_TRIPLE_LASER : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_MT_MIGHTY_SMASH : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  bool Slot05() override;
};

class CSkillAction_MT_ENERGY_CANNONADE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_MT_RUSH_STRIKE : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_MT_POWERFUL_SWING : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_GRADUAL_GRAVITY : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

// OFFSET: 0x0078CC50
// FUNCTION: fn_08652
void CSkillAction_MT_SPARK_BLASTER::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(1938, level / 16);
  }
}

// OFFSET: 0x0078CC80
// FUNCTION: fn_08653
void CSkillAction_MT_SPARK_BLASTER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("spark_blaster"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("spark_blaster_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078CF00
// FUNCTION: fn_08655
void CSkillAction_MT_TRIPLE_LASER::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(1938, level / 16);
  }
}

// OFFSET: 0x0078CF30
// FUNCTION: fn_08656
void CSkillAction_MT_TRIPLE_LASER::Slot11(int level, CGameActor* source, CGameActor* target) {
}

// OFFSET: 0x0078CF40
// FUNCTION: fn_08657
void CSkillAction_MT_TRIPLE_LASER::Slot15(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("triple_laser"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078D0B0
// FUNCTION: fn_08659
void CSkillAction_MT_MIGHTY_SMASH::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mighty_smash"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078D210
// FUNCTION: fn_08660
bool CSkillAction_MT_MIGHTY_SMASH::Slot05() {
  return true;
}

// OFFSET: 0x0078D230
// FUNCTION: fn_08662
void CSkillAction_MT_ENERGY_CANNONADE::Slot08(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_cannonade_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
  effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_cannonade_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x0078D4E0
// FUNCTION: fn_08663
void CSkillAction_MT_ENERGY_CANNONADE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
  const vector3d position = source->render.position;
  CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("energy_cannonade"), position.x, position.y, position.z, 0, -1, 0, 0);
  if (effect) {
    effect->Slot17(angle + 180.0f);
  }
}

// OFFSET: 0x0078D690
// FUNCTION: fn_08664
void CSkillAction_MT_ENERGY_CANNONADE::Slot15(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_cannonade_hit_big"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078D810
// FUNCTION: fn_08665
void CSkillAction_MT_ENERGY_CANNONADE::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_cannonade_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078D9A0
// FUNCTION: fn_08667
void CSkillAction_MT_RUSH_STRIKE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  if (CRagEffect* ragEffect = static_cast<EffectActor*>(source)->LaunchEffect(1568, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    ragEffect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
  }
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rush_strike_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rush_strike"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot26(source->render.previousAnimation % 8 >= 4);
  }
}

// OFFSET: 0x0078DCA0
// FUNCTION: fn_08668
void CSkillAction_MT_RUSH_STRIKE::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rush_strike_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078DE20
// FUNCTION: fn_08670
void CSkillAction_MT_POWERFUL_SWING::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  if (!g_serviceOptions.Get(229)) {
    CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("powerful_swing_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->Slot17(ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values) + 180.0f);
    }
  }
  CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("powerful_swing"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot26(source->render.previousAnimation % 8 >= 4);
  }
}

// OFFSET: 0x0078E130
// FUNCTION: fn_08671
void CSkillAction_MT_POWERFUL_SWING::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("powerful_swing_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078E370
// FUNCTION: fn_08672
void CEfstAction_GRADUAL_GRAVITY::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    static_cast<EffectActor*>(actor)->LaunchEffect(779, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x0078E3C0
// FUNCTION: fn_08673
void CEfstAction_GRADUAL_GRAVITY::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    while (static_cast<EffectActor*>(actor)->state.Attachments().HasId(779)) {
      actor->SendMsg(nullptr, 109, 0, 779, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x0078E420
// FUNCTION: fn_08674
void CEfstAction_GRADUAL_GRAVITY::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}
