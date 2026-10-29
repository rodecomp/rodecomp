#include "Contents/Actors/GameActor.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/Effect.h"
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
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_ANTENPOU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
};

class CSkillAction_SS_KAGEAKUMU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_HITOUAKUMU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_ANKOKURYUUAKUMU : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SS_FOUR_CHARM : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

// OFFSET: 0x007950C0

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

// OFFSET: 0x007955F0

// OFFSET: 0x00795860

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

// OFFSET: 0x00795BD0

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

// OFFSET: 0x00795F60

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

// OFFSET: 0x00796140

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

// OFFSET: 0x00796510

// OFFSET: 0x00796690

// OFFSET: 0x00796810

// OFFSET: 0x00796EF0
