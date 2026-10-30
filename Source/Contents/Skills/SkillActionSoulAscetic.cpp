#include "Contents/Actors/GameActor.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/TalismanMarkEffect.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/Vector3d.h"

class CSkillAction_SOA_TALISMAN_OF_FIVE_ELEMENTS : public CSkillAction {
public:
  bool Slot00() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_TALISMAN_OF_SOUL_STEALING : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int argument1, CGameActor* argument2, CGameActor* argument3) override;
};

class CSkillAction_SOA_EXORCISM_OF_MALICIOUS_SOUL : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_TALISMAN_OF_BLUE_DRAGON : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_T_FIRST_GOD : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SOA_TALISMAN_OF_WHITE_TIGER : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_T_SECOND_GOD : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SOA_TALISMAN_OF_RED_PHOENIX : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_T_THIRD_GOD : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SOA_TALISMAN_OF_BLACK_TORTOISE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot12(vector3d position, int argument2, int argument3) override;
  int Slot14() override;
};

class CEfstAction_T_FOURTH_GOD : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SOA_TALISMAN_OF_FOUR_BEARING_GOD : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_CIRCLE_OF_DIRECTIONS_AND_ELEMENTALS : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_T_FIVETH_GOD : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SOA_SOUL_OF_HEAVEN_AND_EARTH : public CSkillAction {
public:
  bool Slot00() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_HEAVEN_AND_EARTH : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SOA_TALISMAN_OF_PROTECTION : public CSkillAction {
public:
  bool Slot00() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_TALISMAN_OF_WARRIOR : public CSkillAction {
public:
  bool Slot00() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_TALISMAN_OF_MAGICIAN : public CSkillAction {
public:
  bool Slot00() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_SOUL_GATHERING : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SOA_TOTEM_OF_TUTELARY : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  int Slot14() override;
};

class CSkillObjectAction_SOA_TOTEM_OF_TUTELARY : public CSkillObjectAction {
public:
  void Slot00(int argument, CRenderObject* object, CGameActor* source) override;
  void Slot01(CRenderObject* object) override;
};

// OFFSET: 0x0079B9A0
// FUNCTION: fn_08874
bool CSkillAction_SOA_TALISMAN_OF_FIVE_ELEMENTS::Slot00() {
  return true;
}

// OFFSET: 0x0079B9B0
// FUNCTION: fn_08875
void CSkillAction_SOA_TALISMAN_OF_FIVE_ELEMENTS::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x0079B9E0
// FUNCTION: fn_08876
void CSkillAction_SOA_TALISMAN_OF_FIVE_ELEMENTS::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_the_five_elements"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079BB70
// FUNCTION: fn_08877
void CSkillAction_SOA_TALISMAN_OF_SOUL_STEALING::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(441, level / 16);
  }
}

// OFFSET: 0x0079BBA0
// FUNCTION: fn_08878
void CSkillAction_SOA_TALISMAN_OF_SOUL_STEALING::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    target->LaunchEffect(253, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->SendMsg(nullptr, 88, 0, level, level >> 31, static_cast<int>(0xFF0000FF), 0, 114, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0079BC20
// FUNCTION: fn_08879
void CSkillAction_SOA_TALISMAN_OF_SOUL_STEALING::Slot15(int argument1, CGameActor* argument2, CGameActor* argument3) {
  CGameActor* source = reinterpret_cast<CGameActor*>(argument1);
  CGameActor* target = argument2;
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_soul_stealing"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_magician"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079BEC0
// FUNCTION: fn_08880
void CSkillAction_SOA_EXORCISM_OF_MALICIOUS_SOUL::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2367, level / 16);
  }
}

// OFFSET: 0x0079BEF0
// FUNCTION: fn_08881
void CSkillAction_SOA_EXORCISM_OF_MALICIOUS_SOUL::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("exorcism_of_malicuous_soul"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079C080
// FUNCTION: fn_08882
void CSkillAction_SOA_TALISMAN_OF_BLUE_DRAGON::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(845, level / 16);
  }
}

// OFFSET: 0x0079C0B0
// FUNCTION: fn_08883
void CSkillAction_SOA_TALISMAN_OF_BLUE_DRAGON::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_blue_dragon"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079C240
// FUNCTION: fn_08884
void CEfstAction_T_FIRST_GOD::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2355);
    g_effectMgr->Spawn(actor->render.accountId, 2355, actor->render.position.x, actor->render.position.y, actor->render.position.z, 0, 1, 0, 0);
  }
}

// OFFSET: 0x0079C2A0
// FUNCTION: fn_08885
void CEfstAction_T_FIRST_GOD::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    if (CEffect* effect = g_effectMgr->Find(actor->render.accountId, 2355)) {
      static_cast<CTalismanMarkEffect*>(effect)->PlayEnd();
    }
  }
}

// OFFSET: 0x0079C2D0
// FUNCTION: fn_08886
void CEfstAction_T_FIRST_GOD::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079C2F0
// FUNCTION: fn_08887
void CSkillAction_SOA_TALISMAN_OF_WHITE_TIGER::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(843, level / 16);
  }
}

// OFFSET: 0x0079C320
// FUNCTION: fn_08888
void CSkillAction_SOA_TALISMAN_OF_WHITE_TIGER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_white_tiger"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079C4B0
// FUNCTION: fn_08889
void CEfstAction_T_SECOND_GOD::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2355);
    g_effectMgr->Spawn(actor->render.accountId, 2355, actor->render.position.x, actor->render.position.y, actor->render.position.z, 0, 2, 0, 0);
  }
}

// OFFSET: 0x0079C510
// FUNCTION: fn_08890
void CEfstAction_T_SECOND_GOD::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    if (CEffect* effect = g_effectMgr->Find(actor->render.accountId, 2355)) {
      static_cast<CTalismanMarkEffect*>(effect)->PlayEnd();
    }
  }
}

// OFFSET: 0x0079C540
// FUNCTION: fn_08891
void CEfstAction_T_SECOND_GOD::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079C560
// FUNCTION: fn_08892
void CSkillAction_SOA_TALISMAN_OF_RED_PHOENIX::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(840, level / 16);
  }
}

// OFFSET: 0x0079C590
// FUNCTION: fn_08893
void CSkillAction_SOA_TALISMAN_OF_RED_PHOENIX::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_red_phoenix_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_red_phoenix"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079C840
// FUNCTION: fn_08894
void CEfstAction_T_THIRD_GOD::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2355);
    g_effectMgr->Spawn(actor->render.accountId, 2355, actor->render.position.x, actor->render.position.y, actor->render.position.z, 0, 3, 0, 0);
  }
}

// OFFSET: 0x0079C8A0
// FUNCTION: fn_08895
void CEfstAction_T_THIRD_GOD::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    if (CEffect* effect = g_effectMgr->Find(actor->render.accountId, 2355)) {
      static_cast<CTalismanMarkEffect*>(effect)->PlayEnd();
    }
  }
}

// OFFSET: 0x0079C8D0
// FUNCTION: fn_08896
void CEfstAction_T_THIRD_GOD::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079C8F0
// FUNCTION: fn_08897
void CSkillAction_SOA_TALISMAN_OF_BLACK_TORTOISE::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2368, level / 16);
  }
}

// OFFSET: 0x0079C920
// FUNCTION: fn_08898
void CSkillAction_SOA_TALISMAN_OF_BLACK_TORTOISE::Slot12(vector3d position, int argument2, int argument3) {
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_black_tortoise_bottom"), position.x, position.y, position.z, 0, -1, 0, 0);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_black_tortoise"), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x0079CB60
// FUNCTION: fn_08899
int CSkillAction_SOA_TALISMAN_OF_BLACK_TORTOISE::Slot14() {
  return 9;
}

// OFFSET: 0x0079CB70
// FUNCTION: fn_08900
void CEfstAction_T_FOURTH_GOD::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2355);
    g_effectMgr->Spawn(actor->render.accountId, 2355, actor->render.position.x, actor->render.position.y, actor->render.position.z, 0, 4, 0, 0);
  }
}

// OFFSET: 0x0079CBD0
// FUNCTION: fn_08901
void CEfstAction_T_FOURTH_GOD::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    if (CEffect* effect = g_effectMgr->Find(actor->render.accountId, 2355)) {
      static_cast<CTalismanMarkEffect*>(effect)->PlayEnd();
    }
  }
}

// OFFSET: 0x0079CC00
// FUNCTION: fn_08902
void CEfstAction_T_FOURTH_GOD::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079CC20
// FUNCTION: fn_08903
void CSkillAction_SOA_TALISMAN_OF_FOUR_BEARING_GOD::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2369, level / 16);
  }
}

// OFFSET: 0x0079CC50
// FUNCTION: fn_08904
void CSkillAction_SOA_TALISMAN_OF_FOUR_BEARING_GOD::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_four_bearing_god"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079CDD0
// FUNCTION: fn_08905
void CSkillAction_SOA_CIRCLE_OF_DIRECTIONS_AND_ELEMENTALS::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(1565, level / 16);
  }
}

// OFFSET: 0x0079CE00
// FUNCTION: fn_08906
void CSkillAction_SOA_CIRCLE_OF_DIRECTIONS_AND_ELEMENTALS::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("circle_directions_elements"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079CF90
// FUNCTION: fn_08907
void CEfstAction_T_FIVETH_GOD::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("circle_directions_elements_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079D130
// FUNCTION: fn_08908
void CEfstAction_T_FIVETH_GOD::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("circle_directions_elements_loop"));
  }
}

// OFFSET: 0x0079D280
// FUNCTION: fn_08909
void CEfstAction_T_FIVETH_GOD::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079D2A0
// FUNCTION: fn_08910
bool CSkillAction_SOA_SOUL_OF_HEAVEN_AND_EARTH::Slot00() {
  return true;
}

// OFFSET: 0x0079D2B0
// FUNCTION: fn_08911
void CSkillAction_SOA_SOUL_OF_HEAVEN_AND_EARTH::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(1556, level / 16);
  }
}

// OFFSET: 0x0079D2E0
// FUNCTION: fn_08912
void CSkillAction_SOA_SOUL_OF_HEAVEN_AND_EARTH::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("soul_of_heaven_and_earth"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079D460
// FUNCTION: fn_08913
void CEfstAction_HEAVEN_AND_EARTH::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("soul_of_heaven_and_earth_buff"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079D5F0
// FUNCTION: fn_08914
void CEfstAction_HEAVEN_AND_EARTH::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("soul_of_heaven_and_earth_buff"));
  }
}

// OFFSET: 0x0079D740
// FUNCTION: fn_08915
void CEfstAction_HEAVEN_AND_EARTH::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0079D760
// FUNCTION: fn_08916
bool CSkillAction_SOA_TALISMAN_OF_PROTECTION::Slot00() {
  return true;
}

// OFFSET: 0x0079D770
// FUNCTION: fn_08917
void CSkillAction_SOA_TALISMAN_OF_PROTECTION::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x0079D7A0
// FUNCTION: fn_08918
void CSkillAction_SOA_TALISMAN_OF_PROTECTION::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_protection"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079D920
// FUNCTION: fn_08919
bool CSkillAction_SOA_TALISMAN_OF_WARRIOR::Slot00() {
  return true;
}

// OFFSET: 0x0079D930
// FUNCTION: fn_08920
void CSkillAction_SOA_TALISMAN_OF_WARRIOR::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x0079D960
// FUNCTION: fn_08921
void CSkillAction_SOA_TALISMAN_OF_WARRIOR::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_warrior"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079DAE0
// FUNCTION: fn_08922
bool CSkillAction_SOA_TALISMAN_OF_MAGICIAN::Slot00() {
  return true;
}

// OFFSET: 0x0079DAF0
// FUNCTION: fn_08923
void CSkillAction_SOA_TALISMAN_OF_MAGICIAN::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x0079DB20
// FUNCTION: fn_08924
void CSkillAction_SOA_TALISMAN_OF_MAGICIAN::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_magician"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("talisman_of_magician_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079DDB0
// FUNCTION: fn_08925
void CSkillAction_SOA_SOUL_GATHERING::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(454, level / 16);
  }
}

// OFFSET: 0x0079DDE0
// FUNCTION: fn_08926
void CSkillAction_SOA_SOUL_GATHERING::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("soul_gathering"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079DF50
// FUNCTION: fn_08927
void CSkillAction_SOA_TOTEM_OF_TUTELARY::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(911, level / 16);
  }
}

// OFFSET: 0x0079DF80
// FUNCTION: fn_08928
int CSkillAction_SOA_TOTEM_OF_TUTELARY::Slot14() {
  return 7;
}

// OFFSET: 0x0079DF90
// FUNCTION: fn_08929
void CSkillObjectAction_SOA_TOTEM_OF_TUTELARY::Slot00(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_bottom_start"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_bottom_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_start"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0079E460
// FUNCTION: fn_08930
void CSkillObjectAction_SOA_TOTEM_OF_TUTELARY::Slot01(CRenderObject* object) {
  if (object) {
    g_effectMgr->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_bottom_loop"));
    g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_bottom_end"), object->render.position.x, object->render.position.y, object->render.position.z, 0, -1, 0, 0);
    g_effectMgr->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_loop"));
    g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("totem_of_tutelary_end"), object->render.position.x, object->render.position.y, object->render.position.z, 0, -1, 0, 0);
  }
}
