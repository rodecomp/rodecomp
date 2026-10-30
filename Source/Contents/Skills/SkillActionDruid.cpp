#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/BezierArcThrowEffect.h"
#include "Contents/Effects/EZ2STREffectEx.h"
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
#include <format>
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
  bool CanUse(CGameActor* actor);
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
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
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
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_BLOOD_HOWLING : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_DR_NOMERCY_CLAW : public CSkillAction_DamageWereWolfForm {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
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
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_ENRAGE_RAPTOR : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
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
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_PREENING : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_DR_SHOOTING_FEATHER : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
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
  int Slot05(int efstId, int argument4) override;
};

class CSkillAction_Earthing : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_Icing : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_NATURE_PROTECTION : public CEfstAction_BasicAction {
public:
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
  int Slot05(int efstId, int argument4) override;
};

class CEfstAction_THUNDERING_ROD : public CEfstAction_BasicAction {
public:
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  int Slot05(int efstId, int argument4) override;
};

class CEfstAction_THUNDERING_ROD_MAX : public CEfstAction_BasicAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
  int Slot05(int efstId, int argument4) override;
};

class CSkillAction_DR_TRUTH_OF_EARTH : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_TRUTH_OF_EARTH : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_DR_TRUTH_OF_ICE : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_TRUTH_OF_ICE : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_DR_TRUTH_OF_WIND : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_TRUTH_OF_WIND : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_DR_CUTTING_WIND : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_WIND_BOMB : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_AROUND_FLOWER : public CSkillAction_Earthing {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_EARTH_FLOWER : public CSkillAction_Earthing {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_ICE_CLOUD : public CSkillAction_Icing {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_ICE_TOTEM : public CSkillAction_Icing {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_WEREWOLF : public CSkillAction_DamageWereWolfForm {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_WEREWOLF : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
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

// OFFSET: 0x006224E0
// FUNCTION: fn_04508
int CEfstAction_GROUND_GROW::Slot05(int efstId, int argument4) {
  const int step = argument4 - 1;
  if (step < 0 || step > 11) {
    return -1;
  }

  const auto full = []() {
    const std::uint8_t fullSource[] = {0xAF, 0xC2, 0x49, 0xFB, 0xBB};
    thread_local EncryptedName<sizeof(fullSource), 0x078333BB9725B7C9ULL> fullName(fullSource);
    return fullName.Text();
  };
  std::string suffix = step == 11 ? std::string(full()) : std::format("{:02}", step);

  const std::uint8_t prefixSource[] = {0xD2, 0x07, 0x0E, 0x2A, 0x93, 0x3F, 0x44, 0xE8, 0xC7, 0x1A, 0x16, 0x00, 0xFD};
  thread_local EncryptedName<sizeof(prefixSource), 0x8F1B5BFD5F6175B5ULL> prefixName(prefixSource);
  return CEZ2STREffectMgr::GetInstance()->EffectId(std::string(prefixName.Text()) + suffix);
}

// OFFSET: 0x00622920
// FUNCTION: fn_04509
EfstActionMessage CEfstAction_NATURE_PROTECTION::Slot03() {
  return {LookupMessage(4666), 0xFFFF00};
}

// OFFSET: 0x00622950
// FUNCTION: fn_04510
EfstActionMessage CEfstAction_NATURE_PROTECTION::Slot04() {
  return {LookupMessage(4667), 0xFF};
}

// OFFSET: 0x00622980
// FUNCTION: fn_04511
int CEfstAction_NATURE_PROTECTION::Slot05(int efstId, int argument4) {
  const std::uint8_t effectSource[] = {0xAD, 0xA2, 0xA1, 0xFE, 0x29, 0x8E, 0x60, 0xFB, 0xB1, 0xAC, 0xA1, 0xEE, 0x38, 0x9F, 0x56, 0xE4, 0xAD, 0xC3};
  thread_local EncryptedName<sizeof(effectSource), 0x8B3FEB5B8BD5C3C3ULL> effectName(effectSource);
  return CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text());
}

// OFFSET: 0x00622B60
// FUNCTION: fn_04513
void CEfstAction_NATURE_PROTECTION::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    static_cast<CGameActor*>(actor)->ApplyPendingUnknown0408();
  }
  CEfstAction_BasicAction::Slot01(efstId, argument2, argument3, argument4, actor);
}

// OFFSET: 0x00622B90
// FUNCTION: fn_04514
void CEfstAction_THUNDERING_ROD::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  for (int index = 0; index < 6; ++index) {
    g_effectMgr->Retire(actor->render.accountId, this->Slot05(efstId, index + 1));
  }
}

// OFFSET: 0x00622BD0
// FUNCTION: fn_04515
int CEfstAction_THUNDERING_ROD::Slot05(int efstId, int argument4) {
  const int step = argument4 - 1;
  if (static_cast<unsigned int>(step) > 5) {
    return -1;
  }

  std::string suffix = std::format("{:02}", step);
  const std::uint8_t prefixSource[] = {0xD3, 0xDB, 0x70, 0xA7, 0xC1, 0x06, 0x99, 0x62, 0xC9, 0xD4, 0x5A, 0xBB, 0xCA, 0x07, 0xB4, 0x0B};
  thread_local EncryptedName<sizeof(prefixSource), 0x0BEB63A5C905B3A7ULL> prefixName(prefixSource);
  return CEZ2STREffectMgr::GetInstance()->EffectId(std::string(prefixName.Text()) + suffix);
}

// OFFSET: 0x00622DD0
// FUNCTION: fn_04516
EfstActionMessage CEfstAction_THUNDERING_ROD_MAX::Slot03() {
  return {LookupMessage(4688), 0xFFFF00};
}

// OFFSET: 0x00622E00
// FUNCTION: fn_04517
EfstActionMessage CEfstAction_THUNDERING_ROD_MAX::Slot04() {
  return {LookupMessage(4689), 0xFF};
}

// OFFSET: 0x00622E30
// FUNCTION: fn_04518
int CEfstAction_THUNDERING_ROD_MAX::Slot05(int efstId, int argument4) {
  const std::uint8_t effectSource[] = {0x5B, 0xF7, 0xC6, 0xB5, 0x8B, 0xAC, 0x65, 0xF2, 0x41, 0xF8, 0xEC, 0xA9, 0x80, 0xAD, 0x48, 0xFD, 0x5A, 0xF3, 0xDF, 0xDB};
  thread_local EncryptedName<sizeof(effectSource), 0x9B17C9EFDBB39F2FULL> effectName(effectSource);
  return CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text());
}

// OFFSET: 0x00622FD0
// FUNCTION: fn_04519
int CSkillAction_DR_TRUTH_OF_EARTH::Slot01() {
  return 61;
}

// OFFSET: 0x00622FE0
// FUNCTION: fn_04520
int CSkillAction_DR_TRUTH_OF_EARTH::Slot02() {
  return 63;
}

// OFFSET: 0x00622FF0
// FUNCTION: fn_04521
void CSkillAction_DR_TRUTH_OF_EARTH::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(57, level / 16);
  }
}

// OFFSET: 0x00623020
// FUNCTION: fn_04522
void CSkillAction_DR_TRUTH_OF_EARTH::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  const std::uint8_t effectSource[] = {0x05, 0x69, 0x86, 0x8B, 0xEF, 0x7A, 0xFE, 0xA1, 0x2E, 0x7E, 0x92, 0x8D, 0xF3, 0x4D, 0x91};
  thread_local EncryptedName<sizeof(effectSource), 0xC7912587FFF31B71ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const std::uint8_t bottomSource[] = {0x19, 0x41, 0x7A, 0xE1, 0xB3, 0x1A, 0xBC, 0x0F, 0x32, 0x56, 0x6E, 0xE7, 0xAF, 0x2D, 0x8C, 0x0B, 0x02, 0x47, 0x7B, 0xFA, 0xB6, 0x45};
  thread_local EncryptedName<sizeof(bottomSource), 0x69D345DB950F336DULL> bottomName(bottomSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006233D0
// FUNCTION: fn_04523
EfstActionMessage CEfstAction_TRUTH_OF_EARTH::Slot03() {
  return {LookupMessage(4654), 0xFFFF00};
}

// OFFSET: 0x00623400
// FUNCTION: fn_04524
EfstActionMessage CEfstAction_TRUTH_OF_EARTH::Slot04() {
  return {LookupMessage(4655), 0xFF};
}

// OFFSET: 0x00623430
// FUNCTION: fn_04525
int CSkillAction_DR_TRUTH_OF_ICE::Slot01() {
  return 61;
}

// OFFSET: 0x00623440
// FUNCTION: fn_04526
int CSkillAction_DR_TRUTH_OF_ICE::Slot02() {
  return 65;
}

// OFFSET: 0x00623450
// FUNCTION: fn_04527
void CSkillAction_DR_TRUTH_OF_ICE::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x00623480
// FUNCTION: fn_04528
void CSkillAction_DR_TRUTH_OF_ICE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  const std::uint8_t effectSource[] = {0x47, 0x55, 0x46, 0x7B, 0xD5, 0x12, 0x56, 0x4B, 0x6C, 0x4E, 0x50, 0x6A, 0xBD};
  thread_local EncryptedName<sizeof(effectSource), 0x2D394DBD0F332733ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const std::uint8_t bottomSource[] = {0x33, 0x53, 0xCE, 0x9F, 0x83, 0x28, 0x70, 0x73, 0x18, 0x48, 0xD8, 0x8E, 0xB4, 0x15, 0x70, 0x61, 0x33, 0x4E, 0xD6, 0xEB};
  thread_local EncryptedName<sizeof(bottomSource), 0x151F77EBEBBB2147ULL> bottomName(bottomSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00623820
// FUNCTION: fn_04529
EfstActionMessage CEfstAction_TRUTH_OF_ICE::Slot03() {
  return {LookupMessage(4658), 0xFFFF00};
}

// OFFSET: 0x00623850
// FUNCTION: fn_04530
EfstActionMessage CEfstAction_TRUTH_OF_ICE::Slot04() {
  return {LookupMessage(4659), 0xFF};
}

// OFFSET: 0x00623880
// FUNCTION: fn_04531
int CSkillAction_DR_TRUTH_OF_WIND::Slot01() {
  return 61;
}

// OFFSET: 0x00623890
// FUNCTION: fn_04532
int CSkillAction_DR_TRUTH_OF_WIND::Slot02() {
  return 65;
}

// OFFSET: 0x006238A0
// FUNCTION: fn_04533
void CSkillAction_DR_TRUTH_OF_WIND::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x006238D0
// FUNCTION: fn_04534
void CSkillAction_DR_TRUTH_OF_WIND::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  const std::uint8_t effectSource[] = {0x95, 0xE9, 0x86, 0x0F, 0x85, 0xD8, 0x90, 0x07, 0xBE, 0xEC, 0x9A, 0x15, 0x89, 0x87};
  thread_local EncryptedName<sizeof(effectSource), 0x61FF87ED7BF39BE1ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const std::uint8_t bottomSource[] = {0x8B, 0xF7, 0x52, 0x79, 0x0D, 0x60, 0xF6, 0x1B, 0xA0, 0xF2, 0x4E, 0x63, 0x01, 0x60, 0xFB, 0x12, 0x8B, 0xF1, 0x48, 0x60, 0x65};
  thread_local EncryptedName<sizeof(bottomSource), 0x7D993F650D2785FFULL> bottomName(bottomSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00623C70
// FUNCTION: fn_04535
EfstActionMessage CEfstAction_TRUTH_OF_WIND::Slot03() {
  return {LookupMessage(4656), 0xFFFF00};
}

// OFFSET: 0x00623CA0
// FUNCTION: fn_04536
EfstActionMessage CEfstAction_TRUTH_OF_WIND::Slot04() {
  return {LookupMessage(4657), 0xFF};
}

// OFFSET: 0x00623CD0
// FUNCTION: fn_04537
int CSkillAction_DR_CUTTING_WIND::Slot01() {
  return 61;
}

// OFFSET: 0x00623CE0
// FUNCTION: fn_04538
int CSkillAction_DR_CUTTING_WIND::Slot02() {
  return 64;
}

// OFFSET: 0x00623CF0
// FUNCTION: fn_04539
void CSkillAction_DR_CUTTING_WIND::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x00623D20
// FUNCTION: fn_04540
void CSkillAction_DR_CUTTING_WIND::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target || !source) {
    return;
  }

  const std::uint8_t effectSource[] = {0x70, 0xB2, 0x77, 0x61, 0x92, 0xFF, 0x70, 0x10, 0x64, 0xAE, 0x6D, 0x71, 0xFB};
  thread_local EncryptedName<sizeof(effectSource), 0x4F1791FB1503C713ULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot17(ScreenAngle(&target->render.position.x, &source->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values));
  }
}

// OFFSET: 0x00623F70
// FUNCTION: fn_04541
void CSkillAction_DR_CUTTING_WIND::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  const std::uint8_t effectSource[] = {0xB0, 0x98, 0xE3, 0x61, 0x24, 0x35, 0xC2, 0xFE, 0xA4, 0x84, 0xF9, 0x71, 0x12, 0x33, 0xCC, 0xD5, 0xD3};
  thread_local EncryptedName<sizeof(effectSource), 0xA1A55B4D1597EDD3ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00624170
// FUNCTION: fn_04542
int CSkillAction_DR_WIND_BOMB::Slot01() {
  return 61;
}

// OFFSET: 0x00624180
// FUNCTION: fn_04543
int CSkillAction_DR_WIND_BOMB::Slot02() {
  return 64;
}

// OFFSET: 0x00624190
// FUNCTION: fn_04544
void CSkillAction_DR_WIND_BOMB::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x006241C0
// FUNCTION: fn_04545
void CSkillAction_DR_WIND_BOMB::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 2468, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    static_cast<CBezierArcThrowEffect*>(effect)->SetTarget(target->render.position);
  }

  const std::uint8_t effectSource[] = {0x76, 0x3E, 0xF9, 0xBB, 0x4C, 0xE7, 0x3C, 0xBC, 0x63, 0x08, 0xFA, 0xBE, 0x7A, 0xEB, 0x53};
  thread_local EncryptedName<sizeof(effectSource), 0xD1538513DF975701ULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot16(300);
  }
}

// OFFSET: 0x00624430
// FUNCTION: fn_04546
void CSkillAction_DR_WIND_BOMB::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  const std::uint8_t effectSource[] = {0xE8, 0xAA, 0x29, 0xD9, 0xB4, 0x25, 0x8E, 0xFC, 0xFD, 0x9C, 0x2F, 0xD4, 0x9F, 0x47};
  thread_local EncryptedName<sizeof(effectSource), 0x91E147EBBD47C39FULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00624640
// FUNCTION: fn_04547
void CSkillAction_DR_AROUND_FLOWER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  const std::uint8_t effectSource[] = {0xFA, 0xDF, 0x1A, 0xBA, 0xBF, 0x59, 0xB2, 0x0B, 0xF7, 0xC2, 0x02, 0xAA, 0xA3, 0x3D};
  thread_local EncryptedName<sizeof(effectSource), 0x6DED3DD1CF75AD9BULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const std::uint8_t bottomSource[] = {0xA0, 0xA1, 0xB4, 0x02, 0xF5, 0x11, 0x9C, 0x2F, 0xAD, 0xBC, 0xAC, 0x12, 0xE9, 0x2A, 0xA1, 0x26, 0xB5, 0xA7, 0xB4, 0x1A, 0x9B};
  thread_local EncryptedName<sizeof(bottomSource), 0x49C3759B77DBD3C1ULL> bottomName(bottomSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006249E0
// FUNCTION: fn_04548
void CSkillAction_DR_EARTH_FLOWER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const std::uint8_t effectSource[] = {0x98, 0x9C, 0x0F, 0x87, 0x89, 0x4A, 0x9F, 0x77, 0x92, 0x8A, 0x18, 0x81, 0xE1};
  thread_local EncryptedName<sizeof(effectSource), 0x1BF915E1F37DFDFDULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const std::uint8_t bottomSource[] = {0xBA, 0xB8, 0x2F, 0x1D, 0x7B, 0xCC, 0x99, 0x6D, 0xB0, 0xAE, 0x38, 0x1B, 0x4C, 0xF1, 0x90, 0x75, 0xAB, 0xB6, 0x30, 0x69};
  thread_local EncryptedName<sizeof(bottomSource), 0x01FF9313695DD9DFULL> bottomName(bottomSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00624D80
// FUNCTION: fn_04549
void CSkillAction_DR_ICE_CLOUD::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const std::uint8_t effectSource[] = {0x3E, 0xE0, 0x8C, 0x8A, 0xE2, 0xA9, 0x22, 0x32, 0x33, 0x83};
  thread_local EncryptedName<sizeof(effectSource), 0x474DC581D5E98357ULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00624F80
// FUNCTION: fn_04550
void CSkillAction_DR_ICE_TOTEM::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const std::uint8_t effectSource[] = {0x04, 0x3C, 0x9C, 0x6C, 0x97, 0x14, 0x21, 0x5A, 0x00, 0x5F};
  thread_local EncryptedName<sizeof(effectSource), 0x3F557BE333F95F6DULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00625180
// FUNCTION: fn_04551
int CSkillAction_DR_WEREWOLF::Slot02() {
  return 64;
}

// OFFSET: 0x00625190
// FUNCTION: fn_04552
void CSkillAction_DR_WEREWOLF::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }
  if (this->Slot17(source) == true) {
    return;
  }

  const std::uint8_t effectSource[] = {0xCE, 0x68, 0x5D, 0xC4, 0x34, 0x94, 0x63, 0x2F, 0xB9};
  thread_local EncryptedName<sizeof(effectSource), 0x490FFB43A12F0DB9ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006253A0
// FUNCTION: fn_04553
EfstActionMessage CEfstAction_WEREWOLF::Slot03() {
  return {LookupMessage(4642), 0xFFFF00};
}

// OFFSET: 0x006253D0
// FUNCTION: fn_04554
EfstActionMessage CEfstAction_WEREWOLF::Slot04() {
  return {LookupMessage(4643), 0xFF};
}

// OFFSET: 0x00620270
// FUNCTION: fn_04466
void CSkillAction_DR_ENRAGE_WOLF::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  const std::uint8_t effectSource[] = {0x08, 0x8D, 0x37, 0xC2, 0x8A, 0xA4, 0x88, 0x46, 0x02, 0x8F, 0x23, 0xA3};
  thread_local EncryptedName<sizeof(effectSource), 0x31D7C1EDA345E36DULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00620530
// FUNCTION: fn_04472
void CSkillAction_DR_BLOOD_HOWLING::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  const std::uint8_t effectSource[] = {0x0F, 0x0F, 0x5A, 0x42, 0xC1, 0x5C, 0xE7, 0xB6, 0x1A, 0x0F, 0x5C, 0x43, 0xC2, 0x03};
  thread_local EncryptedName<sizeof(effectSource), 0xD98F03A52D35636DULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006207B0
// FUNCTION: fn_04476
void CSkillAction_DR_NOMERCY_CLAW::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  const std::uint8_t effectSource[] = {0xBF, 0x76, 0xF0, 0xEA, 0x19, 0xE8, 0x76, 0x50, 0xB2, 0x75, 0xFC, 0xF8, 0x6B};
  thread_local EncryptedName<sizeof(effectSource), 0x0F0F8B6B8F9D19D1ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006210B0
// FUNCTION: fn_04485
void CSkillAction_DR_ENRAGE_RAPTOR::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  const std::uint8_t effectSource[] = {0xD2, 0x69, 0x25, 0xAC, 0x4C, 0x82, 0x74, 0xF3, 0xD6, 0x77, 0x23, 0xA2, 0x59, 0xE7};
  thread_local EncryptedName<sizeof(effectSource), 0x812BE72BCD5707B7ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00621370
// FUNCTION: fn_04491
void CSkillAction_DR_PREENING::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  const std::uint8_t effectSource[] = {0x67, 0x8B, 0x2A, 0xDC, 0x95, 0xDC, 0x41, 0xA2, 0x17};
  thread_local EncryptedName<sizeof(effectSource), 0xC52FB5FBB94FF917ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00620DF0
// FUNCTION: fn_04481
void CSkillAction_DR_CRUEL_BITE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target || !this->CanUse(source)) {
    return;
  }

  if (CRagEffect* effect = source->LaunchEffect(2439, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
  }

  const std::uint8_t effectSource[] = {0xC2, 0x4F, 0xEA, 0xB2, 0x0F, 0xD0, 0x45, 0x30, 0xD5, 0x58, 0x9F};
  thread_local EncryptedName<sizeof(effectSource), 0x59278F63D79F3DA1ULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006215E0
// FUNCTION: fn_04495
void CSkillAction_DR_SHOOTING_FEATHER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target || !this->CanUse(source)) {
    return;
  }

  const std::uint8_t effectSource[] = {0xB6, 0xA5, 0xE8, 0x9A, 0xD5, 0xCA, 0x89, 0xF4, 0x9A, 0xAB, 0xE2, 0x94, 0xD5, 0xCB, 0x82, 0xE1, 0xC5};
  thread_local EncryptedName<sizeof(effectSource), 0x93E7A3A1F587CDC5ULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot17(ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values));
  }
}
