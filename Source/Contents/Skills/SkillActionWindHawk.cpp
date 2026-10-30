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

class CSkillAction_WH_GALESTORM : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_WH_CALAMITYGALE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_CALAMITYGALE : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_WH_CRESCIVE_BOLT : public CSkillAction {
public:
  int Slot01() override;
  bool Slot06() override;
  bool Slot07() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_WH_WILD_WALK : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

// OFFSET: 0x007A5A80
// FUNCTION: fn_09023
void CSkillAction_WH_GALESTORM::Slot08(int level, CGameActor* source) {
  if (CEffect* effect = g_effectMgr->Spawn(
        source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("galestorm_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
      )) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x007A5BF0
// FUNCTION: fn_09024
void CSkillAction_WH_GALESTORM::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    g_effectMgr->Spawn(target->render.accountId, 1973, target->render.position.x, target->render.position.y, target->render.position.z, 0, level, 0, 0);
  }
}

// OFFSET: 0x007A5C40
// FUNCTION: fn_09025
void CSkillAction_WH_GALESTORM::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("galestorm_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A5DB0
// FUNCTION: fn_09026
void CSkillAction_WH_CALAMITYGALE::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("calumitygale_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007A5F40
// FUNCTION: fn_09027
void CSkillAction_WH_CALAMITYGALE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("calumitygale"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A60A0
// FUNCTION: fn_09028
void CEfstAction_CALAMITYGALE::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("calumitygale_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A6220
// FUNCTION: fn_09029
void CEfstAction_CALAMITYGALE::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("calumitygale_loop"));
  }
}

// OFFSET: 0x007A6350
// FUNCTION: fn_09030
void CEfstAction_CALAMITYGALE::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007A6370
// FUNCTION: fn_09031
EfstActionMessage CEfstAction_CALAMITYGALE::Slot03() {
  return {LookupMessage(3805), 0xFFFF00};
}

// OFFSET: 0x007A63A0
// FUNCTION: fn_09032
EfstActionMessage CEfstAction_CALAMITYGALE::Slot04() {
  return {LookupMessage(3806), 0xFF};
}

// OFFSET: 0x007A63D0
// FUNCTION: fn_09033
int CSkillAction_WH_CRESCIVE_BOLT::Slot01() {
  return 18;
}

// OFFSET: 0x007A63F0
// FUNCTION: fn_09035
bool CSkillAction_WH_CRESCIVE_BOLT::Slot06() {
  return true;
}

// OFFSET: 0x007A6400
// FUNCTION: fn_09036
bool CSkillAction_WH_CRESCIVE_BOLT::Slot07() {
  return true;
}

// OFFSET: 0x007A6410
// FUNCTION: fn_09037
void CSkillAction_WH_CRESCIVE_BOLT::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("crescivebolt_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007A6780
// FUNCTION: fn_09038
void CSkillAction_WH_CRESCIVE_BOLT::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("crescivebolt_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A68F0
// FUNCTION: fn_09039
void CSkillAction_WH_WILD_WALK::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_walk_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
    if (CEffect* effect =
            g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_walk_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007A6B90
// FUNCTION: fn_09040
void CSkillAction_WH_WILD_WALK::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_walk"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    source->LaunchEffect(2425, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x007A6D20
// FUNCTION: fn_09041
void CSkillAction_WH_WILD_WALK::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    if (CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_walk_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot17(ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values));
    }
  }
}
