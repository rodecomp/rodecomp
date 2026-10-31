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
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
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
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_TR_ROKI_CAPRICCIO : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

// OFFSET: 0x007A20F0
// FUNCTION: fn_08975
void CSkillAction_TR_ROSEBLOSSOM::Slot15(int level, CGameActor* source, CGameActor* target) {
  if (CGameActor* actor = reinterpret_cast<CGameActor*>(level)) {
    actor->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
  }
}

// OFFSET: 0x007A2120
// FUNCTION: fn_08976
void CSkillAction_TR_ROSEBLOSSOM::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roseblossom_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A2290
// FUNCTION: fn_08977
void CSkillAction_TR_ROSEBLOSSOM_ATK::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roseblossom_rose"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A2400
// FUNCTION: fn_08978
void CSkillAction_TR_KVASIR_SONATA::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kvasirsonata_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kvasirsonata_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007A26A0
// FUNCTION: fn_08979
void CSkillAction_TR_KVASIR_SONATA::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kvasirsonata"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A2820
// FUNCTION: fn_08980
EfstActionMessage CEfstAction_KVASIR_SONATA::Slot03() {
  return {LookupMessage(3791), 0xFFFF00};
}

// OFFSET: 0x007A2850
// FUNCTION: fn_08981
EfstActionMessage CEfstAction_KVASIR_SONATA::Slot04() {
  return {LookupMessage(3792), 0xFF};
}

// OFFSET: 0x007A2880
// FUNCTION: fn_08982
void CSkillAction_TR_PRON_MARCH::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("pronmarch_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("pronmarch_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007A2B20
// FUNCTION: fn_08983
void CSkillAction_TR_PRON_MARCH::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("pronmarch"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A2CA0
// FUNCTION: fn_08984
void CEfstAction_PRON_MARCH::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    static_cast<EffectActor*>(actor)->LaunchEffect(2294, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x007A2CF0
// FUNCTION: fn_08985
EfstActionMessage CEfstAction_PRON_MARCH::Slot03() {
  return {LookupMessage(3803), 0xFFFF00};
}

// OFFSET: 0x007A2D20
// FUNCTION: fn_08986
EfstActionMessage CEfstAction_PRON_MARCH::Slot04() {
  return {LookupMessage(3804), 0xFF};
}

// OFFSET: 0x007A2D50
// FUNCTION: fn_08987
void CSkillAction_TR_JAWAII_SERENADE::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jawaii_serenade_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jawaii_serenade_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007A3000
// FUNCTION: fn_08988
void CSkillAction_TR_JAWAII_SERENADE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jawaii_serenade"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A3190
// FUNCTION: fn_08989
void CEfstAction_JAWAII_SERENADE::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    static_cast<EffectActor*>(actor)->LaunchEffect(2298, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x007A31E0
// FUNCTION: fn_08990
EfstActionMessage CEfstAction_JAWAII_SERENADE::Slot03() {
  return {LookupMessage(3801), 0xFFFF00};
}

// OFFSET: 0x007A3210
// FUNCTION: fn_08991
EfstActionMessage CEfstAction_JAWAII_SERENADE::Slot04() {
  return {LookupMessage(3802), 0xFF};
}

// OFFSET: 0x007A3240
// FUNCTION: fn_08992
void CSkillAction_TR_AIN_RHAPSODY::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ain_rhapsody_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007A33D0
// FUNCTION: fn_08993
void CSkillAction_TR_AIN_RHAPSODY::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ain_rhapsody"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A3550
// FUNCTION: fn_08994
void CEfstAction_AIN_RHAPSODY::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    static_cast<EffectActor*>(actor)->LaunchEffect(2297, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x007A35A0
// FUNCTION: fn_08995
EfstActionMessage CEfstAction_AIN_RHAPSODY::Slot03() {
  return {LookupMessage(3797), 0xFF};
}

// OFFSET: 0x007A35D0
// FUNCTION: fn_08996
EfstActionMessage CEfstAction_AIN_RHAPSODY::Slot04() {
  return {LookupMessage(3798), 0xFFFF00};
}

// OFFSET: 0x007A3600
// FUNCTION: fn_08997
void CSkillAction_TR_GEF_NOCTURN::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gef_nocturn_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gef_nocturn_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007A38A0
// FUNCTION: fn_08998
void CSkillAction_TR_GEF_NOCTURN::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gef_nocturn"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A3A30
// FUNCTION: fn_08999
void CEfstAction_GEF_NOCTURN::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    static_cast<EffectActor*>(actor)->LaunchEffect(2293, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x007A3A80
// FUNCTION: fn_09000
EfstActionMessage CEfstAction_GEF_NOCTURN::Slot03() {
  return {LookupMessage(3795), 0xFF};
}

// OFFSET: 0x007A3AB0
// FUNCTION: fn_09001
EfstActionMessage CEfstAction_GEF_NOCTURN::Slot04() {
  return {LookupMessage(3796), 0xFFFF00};
}

// OFFSET: 0x007A3AE0
// FUNCTION: fn_09002
void CSkillAction_TR_RHYTHMICAL_WAVE::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmical_wave_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmical_wave_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007A3D90
// FUNCTION: fn_09003
void CSkillAction_TR_RHYTHMICAL_WAVE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmical_wave_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmical_wave"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A4010
// FUNCTION: fn_09004
void CSkillAction_TR_RHYTHMICAL_WAVE::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmical_wave_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A41A0
// FUNCTION: fn_09006
void CSkillAction_TR_METALIC_FURY::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("metalic_fury"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A4300
// FUNCTION: fn_09007
void CSkillAction_TR_MYSTIC_SYMPHONY::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mysticsymphony_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007A4490
// FUNCTION: fn_09008
void CSkillAction_TR_MYSTIC_SYMPHONY::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mysticsymphony"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A4620
// FUNCTION: fn_09009
void CEfstAction_MYSTIC_SYMPHONY::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mysticsymphony_note"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A47A0
// FUNCTION: fn_09010
void CEfstAction_MYSTIC_SYMPHONY::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mysticsymphony_note"));
  }
}

// OFFSET: 0x007A48E0
// FUNCTION: fn_09011
void CEfstAction_MYSTIC_SYMPHONY::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007A4900
// FUNCTION: fn_09012
EfstActionMessage CEfstAction_MYSTIC_SYMPHONY::Slot03() {
  return {LookupMessage(3793), 0xFFFF00};
}

// OFFSET: 0x007A4930
// FUNCTION: fn_09013
EfstActionMessage CEfstAction_MYSTIC_SYMPHONY::Slot04() {
  return {LookupMessage(3794), 0xFF};
}

// OFFSET: 0x007A4960
// FUNCTION: fn_09014
int CSkillAction_TR_RHYTHMSHOOTING::Slot02() {
  return 9;
}

// OFFSET: 0x007A4970
// FUNCTION: fn_09015
void CSkillAction_TR_RHYTHMSHOOTING::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmshooting_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmshooting_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
  }
}

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
// FUNCTION: fn_09017
void CSkillAction_TR_RHYTHMSHOOTING::Slot16(int, CGameActor* source, CGameActor*) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmshooting_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A4F80
// FUNCTION: fn_09018
void CSkillAction_TR_ROKI_CAPRICCIO::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roki_capriccio_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roki_capriccio_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007A5230
// FUNCTION: fn_09019
void CSkillAction_TR_ROKI_CAPRICCIO::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roki_capriccio"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A53C0
// FUNCTION: fn_09020
void CSkillAction_TR_ROSEBLOSSOM::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roseblossom_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roseblossom_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007A5660
// FUNCTION: fn_09021
void CSkillAction_TR_ROSEBLOSSOM::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    if (CEffect* effect = g_effectMgr->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roseblossom_shot"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        )) {
      const float angle = ScreenAngle(
        &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
      );
      effect->Slot17(angle + 180.0f);
    }
  }
}
