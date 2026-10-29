#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Utility/Math/Vector3d.h"

class CSkillAction_IG_GUARDIAN_SHIELD : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_GUARDIAN_SHIELD : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_IG_HOLY_SHIELD : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_HOLY_SHIELD : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_IG_JUDGEMENT_CROSS : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_OVERSLASH : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_REBOUND_SHIELD : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_REBOUND_S : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_IG_ULTIMATE_SACRIFICE : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_ULTIMATE_SACRIFICE : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_IG_IMPERIAL_CROSS : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_RADIANT_SPEAR : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_IMPERIAL_PRESSURE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_SHIELD_SHOOTING : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IG_ATTACK_STANCE : public CSkillAction {
public:
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_ATTACK_STANCE : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_IG_GUARD_STANCE : public CSkillAction {
public:
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IG_GUARD_STANCE : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
};

// OFFSET: 0x00787A30

// OFFSET: 0x00787CC0

// OFFSET: 0x00787F60

// OFFSET: 0x007880A0

// OFFSET: 0x007880C0

// OFFSET: 0x007880F0

// OFFSET: 0x00788120

// OFFSET: 0x007883D0

// OFFSET: 0x00788650

// OFFSET: 0x007888E0

// OFFSET: 0x00788B00

// OFFSET: 0x00788B20

// OFFSET: 0x00788B50

// OFFSET: 0x00788B90

// OFFSET: 0x00788E50

// OFFSET: 0x00788FE0

// OFFSET: 0x00789260

// OFFSET: 0x00789400

// OFFSET: 0x00789590

// OFFSET: 0x00789800

// OFFSET: 0x00789980

// OFFSET: 0x00789C50

// OFFSET: 0x00789EE0

// OFFSET: 0x00789F10

// OFFSET: 0x00789FA0

// OFFSET: 0x0078A220

// OFFSET: 0x0078A3B0

// OFFSET: 0x0078A500

// OFFSET: 0x0078A530

// OFFSET: 0x0078A6A0

// OFFSET: 0x0078A830

// OFFSET: 0x0078AC80

// OFFSET: 0x0078AE00

// OFFSET: 0x0078AF90

// OFFSET: 0x0078B210

// OFFSET: 0x0078B3A0

// OFFSET: 0x0078B520

// OFFSET: 0x0078B5E0

// OFFSET: 0x0078B760
// FUNCTION: fn_08635
int CSkillAction_IG_ATTACK_STANCE::Slot02() {
  return 7;
}

// OFFSET: 0x0078B770

// OFFSET: 0x0078BB40

// OFFSET: 0x0078BCC0

// OFFSET: 0x0078BE50

// OFFSET: 0x0078BFA0

// OFFSET: 0x0078BFC0

// OFFSET: 0x0078BFF0

// OFFSET: 0x0078C020
// FUNCTION: fn_08643
int CSkillAction_IG_GUARD_STANCE::Slot02() {
  return 7;
}

// OFFSET: 0x0078C030

// OFFSET: 0x0078C400

// OFFSET: 0x0078C570
