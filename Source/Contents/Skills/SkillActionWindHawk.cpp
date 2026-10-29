#include "Contents/Actors/GameActor.h"
#include "Contents/EffectResourceSwitcher.h"
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
};

// OFFSET: 0x007A5A80

// OFFSET: 0x007A5BF0

// OFFSET: 0x007A5C40

// OFFSET: 0x007A5DB0

// OFFSET: 0x007A5F40

// OFFSET: 0x007A60A0

// OFFSET: 0x007A6220

// OFFSET: 0x007A6350

// OFFSET: 0x007A6370

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
