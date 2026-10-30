#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <cstdint>
#include <string>

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  std::uint8_t m_text[Length];
  bool m_encrypted;
};

}

class CSkillAction_AnimalForm : public CSkillAction {
public:
  virtual bool Slot17(CGameActor* actor) = 0;
  virtual void Slot18(CGameActor* actor) = 0;
};

class CSkillAction_Direction {
public:
  int GetDirection(float angle);
  bool IsValidDirection(unsigned int direction);
};

class CSkillAction_DamageWereRaptorForm : public CSkillAction_AnimalForm, public CSkillAction_Direction {
public:
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
  bool Slot17(CGameActor* actor) override;
  void Slot18(CGameActor* actor) override;
};

class CSkillAction_DamageWereWolfForm : public CSkillAction_AnimalForm {
public:
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
  bool Slot17(CGameActor* actor) override;
  void Slot18(CGameActor* actor) override;
};

class CEfstAction_BasicAction : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  virtual int Slot05(int efstId, int argument4) = 0;
};

class CSkillAction_DR_WERERAPTOR : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_WERERAPTOR : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_DR_ENRAGE_WOLF : public CSkillAction_DamageWereWolfForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
};

class CEfstAction_ENRAGE_WOLF : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_DR_BLOOD_HOWLING : public CSkillAction_DamageWereWolfForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
};

class CEfstAction_BLOOD_HOWLING : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_DR_NOMERCY_CLAW : public CSkillAction_DamageWereWolfForm {
public:
  int Slot02() override;
};

class CSkillAction_DR_HUNGER : public CSkillAction_DamageWereWolfForm {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int caster, CGameActor* target, CGameActor* argument3) override;
};

class CSkillAction_DR_CRUEL_BITE : public CSkillAction_DamageWereWolfForm {
public:
  int Slot02() override;
};

class CSkillAction_DR_ENRAGE_RAPTOR : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
};

class CEfstAction_ENRAGE_RAPTOR : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_DR_PREENING : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
};

class CEfstAction_PREENING : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_DR_SHOOTING_FEATHER : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot02() override;
};

class CSkillAction_DR_FLICKING_TONADO : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_LOW_FLIGHT : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot02() override;
};

class CSkillAction_DR_NATURE_SHIELD : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_NATURE_SHIELD : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CEfstAction_GROUND_GROW : public CEfstAction_BasicAction {
public:
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
};

// OFFSET: 0x0061FF70
// FUNCTION: fn_04458
int CSkillAction_DR_WERERAPTOR::Slot02() {
  return 64;
}

// OFFSET: 0x0061FF80
// FUNCTION: fn_04459
void CSkillAction_DR_WERERAPTOR::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }
  if (this->Slot17(source) == true) {
    return;
  }

  const std::uint8_t effectSource[] = {0x7E, 0xCC, 0x35, 0x3A, 0xAB, 0x3C, 0x05, 0xA7, 0x66, 0xDB, 0x47};
  thread_local EncryptedName<sizeof(effectSource), 0xD3755DD95F47A909ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00620190
// FUNCTION: fn_04460
EfstActionMessage CEfstAction_WERERAPTOR::Slot03() {
  return {LookupMessage(4648), 0xFFFF00};
}

// OFFSET: 0x006201C0
// FUNCTION: fn_04461
EfstActionMessage CEfstAction_WERERAPTOR::Slot04() {
  return {LookupMessage(4649), 0xFF};
}

// OFFSET: 0x00620220
// FUNCTION: fn_04463
int CSkillAction_DR_ENRAGE_WOLF::Slot01() {
  return 66;
}

// OFFSET: 0x00620230
// FUNCTION: fn_04464
int CSkillAction_DR_ENRAGE_WOLF::Slot02() {
  return 68;
}

// OFFSET: 0x00620240
// FUNCTION: fn_04465
void CSkillAction_DR_ENRAGE_WOLF::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x00620480
// FUNCTION: fn_04467
EfstActionMessage CEfstAction_ENRAGE_WOLF::Slot03() {
  return {LookupMessage(4644), 0xFFFF00};
}

// OFFSET: 0x006204B0
// FUNCTION: fn_04468
EfstActionMessage CEfstAction_ENRAGE_WOLF::Slot04() {
  return {LookupMessage(4645), 0xFF};
}

// OFFSET: 0x006204E0
// FUNCTION: fn_04469
int CSkillAction_DR_BLOOD_HOWLING::Slot01() {
  return 66;
}

// OFFSET: 0x006204F0
// FUNCTION: fn_04470
int CSkillAction_DR_BLOOD_HOWLING::Slot02() {
  return 68;
}

// OFFSET: 0x00620500
// FUNCTION: fn_04471
void CSkillAction_DR_BLOOD_HOWLING::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x00620740
// FUNCTION: fn_04473
EfstActionMessage CEfstAction_BLOOD_HOWLING::Slot03() {
  return {LookupMessage(4646), 0xFFFF00};
}

// OFFSET: 0x00620770
// FUNCTION: fn_04474
EfstActionMessage CEfstAction_BLOOD_HOWLING::Slot04() {
  return {LookupMessage(4647), 0xFF};
}

// OFFSET: 0x006207A0
// FUNCTION: fn_04475
int CSkillAction_DR_NOMERCY_CLAW::Slot02() {
  return 69;
}

// OFFSET: 0x006209C0
// FUNCTION: fn_04477
int CSkillAction_DR_HUNGER::Slot02() {
  return 70;
}

// OFFSET: 0x006209D0
// FUNCTION: fn_04478
void CSkillAction_DR_HUNGER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const std::uint8_t effectSource[] = {0x3D, 0x88, 0xDB, 0xBC, 0xA2, 0xC3, 0x2A, 0x47, 0x34, 0x8F, 0xD2, 0xBE, 0xB3, 0xB1};
  thread_local EncryptedName<sizeof(effectSource), 0x3375B1C7DBB5FD55ULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00620BE0
// FUNCTION: fn_04479
void CSkillAction_DR_HUNGER::Slot15(int caster, CGameActor* target, CGameActor* argument3) {
  CGameActor* source = reinterpret_cast<CGameActor*>(caster);
  if (!source) {
    return;
  }

  const std::uint8_t effectSource[] = {0x77, 0x44, 0xA3, 0x80, 0x88, 0x07, 0x6D};
  thread_local EncryptedName<sizeof(effectSource), 0x1D6D75EDE7CD311FULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00620DE0
// FUNCTION: fn_04480
int CSkillAction_DR_CRUEL_BITE::Slot02() {
  return 73;
}

// OFFSET: 0x00621060
// FUNCTION: fn_04482
int CSkillAction_DR_ENRAGE_RAPTOR::Slot01() {
  return 66;
}

// OFFSET: 0x00621070
// FUNCTION: fn_04483
int CSkillAction_DR_ENRAGE_RAPTOR::Slot02() {
  return 68;
}

// OFFSET: 0x00621080
// FUNCTION: fn_04484
void CSkillAction_DR_ENRAGE_RAPTOR::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x006212C0
// FUNCTION: fn_04486
EfstActionMessage CEfstAction_ENRAGE_RAPTOR::Slot03() {
  return {LookupMessage(4650), 0xFFFF00};
}

// OFFSET: 0x006212F0
// FUNCTION: fn_04487
EfstActionMessage CEfstAction_ENRAGE_RAPTOR::Slot04() {
  return {LookupMessage(4651), 0xFF};
}

// OFFSET: 0x00621320
// FUNCTION: fn_04488
int CSkillAction_DR_PREENING::Slot01() {
  return 66;
}

// OFFSET: 0x00621330
// FUNCTION: fn_04489
int CSkillAction_DR_PREENING::Slot02() {
  return 68;
}

// OFFSET: 0x00621340
// FUNCTION: fn_04490
void CSkillAction_DR_PREENING::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x00621570
// FUNCTION: fn_04492
EfstActionMessage CEfstAction_PREENING::Slot03() {
  return {LookupMessage(4652), 0xFFFF00};
}

// OFFSET: 0x006215A0
// FUNCTION: fn_04493
EfstActionMessage CEfstAction_PREENING::Slot04() {
  return {LookupMessage(4653), 0xFF};
}

// OFFSET: 0x006215D0
// FUNCTION: fn_04494
int CSkillAction_DR_SHOOTING_FEATHER::Slot02() {
  return 72;
}

// OFFSET: 0x00621850
// FUNCTION: fn_04497
int CSkillAction_DR_FLICKING_TONADO::Slot02() {
  return 67;
}

// OFFSET: 0x00621860
// FUNCTION: fn_04498
void CSkillAction_DR_FLICKING_TONADO::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const std::uint8_t effectSource[] = {0x41, 0x6D, 0x6E, 0x0E, 0x68, 0xAC, 0x9D, 0x48, 0x78, 0x75, 0x68, 0x03, 0x62, 0xA1, 0x9C, 0x2F};
  thread_local EncryptedName<sizeof(effectSource), 0x2FF3C5036D070127ULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const std::uint8_t bottomSource[] = {0xD5, 0x0B, 0xAA, 0x30, 0x92, 0xF2, 0x5B, 0x70, 0xEC, 0x13, 0xAC, 0x3D, 0x98, 0xFF, 0x5A, 0x48,
                                       0xD1, 0x08, 0xB7, 0x27, 0x96, 0xF6, 0x35};
  thread_local EncryptedName<sizeof(bottomSource), 0x17359BF953C367B3ULL> bottomName(bottomSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00621C00
// FUNCTION: fn_04499
int CSkillAction_DR_LOW_FLIGHT::Slot02() {
  return 70;
}

// OFFSET: 0x006221F0
// FUNCTION: fn_04501
int CSkillAction_DR_NATURE_SHIELD::Slot01() {
  return 61;
}

// OFFSET: 0x00622200
// FUNCTION: fn_04502
int CSkillAction_DR_NATURE_SHIELD::Slot02() {
  return 65;
}

// OFFSET: 0x00622210
// FUNCTION: fn_04503
void CSkillAction_DR_NATURE_SHIELD::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x00622240
// FUNCTION: fn_04504
void CSkillAction_DR_NATURE_SHIELD::Slot11(int level, CGameActor* source, CGameActor* target) {
  const std::uint8_t effectSource[] = {0xA9, 0xAC, 0x91, 0x48, 0x6D, 0x18, 0x3A, 0x86, 0xAF, 0xA4, 0x80, 0x51, 0x7B, 0x7D};
  thread_local EncryptedName<sizeof(effectSource), 0xF5657D1F3DE5CDC7ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00622440
// FUNCTION: fn_04505
EfstActionMessage CEfstAction_NATURE_SHIELD::Slot03() {
  return {LookupMessage(4660), 0xFFFF00};
}

// OFFSET: 0x00622470
// FUNCTION: fn_04506
EfstActionMessage CEfstAction_NATURE_SHIELD::Slot04() {
  return {LookupMessage(4661), 0xFF};
}

// OFFSET: 0x006224A0
// FUNCTION: fn_04507
void CEfstAction_GROUND_GROW::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  for (int index = 0; index < 12; ++index) {
    g_effectMgr->Retire(actor->render.accountId, this->Slot05(efstId, index + 1));
  }
}
