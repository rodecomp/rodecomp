#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/EZ2STREffectEx.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <string>

class CSkillAction_NW_INTENSIVE_AIM : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_INTENSIVE_AIM_COUNT : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_INTENSIVE_AIM : public CEfstAction {
public:
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_NW_GRENADE_FRAGMENT : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_THE_VIGILANTE_AT_NIGHT : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
};

class CSkillAction_NW_THE_VIGILANTE_AT_NIGHT_GUN_GATLING : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_THE_VIGILANTE_AT_NIGHT_GUN_SHOTGUN : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_ONLY_ONE_BULLET : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot09(int argument1, int argument2, int argument3) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_SPIRAL_SHOOTING : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_MAGAZINE_FOR_ONE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_WILD_FIRE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot12(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_NW_BASIC_GRENADE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot12(vector3d position, int argument2, int argument3) override;
  int Slot14() override;
};

class CSkillAction_NW_HASTY_FIRE_IN_THE_HOLE : public CSkillAction {
public:
  bool Slot03(int argument) override;
  void Slot08(int level, CGameActor* source) override;
  void Slot12(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_NW_GRENADES_DROPPING : public CSkillAction {
public:
  bool Slot03(int argument) override;
  void Slot08(int level, CGameActor* source) override;
  int Slot14() override;
};

class CSkillObjectAction_NW_GRENADES_DROPPING : public CSkillObjectAction {
public:
  void Slot00(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_NW_AUTO_FIRING_LAUNCHER : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_HIDDEN_CARD : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_HIDDEN_CARD : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_NW_MISSION_BOMBARD : public CSkillAction {
public:
  bool Slot03(int argument) override;
  void Slot08(int level, CGameActor* source) override;
  int Slot14() override;
};

class CSkillObjectAction_NW_MISSION_BOMBARD : public CSkillObjectAction {
public:
  void Slot00(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_NW_WILD_SHOT : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_MIDNIGHT_FALLEN : public CSkillAction {
public:
  bool Slot03(int argument) override;
  void Slot08(int level, CGameActor* source) override;
  void Slot12(vector3d position, int argument2, int argument3) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
  int Slot14() override;
};

// OFFSET: 0x0078E470
// FUNCTION: fn_08676
void CSkillAction_NW_INTENSIVE_AIM::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("intensiveaim_helix"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078E5F0
// FUNCTION: fn_08677
void CEfstAction_INTENSIVE_AIM_COUNT::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    for (int index = 0; index <= 10; ++index) {
      g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(FormatOwnedString("intensive_aim_%02d", index)));
    }
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("intensiveaim_full"));
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(FormatOwnedString("intensive_aim_%02d", value)), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (value == 10) {
      g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("intensiveaim_full"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
  }
}

// OFFSET: 0x0078E9A0
// FUNCTION: fn_08678
void CEfstAction_INTENSIVE_AIM_COUNT::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0078E9C0
// FUNCTION: fn_08679
void CEfstAction_INTENSIVE_AIM::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    for (int index = 0; index <= 10; ++index) {
      g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(FormatOwnedString("intensive_aim_%02d", index)));
    }
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("intensiveaim_full"));
  }
}

// OFFSET: 0x0078EBC0
// FUNCTION: fn_08680
void CSkillAction_NW_GRENADE_FRAGMENT::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  switch (level) {
  case 1:
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_water"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 2:
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_wind"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 3:
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_earth"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 4:
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_fire"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 5:
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_darkness"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 6:
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_saint"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 7:
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_reset"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  }
}

// OFFSET: 0x0078F0D0
// FUNCTION: fn_08681
void CSkillAction_NW_THE_VIGILANTE_AT_NIGHT::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2367, level / 16);
  }
}

// OFFSET: 0x0078F110
// FUNCTION: fn_08683
void CSkillAction_NW_THE_VIGILANTE_AT_NIGHT_GUN_GATLING::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("the_vigilante_at_night_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }

  if (!target) {
    return;
  }

  for (int index = 0; index < 7; ++index) {
    const vector3d position = target->render.position;
    CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("the_vigilante_at_night_crack"), position.x, position.y, position.z, 0, -1, 0, 0);
    if (!effect) {
      return;
    }

    const int first = rand() % 15;
    const int second = rand() % 15;
    effect->position[0] = static_cast<float>(index % 2 * 15 - std::max(second, 0)) + target->render.position.x;
    effect->position[2] = target->render.position.z + 0.0f;
    effect->position[1] = static_cast<float>(index % 4 / 2 * 15 - std::max(first, 0)) + target->render.position.y;
    effect->Slot16(index * 32);
    effect->Slot22(static_cast<float>(std::max(rand() % 51, 0) + 50) * 0.01f);
  }
}

// OFFSET: 0x0078F540

// OFFSET: 0x0078F960
// FUNCTION: fn_08687
void CSkillAction_NW_ONLY_ONE_BULLET::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2367, level / 16);
  }
}

// OFFSET: 0x0078F990
// FUNCTION: fn_08688
void CSkillAction_NW_ONLY_ONE_BULLET::Slot09(int argument1, int argument2, int argument3) {
  if (CGameActor* source = reinterpret_cast<CGameActor*>(argument2)) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("only_one_bullet"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078FB00
// FUNCTION: fn_08689
void CSkillAction_NW_ONLY_ONE_BULLET::Slot16(int level, CGameActor* source, CGameActor* target) {
  CGameActor* origin = reinterpret_cast<CGameActor*>(level);
  if (!origin || !source) {
    return;
  }

  const float angle = ScreenAngle(&origin->render.position.x, &source->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("only_one_bullet_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot17(angle);
  }
}

// OFFSET: 0x0078FCE0
// FUNCTION: fn_08691
void CSkillAction_NW_SPIRAL_SHOOTING::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2367, level / 16);
  }
}

// OFFSET: 0x0078FD10
// FUNCTION: fn_08692
void CSkillAction_NW_SPIRAL_SHOOTING::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("spiral_shooting"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078FE90
// FUNCTION: fn_08694
void CSkillAction_NW_MAGAZINE_FOR_ONE::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2367, level / 16);
  }
}

// OFFSET: 0x0078FEC0
// FUNCTION: fn_08695
void CSkillAction_NW_MAGAZINE_FOR_ONE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("magazine_for_one"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00790040
// FUNCTION: fn_08697
void CSkillAction_NW_WILD_FIRE::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2367, level / 16);
  }
}

// OFFSET: 0x00790070
// FUNCTION: fn_08698
void CSkillAction_NW_WILD_FIRE::Slot12(vector3d position, int argument2, int argument3) {
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("wild_fire"), position.x, position.y, position.z, 0, -1, 0, 0);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("wild_fire_bottom"), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x007902A0
// FUNCTION: fn_08701
void CSkillAction_NW_BASIC_GRENADE::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x007902D0
// FUNCTION: fn_08702
void CSkillAction_NW_BASIC_GRENADE::Slot12(vector3d position, int argument2, int argument3) {
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("basic_grenade"), position.x, position.y, position.z, 0, -1, 0, 0);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("basic_grenade_bottom"), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x007904F0
// FUNCTION: fn_08703
int CSkillAction_NW_BASIC_GRENADE::Slot14() {
  return 3;
}

// OFFSET: 0x00790510
// FUNCTION: fn_08705
bool CSkillAction_NW_HASTY_FIRE_IN_THE_HOLE::Slot03(int argument) {
  return true;
}

// OFFSET: 0x00790520
// FUNCTION: fn_08706
void CSkillAction_NW_HASTY_FIRE_IN_THE_HOLE::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x00790550

// OFFSET: 0x007908E0
// FUNCTION: fn_08710
bool CSkillAction_NW_GRENADES_DROPPING::Slot03(int argument) {
  return true;
}

// OFFSET: 0x007908F0
// FUNCTION: fn_08711
void CSkillAction_NW_GRENADES_DROPPING::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x00790920
// FUNCTION: fn_08712
int CSkillAction_NW_GRENADES_DROPPING::Slot14() {
  return 3;
}

// OFFSET: 0x00790930
// FUNCTION: fn_08713
void CSkillObjectAction_NW_GRENADES_DROPPING::Slot00(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    g_effectMgr->Spawn(object->render.accountId, 2351, object->render.position.x, object->render.position.y, object->render.position.z, 0, argument, 0, 0);
  }
}

// OFFSET: 0x00790990
// FUNCTION: fn_08715
void CSkillAction_NW_AUTO_FIRING_LAUNCHER::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x007909C0
// FUNCTION: fn_08716
void CSkillAction_NW_AUTO_FIRING_LAUNCHER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("auto_firing_launcher"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("auto_firing_launcher_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00790C60
// FUNCTION: fn_08718
void CSkillAction_NW_HIDDEN_CARD::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(840, level / 16);
  }
}

// OFFSET: 0x00790C90
// FUNCTION: fn_08719
void CSkillAction_NW_HIDDEN_CARD::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hiddencard"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00790DF0
// FUNCTION: fn_08720
void CEfstAction_HIDDEN_CARD::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hiddencard_thunder"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00790F70
// FUNCTION: fn_08721
void CEfstAction_HIDDEN_CARD::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hiddencard_thunder"));
  }
}

// OFFSET: 0x007910A0
// FUNCTION: fn_08722
void CEfstAction_HIDDEN_CARD::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007910D0
// FUNCTION: fn_08724
bool CSkillAction_NW_MISSION_BOMBARD::Slot03(int argument) {
  return true;
}

// OFFSET: 0x007910E0
// FUNCTION: fn_08725
void CSkillAction_NW_MISSION_BOMBARD::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(840, level / 16);
  }
}

// OFFSET: 0x00791110
// FUNCTION: fn_08726
int CSkillAction_NW_MISSION_BOMBARD::Slot14() {
  return 9;
}

// OFFSET: 0x00791120
// FUNCTION: fn_08727
void CSkillObjectAction_NW_MISSION_BOMBARD::Slot00(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mission_bombard"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mission_bombard_fire"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mission_bombard_fire_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(object->render.accountId, 2354, object->render.position.x, object->render.position.y, object->render.position.z, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007914F0
// FUNCTION: fn_08729
void CSkillAction_NW_WILD_SHOT::Slot08(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_shot_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x00791670

// OFFSET: 0x00791950
// FUNCTION: fn_08731
void CSkillAction_NW_WILD_SHOT::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  g_effectMgr->Retire(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_shot_hit"));
  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_shot_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot22(0.8f);
  }
}

// OFFSET: 0x00791BB0
// FUNCTION: fn_08733
bool CSkillAction_NW_MIDNIGHT_FALLEN::Slot03(int argument) {
  return true;
}

// OFFSET: 0x00791BC0
// FUNCTION: fn_08734
void CSkillAction_NW_MIDNIGHT_FALLEN::Slot08(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("midnight_fallen_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x00791D60

// OFFSET: 0x00791FD0
// FUNCTION: fn_08736
void CSkillAction_NW_MIDNIGHT_FALLEN::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Retire(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("midnight_fallen_hit"));
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("midnight_fallen_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00792230
// FUNCTION: fn_08737
int CSkillAction_NW_MIDNIGHT_FALLEN::Slot14() {
  return 7;
}
