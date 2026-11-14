#include "Client/Mode/CGameMode.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Contents/Effects/Particles/EZ2STREffectEx.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Text/FormatString.h"

#include <cstddef>
#include <cstdint>
#include <string>

class CSkillAction_SH_CHUL_HO_SONIC_CLAW : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SH_HOWLING_OF_CHUL_HO : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_HOGOGONG : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SH_HOGOGONG_STRIKE : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) override;
};

class CSkillAction_SH_KI_SUL_WATER_SPRAYING : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SH_MARINE_FESTIVAL_OF_KI_SUL : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_MARINE_FESTIVAL : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
};

class CSkillAction_SH_SANDY_FESTIVAL_OF_KI_SUL : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_SANDY_FESTIVAL : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
};

class CSkillAction_SH_KI_SUL_RAMPAGE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SH_COLORS_OF_HYUN_ROK : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SH_HYUN_ROKS_BREEZE : public CSkillAction {
public:
  bool KeepSkillAttackMotion(int argument) override;
  void OnSkillCast(int level, CGameActor* source) override;
  int GetSkillScopeType() override;
};

class CSkillAction_SH_HYUN_ROK_CANNON : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillObjectAction_SH_HYUN_ROKS_BREEZE : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_SH_TEMPORARY_COMMUNION : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_TEMPORARY_COMMUNION : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SH_BLESSING_OF_MYSTICAL_CREATURES : public CSkillAction {
public:
  bool IsSupport() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SH_CHUL_HO_BATTERING : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, CGameActor* hitIndex) override;
};

class CSkillAction_SH_HYUN_ROK_SPIRIT_POWER : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, CGameActor* hitIndex) override;
};

// OFFSET: 0x0079EB70
// FUNCTION: fn_08931
int CSkillAction_SH_CHUL_HO_SONIC_CLAW::GetSkillAttackMotion() {
  return 54;
}

// OFFSET: 0x0079EB80
// FUNCTION: fn_08932
void CSkillAction_SH_CHUL_HO_SONIC_CLAW::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chul_ho_sonic_claw"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (!target) {
    return;
  }

  const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
  const int waves[12] = {12, 1, 1, 3, 5, 5, 6, 7, 7, 9, 11, 11};
  const int index = static_cast<int>(angle + 105.0f) % 360 / 30;
  if (index >= 0 && static_cast<std::size_t>(index) < sizeof(waves) / sizeof(waves[0])) {
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(FormatOwnedString("chul_ho_sonic_claw_wave_%d", waves[index])), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079EE10
// FUNCTION: fn_08933
int CSkillAction_SH_HOWLING_OF_CHUL_HO::GetSkillAttackMotion() {
  return 54;
}

// OFFSET: 0x0079EE20
// FUNCTION: fn_08934
void CSkillAction_SH_HOWLING_OF_CHUL_HO::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL_N3, level / 16);
  }
}

// OFFSET: 0x0079EE50
// FUNCTION: fn_08935
void CSkillAction_SH_HOWLING_OF_CHUL_HO::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("howling_of_chul_ho"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079EFD0
// FUNCTION: fn_08936
void CEfstAction_HOGOGONG::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("howling_of_chul_ho_scared"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079F150
// FUNCTION: fn_08937
void CEfstAction_HOGOGONG::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("howling_of_chul_ho_scared"));
  }
}

// OFFSET: 0x0079F290
// FUNCTION: fn_08938
void CEfstAction_HOGOGONG::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079F2B0
// FUNCTION: fn_08939
int CSkillAction_SH_HOGOGONG_STRIKE::GetSkillAttackMotion() {
  return 54;
}

// OFFSET: 0x0079F2C0
// FUNCTION: fn_08940
void CSkillAction_SH_HOGOGONG_STRIKE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL_N3, level / 16);
  }
}

// OFFSET: 0x0079F2F0
// FUNCTION: fn_08941
void CSkillAction_SH_HOGOGONG_STRIKE::OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hogogong_strike"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079F460
// FUNCTION: fn_08942
void CSkillAction_SH_KI_SUL_WATER_SPRAYING::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL6, level / 16);
  }
}

// OFFSET: 0x0079F490
// FUNCTION: fn_08943
void CSkillAction_SH_KI_SUL_WATER_SPRAYING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ki_sul_water_spraying"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079F610
// FUNCTION: fn_08944
void CSkillAction_SH_MARINE_FESTIVAL_OF_KI_SUL::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL_N8, level / 16);
  }
}

// OFFSET: 0x0079F640
// FUNCTION: fn_08945
void CSkillAction_SH_MARINE_FESTIVAL_OF_KI_SUL::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("marine_festival_of_ki_sul_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079F7C0
// FUNCTION: fn_08946
void CEfstAction_MARINE_FESTIVAL::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("marine_festival_of_ki_sul"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079F940
// FUNCTION: fn_08947
void CSkillAction_SH_SANDY_FESTIVAL_OF_KI_SUL::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL_N5, level / 16);
  }
}

// OFFSET: 0x0079F970
// FUNCTION: fn_08948
void CSkillAction_SH_SANDY_FESTIVAL_OF_KI_SUL::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sandy_festival_of_ki_sul_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079FB10
// FUNCTION: fn_08949
void CEfstAction_SANDY_FESTIVAL::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("sandy_festival_of_ki_sul"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079FC90
// FUNCTION: fn_08950
void CSkillAction_SH_KI_SUL_RAMPAGE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL_N2, level / 16);
  }
}

// OFFSET: 0x0079FCC0
// FUNCTION: fn_08951
void CSkillAction_SH_KI_SUL_RAMPAGE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ki_sul_rampage_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ki_sul_rampage"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079FF40
// FUNCTION: fn_08952
void CSkillAction_SH_COLORS_OF_HYUN_ROK::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL6, level / 16);
  }
}

// OFFSET: 0x0079FF70
// FUNCTION: fn_08953
void CSkillAction_SH_COLORS_OF_HYUN_ROK::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  EffectLists* effects;
  switch (level) {
  case 1:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("colors_of_hyun_rok_water"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case 2:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("colors_of_hyun_rok_wind"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case 3:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("colors_of_hyun_rok_earth"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case 4:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("colors_of_hyun_rok_fire"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case 5:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("colors_of_hyun_rok_darkness"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case 6:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("colors_of_hyun_rok_saint"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case 7:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("colors_of_hyun_rok_reset"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  }
}

// OFFSET: 0x007A04A0
// FUNCTION: fn_08954
bool CSkillAction_SH_HYUN_ROKS_BREEZE::KeepSkillAttackMotion(int argument) {
  return true;
}

// OFFSET: 0x007A04B0
// FUNCTION: fn_08955
void CSkillAction_SH_HYUN_ROKS_BREEZE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL_N4, level / 16);
  }
}

// OFFSET: 0x007A04E0
// FUNCTION: fn_08956
int CSkillAction_SH_HYUN_ROKS_BREEZE::GetSkillScopeType() {
  return 9;
}

// OFFSET: 0x007A04F0
// FUNCTION: fn_08957
void CSkillObjectAction_SH_HYUN_ROKS_BREEZE::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hyun_roks_breeze_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hyun_roks_breeze"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A0770
// FUNCTION: fn_08958
void CSkillAction_SH_HYUN_ROK_CANNON::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL_N4, level / 16);
  }
}

// OFFSET: 0x007A07A0
// FUNCTION: fn_08959
void CSkillAction_SH_HYUN_ROK_CANNON::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  const float angle = ScreenAngle(
    &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
  );
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect =
        effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("hyun_rok_cannon"), source->render.position, 0, -1, 0, 0)) {
    effect->SetAngle(angle);
  }
}

// OFFSET: 0x007A0950
// FUNCTION: fn_08960
void CSkillAction_SH_TEMPORARY_COMMUNION::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL_RING_YELLOW, level / 16);
  }
}

// OFFSET: 0x007A0980
// FUNCTION: fn_08961
void CSkillAction_SH_TEMPORARY_COMMUNION::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("temporary_communion"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A0B00
// FUNCTION: fn_08962
void CEfstAction_TEMPORARY_COMMUNION::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("temporary_communion_buff_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("temporary_communion_buff"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A0DA0
// FUNCTION: fn_08963
void CEfstAction_TEMPORARY_COMMUNION::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("temporary_communion_buff_bottom"));
    effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("temporary_communion_buff"));
  }
}

// OFFSET: 0x007A0FD0
// FUNCTION: fn_08964
void CEfstAction_TEMPORARY_COMMUNION::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007A0FF0
// FUNCTION: fn_08965
bool CSkillAction_SH_BLESSING_OF_MYSTICAL_CREATURES::IsSupport() {
  return true;
}

// OFFSET: 0x007A1000
// FUNCTION: fn_08966
void CSkillAction_SH_BLESSING_OF_MYSTICAL_CREATURES::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL_RING_YELLOW, level / 16);
  }
}

// OFFSET: 0x007A1030
// FUNCTION: fn_08967
void CSkillAction_SH_BLESSING_OF_MYSTICAL_CREATURES::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("blessing_of_mystical_creatures"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A11C0
// FUNCTION: fn_08968
int CSkillAction_SH_CHUL_HO_BATTERING::GetSkillAttackMotion() {
  return 54;
}

// OFFSET: 0x007A11D0
// FUNCTION: fn_08969
void CSkillAction_SH_CHUL_HO_BATTERING::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chul_ho_battering_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A1360
// FUNCTION: fn_08970
void CSkillAction_SH_CHUL_HO_BATTERING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target || !source) {
    return;
  }

  const float scale = source->attackMotion * source->render.speed * 16.0f;
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(
        target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chul_ho_battering"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
      )) {
    effect->SetDelay(static_cast<int>(scale));
  }
  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(
        target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chul_ho_battering_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
      )) {
    effect->SetDelay(static_cast<int>(scale));
  }
}

// OFFSET: 0x007A1630
// FUNCTION: fn_08971
void CSkillAction_SH_CHUL_HO_BATTERING::OnSkillDamage(CGameActor* source, CGameActor* target, CGameActor* hitIndex) {
  if (target) {
    if (CEffect* effect = g_effectMgr->Spawn(
          target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chul_ho_battering_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetFlipX(static_cast<bool>(reinterpret_cast<std::uintptr_t>(hitIndex) & 1));
    }
  }
}

// OFFSET: 0x007A17D0
// FUNCTION: fn_08972
void CSkillAction_SH_HYUN_ROK_SPIRIT_POWER::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hyun_rok_spirit_power_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hyun_rok_spirit_power_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        )) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007A1A80
// FUNCTION: fn_08973
void CSkillAction_SH_HYUN_ROK_SPIRIT_POWER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hyun_rok_spirit_power"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hyun_rok_spirit_power_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007A1D10
// FUNCTION: fn_08974
void CSkillAction_SH_HYUN_ROK_SPIRIT_POWER::OnSkillDamage(CGameActor* source, CGameActor* target, CGameActor* hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hyun_rok_spirit_power_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// GLOBAL: gl_25915
static EfstActionRegistrar s_efstAction1366(1366, NewAction<CEfstAction_HOGOGONG, CEfstAction>);
// GLOBAL: gl_25916
static EfstActionRegistrar s_efstAction1367(1367, NewAction<CEfstAction_MARINE_FESTIVAL, CEfstAction>);
// GLOBAL: gl_25917
static EfstActionRegistrar s_efstAction1368(1368, NewAction<CEfstAction_SANDY_FESTIVAL, CEfstAction>);
// GLOBAL: gl_25918
static EfstActionRegistrar s_efstAction1377(1377, NewAction<CEfstAction_TEMPORARY_COMMUNION, CEfstAction>);

// OFFSET: 0x004616C0
// FUNCTION: fn_45567
static bool s_skillObjectActionRegistered293 = [](std::function<CSkillObjectAction*()> factory) {
  CSkillActionMgr::GetInstance()->RegisterSkillObjectAction(293, factory);
  return true;
}(NewAction<CSkillObjectAction_SH_HYUN_ROKS_BREEZE, CSkillObjectAction>);

// GLOBAL: gl_25919
static SkillActionRegistrar s_skillAction5448(5448, NewAction<CSkillAction_SH_BLESSING_OF_MYSTICAL_CREATURES, CSkillAction>);
// GLOBAL: gl_25920
static SkillActionRegistrar s_skillAction5506(5506, NewAction<CSkillAction_SH_CHUL_HO_BATTERING, CSkillAction>);
// GLOBAL: gl_25921
static SkillActionRegistrar s_skillAction5435(5435, NewAction<CSkillAction_SH_CHUL_HO_SONIC_CLAW, CSkillAction>);
// GLOBAL: gl_25922
static SkillActionRegistrar s_skillAction5444(5444, NewAction<CSkillAction_SH_COLORS_OF_HYUN_ROK, CSkillAction>);
// GLOBAL: gl_25923
static SkillActionRegistrar s_skillAction5437(5437, NewAction<CSkillAction_SH_HOGOGONG_STRIKE, CSkillAction>);
// GLOBAL: gl_25924
static SkillActionRegistrar s_skillAction5436(5436, NewAction<CSkillAction_SH_HOWLING_OF_CHUL_HO, CSkillAction>);
// GLOBAL: gl_25925
static SkillActionRegistrar s_skillAction5445(5445, NewAction<CSkillAction_SH_HYUN_ROKS_BREEZE, CSkillAction>);
// GLOBAL: gl_25926
static SkillActionRegistrar s_skillAction5446(5446, NewAction<CSkillAction_SH_HYUN_ROK_CANNON, CSkillAction>);
// GLOBAL: gl_25927
static SkillActionRegistrar s_skillAction5507(5507, NewAction<CSkillAction_SH_HYUN_ROK_SPIRIT_POWER, CSkillAction>);
// GLOBAL: gl_25928
static SkillActionRegistrar s_skillAction5442(5442, NewAction<CSkillAction_SH_KI_SUL_RAMPAGE, CSkillAction>);
// GLOBAL: gl_25929
static SkillActionRegistrar s_skillAction5439(5439, NewAction<CSkillAction_SH_KI_SUL_WATER_SPRAYING, CSkillAction>);
// GLOBAL: gl_25930
static SkillActionRegistrar s_skillAction5440(5440, NewAction<CSkillAction_SH_MARINE_FESTIVAL_OF_KI_SUL, CSkillAction>);
// GLOBAL: gl_25931
static SkillActionRegistrar s_skillAction5441(5441, NewAction<CSkillAction_SH_SANDY_FESTIVAL_OF_KI_SUL, CSkillAction>);
// GLOBAL: gl_25932
static SkillActionRegistrar s_skillAction5447(5447, NewAction<CSkillAction_SH_TEMPORARY_COMMUNION, CSkillAction>);
