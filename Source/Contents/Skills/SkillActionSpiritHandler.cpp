#include "Contents/Actors/GameActor.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Skills/SkillInfo.h"

#include <cstdint>

class CEfstAction_SANDY_FESTIVAL : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
};

class CSkillAction_SH_KI_SUL_RAMPAGE : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SH_COLORS_OF_HYUN_ROK : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SH_HYUN_ROKS_BREEZE : public CSkillAction {
public:
  bool Slot03(int argument) override;
  int Slot14() override;
};

class CSkillObjectAction_SH_HYUN_ROKS_BREEZE : public CSkillObjectAction {
public:
  void Slot00(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_SH_TEMPORARY_COMMUNION : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_TEMPORARY_COMMUNION : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SH_BLESSING_OF_MYSTICAL_CREATURES : public CSkillAction {
public:
  bool Slot00() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SH_CHUL_HO_BATTERING : public CSkillAction {
public:
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SH_HYUN_ROK_SPIRIT_POWER : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

// OFFSET: 0x0079FB10

// OFFSET: 0x0079FCC0

// OFFSET: 0x0079FF70

// OFFSET: 0x007A04A0
// FUNCTION: fn_08954
bool CSkillAction_SH_HYUN_ROKS_BREEZE::Slot03(int argument) {
  return true;
}

// OFFSET: 0x007A04E0
// FUNCTION: fn_08956
int CSkillAction_SH_HYUN_ROKS_BREEZE::Slot14() {
  return 9;
}

// OFFSET: 0x007A04F0

// OFFSET: 0x007A0980

// OFFSET: 0x007A0B00

// OFFSET: 0x007A0DA0

// OFFSET: 0x007A0FD0

// OFFSET: 0x007A0FF0
// FUNCTION: fn_08965
bool CSkillAction_SH_BLESSING_OF_MYSTICAL_CREATURES::Slot00() {
  return true;
}

// OFFSET: 0x007A1030

// OFFSET: 0x007A11C0
// FUNCTION: fn_08968
int CSkillAction_SH_CHUL_HO_BATTERING::Slot02() {
  return 54;
}

// OFFSET: 0x007A11D0

// OFFSET: 0x007A1360

// OFFSET: 0x007A1630

// OFFSET: 0x007A17D0

// OFFSET: 0x007A1A80

// OFFSET: 0x007A1D10
