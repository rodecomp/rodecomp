#include "Client/Mode/CGameMode.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Particles/EZ2STREffectEx.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/Vector3d.h"

#include "Resource/MSGSTRINGID.h"
class CSkillAction_TR_ROSEBLOSSOM : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_TR_ROSEBLOSSOM_ATK : public CSkillAction {
public:
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_TR_KVASIR_SONATA : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_KVASIR_SONATA : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_TR_PRON_MARCH : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_PRON_MARCH : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_TR_JAWAII_SERENADE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_JAWAII_SERENADE : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_TR_AIN_RHAPSODY : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_AIN_RHAPSODY : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_TR_GEF_NOCTURN : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_GEF_NOCTURN : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_TR_RHYTHMICAL_WAVE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_TR_METALIC_FURY : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_TR_MYSTIC_SYMPHONY : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_MYSTIC_SYMPHONY : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_TR_RHYTHMSHOOTING : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_TR_ROKI_CAPRICCIO : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

// OFFSET: 0x007A20F0
// FUNCTION: fn_08975
void CSkillAction_TR_ROSEBLOSSOM::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  if (CGameActor* actor = reinterpret_cast<CGameActor*>(level)) {
    actor->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
  }
}

// OFFSET: 0x007A2120
// FUNCTION: fn_08976
void CSkillAction_TR_ROSEBLOSSOM::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roseblossom_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A2290
// FUNCTION: fn_08977
void CSkillAction_TR_ROSEBLOSSOM_ATK::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roseblossom_rose"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A2400
// FUNCTION: fn_08978
void CSkillAction_TR_KVASIR_SONATA::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kvasirsonata_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kvasirsonata_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A26A0
// FUNCTION: fn_08979
void CSkillAction_TR_KVASIR_SONATA::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kvasirsonata"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A2820
// FUNCTION: fn_08980
EfstActionMessage CEfstAction_KVASIR_SONATA::GetEfstBeginMessage() {
  return {LookupMessage(MSI_TR_MYSTIC_SYMPHONY_OFF), 0xFFFF00};
}

// OFFSET: 0x007A2850
// FUNCTION: fn_08981
EfstActionMessage CEfstAction_KVASIR_SONATA::GetEfstEndMessage() {
  return {LookupMessage(MSI_TR_GEF_NOCTURN_ON), 0xFF};
}

// OFFSET: 0x007A2880
// FUNCTION: fn_08982
void CSkillAction_TR_PRON_MARCH::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("pronmarch_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("pronmarch_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A2B20
// FUNCTION: fn_08983
void CSkillAction_TR_PRON_MARCH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("pronmarch"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A2CA0
// FUNCTION: fn_08984
void CEfstAction_PRON_MARCH::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    static_cast<EffectActor*>(actor)->LaunchEffect(EF_STATUS_PATK_UP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x007A2CF0
// FUNCTION: fn_08985
EfstActionMessage CEfstAction_PRON_MARCH::GetEfstBeginMessage() {
  return {LookupMessage(MSI_WH_CALAMITYGALE_OFF), 0xFFFF00};
}

// OFFSET: 0x007A2D20
// FUNCTION: fn_08986
EfstActionMessage CEfstAction_PRON_MARCH::GetEfstEndMessage() {
  return {LookupMessage(MSI_WH_WIND_SIGN_ON), 0xFF};
}

// OFFSET: 0x007A2D50
// FUNCTION: fn_08987
void CSkillAction_TR_JAWAII_SERENADE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jawaii_serenade_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jawaii_serenade_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A3000
// FUNCTION: fn_08988
void CSkillAction_TR_JAWAII_SERENADE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jawaii_serenade"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A3190
// FUNCTION: fn_08989
void CEfstAction_JAWAII_SERENADE::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    static_cast<EffectActor*>(actor)->LaunchEffect(EF_STATUS_SMATK_UP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x007A31E0
// FUNCTION: fn_08990
EfstActionMessage CEfstAction_JAWAII_SERENADE::GetEfstBeginMessage() {
  return {LookupMessage(MSI_TR_PRON_MARCH_OFF), 0xFFFF00};
}

// OFFSET: 0x007A3210
// FUNCTION: fn_08991
EfstActionMessage CEfstAction_JAWAII_SERENADE::GetEfstEndMessage() {
  return {LookupMessage(MSI_WH_CALAMITYGALE_ON), 0xFF};
}

// OFFSET: 0x007A3240
// FUNCTION: fn_08992
void CSkillAction_TR_AIN_RHAPSODY::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ain_rhapsody_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A33D0
// FUNCTION: fn_08993
void CSkillAction_TR_AIN_RHAPSODY::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ain_rhapsody"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A3550
// FUNCTION: fn_08994
void CEfstAction_AIN_RHAPSODY::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    static_cast<EffectActor*>(actor)->LaunchEffect(EF_STATUS_RES_DOWN, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x007A35A0
// FUNCTION: fn_08995
EfstActionMessage CEfstAction_AIN_RHAPSODY::GetEfstBeginMessage() {
  return {LookupMessage(MSI_TR_MUSICAL_INTERLUDE_OFF), 0xFF};
}

// OFFSET: 0x007A35D0
// FUNCTION: fn_08996
EfstActionMessage CEfstAction_AIN_RHAPSODY::GetEfstEndMessage() {
  return {LookupMessage(MSI_TR_JAWAII_SERENADE_ON), 0xFFFF00};
}

// OFFSET: 0x007A3600
// FUNCTION: fn_08997
void CSkillAction_TR_GEF_NOCTURN::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gef_nocturn_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gef_nocturn_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A38A0
// FUNCTION: fn_08998
void CSkillAction_TR_GEF_NOCTURN::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gef_nocturn"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A3A30
// FUNCTION: fn_08999
void CEfstAction_GEF_NOCTURN::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    static_cast<EffectActor*>(actor)->LaunchEffect(EF_STATUS_MRES_DOWN, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x007A3A80
// FUNCTION: fn_09000
EfstActionMessage CEfstAction_GEF_NOCTURN::GetEfstBeginMessage() {
  return {LookupMessage(MSI_TR_AIN_RHAPSODY_OFF), 0xFF};
}

// OFFSET: 0x007A3AB0
// FUNCTION: fn_09001
EfstActionMessage CEfstAction_GEF_NOCTURN::GetEfstEndMessage() {
  return {LookupMessage(MSI_TR_MUSICAL_INTERLUDE_ON), 0xFFFF00};
}

// OFFSET: 0x007A3AE0
// FUNCTION: fn_09002
void CSkillAction_TR_RHYTHMICAL_WAVE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmical_wave_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmical_wave_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A3D90
// FUNCTION: fn_09003
void CSkillAction_TR_RHYTHMICAL_WAVE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmical_wave_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmical_wave"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A4010
// FUNCTION: fn_09004
void CSkillAction_TR_RHYTHMICAL_WAVE::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmical_wave_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A4190
// FUNCTION: fn_09005
int CSkillAction_TR_METALIC_FURY::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x007A41A0
// FUNCTION: fn_09006
void CSkillAction_TR_METALIC_FURY::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("metalic_fury"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A4300
// FUNCTION: fn_09007
void CSkillAction_TR_MYSTIC_SYMPHONY::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mysticsymphony_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A4490
// FUNCTION: fn_09008
void CSkillAction_TR_MYSTIC_SYMPHONY::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mysticsymphony"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A4620
// FUNCTION: fn_09009
void CEfstAction_MYSTIC_SYMPHONY::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mysticsymphony_note"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A47A0
// FUNCTION: fn_09010
void CEfstAction_MYSTIC_SYMPHONY::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mysticsymphony_note"));
  }
}

// OFFSET: 0x007A48E0
// FUNCTION: fn_09011
void CEfstAction_MYSTIC_SYMPHONY::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007A4900
// FUNCTION: fn_09012
EfstActionMessage CEfstAction_MYSTIC_SYMPHONY::GetEfstBeginMessage() {
  return {LookupMessage(MSI_TR_GEF_NOCTURN_OFF), 0xFFFF00};
}

// OFFSET: 0x007A4930
// FUNCTION: fn_09013
EfstActionMessage CEfstAction_MYSTIC_SYMPHONY::GetEfstEndMessage() {
  return {LookupMessage(MSI_TR_AIN_RHAPSODY_ON), 0xFF};
}

// OFFSET: 0x007A4960
// FUNCTION: fn_09014
int CSkillAction_TR_RHYTHMSHOOTING::GetSkillAttackMotion() {
  return 9;
}

// OFFSET: 0x007A4970
// FUNCTION: fn_09015
void CSkillAction_TR_RHYTHMSHOOTING::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmshooting_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmshooting_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A4C20
// FUNCTION: fn_09016
void CSkillAction_TR_RHYTHMSHOOTING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmshooting_shot"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
    effect->SetAngle(angle + 180.0f);
  }
}

// OFFSET: 0x007A4E00
// FUNCTION: fn_09017
void CSkillAction_TR_RHYTHMSHOOTING::OnSkillHit(int, CGameActor* source, CGameActor*) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rhythmshooting_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A4F80
// FUNCTION: fn_09018
void CSkillAction_TR_ROKI_CAPRICCIO::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roki_capriccio_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roki_capriccio_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A5230
// FUNCTION: fn_09019
void CSkillAction_TR_ROKI_CAPRICCIO::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    source->SetFrameSequence(96, 1, 3, 96, 3, 1, 500, 0);
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roki_capriccio"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A53C0
// FUNCTION: fn_09020
void CSkillAction_TR_ROSEBLOSSOM::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roseblossom_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roseblossom_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A5660
// FUNCTION: fn_09021
void CSkillAction_TR_ROSEBLOSSOM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("roseblossom_shot"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      const float angle = ScreenAngle(
        &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
      );
      effect->SetAngle(angle + 180.0f);
    }
  }
}

// GLOBAL: gl_25933
static EfstActionRegistrar s_efstAction1260(1260, NewAction<CEfstAction_AIN_RHAPSODY, CEfstAction>);
// GLOBAL: gl_25934
static EfstActionRegistrar s_efstAction1259(1259, NewAction<CEfstAction_GEF_NOCTURN, CEfstAction>);
// GLOBAL: gl_25935
static EfstActionRegistrar s_efstAction1262(1262, NewAction<CEfstAction_JAWAII_SERENADE, CEfstAction>);
// GLOBAL: gl_25936
static EfstActionRegistrar s_efstAction1257(1257, NewAction<CEfstAction_KVASIR_SONATA, CEfstAction>);
// GLOBAL: gl_25937
static EfstActionRegistrar s_efstAction1256(1256, NewAction<CEfstAction_MYSTIC_SYMPHONY, CEfstAction>);
// GLOBAL: gl_25938
static EfstActionRegistrar s_efstAction1263(1263, NewAction<CEfstAction_PRON_MARCH, CEfstAction>);
// GLOBAL: gl_25939
static SkillActionRegistrar s_skillAction5360(5360, NewAction<CSkillAction_TR_AIN_RHAPSODY, CSkillAction>);
// GLOBAL: gl_25940
static SkillActionRegistrar s_skillAction5358(5358, NewAction<CSkillAction_TR_GEF_NOCTURN, CSkillAction>);
// GLOBAL: gl_25941
static SkillActionRegistrar s_skillAction5362(5362, NewAction<CSkillAction_TR_JAWAII_SERENADE, CSkillAction>);
// GLOBAL: gl_25942
static SkillActionRegistrar s_skillAction5352(5352, NewAction<CSkillAction_TR_KVASIR_SONATA, CSkillAction>);
// GLOBAL: gl_25943
static SkillActionRegistrar s_skillAction5356(5356, NewAction<CSkillAction_TR_METALIC_FURY, CSkillAction>);
// GLOBAL: gl_25944
static SkillActionRegistrar s_skillAction5351(5351, NewAction<CSkillAction_TR_MYSTIC_SYMPHONY, CSkillAction>);
// GLOBAL: gl_25945
static SkillActionRegistrar s_skillAction5364(5364, NewAction<CSkillAction_TR_PRON_MARCH, CSkillAction>);
// GLOBAL: gl_25946
static SkillActionRegistrar s_skillAction6521(6521, NewAction<CSkillAction_TR_RHYTHMICAL_WAVE, CSkillAction>);
// GLOBAL: gl_25947
static SkillActionRegistrar s_skillAction5355(5355, NewAction<CSkillAction_TR_RHYTHMSHOOTING, CSkillAction>);
// GLOBAL: gl_25948
static SkillActionRegistrar s_skillAction5359(5359, NewAction<CSkillAction_TR_ROKI_CAPRICCIO, CSkillAction>);
// GLOBAL: gl_25949
static SkillActionRegistrar s_skillAction5353(5353, NewAction<CSkillAction_TR_ROSEBLOSSOM, CSkillAction>);
// GLOBAL: gl_25950
static SkillActionRegistrar s_skillAction5354(5354, NewAction<CSkillAction_TR_ROSEBLOSSOM_ATK, CSkillAction>);
