#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Contents/Effects/Particles/EZ2STREffectEx.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <string>

class CSkillAction_NW_INTENSIVE_AIM : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_INTENSIVE_AIM_COUNT : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_INTENSIVE_AIM : public CEfstAction {
public:
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_NW_GRENADE_FRAGMENT : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_THE_VIGILANTE_AT_NIGHT : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
};

class CSkillAction_NW_THE_VIGILANTE_AT_NIGHT_GUN_GATLING : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_THE_VIGILANTE_AT_NIGHT_GUN_SHOTGUN : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_ONLY_ONE_BULLET : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnTargetSkillCast(int argument1, int argument2, int argument3) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_SPIRAL_SHOOTING : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_MAGAZINE_FOR_ONE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_WILD_FIRE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_NW_BASIC_GRENADE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
  int GetSkillScopeType() override;
};

class CSkillAction_NW_HASTY_FIRE_IN_THE_HOLE : public CSkillAction {
public:
  bool KeepSkillAttackMotion(int argument) override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_NW_GRENADES_DROPPING : public CSkillAction {
public:
  bool KeepSkillAttackMotion(int argument) override;
  void OnSkillCast(int level, CGameActor* source) override;
  int GetSkillScopeType() override;
};

class CSkillObjectAction_NW_GRENADES_DROPPING : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_NW_AUTO_FIRING_LAUNCHER : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_HIDDEN_CARD : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_HIDDEN_CARD : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_NW_MISSION_BOMBARD : public CSkillAction {
public:
  bool KeepSkillAttackMotion(int argument) override;
  void OnSkillCast(int level, CGameActor* source) override;
  int GetSkillScopeType() override;
};

class CSkillObjectAction_NW_MISSION_BOMBARD : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_NW_WILD_SHOT : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NW_MIDNIGHT_FALLEN : public CSkillAction {
public:
  bool KeepSkillAttackMotion(int argument) override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
  int GetSkillScopeType() override;
};

// OFFSET: 0x0078E470
// FUNCTION: fn_08676
void CSkillAction_NW_INTENSIVE_AIM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("intensiveaim_helix"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078E5F0
// FUNCTION: fn_08677
void CEfstAction_INTENSIVE_AIM_COUNT::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    for (int index = 0; index <= 10; ++index) {
      EffectLists* effects = g_effectMgr;
      effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(FormatOwnedString("intensive_aim_%02d", index)));
    }
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("intensiveaim_full"));
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(FormatOwnedString("intensive_aim_%02d", value)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (value == 10) {
      effects = g_effectMgr;
      effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("intensiveaim_full"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    }
  }
}

// OFFSET: 0x0078E9A0
// FUNCTION: fn_08678
void CEfstAction_INTENSIVE_AIM_COUNT::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0078E9C0
// FUNCTION: fn_08679
void CEfstAction_INTENSIVE_AIM::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    for (int index = 0; index <= 10; ++index) {
      EffectLists* effects = g_effectMgr;
      effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(FormatOwnedString("intensive_aim_%02d", index)));
    }
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("intensiveaim_full"));
  }
}

// OFFSET: 0x0078EBC0
// FUNCTION: fn_08680
void CSkillAction_NW_GRENADE_FRAGMENT::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  EffectLists* effects;
  switch (level) {
  case 1:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_water"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case 2:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_wind"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case 3:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_earth"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case 4:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_fire"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case 5:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_darkness"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case 6:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_saint"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case 7:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("grenade_fragment_reset"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  }
}

// OFFSET: 0x0078F0D0
// FUNCTION: fn_08681
void CSkillAction_NW_THE_VIGILANTE_AT_NIGHT::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2367, level / 16);
  }
}

// OFFSET: 0x0078F110
// FUNCTION: fn_08683
void CSkillAction_NW_THE_VIGILANTE_AT_NIGHT_GUN_GATLING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("the_vigilante_at_night_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }

  if (!target) {
    return;
  }

  for (int index = 0; index < 7; ++index) {
    const vector3d position = target->render.position;
    CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("the_vigilante_at_night_crack"), position, 0, -1, 0, 0);
    if (!effect) {
      return;
    }

    const int first = rand() % 15;
    const int second = rand() % 15;
    effect->position[0] = static_cast<float>(index % 2 * 15 - std::max(second, 0)) + target->render.position.x;
    effect->position[2] = target->render.position.z + 0.0f;
    effect->position[1] = static_cast<float>(index % 4 / 2 * 15 - std::max(first, 0)) + target->render.position.y;
    effect->SetDelay(index * 32);
    effect->SetScale(static_cast<float>(std::max(rand() % 51, 0) + 50) * 0.01f);
  }
}

// OFFSET: 0x0078F540
// FUNCTION: fn_08685
void CSkillAction_NW_THE_VIGILANTE_AT_NIGHT_GUN_SHOTGUN::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("the_vigilante_at_night_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }

  if (!target) {
    return;
  }

  for (int index = 0; index < 4; ++index) {
    const vector3d position = target->render.position;
    CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("the_vigilante_at_night_crack"), position, 0, -1, 0, 0);
    if (!effect) {
      return;
    }

    const int first = rand() % 10;
    const int second = rand() % 10;
    effect->position[0] = static_cast<float>(index % 2 * 10 - std::max(second, 0)) + target->render.position.x;
    effect->position[2] = target->render.position.z + 0.0f;
    effect->position[1] = static_cast<float>(index % 4 / 2 * 10 - std::max(first, 0)) + target->render.position.y;
    effect->SetDelay(index * 32);
    effect->SetScale(static_cast<float>(std::max(rand() % 31, 0) + 70) * 0.01f);
  }
}

// OFFSET: 0x0078F960
// FUNCTION: fn_08687
void CSkillAction_NW_ONLY_ONE_BULLET::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2367, level / 16);
  }
}

// OFFSET: 0x0078F990
// FUNCTION: fn_08688
void CSkillAction_NW_ONLY_ONE_BULLET::OnTargetSkillCast(int argument1, int argument2, int argument3) {
  if (CGameActor* source = reinterpret_cast<CGameActor*>(argument2)) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("only_one_bullet"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078FB00
// FUNCTION: fn_08689
void CSkillAction_NW_ONLY_ONE_BULLET::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  CGameActor* origin = reinterpret_cast<CGameActor*>(level);
  if (!origin || !source) {
    return;
  }

  const float angle = ScreenAngle(&origin->render.position.x, &source->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("only_one_bullet_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetAngle(angle);
  }
}

// OFFSET: 0x0078FCE0
// FUNCTION: fn_08691
void CSkillAction_NW_SPIRAL_SHOOTING::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2367, level / 16);
  }
}

// OFFSET: 0x0078FD10
// FUNCTION: fn_08692
void CSkillAction_NW_SPIRAL_SHOOTING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("spiral_shooting"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0078FE90
// FUNCTION: fn_08694
void CSkillAction_NW_MAGAZINE_FOR_ONE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2367, level / 16);
  }
}

// OFFSET: 0x0078FEC0
// FUNCTION: fn_08695
void CSkillAction_NW_MAGAZINE_FOR_ONE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("magazine_for_one"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00790040
// FUNCTION: fn_08697
void CSkillAction_NW_WILD_FIRE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2367, level / 16);
  }
}

// OFFSET: 0x00790070
// FUNCTION: fn_08698
void CSkillAction_NW_WILD_FIRE::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("wild_fire"), position.x, position.y, position.z, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("wild_fire_bottom"), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x007902A0
// FUNCTION: fn_08701
void CSkillAction_NW_BASIC_GRENADE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x007902D0
// FUNCTION: fn_08702
void CSkillAction_NW_BASIC_GRENADE::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("basic_grenade"), position.x, position.y, position.z, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("basic_grenade_bottom"), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x007904F0
// FUNCTION: fn_08703
int CSkillAction_NW_BASIC_GRENADE::GetSkillScopeType() {
  return 3;
}

// OFFSET: 0x00790510
// FUNCTION: fn_08705
bool CSkillAction_NW_HASTY_FIRE_IN_THE_HOLE::KeepSkillAttackMotion(int argument) {
  return true;
}

// OFFSET: 0x00790520
// FUNCTION: fn_08706
void CSkillAction_NW_HASTY_FIRE_IN_THE_HOLE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x00790550
// FUNCTION: fn_08707
void CSkillAction_NW_HASTY_FIRE_IN_THE_HOLE::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  for (int index = 0; index < 3; ++index) {
    CEffect* effect = g_effectMgr->Spawn(
      0, CEZ2STREffectMgr::GetInstance()->EffectId(FormatOwnedString("hasty_fire_in_the_hole_%d", index).c_str()), position, 0, -1, 0, 0
    );
    if (effect) {
      effect->SetDelay(index * 288);
    }

    effect = g_effectMgr->Spawn(
      0, CEZ2STREffectMgr::GetInstance()->EffectId(FormatOwnedString("hasty_fire_in_the_hole_bottom_%d", index).c_str()), position, 0, -1, 0, 0
    );
    if (effect) {
      effect->SetDelay(index * 288);
    }
  }
}

// OFFSET: 0x007908E0
// FUNCTION: fn_08710
bool CSkillAction_NW_GRENADES_DROPPING::KeepSkillAttackMotion(int argument) {
  return true;
}

// OFFSET: 0x007908F0
// FUNCTION: fn_08711
void CSkillAction_NW_GRENADES_DROPPING::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x00790920
// FUNCTION: fn_08712
int CSkillAction_NW_GRENADES_DROPPING::GetSkillScopeType() {
  return 3;
}

// OFFSET: 0x00790930
// FUNCTION: fn_08713
void CSkillObjectAction_NW_GRENADES_DROPPING::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    g_effectMgr->Spawn(object->render.accountId, 2351, object->render.position, 0, argument, 0, 0);
  }
}

// OFFSET: 0x00790990
// FUNCTION: fn_08715
void CSkillAction_NW_AUTO_FIRING_LAUNCHER::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(830, level / 16);
  }
}

// OFFSET: 0x007909C0
// FUNCTION: fn_08716
void CSkillAction_NW_AUTO_FIRING_LAUNCHER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("auto_firing_launcher"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("auto_firing_launcher_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00790C60
// FUNCTION: fn_08718
void CSkillAction_NW_HIDDEN_CARD::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(840, level / 16);
  }
}

// OFFSET: 0x00790C90
// FUNCTION: fn_08719
void CSkillAction_NW_HIDDEN_CARD::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hiddencard"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00790DF0
// FUNCTION: fn_08720
void CEfstAction_HIDDEN_CARD::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hiddencard_thunder"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00790F70
// FUNCTION: fn_08721
void CEfstAction_HIDDEN_CARD::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hiddencard_thunder"));
  }
}

// OFFSET: 0x007910A0
// FUNCTION: fn_08722
void CEfstAction_HIDDEN_CARD::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007910D0
// FUNCTION: fn_08724
bool CSkillAction_NW_MISSION_BOMBARD::KeepSkillAttackMotion(int argument) {
  return true;
}

// OFFSET: 0x007910E0
// FUNCTION: fn_08725
void CSkillAction_NW_MISSION_BOMBARD::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(840, level / 16);
  }
}

// OFFSET: 0x00791110
// FUNCTION: fn_08726
int CSkillAction_NW_MISSION_BOMBARD::GetSkillScopeType() {
  return 9;
}

// OFFSET: 0x00791120
// FUNCTION: fn_08727
void CSkillObjectAction_NW_MISSION_BOMBARD::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mission_bombard"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mission_bombard_fire"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mission_bombard_fire_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    g_effectMgr->Spawn(object->render.accountId, 2354, object->render.position, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007914F0
// FUNCTION: fn_08729
void CSkillAction_NW_WILD_SHOT::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_shot_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x00791670
// FUNCTION: fn_08730
void CSkillAction_NW_WILD_SHOT::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  int delay = 0;
  for (int angle = 0; angle < 360; angle += 45) {
    int first = rand() % 5;
    if (first < 3) {
      first = 3;
    }
    const float radians = static_cast<float>(angle) / 180.0f * 3.141592f;
    const float offsetZ = ClientCos(radians) * static_cast<float>(first * 5);
    int second = rand() % 5;
    if (second < 3) {
      second = 3;
    }
    const float offsetX = ClientSin(radians) * static_cast<float>(second * 5);
    const float x = offsetX + target->render.position.x;
    const float y = target->render.position.y + 0.0f;
    const float z = offsetZ + target->render.position.z;
    CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("wild_shot"), vector3d{x, y, z}, 0, -1, 0, 0);
    if (effect) {
      effect->SetScale(0.8f);
      effect->SetDelay(delay);
      effect->SetAngle(static_cast<float>(std::ranges::max(rand() % 360, 0)));
    }
    delay += 240;
  }
}

// OFFSET: 0x00791950
// FUNCTION: fn_08731
void CSkillAction_NW_WILD_SHOT::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Retire(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_shot_hit"));
  effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("wild_shot_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetScale(0.8f);
  }
}

// OFFSET: 0x00791BB0
// FUNCTION: fn_08733
bool CSkillAction_NW_MIDNIGHT_FALLEN::KeepSkillAttackMotion(int argument) {
  return true;
}

// OFFSET: 0x00791BC0
// FUNCTION: fn_08734
void CSkillAction_NW_MIDNIGHT_FALLEN::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("midnight_fallen_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x00791D60
// FUNCTION: fn_08735
void CSkillAction_NW_MIDNIGHT_FALLEN::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  int delay = 0;
  for (int angle = 45; angle < 405; angle += 120) {
    const float radians = static_cast<float>(angle) / 180.0f * 3.141592f;
    const float offsetZ = ClientSin(radians) * 10.0f;
    const float offsetX = ClientCos(radians) * 10.0f;
    const float z = offsetZ + position.z;
    const float y = position.y + 0.0f;
    const float x = offsetX + position.x;
    EffectLists* effects = g_effectMgr;
    CEffect* effect = effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("midnight_fallen_bottom"), x, y, z, 0, -1, 0, 0);
    if (effect) {
      effect->SetDelay(delay);
    }
    delay += 96;
  }
}

// OFFSET: 0x00791FD0
// FUNCTION: fn_08736
void CSkillAction_NW_MIDNIGHT_FALLEN::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("midnight_fallen_hit"));
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("midnight_fallen_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00792230
// FUNCTION: fn_08737
int CSkillAction_NW_MIDNIGHT_FALLEN::GetSkillScopeType() {
  return 7;
}
