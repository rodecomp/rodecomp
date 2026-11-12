#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Skills/TalismanMarkEffect.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/Vector3d.h"

class CSkillAction_SOA_TALISMAN_OF_FIVE_ELEMENTS : public CSkillAction {
public:
  bool IsSupport() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_TALISMAN_OF_SOUL_STEALING : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int argument1, CGameActor* argument2, CGameActor* argument3) override;
};

class CSkillAction_SOA_EXORCISM_OF_MALICIOUS_SOUL : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_TALISMAN_OF_BLUE_DRAGON : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_T_FIRST_GOD : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SOA_TALISMAN_OF_WHITE_TIGER : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_T_SECOND_GOD : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SOA_TALISMAN_OF_RED_PHOENIX : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_T_THIRD_GOD : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SOA_TALISMAN_OF_BLACK_TORTOISE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
  int GetSkillScopeType() override;
};

class CEfstAction_T_FOURTH_GOD : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SOA_TALISMAN_OF_FOUR_BEARING_GOD : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_CIRCLE_OF_DIRECTIONS_AND_ELEMENTALS : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_T_FIVETH_GOD : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SOA_SOUL_OF_HEAVEN_AND_EARTH : public CSkillAction {
public:
  bool IsSupport() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_HEAVEN_AND_EARTH : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SOA_TALISMAN_OF_PROTECTION : public CSkillAction {
public:
  bool IsSupport() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_TALISMAN_OF_WARRIOR : public CSkillAction {
public:
  bool IsSupport() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_TALISMAN_OF_MAGICIAN : public CSkillAction {
public:
  bool IsSupport() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_SOUL_GATHERING : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_TOTEM_OF_TUTELARY : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  int GetSkillScopeType() override;
};

class CSkillObjectAction_SOA_TOTEM_OF_TUTELARY : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
  void OnSkillObjectRemove(CRenderObject* object) override;
};

// OFFSET: 0x0079B9A0
// FUNCTION: fn_08874
bool CSkillAction_SOA_TALISMAN_OF_FIVE_ELEMENTS::IsSupport() {
  return true;
}

// OFFSET: 0x0079B9B0
// FUNCTION: fn_08875
void CSkillAction_SOA_TALISMAN_OF_FIVE_ELEMENTS::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x0079B9E0
// FUNCTION: fn_08876
void CSkillAction_SOA_TALISMAN_OF_FIVE_ELEMENTS::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_the_five_elements"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079BB70
// FUNCTION: fn_08877
void CSkillAction_SOA_TALISMAN_OF_SOUL_STEALING::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(441, level / 16);
  }
}

// OFFSET: 0x0079BBA0
// FUNCTION: fn_08878
void CSkillAction_SOA_TALISMAN_OF_SOUL_STEALING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    target->LaunchEffect(253, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->SendMsg(nullptr, AM_MAKE_NUMBER, level, 0xFF0000FFu, 114, 0, 0, 0);
  }
}

// OFFSET: 0x0079BC20
// FUNCTION: fn_08879
void CSkillAction_SOA_TALISMAN_OF_SOUL_STEALING::OnSkillHit(int argument1, CGameActor* argument2, CGameActor* argument3) {
  CGameActor* source = reinterpret_cast<CGameActor*>(argument1);
  CGameActor* target = argument2;
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_soul_stealing"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_magician"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079BEC0
// FUNCTION: fn_08880
void CSkillAction_SOA_EXORCISM_OF_MALICIOUS_SOUL::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2367, level / 16);
  }
}

// OFFSET: 0x0079BEF0
// FUNCTION: fn_08881
void CSkillAction_SOA_EXORCISM_OF_MALICIOUS_SOUL::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("exorcism_of_malicuous_soul"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079C080
// FUNCTION: fn_08882
void CSkillAction_SOA_TALISMAN_OF_BLUE_DRAGON::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(845, level / 16);
  }
}

// OFFSET: 0x0079C0B0
// FUNCTION: fn_08883
void CSkillAction_SOA_TALISMAN_OF_BLUE_DRAGON::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_blue_dragon"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079C240
// FUNCTION: fn_08884
void CEfstAction_T_FIRST_GOD::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2355);
    g_effectMgr->Spawn(actor->render.accountId, 2355, actor->render.position, 0, 1, 0, 0);
  }
}

// OFFSET: 0x0079C2A0
// FUNCTION: fn_08885
void CEfstAction_T_FIRST_GOD::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    if (CEffect* effect = g_effectMgr->Find(actor->render.accountId, 2355)) {
      static_cast<CTalismanMarkEffect*>(effect)->PlayEnd();
    }
  }
}

// OFFSET: 0x0079C2D0
// FUNCTION: fn_08886
void CEfstAction_T_FIRST_GOD::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079C2F0
// FUNCTION: fn_08887
void CSkillAction_SOA_TALISMAN_OF_WHITE_TIGER::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(843, level / 16);
  }
}

// OFFSET: 0x0079C320
// FUNCTION: fn_08888
void CSkillAction_SOA_TALISMAN_OF_WHITE_TIGER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_white_tiger"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079C4B0
// FUNCTION: fn_08889
void CEfstAction_T_SECOND_GOD::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2355);
    g_effectMgr->Spawn(actor->render.accountId, 2355, actor->render.position, 0, 2, 0, 0);
  }
}

// OFFSET: 0x0079C510
// FUNCTION: fn_08890
void CEfstAction_T_SECOND_GOD::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    if (CEffect* effect = g_effectMgr->Find(actor->render.accountId, 2355)) {
      static_cast<CTalismanMarkEffect*>(effect)->PlayEnd();
    }
  }
}

// OFFSET: 0x0079C540
// FUNCTION: fn_08891
void CEfstAction_T_SECOND_GOD::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079C560
// FUNCTION: fn_08892
void CSkillAction_SOA_TALISMAN_OF_RED_PHOENIX::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(840, level / 16);
  }
}

// OFFSET: 0x0079C590
// FUNCTION: fn_08893
void CSkillAction_SOA_TALISMAN_OF_RED_PHOENIX::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_red_phoenix_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_red_phoenix"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079C840
// FUNCTION: fn_08894
void CEfstAction_T_THIRD_GOD::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2355);
    g_effectMgr->Spawn(actor->render.accountId, 2355, actor->render.position, 0, 3, 0, 0);
  }
}

// OFFSET: 0x0079C8A0
// FUNCTION: fn_08895
void CEfstAction_T_THIRD_GOD::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    if (CEffect* effect = g_effectMgr->Find(actor->render.accountId, 2355)) {
      static_cast<CTalismanMarkEffect*>(effect)->PlayEnd();
    }
  }
}

// OFFSET: 0x0079C8D0
// FUNCTION: fn_08896
void CEfstAction_T_THIRD_GOD::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079C8F0
// FUNCTION: fn_08897
void CSkillAction_SOA_TALISMAN_OF_BLACK_TORTOISE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2368, level / 16);
  }
}

// OFFSET: 0x0079C920
// FUNCTION: fn_08898
void CSkillAction_SOA_TALISMAN_OF_BLACK_TORTOISE::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_black_tortoise_bottom"), position.x, position.y, position.z, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_black_tortoise"), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x0079CB60
// FUNCTION: fn_08899
int CSkillAction_SOA_TALISMAN_OF_BLACK_TORTOISE::GetSkillScopeType() {
  return 9;
}

// OFFSET: 0x0079CB70
// FUNCTION: fn_08900
void CEfstAction_T_FOURTH_GOD::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2355);
    g_effectMgr->Spawn(actor->render.accountId, 2355, actor->render.position, 0, 4, 0, 0);
  }
}

// OFFSET: 0x0079CBD0
// FUNCTION: fn_08901
void CEfstAction_T_FOURTH_GOD::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    if (CEffect* effect = g_effectMgr->Find(actor->render.accountId, 2355)) {
      static_cast<CTalismanMarkEffect*>(effect)->PlayEnd();
    }
  }
}

// OFFSET: 0x0079CC00
// FUNCTION: fn_08902
void CEfstAction_T_FOURTH_GOD::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079CC20
// FUNCTION: fn_08903
void CSkillAction_SOA_TALISMAN_OF_FOUR_BEARING_GOD::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2369, level / 16);
  }
}

// OFFSET: 0x0079CC50
// FUNCTION: fn_08904
void CSkillAction_SOA_TALISMAN_OF_FOUR_BEARING_GOD::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_four_bearing_god"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079CDD0
// FUNCTION: fn_08905
void CSkillAction_SOA_CIRCLE_OF_DIRECTIONS_AND_ELEMENTALS::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(1565, level / 16);
  }
}

// OFFSET: 0x0079CE00
// FUNCTION: fn_08906
void CSkillAction_SOA_CIRCLE_OF_DIRECTIONS_AND_ELEMENTALS::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("circle_directions_elements"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079CF90
// FUNCTION: fn_08907
void CEfstAction_T_FIVETH_GOD::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("circle_directions_elements_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079D130
// FUNCTION: fn_08908
void CEfstAction_T_FIVETH_GOD::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("circle_directions_elements_loop"));
  }
}

// OFFSET: 0x0079D280
// FUNCTION: fn_08909
void CEfstAction_T_FIVETH_GOD::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079D2A0
// FUNCTION: fn_08910
bool CSkillAction_SOA_SOUL_OF_HEAVEN_AND_EARTH::IsSupport() {
  return true;
}

// OFFSET: 0x0079D2B0
// FUNCTION: fn_08911
void CSkillAction_SOA_SOUL_OF_HEAVEN_AND_EARTH::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(1556, level / 16);
  }
}

// OFFSET: 0x0079D2E0
// FUNCTION: fn_08912
void CSkillAction_SOA_SOUL_OF_HEAVEN_AND_EARTH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("soul_of_heaven_and_earth"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079D460
// FUNCTION: fn_08913
void CEfstAction_HEAVEN_AND_EARTH::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("soul_of_heaven_and_earth_buff"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079D5F0
// FUNCTION: fn_08914
void CEfstAction_HEAVEN_AND_EARTH::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("soul_of_heaven_and_earth_buff"));
  }
}

// OFFSET: 0x0079D740
// FUNCTION: fn_08915
void CEfstAction_HEAVEN_AND_EARTH::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079D760
// FUNCTION: fn_08916
bool CSkillAction_SOA_TALISMAN_OF_PROTECTION::IsSupport() {
  return true;
}

// OFFSET: 0x0079D770
// FUNCTION: fn_08917
void CSkillAction_SOA_TALISMAN_OF_PROTECTION::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x0079D7A0
// FUNCTION: fn_08918
void CSkillAction_SOA_TALISMAN_OF_PROTECTION::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_protection"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079D920
// FUNCTION: fn_08919
bool CSkillAction_SOA_TALISMAN_OF_WARRIOR::IsSupport() {
  return true;
}

// OFFSET: 0x0079D930
// FUNCTION: fn_08920
void CSkillAction_SOA_TALISMAN_OF_WARRIOR::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x0079D960
// FUNCTION: fn_08921
void CSkillAction_SOA_TALISMAN_OF_WARRIOR::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_warrior"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079DAE0
// FUNCTION: fn_08922
bool CSkillAction_SOA_TALISMAN_OF_MAGICIAN::IsSupport() {
  return true;
}

// OFFSET: 0x0079DAF0
// FUNCTION: fn_08923
void CSkillAction_SOA_TALISMAN_OF_MAGICIAN::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x0079DB20
// FUNCTION: fn_08924
void CSkillAction_SOA_TALISMAN_OF_MAGICIAN::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_magician"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_magician_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079DDB0
// FUNCTION: fn_08925
void CSkillAction_SOA_SOUL_GATHERING::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(454, level / 16);
  }
}

// OFFSET: 0x0079DDE0
// FUNCTION: fn_08926
void CSkillAction_SOA_SOUL_GATHERING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("soul_gathering"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079DF50
// FUNCTION: fn_08927
void CSkillAction_SOA_TOTEM_OF_TUTELARY::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(911, level / 16);
  }
}

// OFFSET: 0x0079DF80
// FUNCTION: fn_08928
int CSkillAction_SOA_TOTEM_OF_TUTELARY::GetSkillScopeType() {
  return 7;
}

// OFFSET: 0x0079DF90
// FUNCTION: fn_08929
void CSkillObjectAction_SOA_TOTEM_OF_TUTELARY::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_bottom_start"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_bottom_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_start"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079E460
// FUNCTION: fn_08930
void CSkillObjectAction_SOA_TOTEM_OF_TUTELARY::OnSkillObjectRemove(CRenderObject* object) {
  if (object) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_bottom_loop"));
    effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_bottom_end"), object->render.position, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_loop"));
    effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_end"), object->render.position, 0, -1, 0, 0);
  }
}

// GLOBAL: gl_25894
static EfstActionRegistrar s_efstAction1365(1365, NewAction<CEfstAction_HEAVEN_AND_EARTH, CEfstAction>);
// GLOBAL: gl_25895
static EfstActionRegistrar s_efstAction1360(1360, NewAction<CEfstAction_T_FIRST_GOD, CEfstAction>);
// GLOBAL: gl_25896
static EfstActionRegistrar s_efstAction1364(1364, NewAction<CEfstAction_T_FIVETH_GOD, CEfstAction>);
// GLOBAL: gl_25897
static EfstActionRegistrar s_efstAction1363(1363, NewAction<CEfstAction_T_FOURTH_GOD, CEfstAction>);
// GLOBAL: gl_25898
static EfstActionRegistrar s_efstAction1361(1361, NewAction<CEfstAction_T_SECOND_GOD, CEfstAction>);
// GLOBAL: gl_25899
static EfstActionRegistrar s_efstAction1362(1362, NewAction<CEfstAction_T_THIRD_GOD, CEfstAction>);

// OFFSET: 0x00460720
// FUNCTION: fn_45566
static bool s_skillObjectActionRegistered292 = [](std::function<CSkillObjectAction*()> factory) {
  CSkillActionMgr::GetInstance()->RegisterSkillObjectAction(292, factory);
  return true;
}(NewAction<CSkillObjectAction_SOA_TOTEM_OF_TUTELARY, CSkillObjectAction>);

// GLOBAL: gl_25900
static SkillActionRegistrar s_skillAction5431(5431, NewAction<CSkillAction_SOA_CIRCLE_OF_DIRECTIONS_AND_ELEMENTALS, CSkillAction>);
// GLOBAL: gl_25901
static SkillActionRegistrar s_skillAction5425(5425, NewAction<CSkillAction_SOA_EXORCISM_OF_MALICIOUS_SOUL, CSkillAction>);
// GLOBAL: gl_25902
static SkillActionRegistrar s_skillAction5421(5421, NewAction<CSkillAction_SOA_SOUL_GATHERING, CSkillAction>);
// GLOBAL: gl_25903
static SkillActionRegistrar s_skillAction5432(5432, NewAction<CSkillAction_SOA_SOUL_OF_HEAVEN_AND_EARTH, CSkillAction>);
// GLOBAL: gl_25904
static SkillActionRegistrar s_skillAction5429(5429, NewAction<CSkillAction_SOA_TALISMAN_OF_BLACK_TORTOISE, CSkillAction>);
// GLOBAL: gl_25905
static SkillActionRegistrar s_skillAction5426(5426, NewAction<CSkillAction_SOA_TALISMAN_OF_BLUE_DRAGON, CSkillAction>);
// GLOBAL: gl_25906
static SkillActionRegistrar s_skillAction5423(5423, NewAction<CSkillAction_SOA_TALISMAN_OF_FIVE_ELEMENTS, CSkillAction>);
// GLOBAL: gl_25907
static SkillActionRegistrar s_skillAction5430(5430, NewAction<CSkillAction_SOA_TALISMAN_OF_FOUR_BEARING_GOD, CSkillAction>);
// GLOBAL: gl_25908
static SkillActionRegistrar s_skillAction5420(5420, NewAction<CSkillAction_SOA_TALISMAN_OF_MAGICIAN, CSkillAction>);
// GLOBAL: gl_25909
static SkillActionRegistrar s_skillAction5418(5418, NewAction<CSkillAction_SOA_TALISMAN_OF_PROTECTION, CSkillAction>);
// GLOBAL: gl_25910
static SkillActionRegistrar s_skillAction5428(5428, NewAction<CSkillAction_SOA_TALISMAN_OF_RED_PHOENIX, CSkillAction>);
// GLOBAL: gl_25911
static SkillActionRegistrar s_skillAction5424(5424, NewAction<CSkillAction_SOA_TALISMAN_OF_SOUL_STEALING, CSkillAction>);
// GLOBAL: gl_25912
static SkillActionRegistrar s_skillAction5419(5419, NewAction<CSkillAction_SOA_TALISMAN_OF_WARRIOR, CSkillAction>);
// GLOBAL: gl_25913
static SkillActionRegistrar s_skillAction5427(5427, NewAction<CSkillAction_SOA_TALISMAN_OF_WHITE_TIGER, CSkillAction>);
// GLOBAL: gl_25914
static SkillActionRegistrar s_skillAction5422(5422, NewAction<CSkillAction_SOA_TOTEM_OF_TUTELARY, CSkillAction>);
