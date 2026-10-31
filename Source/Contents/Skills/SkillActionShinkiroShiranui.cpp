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
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

namespace {

struct ShuffledSequence {
  std::vector<int> operator()(int count) const;
};

}

class CSkillAction_SS_REIKETSUHOU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot12(vector3d position, int level, int argument3) override;
};

class CSkillAction_SS_RAIDENPOU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_KINRYUUHOU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_ANTENPOU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_KAGEAKUMU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_HITOUAKUMU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_ANKOKURYUUAKUMU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_FOUR_CHARM : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

// OFFSET: 0x007950C0
// FUNCTION: fn_08785
void CSkillAction_SS_REIKETSUHOU::Slot12(vector3d position, int level, int argument3) {
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("reiketsuhou_bottom"), position.x, position.y, position.z, 0, -1, 0, 0);

  const int counts[] = {5, 5, 5, 5, 5, 6, 6, 6, 7, 7};
  const int count = counts[std::min<unsigned int>(level, 10) - 1];
  for (int index = 0; index < count; ++index) {
    vector3d spot = position;
    spot.x += (std::max(0, std::rand() % (count * 50 + 1)) - count * 50 * 0.5f) * 0.1f;
    spot.z += (std::max(0, std::rand() % (count * 50 + 1)) - count * 50 * 0.5f) * 0.1f;
    if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("reiketsuhou"), spot.x, spot.y, spot.z, 0, -1, 0, 0)) {
      effect->Slot16(index * 80);
    }
  }
}

// OFFSET: 0x00795450
// FUNCTION: fn_08786
int CSkillAction_SS_RAIDENPOU::Slot01() {
  return 55;
}

// OFFSET: 0x00795460
// FUNCTION: fn_08787
int CSkillAction_SS_RAIDENPOU::Slot02() {
  return 57;
}

// OFFSET: 0x00795470
// FUNCTION: fn_08788
void CSkillAction_SS_RAIDENPOU::Slot08(int level, CGameActor* source) {
  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_raidenpou_cast"),
                                           0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x007955F0
// FUNCTION: fn_08789
void CSkillAction_SS_RAIDENPOU::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_raidenpou_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_raidenpou"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00795860
// FUNCTION: fn_08790
void CSkillAction_SS_RAIDENPOU::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_raidenpou_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007959E0
// FUNCTION: fn_08791
int CSkillAction_SS_KINRYUUHOU::Slot01() {
  return 55;
}

// OFFSET: 0x007959F0
// FUNCTION: fn_08792
int CSkillAction_SS_KINRYUUHOU::Slot02() {
  return 57;
}

// OFFSET: 0x00795A00
// FUNCTION: fn_08793
void CSkillAction_SS_KINRYUUHOU::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(58, level / 16);
  }
}

// OFFSET: 0x00795A30
// FUNCTION: fn_08794
void CSkillAction_SS_KINRYUUHOU::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
    const vector3d position = source->render.position;
    if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("kinryuuhou"), position.x, position.y, position.z, 0, -1, 0, 0)) {
      effect->Slot17(angle);
    }
  }
}

// OFFSET: 0x00795BD0
// FUNCTION: fn_08795
void CSkillAction_SS_KINRYUUHOU::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kinryuuhou_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00795D40
// FUNCTION: fn_08796
int CSkillAction_SS_ANTENPOU::Slot01() {
  return 55;
}

// OFFSET: 0x00795D50
// FUNCTION: fn_08797
int CSkillAction_SS_ANTENPOU::Slot02() {
  return 57;
}

// OFFSET: 0x00795D60
// FUNCTION: fn_08798
void CSkillAction_SS_ANTENPOU::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(59, level / 16);
  }
}

// OFFSET: 0x00795D90
// FUNCTION: fn_08799
void CSkillAction_SS_ANTENPOU::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    const vector3d position = source->render.position;
    g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("antenpou"), position.x, position.y, position.z, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00795D60

// OFFSET: 0x00795EE0
// FUNCTION: fn_08800
int CSkillAction_SS_KAGEAKUMU::Slot01() {
  return 55;
}

// OFFSET: 0x00795EF0
// FUNCTION: fn_08801
int CSkillAction_SS_KAGEAKUMU::Slot02() {
  return 57;
}

// OFFSET: 0x00795F00
// FUNCTION: fn_08802
void CSkillAction_SS_KAGEAKUMU::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(812, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    static_cast<EffectActor*>(source)->LaunchEffect(1656, level / 16);
  }
}

// OFFSET: 0x00795F00

// OFFSET: 0x00795F60
// FUNCTION: fn_08803
void CSkillAction_SS_KAGEAKUMU::Slot15(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kageakumu"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007960C0
// FUNCTION: fn_08804
int CSkillAction_SS_HITOUAKUMU::Slot01() {
  return 55;
}

// OFFSET: 0x007960D0
// FUNCTION: fn_08805
int CSkillAction_SS_HITOUAKUMU::Slot02() {
  return 57;
}

// OFFSET: 0x007960E0
// FUNCTION: fn_08806
void CSkillAction_SS_HITOUAKUMU::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(812, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    static_cast<EffectActor*>(source)->LaunchEffect(1656, level / 16);
  }
}

// OFFSET: 0x007960E0

// OFFSET: 0x00796140
// FUNCTION: fn_08807
void CSkillAction_SS_HITOUAKUMU::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("hitouakumu"), source->render.position.x, source->render.position.y,
                     source->render.position.z, 0, -1, 0, 0);

  ShuffledSequence shuffle;
  for (int ring = 0; ring < 3; ++ring) {
    const int count = std::max(0, std::rand() % 5) + 15;
    const float step = 360.0f / count;
    const std::vector<int> order = shuffle(count);
    for (int index = 0; index < count; ++index) {
      CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(FormatOwnedString("hitouakumu_%02d", ring)), source->render.position.x,
                                           source->render.position.y, source->render.position.z, 0, -1, 0, 0);
      if (effect) {
        const int spread = static_cast<int>(step);
        effect->Slot17(index * step + (spread ? std::max(0, std::rand() % spread) : 0));
        effect->Slot16((order[index] + ring * 10) * 32);
      }
    }
  }
}

// OFFSET: 0x00796490
// FUNCTION: fn_08808
int CSkillAction_SS_ANKOKURYUUAKUMU::Slot01() {
  return 55;
}

// OFFSET: 0x007964A0
// FUNCTION: fn_08809
int CSkillAction_SS_ANKOKURYUUAKUMU::Slot02() {
  return 57;
}

// OFFSET: 0x007964B0
// FUNCTION: fn_08810
void CSkillAction_SS_ANKOKURYUUAKUMU::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(812, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    static_cast<EffectActor*>(source)->LaunchEffect(1656, level / 16);
  }
}

// OFFSET: 0x007964B0

// OFFSET: 0x00796510
// FUNCTION: fn_08811
void CSkillAction_SS_ANKOKURYUUAKUMU::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ankokuryuuakumu_00"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00796690
// FUNCTION: fn_08812
void CSkillAction_SS_ANKOKURYUUAKUMU::Slot15(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ankokuryuuakumu_01"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00796810
// FUNCTION: fn_08813
void CSkillAction_SS_FOUR_CHARM::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  switch (source->charmType) {
  case 1:
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_ice_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_ice"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 2:
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_earth_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_earth"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 3:
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_fire_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_fire"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 4:
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_wind_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("four_charm_wind"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_SHADOW_CLOCK : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillObjectAction_SS_SBUNSHIN : public CSkillObjectAction {
public:
  void Slot00(int argument, CRenderObject* object, CGameActor* source) override;
  void Slot01(CRenderObject* object) override;
};

class CSkillObjectAction_SS_KUNAITRAP : public CSkillObjectAction {
public:
  void Slot00(int argument, CRenderObject* object, CGameActor* source) override;
  void Slot01(CRenderObject* object) override;
};

class CSkillAction_SS_TOKEDASU : public CSkillAction {
public:
  void Slot12(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_SS_AKUMUKESU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_SHINKIROU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
};

class CSkillAction_SS_KAGEGARI : public CSkillAction {
public:
  void Slot12(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_SS_KAGENOMAI : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_KAGEGISSEN : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_FUUMASHOUAKU : public CSkillAction {
public:
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
};

class CSkillObjectAction_NPC_FUUMA : public CSkillObjectAction {
public:
  void Slot00(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_SS_FUUMAKOUCHIKU : public CSkillAction {
public:
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot12(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_SS_FUUMASHOUAKU_HIT : public CSkillAction {
public:
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_KUNAIWAIKYOKU : public CSkillAction {
public:
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot12(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_SS_KUNAIKAITEN : public CSkillAction {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_KUNAIKUSSETSU : public CSkillAction {
public:
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_SEKIENHOU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

// OFFSET: 0x00792500
// FUNCTION: fn_08738
void CEfstAction_NIGHTMARE::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("efst_nightmare"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00792670
// FUNCTION: fn_08739
void CEfstAction_NIGHTMARE::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("efst_nightmare"));
  }
}

// OFFSET: 0x00792790
// FUNCTION: fn_08740
void CEfstAction_NIGHTMARE::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007927B0
// FUNCTION: fn_08741
void CEfstAction_SHADOW_CLOCK::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  const vector3d position = actor->render.position;
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("kagekakusi"), position.x, position.y, position.z, 0, -1, 0, 0);
  static_cast<EffectActor*>(actor)->LaunchEffect(963, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
}

// OFFSET: 0x00792930
// FUNCTION: fn_08742
void CEfstAction_SHADOW_CLOCK::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    while (static_cast<EffectActor*>(actor)->state.Attachments().HasId(963)) {
      actor->SendMsg(nullptr, 109, 0, 963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00792990
// FUNCTION: fn_08743
void CEfstAction_SHADOW_CLOCK::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    static_cast<EffectActor*>(actor)->LaunchEffect(963, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x007929E0
// FUNCTION: fn_08744
void CSkillObjectAction_SS_SBUNSHIN::Slot00(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shinkirou_idle_start"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shinkirou_idle_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00792C60
// FUNCTION: fn_08745
void CSkillObjectAction_SS_SBUNSHIN::Slot01(CRenderObject* object) {
  if (object) {
    g_effectMgr->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shinkirou_idle_start"));
    g_effectMgr->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shinkirou_idle_loop"));
    const vector3d position = object->render.position;
    g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("shinkirou_idle_end"), position.x, position.y, position.z, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00792F90
// FUNCTION: fn_08746
void CSkillObjectAction_SS_KUNAITRAP::Slot00(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaiarea_01_start"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaiarea_01_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00793210
// FUNCTION: fn_08747
void CSkillObjectAction_SS_KUNAITRAP::Slot01(CRenderObject* object) {
  if (object) {
    g_effectMgr->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaiarea_01_loop"));
    const vector3d position = object->render.position;
    g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("kunaiarea_01_end"), position.x, position.y, position.z, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00793450
// FUNCTION: fn_08748
void CSkillAction_SS_TOKEDASU::Slot12(vector3d position, int argument2, int argument3) {
  CGameActor* shadow = reinterpret_cast<CGameActor*>(argument3);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("tokedasu"), position.x, position.y, position.z, 0, -1, 0, 0);
  if (shadow) {
    g_effectMgr->Spawn(shadow->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("tokedasu_shadow"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007936A0
// FUNCTION: fn_08749
int CSkillAction_SS_AKUMUKESU::Slot01() {
  return 55;
}

// OFFSET: 0x007936B0
// FUNCTION: fn_08750
int CSkillAction_SS_AKUMUKESU::Slot02() {
  return 56;
}

// OFFSET: 0x007936C0
// FUNCTION: fn_08751
void CSkillAction_SS_AKUMUKESU::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(934, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x00793720
// FUNCTION: fn_08752
void CSkillAction_SS_AKUMUKESU::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("akumukesu"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00793880
// FUNCTION: fn_08753
int CSkillAction_SS_SHINKIROU::Slot01() {
  return 55;
}

// OFFSET: 0x00793890
// FUNCTION: fn_08754
int CSkillAction_SS_SHINKIROU::Slot02() {
  return 56;
}

// OFFSET: 0x007938A0
// FUNCTION: fn_08755
void CSkillAction_SS_SHINKIROU::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2373, level / 16);
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 1774, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x00793920
// FUNCTION: fn_08756
void CSkillAction_SS_KAGEGARI::Slot12(vector3d position, int argument2, int argument3) {
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("kagegari"), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x00793A50
// FUNCTION: fn_08758
void CSkillAction_SS_KAGENOMAI::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(454, level / 16);
  }
}

// OFFSET: 0x00793A80
// FUNCTION: fn_08759
void CSkillAction_SS_KAGENOMAI::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kagenomai"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00793BF0
// FUNCTION: fn_08761
void CSkillAction_SS_KAGEGISSEN::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(454, level / 16);
  }
}

// OFFSET: 0x00793C20
// FUNCTION: fn_08762
void CSkillAction_SS_KAGEGISSEN::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_kagegissen"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_kagegissen_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_kagegissen_moon_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("new_kagegissen_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007940C0
// FUNCTION: fn_08763
int CSkillAction_SS_FUUMASHOUAKU::Slot02() {
  return 9;
}

// OFFSET: 0x007940D0
// FUNCTION: fn_08764
void CSkillAction_SS_FUUMASHOUAKU::Slot08(int level, CGameActor* source) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 986, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    static_cast<EffectActor*>(source)->LaunchEffect(454, level / 16);
  }
}

// OFFSET: 0x00794140
// FUNCTION: fn_08765
void CSkillObjectAction_NPC_FUUMA::Slot00(int argument, CRenderObject* object, CGameActor* source) {
  if (object && source) {
    g_effectMgr->Spawn(object->render.accountId, 2352, source->render.position.x, source->render.position.y, source->render.position.z,
                       object->render.accountId, -1, 0, 0);
  }
}

// OFFSET: 0x00794190
// FUNCTION: fn_08766
int CSkillAction_SS_FUUMAKOUCHIKU::Slot02() {
  return 9;
}

// OFFSET: 0x007941A0
// FUNCTION: fn_08767
void CSkillAction_SS_FUUMAKOUCHIKU::Slot08(int level, CGameActor* source) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 986, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    static_cast<EffectActor*>(source)->LaunchEffect(454, level / 16);
  }
}

// OFFSET: 0x00794210
// FUNCTION: fn_08768
void CSkillAction_SS_FUUMAKOUCHIKU::Slot12(vector3d position, int argument2, int argument3) {
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("new_fuumakouchiku"), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x00794350
// FUNCTION: fn_08769
void CSkillAction_SS_FUUMASHOUAKU_HIT::Slot15(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    const vector3d position = target->render.position;
    g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("fuumakouchiku_blasting"), position.x, position.y, position.z, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007944C0
// FUNCTION: fn_08770
int CSkillAction_SS_KUNAIWAIKYOKU::Slot02() {
  return 9;
}

// OFFSET: 0x007944D0
// FUNCTION: fn_08771
void CSkillAction_SS_KUNAIWAIKYOKU::Slot08(int level, CGameActor* source) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 986, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    static_cast<EffectActor*>(source)->LaunchEffect(454, level / 16);
  }
}

// OFFSET: 0x00794540
// FUNCTION: fn_08772
void CSkillAction_SS_KUNAIWAIKYOKU::Slot12(vector3d position, int argument2, int argument3) {
  CGameActor* shadow = reinterpret_cast<CGameActor*>(argument3);
  if (shadow) {
    const float angle = ScreenAngle(&shadow->render.position.x, &position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
    const vector3d spot = shadow->render.position;
    if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("kunaiwaikyoku"), spot.x, spot.y, spot.z, 0, -1, 0, 0)) {
      effect->Slot17(angle);
    }
  }
}

// OFFSET: 0x007946E0
// FUNCTION: fn_08773
int CSkillAction_SS_KUNAIKAITEN::Slot02() {
  return 9;
}

// OFFSET: 0x007946F0
// FUNCTION: fn_08774
void CSkillAction_SS_KUNAIKAITEN::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    std::string names[] = {"kunaikaiten_bottom", "kunaikaiten_00", "kunaikaiten_01", "kunaikaiten_02"};
    for (int wave = 0; wave < 4; ++wave) {
      for (const std::string& name : names) {
        CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(name), source->render.position.x, source->render.position.y,
                                             source->render.position.z, 0, -1, 0, 0);
        if (effect) {
          effect->Slot16(wave * 480);
        }
      }
    }
  }
}

// OFFSET: 0x00794970
// FUNCTION: fn_08775
int CSkillAction_SS_KUNAIKUSSETSU::Slot02() {
  return 56;
}

// OFFSET: 0x00794980
// FUNCTION: fn_08776
void CSkillAction_SS_KUNAIKUSSETSU::Slot08(int level, CGameActor* source) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 986, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    static_cast<EffectActor*>(source)->LaunchEffect(454, level / 16);
  }
}

// OFFSET: 0x007949F0
// FUNCTION: fn_08777
void CSkillAction_SS_KUNAIKUSSETSU::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaikussetsu"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaikussetsu_end"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (!g_serviceOptions.Get(229)) {
      g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaikussetsu_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("kunaikussetsu_bottom_end"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
  }
}

// OFFSET: 0x00794E80
// FUNCTION: fn_08778
int CSkillAction_SS_SEKIENHOU::Slot01() {
  return 55;
}

// OFFSET: 0x00794E90
// FUNCTION: fn_08779
int CSkillAction_SS_SEKIENHOU::Slot02() {
  return 57;
}

// OFFSET: 0x00794EA0
// FUNCTION: fn_08780
void CSkillAction_SS_SEKIENHOU::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(55, level / 16);
  }
}

// OFFSET: 0x00794ED0
// FUNCTION: fn_08781
void CSkillAction_SS_SEKIENHOU::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(0, 1270, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    const vector3d position = target->render.position;
    if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("new_sekienhou"), position.x, position.y, position.z, 0, -1, 0, 0)) {
      effect->Slot17(270.0f);
    }
  }
}

// OFFSET: 0x00795070
// FUNCTION: fn_08782
int CSkillAction_SS_REIKETSUHOU::Slot01() {
  return 55;
}

// OFFSET: 0x00795080
// FUNCTION: fn_08783
int CSkillAction_SS_REIKETSUHOU::Slot02() {
  return 57;
}

// OFFSET: 0x00795090
// FUNCTION: fn_08784
void CSkillAction_SS_REIKETSUHOU::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(54, level / 16);
  }
}
