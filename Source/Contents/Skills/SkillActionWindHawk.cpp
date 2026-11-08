#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Camera/Camera.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/EZ2STREffectEx.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/SkillInfo.h"

#include "Resource/MSGSTRINGID.h"
class CSkillAction_WH_GALESTORM : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_WH_CALAMITYGALE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_CALAMITYGALE : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_WH_CRESCIVE_BOLT : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  bool SuppressSupportSkillArrow() override;
  bool SuppressAttackSkillArrow() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillHit(int sourceActor, CGameActor* target, CGameActor* argument3) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_WH_WILD_WALK : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

// OFFSET: 0x007A5A80
// FUNCTION: fn_09023
void CSkillAction_WH_GALESTORM::OnSkillCast(int level, CGameActor* source) {
  if (CEffect* effect = g_effectMgr->Spawn(
        source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("galestorm_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
      )) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x007A5BF0
// FUNCTION: fn_09024
void CSkillAction_WH_GALESTORM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    g_effectMgr->Spawn(target->render.accountId, 1973, target->render.position.x, target->render.position.y, target->render.position.z, 0, level, 0, 0);
  }
}

// OFFSET: 0x007A5C40
// FUNCTION: fn_09025
void CSkillAction_WH_GALESTORM::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("galestorm_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A5DB0
// FUNCTION: fn_09026
void CSkillAction_WH_CALAMITYGALE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("calumitygale_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A5F40
// FUNCTION: fn_09027
void CSkillAction_WH_CALAMITYGALE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("calumitygale"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A60A0
// FUNCTION: fn_09028
void CEfstAction_CALAMITYGALE::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("calumitygale_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A6220
// FUNCTION: fn_09029
void CEfstAction_CALAMITYGALE::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("calumitygale_loop"));
  }
}

// OFFSET: 0x007A6350
// FUNCTION: fn_09030
void CEfstAction_CALAMITYGALE::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007A6370
// FUNCTION: fn_09031
EfstActionMessage CEfstAction_CALAMITYGALE::GetEfstBeginMessage() {
  return {LookupMessage(MSI_WH_WIND_SIGN_OFF), 0xFFFF00};
}

// OFFSET: 0x007A63A0
// FUNCTION: fn_09032
EfstActionMessage CEfstAction_CALAMITYGALE::GetEfstEndMessage() {
  return {LookupMessage(MSI_THAI_PERSONALINFO_EXP2), 0xFF};
}

// OFFSET: 0x007A63D0
// FUNCTION: fn_09033
int CSkillAction_WH_CRESCIVE_BOLT::GetSkillCastMotion() {
  return 18;
}

// OFFSET: 0x007A63F0
// FUNCTION: fn_09035
bool CSkillAction_WH_CRESCIVE_BOLT::SuppressSupportSkillArrow() {
  return true;
}

// OFFSET: 0x007A6400
// FUNCTION: fn_09036
bool CSkillAction_WH_CRESCIVE_BOLT::SuppressAttackSkillArrow() {
  return true;
}

// OFFSET: 0x007A6410
// FUNCTION: fn_09037
void CSkillAction_WH_CRESCIVE_BOLT::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("crescivebolt_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A65A0
// FUNCTION: fn_45492
void CSkillAction_WH_CRESCIVE_BOLT::OnSkillHit(int sourceActor, CGameActor* target, CGameActor* argument3) {
  CGameActor* source = reinterpret_cast<CGameActor*>(sourceActor);
  if (source && target) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("crescivebolt"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetAngle(
        ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values) + 180.0f
      );
      effect->SetDelay(static_cast<int>(source->unknown_0378 * source->render.speed * 16.0f));
    }
  }
}

// OFFSET: 0x007A6780
// FUNCTION: fn_09038
void CSkillAction_WH_CRESCIVE_BOLT::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("crescivebolt_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A68F0
// FUNCTION: fn_09039
void CSkillAction_WH_WILD_WALK::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_walk_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect =
            effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_walk_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A6B90
// FUNCTION: fn_09040
void CSkillAction_WH_WILD_WALK::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_walk"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    source->LaunchEffect(2425, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x007A6D20
// FUNCTION: fn_09041
void CSkillAction_WH_WILD_WALK::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    if (CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_walk_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetAngle(ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values));
    }
  }
}
