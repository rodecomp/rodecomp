#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Camera/Camera.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EZ2STREffectEx.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/Vector3d.h"

class CSkillAction_TR_ROSEBLOSSOM : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_TR_ROSEBLOSSOM_ATK : public CSkillAction {
public:
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_TR_KVASIR_SONATA : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_KVASIR_SONATA : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_TR_PRON_MARCH : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_PRON_MARCH : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_TR_JAWAII_SERENADE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_JAWAII_SERENADE : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_TR_AIN_RHAPSODY : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_AIN_RHAPSODY : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_TR_GEF_NOCTURN : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_GEF_NOCTURN : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_TR_RHYTHMICAL_WAVE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_TR_METALIC_FURY : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_TR_MYSTIC_SYMPHONY : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_MYSTIC_SYMPHONY : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_TR_RHYTHMSHOOTING : public CSkillAction {
public:
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_TR_ROKI_CAPRICCIO : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

// OFFSET: 0x007A20F0

// OFFSET: 0x007A2120

// OFFSET: 0x007A2290

// OFFSET: 0x007A2400

// OFFSET: 0x007A26A0

// OFFSET: 0x007A2820

// OFFSET: 0x007A2850

// OFFSET: 0x007A2880

// OFFSET: 0x007A2B20

// OFFSET: 0x007A2CA0

// OFFSET: 0x007A2CF0

// OFFSET: 0x007A2D20

// OFFSET: 0x007A2D50

// OFFSET: 0x007A3000

// OFFSET: 0x007A3190

// OFFSET: 0x007A31E0

// OFFSET: 0x007A3210

// OFFSET: 0x007A3240

// OFFSET: 0x007A33D0

// OFFSET: 0x007A3550

// OFFSET: 0x007A35A0

// OFFSET: 0x007A35D0

// OFFSET: 0x007A3600

// OFFSET: 0x007A38A0

// OFFSET: 0x007A3A30

// OFFSET: 0x007A3A80

// OFFSET: 0x007A3AB0

// OFFSET: 0x007A3AE0

// OFFSET: 0x007A3D90

// OFFSET: 0x007A4010

// OFFSET: 0x007A41A0

// OFFSET: 0x007A4300

// OFFSET: 0x007A4490

// OFFSET: 0x007A4620

// OFFSET: 0x007A47A0

// OFFSET: 0x007A48E0

// OFFSET: 0x007A4900

// OFFSET: 0x007A4930

// OFFSET: 0x007A4960
// FUNCTION: fn_09014
int CSkillAction_TR_RHYTHMSHOOTING::Slot02() {
  return 9;
}

// OFFSET: 0x007A4970

// OFFSET: 0x007A4C20
// FUNCTION: fn_09016
void CSkillAction_TR_RHYTHMSHOOTING::Slot15(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmshooting_shot"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
    effect->Slot17(angle + 180.0f);
  }
}

// OFFSET: 0x007A4E00

// OFFSET: 0x007A4F80

// OFFSET: 0x007A5230

// OFFSET: 0x007A53C0
