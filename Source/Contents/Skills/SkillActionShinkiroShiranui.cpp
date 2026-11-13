#include "Contents/Actors/ActorEnums.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/CombatTypes.h"
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
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

template __declspec(noinline) std::vector<int>::~vector() noexcept;

namespace {

struct ShuffledSequence {
  std::vector<int> operator()(int count) const;
};

}

class CSkillAction_SS_REIKETSUHOU : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnGroundSkillEffect(vector3d position, int level, int argument3) override;
};

class CSkillAction_SS_RAIDENPOU : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_KINRYUUHOU : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_ANTENPOU : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_KAGEAKUMU : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_HITOUAKUMU : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_ANKOKURYUUAKUMU : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_FOUR_CHARM : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

// OFFSET: 0x007950C0
// FUNCTION: fn_08785
void CSkillAction_SS_REIKETSUHOU::OnGroundSkillEffect(vector3d position, int level, int argument3) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("reiketsuhou_bottom"), position.x, position.y, position.z, 0, -1, 0, 0);

  const int counts[] = {5, 5, 5, 5, 5, 6, 6, 6, 7, 7};
  const int count = counts[std::min<unsigned int>(level, 10) - 1];
  for (int index = 0; index < count; ++index) {
    vector3d spot = position;
    const int rollX = std::rand() % (count * 50 + 1);
    spot.x += ((rollX >= 0 ? rollX : 0) - count * 50 * 0.5f) * 0.1f;
    const int rollZ = std::rand() % (count * 50 + 1);
    spot.z += ((rollZ >= 0 ? rollZ : 0) - count * 50 * 0.5f) * 0.1f;
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("reiketsuhou"), spot.x, spot.y, spot.z, 0, -1, 0, 0)) {
      effect->SetDelay(index * 80);
    }
  }
}

// OFFSET: 0x00795450
// FUNCTION: fn_08786
int CSkillAction_SS_RAIDENPOU::GetSkillCastMotion() {
  return 55;
}

// OFFSET: 0x00795460
// FUNCTION: fn_08787
int CSkillAction_SS_RAIDENPOU::GetSkillAttackMotion() {
  return 57;
}

// OFFSET: 0x00795470
// FUNCTION: fn_08788
void CSkillAction_SS_RAIDENPOU::OnSkillCast(int level, CGameActor* source) {
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_raidenpou_cast"),
                                       vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x007955F0
// FUNCTION: fn_08789
void CSkillAction_SS_RAIDENPOU::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_raidenpou_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_raidenpou"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00795860
// FUNCTION: fn_08790
void CSkillAction_SS_RAIDENPOU::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_raidenpou_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007959E0
// FUNCTION: fn_08791
int CSkillAction_SS_KINRYUUHOU::GetSkillCastMotion() {
  return 55;
}

// OFFSET: 0x007959F0
// FUNCTION: fn_08792
int CSkillAction_SS_KINRYUUHOU::GetSkillAttackMotion() {
  return 57;
}

// OFFSET: 0x00795A00
// FUNCTION: fn_08793
void CSkillAction_SS_KINRYUUHOU::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL6, level / 16);
  }
}

// OFFSET: 0x00795A30
// FUNCTION: fn_08794
void CSkillAction_SS_KINRYUUHOU::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("kinryuuhou"), source->render.position, 0, -1, 0, 0)) {
      effect->SetAngle(angle);
    }
  }
}

// OFFSET: 0x00795BD0
// FUNCTION: fn_08795
void CSkillAction_SS_KINRYUUHOU::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kinryuuhou_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00795D40
// FUNCTION: fn_08796
int CSkillAction_SS_ANTENPOU::GetSkillCastMotion() {
  return 55;
}

// OFFSET: 0x00795D50
// FUNCTION: fn_08797
int CSkillAction_SS_ANTENPOU::GetSkillAttackMotion() {
  return 57;
}

// OFFSET: 0x00795D60
// FUNCTION: fn_08798
void CSkillAction_SS_ANTENPOU::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL7, level / 16);
  }
}

// OFFSET: 0x00795D90
// FUNCTION: fn_08799
void CSkillAction_SS_ANTENPOU::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("antenpou"), source->render.position, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00795EE0
// FUNCTION: fn_08800
int CSkillAction_SS_KAGEAKUMU::GetSkillCastMotion() {
  return 55;
}

// OFFSET: 0x00795EF0
// FUNCTION: fn_08801
int CSkillAction_SS_KAGEAKUMU::GetSkillAttackMotion() {
  return 57;
}

// OFFSET: 0x00795F00
// FUNCTION: fn_08802
void CSkillAction_SS_KAGEAKUMU::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BLACKBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    static_cast<EffectActor*>(source)->LaunchEffect(EF_NEW_EPICLESIS_CAST, level / 16);
  }
}

// OFFSET: 0x00795F60
// FUNCTION: fn_08803
void CSkillAction_SS_KAGEAKUMU::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kageakumu"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007960C0
// FUNCTION: fn_08804
int CSkillAction_SS_HITOUAKUMU::GetSkillCastMotion() {
  return 55;
}

// OFFSET: 0x007960D0
// FUNCTION: fn_08805
int CSkillAction_SS_HITOUAKUMU::GetSkillAttackMotion() {
  return 57;
}

// OFFSET: 0x007960E0
// FUNCTION: fn_08806
void CSkillAction_SS_HITOUAKUMU::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BLACKBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    static_cast<EffectActor*>(source)->LaunchEffect(EF_NEW_EPICLESIS_CAST, level / 16);
  }
}

// OFFSET: 0x00796140
// FUNCTION: fn_08807
void CSkillAction_SS_HITOUAKUMU::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("hitouakumu"), source->render.position, 0, -1, 0, 0);

  ShuffledSequence shuffle;
  for (int ring = 0; ring < 3; ++ring) {
    const int roll = std::rand() % 5;
    const int count = (roll >= 0 ? roll : 0) + 15;
    const float step = 360.0f / count;
    const std::vector<int> order = shuffle(count);
    for (int index = 0; index < count; ++index) {
      effects = g_effectMgr;
      CEffect* effect = effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(FormatOwnedString("hitouakumu_%02d", ring)), source->render.position, 0, -1, 0, 0);
      if (effect) {
        const int spread = static_cast<int>(step);
        effect->SetAngle(index * step + (spread ? std::max(0, std::rand() % spread) : 0));
        effect->SetDelay((order[index] + ring * 10) * 32);
      }
    }
  }
}

// OFFSET: 0x00796490
// FUNCTION: fn_08808
int CSkillAction_SS_ANKOKURYUUAKUMU::GetSkillCastMotion() {
  return 55;
}

// OFFSET: 0x007964A0
// FUNCTION: fn_08809
int CSkillAction_SS_ANKOKURYUUAKUMU::GetSkillAttackMotion() {
  return 57;
}

// OFFSET: 0x007964B0
// FUNCTION: fn_08810
void CSkillAction_SS_ANKOKURYUUAKUMU::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BLACKBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    static_cast<EffectActor*>(source)->LaunchEffect(EF_NEW_EPICLESIS_CAST, level / 16);
  }
}

// OFFSET: 0x00796510
// FUNCTION: fn_08811
void CSkillAction_SS_ANKOKURYUUAKUMU::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ankokuryuuakumu_00"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00796690
// FUNCTION: fn_08812
void CSkillAction_SS_ANKOKURYUUAKUMU::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ankokuryuuakumu_01"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00796810
// FUNCTION: fn_08813
void CSkillAction_SS_FOUR_CHARM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  EffectLists* effects;
  switch (source->charmType) {
  case CHARM_TYPE_WATER:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_ice_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_ice"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case CHARM_TYPE_LAND:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_earth_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_earth"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case CHARM_TYPE_FIRE:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_fire_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_fire"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  case CHARM_TYPE_WIND:
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_wind_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_wind"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    break;
  }
}

// OFFSET: 0x00796EF0
// FUNCTION: fn_08814
std::vector<int> ShuffledSequence::operator()(int count) const {
  std::vector<int> sequence;
  for (int index = 0; index < count; ++index) {
    sequence.push_back(index);
  }

  std::random_device device;
  std::mt19937 engine(device());
  std::shuffle(sequence.begin(), sequence.end(), engine);
  return sequence;
}

class CEfstAction_NIGHTMARE : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_SHADOW_CLOCK : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillObjectAction_SS_SBUNSHIN : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
  void OnSkillObjectRemove(CRenderObject* object) override;
};

class CSkillObjectAction_SS_KUNAITRAP : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
  void OnSkillObjectRemove(CRenderObject* object) override;
};

class CSkillAction_SS_TOKEDASU : public CSkillAction {
public:
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_SS_AKUMUKESU : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_SHINKIROU : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
};

class CSkillAction_SS_KAGEGARI : public CSkillAction {
public:
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_SS_KAGENOMAI : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_KAGEGISSEN : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_FUUMASHOUAKU : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
};

class CSkillObjectAction_NPC_FUUMA : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_SS_FUUMAKOUCHIKU : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_SS_FUUMASHOUAKU_HIT : public CSkillAction {
public:
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_KUNAIWAIKYOKU : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_SS_KUNAIKAITEN : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_KUNAIKUSSETSU : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_SEKIENHOU : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

// OFFSET: 0x00793A40
// FUNCTION: fn_08757
int CSkillAction_SS_KAGENOMAI::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00793BE0
// FUNCTION: fn_08760
int CSkillAction_SS_KAGEGISSEN::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00792500
// FUNCTION: fn_08738
void CEfstAction_NIGHTMARE::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("efst_nightmare"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00792670
// FUNCTION: fn_08739
void CEfstAction_NIGHTMARE::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("efst_nightmare"));
  }
}

// OFFSET: 0x00792790
// FUNCTION: fn_08740
void CEfstAction_NIGHTMARE::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007927B0
// FUNCTION: fn_08741
void CEfstAction_SHADOW_CLOCK::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("kagekakusi"), actor->render.position, 0, -1, 0, 0);
  static_cast<EffectActor*>(actor)->LaunchEffect(EF_GRAYBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
}

// OFFSET: 0x00792930
// FUNCTION: fn_08742
void CEfstAction_SHADOW_CLOCK::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    while (static_cast<EffectActor*>(actor)->HasId(EF_GRAYBODY)) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, EF_GRAYBODY, 0, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00792990
// FUNCTION: fn_08743
void CEfstAction_SHADOW_CLOCK::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    static_cast<EffectActor*>(actor)->LaunchEffect(EF_GRAYBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x007929E0
// FUNCTION: fn_08744
void CSkillObjectAction_SS_SBUNSHIN::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shinkirou_idle_start"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shinkirou_idle_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00792C60
// FUNCTION: fn_08745
void CSkillObjectAction_SS_SBUNSHIN::OnSkillObjectRemove(CRenderObject* object) {
  if (object) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shinkirou_idle_start"));
    effects = g_effectMgr;
    effects->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shinkirou_idle_loop"));
    effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("shinkirou_idle_end"), object->render.position, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00792F90
// FUNCTION: fn_08746
void CSkillObjectAction_SS_KUNAITRAP::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaiarea_01_start"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaiarea_01_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00793210
// FUNCTION: fn_08747
void CSkillObjectAction_SS_KUNAITRAP::OnSkillObjectRemove(CRenderObject* object) {
  if (object) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaiarea_01_loop"));
    effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("kunaiarea_01_end"), object->render.position, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00793450
// FUNCTION: fn_08748
void CSkillAction_SS_TOKEDASU::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  CGameActor* shadow = reinterpret_cast<CGameActor*>(argument3);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("tokedasu"), position, 0, -1, 0, 0);
  if (shadow) {
    effects = g_effectMgr;
    effects->Spawn(shadow->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("tokedasu_shadow"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007936A0
// FUNCTION: fn_08749
int CSkillAction_SS_AKUMUKESU::GetSkillCastMotion() {
  return 55;
}

// OFFSET: 0x007936B0
// FUNCTION: fn_08750
int CSkillAction_SS_AKUMUKESU::GetSkillAttackMotion() {
  return 56;
}

// OFFSET: 0x007936C0
// FUNCTION: fn_08751
void CSkillAction_SS_AKUMUKESU::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_GUMGANG6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL, level / 16);
  }
}

// OFFSET: 0x00793720
// FUNCTION: fn_08752
void CSkillAction_SS_AKUMUKESU::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("akumukesu"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00793880
// FUNCTION: fn_08753
int CSkillAction_SS_SHINKIROU::GetSkillCastMotion() {
  return 55;
}

// OFFSET: 0x00793890
// FUNCTION: fn_08754
int CSkillAction_SS_SHINKIROU::GetSkillAttackMotion() {
  return 56;
}

// OFFSET: 0x007938A0
// FUNCTION: fn_08755
void CSkillAction_SS_SHINKIROU::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_VOLCANO_BLACK, level / 16);
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, EF_MYSTERY_ILLUSION_CAST_BOTTOM, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x00793920
// FUNCTION: fn_08756
void CSkillAction_SS_KAGEGARI::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("kagegari"), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x00793A50
// FUNCTION: fn_08758
void CSkillAction_SS_KAGENOMAI::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_DARKCASTING, level / 16);
  }
}

// OFFSET: 0x00793A80
// FUNCTION: fn_08759
void CSkillAction_SS_KAGENOMAI::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kagenomai"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00793BF0
// FUNCTION: fn_08761
void CSkillAction_SS_KAGEGISSEN::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_DARKCASTING, level / 16);
  }
}

// OFFSET: 0x00793C20
// FUNCTION: fn_08762
void CSkillAction_SS_KAGEGISSEN::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_kagegissen"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_kagegissen_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_kagegissen_moon_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_kagegissen_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007940C0
// FUNCTION: fn_08763
int CSkillAction_SS_FUUMASHOUAKU::GetSkillAttackMotion() {
  return 9;
}

// OFFSET: 0x007940D0
// FUNCTION: fn_08764
void CSkillAction_SS_FUUMASHOUAKU::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, EF_ROTATE_LINE_GRAY, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    static_cast<EffectActor*>(source)->LaunchEffect(EF_DARKCASTING, level / 16);
  }
}

// OFFSET: 0x00794140
// FUNCTION: fn_08765
void CSkillObjectAction_NPC_FUUMA::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  if (object && source) {
    g_effectMgr->Spawn(object->render.accountId, 0x930, source->render.position,
                       object->render.accountId, -1, 0, 0);
  }
}

// OFFSET: 0x00794190
// FUNCTION: fn_08766
int CSkillAction_SS_FUUMAKOUCHIKU::GetSkillAttackMotion() {
  return 9;
}

// OFFSET: 0x007941A0
// FUNCTION: fn_08767
void CSkillAction_SS_FUUMAKOUCHIKU::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, EF_ROTATE_LINE_GRAY, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    static_cast<EffectActor*>(source)->LaunchEffect(EF_DARKCASTING, level / 16);
  }
}

// OFFSET: 0x00794210
// FUNCTION: fn_08768
void CSkillAction_SS_FUUMAKOUCHIKU::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("new_fuumakouchiku"), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x00794350
// FUNCTION: fn_08769
void CSkillAction_SS_FUUMASHOUAKU_HIT::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("fuumakouchiku_blasting"), target->render.position, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007944C0
// FUNCTION: fn_08770
int CSkillAction_SS_KUNAIWAIKYOKU::GetSkillAttackMotion() {
  return 9;
}

// OFFSET: 0x007944D0
// FUNCTION: fn_08771
void CSkillAction_SS_KUNAIWAIKYOKU::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, EF_ROTATE_LINE_GRAY, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    static_cast<EffectActor*>(source)->LaunchEffect(EF_DARKCASTING, level / 16);
  }
}

// OFFSET: 0x00794540
// FUNCTION: fn_08772
void CSkillAction_SS_KUNAIWAIKYOKU::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  CGameActor* shadow = reinterpret_cast<CGameActor*>(argument3);
  if (shadow) {
    const float angle = ScreenAngle(&shadow->render.position.x, &position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
    const vector3d spot = shadow->render.position;
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("kunaiwaikyoku"), spot.x, spot.y, spot.z, 0, -1, 0, 0)) {
      effect->SetAngle(angle);
    }
  }
}

// OFFSET: 0x007946E0
// FUNCTION: fn_08773
int CSkillAction_SS_KUNAIKAITEN::GetSkillAttackMotion() {
  return 9;
}

// OFFSET: 0x007946F0
// FUNCTION: fn_08774
void CSkillAction_SS_KUNAIKAITEN::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    std::string names[] = {"kunaikaiten_bottom", "kunaikaiten_00", "kunaikaiten_01", "kunaikaiten_02"};
    for (int wave = 0; wave < 4; ++wave) {
      for (const std::string& name : names) {
        EffectLists* effects = g_effectMgr;
        CEffect* effect = effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(name), source->render.position, 0, -1, 0, 0);
        if (effect) {
          effect->SetDelay(wave * 480);
        }
      }
    }
  }
}

// OFFSET: 0x00794970
// FUNCTION: fn_08775
int CSkillAction_SS_KUNAIKUSSETSU::GetSkillAttackMotion() {
  return 56;
}

// OFFSET: 0x00794980
// FUNCTION: fn_08776
void CSkillAction_SS_KUNAIKUSSETSU::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, EF_ROTATE_LINE_GRAY, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    static_cast<EffectActor*>(source)->LaunchEffect(EF_DARKCASTING, level / 16);
  }
}

// OFFSET: 0x007949F0
// FUNCTION: fn_08777
void CSkillAction_SS_KUNAIKUSSETSU::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaikussetsu"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaikussetsu_end"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (!g_serviceOptions.Get(229)) {
      effects = g_effectMgr;
      effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaikussetsu_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      effects = g_effectMgr;
      effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaikussetsu_bottom_end"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    }
  }
}

// OFFSET: 0x00794E80
// FUNCTION: fn_08778
int CSkillAction_SS_SEKIENHOU::GetSkillCastMotion() {
  return 55;
}

// OFFSET: 0x00794E90
// FUNCTION: fn_08779
int CSkillAction_SS_SEKIENHOU::GetSkillAttackMotion() {
  return 57;
}

// OFFSET: 0x00794EA0
// FUNCTION: fn_08780
void CSkillAction_SS_SEKIENHOU::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL3, level / 16);
  }
}

// OFFSET: 0x00794ED0
// FUNCTION: fn_08781
void CSkillAction_SS_SEKIENHOU::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(0, EF_TETRA_VORTEX_FIRE_GROUND, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    const vector3d position = target->render.position;
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("new_sekienhou"), position.x, position.y, position.z, 0, -1, 0, 0)) {
      effect->SetAngle(270.0f);
    }
  }
}

// OFFSET: 0x00795070
// FUNCTION: fn_08782
int CSkillAction_SS_REIKETSUHOU::GetSkillCastMotion() {
  return 55;
}

// OFFSET: 0x00795080
// FUNCTION: fn_08783
int CSkillAction_SS_REIKETSUHOU::GetSkillAttackMotion() {
  return 57;
}

// OFFSET: 0x00795090
// FUNCTION: fn_08784
void CSkillAction_SS_REIKETSUHOU::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL2, level / 16);
  }
}

// GLOBAL: gl_25844
static EfstActionRegistrar s_efstAction1395(1395, NewAction<CEfstAction_NIGHTMARE, CEfstAction>);
// GLOBAL: gl_25845
static EfstActionRegistrar s_efstAction1393(1393, NewAction<CEfstAction_SHADOW_CLOCK, CEfstAction>);
// GLOBAL: gl_25846
static CSkillObjectActionRegistration s_skillObjectAction290(290, NewAction<CSkillObjectAction_NPC_FUUMA, CSkillObjectAction>);
// GLOBAL: gl_25847
static CSkillObjectActionRegistration s_skillObjectAction298(298, NewAction<CSkillObjectAction_SS_KUNAITRAP, CSkillObjectAction>);
// GLOBAL: gl_25848
static CSkillObjectActionRegistration s_skillObjectAction294(294, NewAction<CSkillObjectAction_SS_SBUNSHIN, CSkillObjectAction>);
// GLOBAL: gl_25849
static SkillActionRegistrar s_skillAction5478(5478, NewAction<CSkillAction_SS_AKUMUKESU, CSkillAction>);
// GLOBAL: gl_25850
static SkillActionRegistrar s_skillAction5495(5495, NewAction<CSkillAction_SS_ANKOKURYUUAKUMU, CSkillAction>);
// GLOBAL: gl_25851
static SkillActionRegistrar s_skillAction5492(5492, NewAction<CSkillAction_SS_ANTENPOU, CSkillAction>);
// GLOBAL: gl_25852
static SkillActionRegistrar s_skillAction5499(5499, NewAction<CSkillAction_SS_FOUR_CHARM, CSkillAction>);
// GLOBAL: gl_25853
static SkillActionRegistrar s_skillAction5484(5484, NewAction<CSkillAction_SS_FUUMAKOUCHIKU, CSkillAction>);
// GLOBAL: gl_25854
static SkillActionRegistrar s_skillAction5483(5483, NewAction<CSkillAction_SS_FUUMASHOUAKU, CSkillAction>);
// GLOBAL: gl_25855
static SkillActionRegistrar s_skillAction5498(5498, NewAction<CSkillAction_SS_FUUMASHOUAKU_HIT, CSkillAction>);
// GLOBAL: gl_25856
static SkillActionRegistrar s_skillAction5494(5494, NewAction<CSkillAction_SS_HITOUAKUMU, CSkillAction>);
// GLOBAL: gl_25857
static SkillActionRegistrar s_skillAction5493(5493, NewAction<CSkillAction_SS_KAGEAKUMU, CSkillAction>);
// GLOBAL: gl_25858
static SkillActionRegistrar s_skillAction5480(5480, NewAction<CSkillAction_SS_KAGEGARI, CSkillAction>);
// GLOBAL: gl_25859
static SkillActionRegistrar s_skillAction5482(5482, NewAction<CSkillAction_SS_KAGEGISSEN, CSkillAction>);
// GLOBAL: gl_25860
static SkillActionRegistrar s_skillAction5481(5481, NewAction<CSkillAction_SS_KAGENOMAI, CSkillAction>);
// GLOBAL: gl_25861
static SkillActionRegistrar s_skillAction5491(5491, NewAction<CSkillAction_SS_KINRYUUHOU, CSkillAction>);
// GLOBAL: gl_25862
static SkillActionRegistrar s_skillAction5486(5486, NewAction<CSkillAction_SS_KUNAIKAITEN, CSkillAction>);
// GLOBAL: gl_25863
static SkillActionRegistrar s_skillAction5487(5487, NewAction<CSkillAction_SS_KUNAIKUSSETSU, CSkillAction>);
// GLOBAL: gl_25864
static SkillActionRegistrar s_skillAction5485(5485, NewAction<CSkillAction_SS_KUNAIWAIKYOKU, CSkillAction>);
// GLOBAL: gl_25865
static SkillActionRegistrar s_skillAction5490(5490, NewAction<CSkillAction_SS_RAIDENPOU, CSkillAction>);
// GLOBAL: gl_25866
static SkillActionRegistrar s_skillAction5489(5489, NewAction<CSkillAction_SS_REIKETSUHOU, CSkillAction>);
// GLOBAL: gl_25867
static SkillActionRegistrar s_skillAction5488(5488, NewAction<CSkillAction_SS_SEKIENHOU, CSkillAction>);
// GLOBAL: gl_25868
static SkillActionRegistrar s_skillAction5479(5479, NewAction<CSkillAction_SS_SHINKIROU, CSkillAction>);
// GLOBAL: gl_25869
static SkillActionRegistrar s_skillAction5476(5476, NewAction<CSkillAction_SS_TOKEDASU, CSkillAction>);
