#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Contents/Effects/Particles/EZ2STREffectEx.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/Vector3d.h"

class CSkillAction_MT_SPARK_BLASTER : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_MT_TRIPLE_LASER : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_MT_MIGHTY_SMASH : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  bool KeepSkillAttackSpeed() override;
};

class CSkillAction_MT_ENERGY_CANNONADE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_MT_RUSH_STRIKE : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_MT_POWERFUL_SWING : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_GRADUAL_GRAVITY : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

// OFFSET: 0x0078CC50
// FUNCTION: fn_08652
void CSkillAction_MT_SPARK_BLASTER::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(1938, level / 16);
  }
}

// OFFSET: 0x0078CC80
// FUNCTION: fn_08653
void CSkillAction_MT_SPARK_BLASTER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("spark_blaster"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("spark_blaster_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078CF00
// FUNCTION: fn_08655
void CSkillAction_MT_TRIPLE_LASER::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(1938, level / 16);
  }
}

// OFFSET: 0x0078CF30
// FUNCTION: fn_08656
void CSkillAction_MT_TRIPLE_LASER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
}

// OFFSET: 0x0078CF40
// FUNCTION: fn_08657
void CSkillAction_MT_TRIPLE_LASER::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("triple_laser"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078D0B0
// FUNCTION: fn_08659
void CSkillAction_MT_MIGHTY_SMASH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mighty_smash"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078D210
// FUNCTION: fn_08660
bool CSkillAction_MT_MIGHTY_SMASH::KeepSkillAttackSpeed() {
  return true;
}

// OFFSET: 0x0078D230
// FUNCTION: fn_08662
void CSkillAction_MT_ENERGY_CANNONADE::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_cannonade_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
  effects = g_effectMgr;
  effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_cannonade_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x0078D4E0
// FUNCTION: fn_08663
void CSkillAction_MT_ENERGY_CANNONADE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("energy_cannonade"), source->render.position, 0, -1, 0, 0);
  if (effect) {
    effect->SetAngle(angle + 180.0f);
  }
}

// OFFSET: 0x0078D690
// FUNCTION: fn_08664
void CSkillAction_MT_ENERGY_CANNONADE::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_cannonade_hit_big"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078D810
// FUNCTION: fn_08665
void CSkillAction_MT_ENERGY_CANNONADE::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_cannonade_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078D9A0
// FUNCTION: fn_08667
void CSkillAction_MT_RUSH_STRIKE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  if (CRagEffect* ragEffect = static_cast<EffectActor*>(source)->LaunchEffect(1568, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    ragEffect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
  }
  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rush_strike_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  effects = g_effectMgr;
  CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rush_strike"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetFlipX(source->render.previousAnimation % 8 >= 4);
  }
}

// OFFSET: 0x0078DCA0
// FUNCTION: fn_08668
void CSkillAction_MT_RUSH_STRIKE::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rush_strike_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078DE20
// FUNCTION: fn_08670
void CSkillAction_MT_POWERFUL_SWING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  if (!g_serviceOptions.Get(229)) {
    EffectLists* effects = g_effectMgr;
    CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("powerful_swing_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetAngle(ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values) + 180.0f);
    }
  }
  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("powerful_swing"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetFlipX(source->render.previousAnimation % 8 >= 4);
  }
}

// OFFSET: 0x0078E130
// FUNCTION: fn_08671
void CSkillAction_MT_POWERFUL_SWING::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("powerful_swing_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078E370
// FUNCTION: fn_08672
void CEfstAction_GRADUAL_GRAVITY::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    static_cast<EffectActor*>(actor)->LaunchEffect(779, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x0078E3C0
// FUNCTION: fn_08673
void CEfstAction_GRADUAL_GRAVITY::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    while (static_cast<EffectActor*>(actor)->HasId(779)) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 779, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x0078E420
// FUNCTION: fn_08674
void CEfstAction_GRADUAL_GRAVITY::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}
