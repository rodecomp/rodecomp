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
  // INLINED
  template <typename Byte>
  explicit EncryptedName(const Byte (&source)[Length]) : m_encrypted(true) {
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
  void SpawnAttack00Effect(int sourceActor, CGameActor* target);
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

class CEfstAction_AnimalForm : public CEfstAction {
public:
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void ResetAnimalForm(CRenderObject* actor);
  void ApplyAnimalForm(CRenderObject* actor, int form);
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_WERERAPTOR : public CEfstAction_AnimalForm {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
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
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_FLICKING_TONADO : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_LOW_FLIGHT : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
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
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
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

class CEfstAction_WEREWOLF : public CEfstAction_AnimalForm {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
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

  const char effectSource[] = {'\x7E', '\xCC', '\x35', '\x3A', '\xAB', '\x3C', '\x05', '\xA7', '\x66', '\xDB', '\x47'};
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

  const char effectSource[] = {'\x3D', '\x88', '\xDB', '\xBC', '\xA2', '\xC3', '\x2A', '\x47', '\x34', '\x8F', '\xD2', '\xBE', '\xB3', '\xB1'};
  thread_local EncryptedName<sizeof(effectSource), 0x3375B1C7DBB5FD55ULL> effectName(effectSource);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00620BE0
// FUNCTION: fn_04479
void CSkillAction_DR_HUNGER::Slot15(int caster, CGameActor* target, CGameActor* argument3) {
  CGameActor* source = reinterpret_cast<CGameActor*>(caster);
  if (!source) {
    return;
  }

  const char effectSource[] = {'\x77', '\x44', '\xA3', '\x80', '\x88', '\x07', '\x6D'};
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

// OFFSET: 0x00621C10
// FUNCTION: fn_04500
void CSkillAction_DR_LOW_FLIGHT::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target || !this->CanUse(source)) {
    return;
  }

  const float angle = ScreenAngle(
    &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
  );
  const int direction = this->GetDirection(angle);
  if (!this->IsValidDirection(direction)) {
    return;
  }

  const float scale = source->unknown_0378 * source->render.speed * 16.0f;
  const bool flag = direction != 1 && direction != 2;

  const std::string effectName(
    (direction & 1) != 0 ? std::string([] {
      const std::uint8_t horizonSource[] = {0x5D, 0x36, 0x5C, 0x5E, 0x07, 0xA1, 0x0C, 0xEC, 0x59, 0x2D, 0x74, 0x69, 0x0E, 0xBF, 0x0C, 0xF1, 0x5E, 0x37, 0x2B};
      thread_local EncryptedName<sizeof(horizonSource), 0x8B65CD61012B5931ULL> horizonName(horizonSource);
      return horizonName.Text();
    }()) : std::string([] {
      const std::uint8_t verticalSource[] = {0x89, 0xE0, 0x3A, 0x4A, 0x51, 0x6D, 0x3A, 0x90, 0x8D, 0xFB, 0x12, 0x63, 0x52, 0x73, 0x27, 0x9E, 0x86, 0xEE, 0x21, 0x15};
      thread_local EncryptedName<sizeof(verticalSource), 0xF7530137154D8FE5ULL> verticalName(verticalSource);
      return verticalName.Text();
    }())
  );

  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot16(static_cast<int>(scale));
    float angleOffset = 0.0f;
    if ((direction & 1) != 0) {
      effect->Slot26(flag);
      if (!flag) {
        angleOffset = -180.0f;
      }
    } else {
      effect->Slot27(flag);
      if (flag) {
        angleOffset = -90.0f;
      } else {
        angleOffset = -270.0f;
      }
    }
    effect->Slot17(static_cast<float>((static_cast<int>(angle + angleOffset) + 360) % 360));
  }

  const int bottomId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bottomSource[] = {0xAD, 0xA0, 0xA4, 0x6C, 0xF1, 0x71, 0x94, 0x0E, 0xA9, 0xBB, 0x8C, 0x51, 0xF8, 0x69, 0x89, 0x06, 0xAC, 0xCF};
    thread_local EncryptedName<sizeof(bottomSource), 0x69FD1D9733D3CFC1ULL> bottomName(bottomSource);
    return bottomName.Text();
  }());
  if (CEffect* effect = g_effectMgr->Spawn(target->render.accountId, bottomId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot16(static_cast<int>(scale));
  }
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
  const char effectSource[] = {'\xA9', '\xAC', '\x91', '\x48', '\x6D', '\x18', '\x3A', '\x86', '\xAF', '\xA4', '\x80', '\x51', '\x7B', '\x7D'};
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
    const char fullSource[] = {'\xAF', '\xC2', '\x49', '\xFB', '\xBB'};
    thread_local EncryptedName<sizeof(fullSource), 0x078333BB9725B7C9ULL> fullName(fullSource);
    return fullName.Text();
  };
  std::string suffix = step == 11 ? std::string(full()) : std::format("{:02}", step);

  const char prefixSource[] = {'\xD2', '\x07', '\x0E', '\x2A', '\x93', '\x3F', '\x44', '\xE8', '\xC7', '\x1A', '\x16', '\x00', '\xFD'};
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
  const char effectSource[] = {'\xAD', '\xA2', '\xA1', '\xFE', '\x29', '\x8E', '\x60', '\xFB', '\xB1', '\xAC', '\xA1', '\xEE', '\x38', '\x9F', '\x56', '\xE4', '\xAD', '\xC3'};
  thread_local EncryptedName<sizeof(effectSource), 0x8B3FEB5B8BD5C3C3ULL> effectName(effectSource);
  return CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text());
}

// OFFSET: 0x00622B20
// FUNCTION: fn_04512
void CEfstAction_NATURE_PROTECTION::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    static_cast<CGameActor*>(actor)->ApplyNatureProtection();
  }
  CEfstAction_BasicAction::Slot00(efstId, value, argument3, argument4, actor, argument6);
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
  const char prefixSource[] = {'\xD3', '\xDB', '\x70', '\xA7', '\xC1', '\x06', '\x99', '\x62', '\xC9', '\xD4', '\x5A', '\xBB', '\xCA', '\x07', '\xB4', '\x0B'};
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
  const char effectSource[] = {'\x5B', '\xF7', '\xC6', '\xB5', '\x8B', '\xAC', '\x65', '\xF2', '\x41', '\xF8', '\xEC', '\xA9', '\x80', '\xAD', '\x48', '\xFD', '\x5A', '\xF3', '\xDF', '\xDB'};
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

  const char effectSource[] = {'\x05', '\x69', '\x86', '\x8B', '\xEF', '\x7A', '\xFE', '\xA1', '\x2E', '\x7E', '\x92', '\x8D', '\xF3', '\x4D', '\x91'};
  thread_local EncryptedName<sizeof(effectSource), 0xC7912587FFF31B71ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const char bottomSource[] = {'\x19', '\x41', '\x7A', '\xE1', '\xB3', '\x1A', '\xBC', '\x0F', '\x32', '\x56', '\x6E', '\xE7', '\xAF', '\x2D', '\x8C', '\x0B', '\x02', '\x47', '\x7B', '\xFA', '\xB6', '\x45'};
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

  const char effectSource[] = {'\x47', '\x55', '\x46', '\x7B', '\xD5', '\x12', '\x56', '\x4B', '\x6C', '\x4E', '\x50', '\x6A', '\xBD'};
  thread_local EncryptedName<sizeof(effectSource), 0x2D394DBD0F332733ULL> effectName(effectSource);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const char bottomSource[] = {'\x33', '\x53', '\xCE', '\x9F', '\x83', '\x28', '\x70', '\x73', '\x18', '\x48', '\xD8', '\x8E', '\xB4', '\x15', '\x70', '\x61', '\x33', '\x4E', '\xD6', '\xEB'};
  thread_local EncryptedName<sizeof(bottomSource), 0x151F77EBEBBB2147ULL> bottomName(bottomSource);
  effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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

  const char effectSource[] = {'\x95', '\xE9', '\x86', '\x0F', '\x85', '\xD8', '\x90', '\x07', '\xBE', '\xEC', '\x9A', '\x15', '\x89', '\x87'};
  thread_local EncryptedName<sizeof(effectSource), 0x61FF87ED7BF39BE1ULL> effectName(effectSource);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const char bottomSource[] = {'\x8B', '\xF7', '\x52', '\x79', '\x0D', '\x60', '\xF6', '\x1B', '\xA0', '\xF2', '\x4E', '\x63', '\x01', '\x60', '\xFB', '\x12', '\x8B', '\xF1', '\x48', '\x60', '\x65'};
  thread_local EncryptedName<sizeof(bottomSource), 0x7D993F650D2785FFULL> bottomName(bottomSource);
  effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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

  const char effectSource[] = {'\x70', '\xB2', '\x77', '\x61', '\x92', '\xFF', '\x70', '\x10', '\x64', '\xAE', '\x6D', '\x71', '\xFB'};
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
  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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

  const char effectSource[] = {'\x76', '\x3E', '\xF9', '\xBB', '\x4C', '\xE7', '\x3C', '\xBC', '\x63', '\x08', '\xFA', '\xBE', '\x7A', '\xEB', '\x53'};
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

  const char effectSource[] = {'\xE8', '\xAA', '\x29', '\xD9', '\xB4', '\x25', '\x8E', '\xFC', '\xFD', '\x9C', '\x2F', '\xD4', '\x9F', '\x47'};
  thread_local EncryptedName<sizeof(effectSource), 0x91E147EBBD47C39FULL> effectName(effectSource);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00624640
// FUNCTION: fn_04547
void CSkillAction_DR_AROUND_FLOWER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  const char effectSource[] = {'\xFA', '\xDF', '\x1A', '\xBA', '\xBF', '\x59', '\xB2', '\x0B', '\xF7', '\xC2', '\x02', '\xAA', '\xA3', '\x3D'};
  thread_local EncryptedName<sizeof(effectSource), 0x6DED3DD1CF75AD9BULL> effectName(effectSource);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const char bottomSource[] = {'\xA0', '\xA1', '\xB4', '\x02', '\xF5', '\x11', '\x9C', '\x2F', '\xAD', '\xBC', '\xAC', '\x12', '\xE9', '\x2A', '\xA1', '\x26', '\xB5', '\xA7', '\xB4', '\x1A', '\x9B'};
  thread_local EncryptedName<sizeof(bottomSource), 0x49C3759B77DBD3C1ULL> bottomName(bottomSource);
  effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006249E0
// FUNCTION: fn_04548
void CSkillAction_DR_EARTH_FLOWER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const char effectSource[] = {'\x98', '\x9C', '\x0F', '\x87', '\x89', '\x4A', '\x9F', '\x77', '\x92', '\x8A', '\x18', '\x81', '\xE1'};
  thread_local EncryptedName<sizeof(effectSource), 0x1BF915E1F37DFDFDULL> effectName(effectSource);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const char bottomSource[] = {'\xBA', '\xB8', '\x2F', '\x1D', '\x7B', '\xCC', '\x99', '\x6D', '\xB0', '\xAE', '\x38', '\x1B', '\x4C', '\xF1', '\x90', '\x75', '\xAB', '\xB6', '\x30', '\x69'};
  thread_local EncryptedName<sizeof(bottomSource), 0x01FF9313695DD9DFULL> bottomName(bottomSource);
  effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00624D80
// FUNCTION: fn_04549
void CSkillAction_DR_ICE_CLOUD::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const char effectSource[] = {'\x3E', '\xE0', '\x8C', '\x8A', '\xE2', '\xA9', '\x22', '\x32', '\x33', '\x83'};
  thread_local EncryptedName<sizeof(effectSource), 0x474DC581D5E98357ULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00624F80
// FUNCTION: fn_04550
void CSkillAction_DR_ICE_TOTEM::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const char effectSource[] = {'\x04', '\x3C', '\x9C', '\x6C', '\x97', '\x14', '\x21', '\x5A', '\x00', '\x5F'};
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

  const char effectSource[] = {'\xCE', '\x68', '\x5D', '\xC4', '\x34', '\x94', '\x63', '\x2F', '\xB9'};
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

  const char effectSource[] = {'\x08', '\x8D', '\x37', '\xC2', '\x8A', '\xA4', '\x88', '\x46', '\x02', '\x8F', '\x23', '\xA3'};
  thread_local EncryptedName<sizeof(effectSource), 0x31D7C1EDA345E36DULL> effectName(effectSource);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00620530
// FUNCTION: fn_04472
void CSkillAction_DR_BLOOD_HOWLING::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  const char effectSource[] = {'\x0F', '\x0F', '\x5A', '\x42', '\xC1', '\x5C', '\xE7', '\xB6', '\x1A', '\x0F', '\x5C', '\x43', '\xC2', '\x03'};
  thread_local EncryptedName<sizeof(effectSource), 0xD98F03A52D35636DULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006207B0
// FUNCTION: fn_04476
void CSkillAction_DR_NOMERCY_CLAW::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  const char effectSource[] = {'\xBF', '\x76', '\xF0', '\xEA', '\x19', '\xE8', '\x76', '\x50', '\xB2', '\x75', '\xFC', '\xF8', '\x6B'};
  thread_local EncryptedName<sizeof(effectSource), 0x0F0F8B6B8F9D19D1ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006210B0
// FUNCTION: fn_04485
void CSkillAction_DR_ENRAGE_RAPTOR::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  const char effectSource[] = {'\xD2', '\x69', '\x25', '\xAC', '\x4C', '\x82', '\x74', '\xF3', '\xD6', '\x77', '\x23', '\xA2', '\x59', '\xE7'};
  thread_local EncryptedName<sizeof(effectSource), 0x812BE72BCD5707B7ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00621370
// FUNCTION: fn_04491
void CSkillAction_DR_PREENING::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  const char effectSource[] = {'\x67', '\x8B', '\x2A', '\xDC', '\x95', '\xDC', '\x41', '\xA2', '\x17'};
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

  const char effectSource[] = {'\xC2', '\x4F', '\xEA', '\xB2', '\x0F', '\xD0', '\x45', '\x30', '\xD5', '\x58', '\x9F'};
  thread_local EncryptedName<sizeof(effectSource), 0x59278F63D79F3DA1ULL> effectName(effectSource);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006215E0
// FUNCTION: fn_04495
void CSkillAction_DR_SHOOTING_FEATHER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target || !this->CanUse(source)) {
    return;
  }

  const std::uint8_t effectSource[] = {0xB6, 0xA5, 0xE8, 0x9A, 0xD5, 0xCA, 0x89, 0xF4, 0x9A, 0xAB, 0xE2, 0x94, 0xD5, 0xCB, 0x82, 0xE1, 0xC5};
  thread_local EncryptedName<sizeof(effectSource), 0x93E7A3A1F587CDC5ULL> effectName(effectSource);
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot17(ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values));
  }
}

// OFFSET: 0x00621830
// FUNCTION: fn_04496
void CSkillAction_DR_SHOOTING_FEATHER::Slot15(int level, CGameActor* source, CGameActor* target) {
  this->SpawnAttack00Effect(level, source);
}

// OFFSET: 0x0061FF40
// FUNCTION: fn_04457
void CEfstAction_WEREWOLF::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    this->ApplyAnimalForm(actor, 1);
    static_cast<CGameActor*>(actor)->Slot32();
  }
}

// OFFSET: 0x006201F0
// FUNCTION: fn_04462
void CEfstAction_WERERAPTOR::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    this->ApplyAnimalForm(actor, 2);
    static_cast<CGameActor*>(actor)->Slot32();
  }
}
