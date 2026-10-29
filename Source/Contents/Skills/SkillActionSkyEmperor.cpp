#include "Contents/Actors/GameActor.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Math/Vector3d.h"

#include <algorithm>
#include <cstdlib>

class CSkillObjectAction_SKE_STAR_CANNON : public CSkillObjectAction {
public:
  void Slot00(int argument, CRenderObject* object, CGameActor* source) override;
};

class CSkillAction_SKE_ALL_IN_THE_SKY : public CSkillAction {
public:
  int Slot02() override;
};

class CEfstAction_SKY_ENCHANT : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SKE_SKY_SUN : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_SKY_MOON : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_STAR_LIGHT_KICK : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_RISING_SUN : public CSkillAction {
public:
  int Slot02() override;
};

class CEfstAction_RISING_SUN : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_NOON_SUN : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_SUNSET_SUN : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SKE_NOON_BLAST : public CSkillAction {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_SUNSET_BLAST : public CSkillAction {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_RISING_MOON : public CSkillAction {
public:
  int Slot02() override;
};

class CEfstAction_RISING_MOON : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_MIDNIGHT_MOON : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_DAWN_MOON : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SKE_MIDNIGHT_KICK : public CSkillAction {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SKE_DAWN_BREAK : public CSkillAction {
public:
  int Slot02() override;
};

// OFFSET: 0x007975A0

// OFFSET: 0x007975F0
// FUNCTION: fn_08816
int CSkillAction_SKE_ALL_IN_THE_SKY::Slot02() {
  return 53;
}

// OFFSET: 0x007977E0

// OFFSET: 0x00797A70

// OFFSET: 0x00797BB0

// OFFSET: 0x00797D40

// OFFSET: 0x007980C0

// OFFSET: 0x00798240

// OFFSET: 0x007984D0

// OFFSET: 0x00798770

// OFFSET: 0x007988D0

// OFFSET: 0x00798B90

// OFFSET: 0x00798D10
// FUNCTION: fn_08832
int CSkillAction_SKE_RISING_SUN::Slot02() {
  return 52;
}

// OFFSET: 0x00798D20

// OFFSET: 0x00798FA0

// OFFSET: 0x007990D0

// OFFSET: 0x00799250

// OFFSET: 0x007994D0

// OFFSET: 0x00799600

// OFFSET: 0x00799780

// OFFSET: 0x00799A00

// OFFSET: 0x00799B30

// OFFSET: 0x00799CB0
// FUNCTION: fn_08842
int CSkillAction_SKE_NOON_BLAST::Slot02() {
  return 52;
}

// OFFSET: 0x00799CC0

// OFFSET: 0x00799E30
// FUNCTION: fn_08844
int CSkillAction_SKE_SUNSET_BLAST::Slot02() {
  return 52;
}

// OFFSET: 0x00799E40

// OFFSET: 0x0079A1C0
// FUNCTION: fn_08846
int CSkillAction_SKE_RISING_MOON::Slot02() {
  return 53;
}

// OFFSET: 0x0079A1D0

// OFFSET: 0x0079A450

// OFFSET: 0x0079A580

// OFFSET: 0x0079A700

// OFFSET: 0x0079A980

// OFFSET: 0x0079AAB0

// OFFSET: 0x0079AC30

// OFFSET: 0x0079AEB0

// OFFSET: 0x0079AFE0

// OFFSET: 0x0079B160
// FUNCTION: fn_08856
int CSkillAction_SKE_MIDNIGHT_KICK::Slot02() {
  return 53;
}

// OFFSET: 0x0079B1A0

// OFFSET: 0x0079B320
// FUNCTION: fn_08859
int CSkillAction_SKE_DAWN_BREAK::Slot02() {
  return 53;
}
