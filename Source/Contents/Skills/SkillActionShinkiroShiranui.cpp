#include "Contents/Actors/GameActor.h"
#include "Contents/EffectResourceSwitcher.h"
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
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_ANTENPOU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
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
