#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Contents/Effects/Skills/BezierArcThrowEffect.h"
#include "Contents/Effects/Particles/EZ2STREffectEx.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <format>
#include <string>

class CSkillAction_AnimalForm : public CSkillAction {
public:
  bool CanUse(CGameActor* actor);
  virtual bool IsInForm(CGameActor* actor) = 0;
  virtual void PlayFormAttackEffect(CGameActor* actor) = 0;
};

class CSkillAction_Direction {
public:
  int GetDirection(float angle);
  bool IsValidDirection(unsigned int direction);
};

class CSkillAction_DamageWereRaptorForm : public CSkillAction_AnimalForm, public CSkillAction_Direction {
public:
  void SpawnAttack00Effect(int sourceActor, CGameActor* target);
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
  bool IsInForm(CGameActor* actor) override;
  void PlayFormAttackEffect(CGameActor* actor) override;
};

class CSkillAction_DamageWereWolfForm : public CSkillAction_AnimalForm {
public:
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
  bool IsInForm(CGameActor* actor) override;
  void PlayFormAttackEffect(CGameActor* actor) override;
};

class CEfstAction_BasicAction : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  virtual int GetEffectId(int efstId, int argument4) = 0;
};

class CSkillAction_DR_WERERAPTOR : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_AnimalForm : public CEfstAction {
public:
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void ResetAnimalForm(CRenderObject* actor);
  void ApplyAnimalForm(CRenderObject* actor, int form);
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_WERERAPTOR : public CEfstAction_AnimalForm {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_DR_ENRAGE_WOLF : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_ENRAGE_WOLF : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_DR_BLOOD_HOWLING : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_BLOOD_HOWLING : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_DR_NOMERCY_CLAW : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_HUNGER : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int caster, CGameActor* target, CGameActor* argument3) override;
};

class CSkillAction_DR_CRUEL_BITE : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_ENRAGE_RAPTOR : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_ENRAGE_RAPTOR : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_DR_PREENING : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_PREENING : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_DR_SHOOTING_FEATHER : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_FLICKING_TONADO : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_LOW_FLIGHT : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_NATURE_SHIELD : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_NATURE_SHIELD : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CEfstAction_GROUND_GROW : public CEfstAction_BasicAction {
public:
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  int GetEffectId(int efstId, int argument4) override;
};

class CSkillAction_Earthing : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_Icing : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_NATURE_PROTECTION : public CEfstAction_BasicAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
  int GetEffectId(int efstId, int argument4) override;
};

class CEfstAction_THUNDERING_ROD : public CEfstAction_BasicAction {
public:
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  int GetEffectId(int efstId, int argument4) override;
};

class CEfstAction_THUNDERING_ROD_MAX : public CEfstAction_BasicAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
  int GetEffectId(int efstId, int argument4) override;
};

class CSkillAction_DR_TRUTH_OF_EARTH : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_TRUTH_OF_EARTH : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_DR_TRUTH_OF_ICE : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_TRUTH_OF_ICE : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_DR_TRUTH_OF_WIND : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_TRUTH_OF_WIND : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_DR_CUTTING_WIND : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_WIND_BOMB : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_AROUND_FLOWER : public CSkillAction_Earthing {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_EARTH_FLOWER : public CSkillAction_Earthing {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_ICE_CLOUD : public CSkillAction_Icing {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_ICE_TOTEM : public CSkillAction_Icing {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DR_WEREWOLF : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_WEREWOLF : public CEfstAction_AnimalForm {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

// OFFSET: 0x0061FF70
// FUNCTION: fn_04458
int CSkillAction_DR_WERERAPTOR::GetSkillAttackMotion() {
  return 64;
}

// OFFSET: 0x0061FF80
// FUNCTION: fn_04459
void CSkillAction_DR_WERERAPTOR::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }
  if (this->IsInForm(source) == true) {
    return;
  }

  constexpr auto effectSourceText = CRYPT_NAME("wereraptor", 0xD3755DD95F47A909ULL);
  const auto& effectSource = effectSourceText.data;
  thread_local EncryptedName<sizeof(effectSource), 0xD3755DD95F47A909ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00620190
// FUNCTION: fn_04460
EfstActionMessage CEfstAction_WERERAPTOR::GetEfstBeginMessage() {
  return {LookupMessage(4648), 0xFFFF00};
}

// OFFSET: 0x006201C0
// FUNCTION: fn_04461
EfstActionMessage CEfstAction_WERERAPTOR::GetEfstEndMessage() {
  return {LookupMessage(4649), 0xFF};
}

// OFFSET: 0x00620220
// FUNCTION: fn_04463
int CSkillAction_DR_ENRAGE_WOLF::GetSkillCastMotion() {
  return 66;
}

// OFFSET: 0x00620230
// FUNCTION: fn_04464
int CSkillAction_DR_ENRAGE_WOLF::GetSkillAttackMotion() {
  return 68;
}

// OFFSET: 0x00620240
// FUNCTION: fn_04465
void CSkillAction_DR_ENRAGE_WOLF::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x00620480
// FUNCTION: fn_04467
EfstActionMessage CEfstAction_ENRAGE_WOLF::GetEfstBeginMessage() {
  return {LookupMessage(4644), 0xFFFF00};
}

// OFFSET: 0x006204B0
// FUNCTION: fn_04468
EfstActionMessage CEfstAction_ENRAGE_WOLF::GetEfstEndMessage() {
  return {LookupMessage(4645), 0xFF};
}

// OFFSET: 0x006204E0
// FUNCTION: fn_04469
int CSkillAction_DR_BLOOD_HOWLING::GetSkillCastMotion() {
  return 66;
}

// OFFSET: 0x006204F0
// FUNCTION: fn_04470
int CSkillAction_DR_BLOOD_HOWLING::GetSkillAttackMotion() {
  return 68;
}

// OFFSET: 0x00620500
// FUNCTION: fn_04471
void CSkillAction_DR_BLOOD_HOWLING::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x00620740
// FUNCTION: fn_04473
EfstActionMessage CEfstAction_BLOOD_HOWLING::GetEfstBeginMessage() {
  return {LookupMessage(4646), 0xFFFF00};
}

// OFFSET: 0x00620770
// FUNCTION: fn_04474
EfstActionMessage CEfstAction_BLOOD_HOWLING::GetEfstEndMessage() {
  return {LookupMessage(4647), 0xFF};
}

// OFFSET: 0x006207A0
// FUNCTION: fn_04475
int CSkillAction_DR_NOMERCY_CLAW::GetSkillAttackMotion() {
  return 69;
}

// OFFSET: 0x006209C0
// FUNCTION: fn_04477
int CSkillAction_DR_HUNGER::GetSkillAttackMotion() {
  return 70;
}

// OFFSET: 0x006209D0
// FUNCTION: fn_04478
void CSkillAction_DR_HUNGER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("hunger_target", 0x3375B1C7DBB5FD55ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x3375B1C7DBB5FD55ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00620BE0
// FUNCTION: fn_04479
void CSkillAction_DR_HUNGER::OnSkillHit(int caster, CGameActor* target, CGameActor* argument3) {
  CGameActor* source = reinterpret_cast<CGameActor*>(caster);
  if (!source) {
    return;
  }

  constexpr auto effectSourceText = CRYPT_NAME("hunger", 0x1D6D75EDE7CD311FULL);
  const auto& effectSource = effectSourceText.data;
  thread_local EncryptedName<sizeof(effectSource), 0x1D6D75EDE7CD311FULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00620DE0
// FUNCTION: fn_04480
int CSkillAction_DR_CRUEL_BITE::GetSkillAttackMotion() {
  return 73;
}

// OFFSET: 0x00621060
// FUNCTION: fn_04482
int CSkillAction_DR_ENRAGE_RAPTOR::GetSkillCastMotion() {
  return 66;
}

// OFFSET: 0x00621070
// FUNCTION: fn_04483
int CSkillAction_DR_ENRAGE_RAPTOR::GetSkillAttackMotion() {
  return 68;
}

// OFFSET: 0x00621080
// FUNCTION: fn_04484
void CSkillAction_DR_ENRAGE_RAPTOR::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x006212C0
// FUNCTION: fn_04486
EfstActionMessage CEfstAction_ENRAGE_RAPTOR::GetEfstBeginMessage() {
  return {LookupMessage(4650), 0xFFFF00};
}

// OFFSET: 0x006212F0
// FUNCTION: fn_04487
EfstActionMessage CEfstAction_ENRAGE_RAPTOR::GetEfstEndMessage() {
  return {LookupMessage(4651), 0xFF};
}

// OFFSET: 0x00621320
// FUNCTION: fn_04488
int CSkillAction_DR_PREENING::GetSkillCastMotion() {
  return 66;
}

// OFFSET: 0x00621330
// FUNCTION: fn_04489
int CSkillAction_DR_PREENING::GetSkillAttackMotion() {
  return 68;
}

// OFFSET: 0x00621340
// FUNCTION: fn_04490
void CSkillAction_DR_PREENING::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x00621570
// FUNCTION: fn_04492
EfstActionMessage CEfstAction_PREENING::GetEfstBeginMessage() {
  return {LookupMessage(4652), 0xFFFF00};
}

// OFFSET: 0x006215A0
// FUNCTION: fn_04493
EfstActionMessage CEfstAction_PREENING::GetEfstEndMessage() {
  return {LookupMessage(4653), 0xFF};
}

// OFFSET: 0x006215D0
// FUNCTION: fn_04494
int CSkillAction_DR_SHOOTING_FEATHER::GetSkillAttackMotion() {
  return 72;
}

// OFFSET: 0x00621850
// FUNCTION: fn_04497
int CSkillAction_DR_FLICKING_TONADO::GetSkillAttackMotion() {
  return 67;
}

// OFFSET: 0x00621860
// FUNCTION: fn_04498
void CSkillAction_DR_FLICKING_TONADO::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
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
int CSkillAction_DR_LOW_FLIGHT::GetSkillAttackMotion() {
  return 70;
}

// OFFSET: 0x00621C10
// FUNCTION: fn_04500
void CSkillAction_DR_LOW_FLIGHT::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
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

  const float scale = source->attackMotion * source->render.speed * 16.0f;
  const bool flag = direction != 1 && direction != 2;

  const std::string effectName(
    (direction & 1) != 0 ? std::string([] {
      constexpr auto horizonSourceText = CRYPT_NAME("low_flight_horizon", 0x8B65CD61012B5931ULL);
      const auto& horizonSource = horizonSourceText.data;
      thread_local EncryptedName<sizeof(horizonSource), 0x8B65CD61012B5931ULL> horizonName(horizonSource);
      return horizonName.Text();
    }()) : std::string([] {
      constexpr auto verticalSourceText = CRYPT_NAME("low_flight_vertical", 0xF7530137154D8FE5ULL);
      const auto& verticalSource = verticalSourceText.data;
      thread_local EncryptedName<sizeof(verticalSource), 0xF7530137154D8FE5ULL> verticalName(verticalSource);
      return verticalName.Text();
    }())
  );

  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetDelay(static_cast<int>(scale));
    float angleOffset = 0.0f;
    if ((direction & 1) != 0) {
      effect->SetFlipX(flag);
      if (!flag) {
        angleOffset = -180.0f;
      }
    } else {
      effect->SetFlipY(flag);
      if (flag) {
        angleOffset = -90.0f;
      } else {
        angleOffset = -270.0f;
      }
    }
    effect->SetAngle(static_cast<float>((static_cast<int>(angle + angleOffset) + 360) % 360));
  }

  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        constexpr auto bottomSourceText = CRYPT_NAME("low_flight_bottom", 0x69FD1D9733D3CFC1ULL);
        const auto& bottomSource = bottomSourceText.data;
        thread_local EncryptedName<sizeof(bottomSource), 0x69FD1D9733D3CFC1ULL> bottomName(bottomSource);
        return bottomName.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetDelay(static_cast<int>(scale));
  }
}

// OFFSET: 0x006221F0
// FUNCTION: fn_04501
int CSkillAction_DR_NATURE_SHIELD::GetSkillCastMotion() {
  return 61;
}

// OFFSET: 0x00622200
// FUNCTION: fn_04502
int CSkillAction_DR_NATURE_SHIELD::GetSkillAttackMotion() {
  return 65;
}

// OFFSET: 0x00622210
// FUNCTION: fn_04503
void CSkillAction_DR_NATURE_SHIELD::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x00622240
// FUNCTION: fn_04504
void CSkillAction_DR_NATURE_SHIELD::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("nature_shield", 0xF5657D1F3DE5CDC7ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0xF5657D1F3DE5CDC7ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00622440
// FUNCTION: fn_04505
EfstActionMessage CEfstAction_NATURE_SHIELD::GetEfstBeginMessage() {
  return {LookupMessage(4660), 0xFFFF00};
}

// OFFSET: 0x00622470
// FUNCTION: fn_04506
EfstActionMessage CEfstAction_NATURE_SHIELD::GetEfstEndMessage() {
  return {LookupMessage(4661), 0xFF};
}

// OFFSET: 0x006224A0
// FUNCTION: fn_04507
void CEfstAction_GROUND_GROW::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  for (int index = 0; index < 12; ++index) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, this->GetEffectId(efstId, index + 1));
  }
}

// OFFSET: 0x006224E0
// FUNCTION: fn_04508
int CEfstAction_GROUND_GROW::GetEffectId(int efstId, int argument4) {
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
EfstActionMessage CEfstAction_NATURE_PROTECTION::GetEfstBeginMessage() {
  return {LookupMessage(4666), 0xFFFF00};
}

// OFFSET: 0x00622950
// FUNCTION: fn_04510
EfstActionMessage CEfstAction_NATURE_PROTECTION::GetEfstEndMessage() {
  return {LookupMessage(4667), 0xFF};
}

// OFFSET: 0x00622980
// FUNCTION: fn_04511
int CEfstAction_NATURE_PROTECTION::GetEffectId(int efstId, int argument4) {
  return CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 18> bytes = CRYPT_NAME("nature_protection", 0x8B3FEB5B8BD5C3C3ULL);
    thread_local EncryptedName<sizeof(bytes), 0x8B3FEB5B8BD5C3C3ULL> name(bytes);
    return name.Text();
  }());
}

// OFFSET: 0x00622B20
// FUNCTION: fn_04512
void CEfstAction_NATURE_PROTECTION::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    static_cast<CGameActor*>(actor)->ApplyNatureProtection();
  }
  CEfstAction_BasicAction::OnEfstBegin(efstId, value, argument3, argument4, actor, argument6);
}

// OFFSET: 0x00622B60
// FUNCTION: fn_04513
void CEfstAction_NATURE_PROTECTION::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    static_cast<CGameActor*>(actor)->ApplySavedShapeShift();
  }
  CEfstAction_BasicAction::OnEfstEnd(efstId, argument2, argument3, argument4, actor);
}

// OFFSET: 0x00622B90
// FUNCTION: fn_04514
void CEfstAction_THUNDERING_ROD::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  for (int index = 0; index < 6; ++index) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, this->GetEffectId(efstId, index + 1));
  }
}

// OFFSET: 0x00622BD0
// FUNCTION: fn_04515
int CEfstAction_THUNDERING_ROD::GetEffectId(int efstId, int argument4) {
  const int step = argument4 - 1;
  if (static_cast<unsigned int>(step) > 5) {
    return -1;
  }

  std::string suffix = std::format("{:02}", step);
  constexpr auto prefixSourceText = CRYPT_NAME("thundering_rod_", 0x0BEB63A5C905B3A7ULL);
  const auto& prefixSource = prefixSourceText.data;
  thread_local EncryptedName<sizeof(prefixSource), 0x0BEB63A5C905B3A7ULL> prefixName(prefixSource);
  return CEZ2STREffectMgr::GetInstance()->EffectId(std::string(prefixName.Text()) + suffix);
}

// OFFSET: 0x00622DD0
// FUNCTION: fn_04516
EfstActionMessage CEfstAction_THUNDERING_ROD_MAX::GetEfstBeginMessage() {
  return {LookupMessage(4688), 0xFFFF00};
}

// OFFSET: 0x00622E00
// FUNCTION: fn_04517
EfstActionMessage CEfstAction_THUNDERING_ROD_MAX::GetEfstEndMessage() {
  return {LookupMessage(4689), 0xFF};
}

// OFFSET: 0x00622E30
// FUNCTION: fn_04518
int CEfstAction_THUNDERING_ROD_MAX::GetEffectId(int efstId, int argument4) {
  return CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 20> bytes = CRYPT_NAME("thundering_rod_full", 0x9B17C9EFDBB39F2FULL);
    thread_local EncryptedName<sizeof(bytes), 0x9B17C9EFDBB39F2FULL> name(bytes);
    return name.Text();
  }());
}

// OFFSET: 0x00622FD0
// FUNCTION: fn_04519
int CSkillAction_DR_TRUTH_OF_EARTH::GetSkillCastMotion() {
  return 61;
}

// OFFSET: 0x00622FE0
// FUNCTION: fn_04520
int CSkillAction_DR_TRUTH_OF_EARTH::GetSkillAttackMotion() {
  return 63;
}

// OFFSET: 0x00622FF0
// FUNCTION: fn_04521
void CSkillAction_DR_TRUTH_OF_EARTH::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(57, level / 16);
  }
}

// OFFSET: 0x00623020
// FUNCTION: fn_04522
void CSkillAction_DR_TRUTH_OF_EARTH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("truth_of_earth", 0xC7912587FFF31B71ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0xC7912587FFF31B71ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("truth_of_earth_bottom", 0x69D345DB950F336DULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x69D345DB950F336DULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006233D0
// FUNCTION: fn_04523
EfstActionMessage CEfstAction_TRUTH_OF_EARTH::GetEfstBeginMessage() {
  return {LookupMessage(4654), 0xFFFF00};
}

// OFFSET: 0x00623400
// FUNCTION: fn_04524
EfstActionMessage CEfstAction_TRUTH_OF_EARTH::GetEfstEndMessage() {
  return {LookupMessage(4655), 0xFF};
}

// OFFSET: 0x00623430
// FUNCTION: fn_04525
int CSkillAction_DR_TRUTH_OF_ICE::GetSkillCastMotion() {
  return 61;
}

// OFFSET: 0x00623440
// FUNCTION: fn_04526
int CSkillAction_DR_TRUTH_OF_ICE::GetSkillAttackMotion() {
  return 65;
}

// OFFSET: 0x00623450
// FUNCTION: fn_04527
void CSkillAction_DR_TRUTH_OF_ICE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x00623480
// FUNCTION: fn_04528
void CSkillAction_DR_TRUTH_OF_ICE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 13> bytes = CRYPT_NAME("truth_of_ice", 0x2D394DBD0F332733ULL);
    thread_local EncryptedName<sizeof(bytes), 0x2D394DBD0F332733ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 20> bytes = CRYPT_NAME("truth_of_ice_bottom", 0x151F77EBEBBB2147ULL);
    thread_local EncryptedName<sizeof(bytes), 0x151F77EBEBBB2147ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00623820
// FUNCTION: fn_04529
EfstActionMessage CEfstAction_TRUTH_OF_ICE::GetEfstBeginMessage() {
  return {LookupMessage(4658), 0xFFFF00};
}

// OFFSET: 0x00623850
// FUNCTION: fn_04530
EfstActionMessage CEfstAction_TRUTH_OF_ICE::GetEfstEndMessage() {
  return {LookupMessage(4659), 0xFF};
}

// OFFSET: 0x00623880
// FUNCTION: fn_04531
int CSkillAction_DR_TRUTH_OF_WIND::GetSkillCastMotion() {
  return 61;
}

// OFFSET: 0x00623890
// FUNCTION: fn_04532
int CSkillAction_DR_TRUTH_OF_WIND::GetSkillAttackMotion() {
  return 65;
}

// OFFSET: 0x006238A0
// FUNCTION: fn_04533
void CSkillAction_DR_TRUTH_OF_WIND::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x006238D0
// FUNCTION: fn_04534
void CSkillAction_DR_TRUTH_OF_WIND::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 14> bytes = CRYPT_NAME("truth_of_wind", 0x61FF87ED7BF39BE1ULL);
    thread_local EncryptedName<sizeof(bytes), 0x61FF87ED7BF39BE1ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 21> bytes = CRYPT_NAME("truth_of_wind_bottom", 0x7D993F650D2785FFULL);
    thread_local EncryptedName<sizeof(bytes), 0x7D993F650D2785FFULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00623C70
// FUNCTION: fn_04535
EfstActionMessage CEfstAction_TRUTH_OF_WIND::GetEfstBeginMessage() {
  return {LookupMessage(4656), 0xFFFF00};
}

// OFFSET: 0x00623CA0
// FUNCTION: fn_04536
EfstActionMessage CEfstAction_TRUTH_OF_WIND::GetEfstEndMessage() {
  return {LookupMessage(4657), 0xFF};
}

// OFFSET: 0x00623CD0
// FUNCTION: fn_04537
int CSkillAction_DR_CUTTING_WIND::GetSkillCastMotion() {
  return 61;
}

// OFFSET: 0x00623CE0
// FUNCTION: fn_04538
int CSkillAction_DR_CUTTING_WIND::GetSkillAttackMotion() {
  return 64;
}

// OFFSET: 0x00623CF0
// FUNCTION: fn_04539
void CSkillAction_DR_CUTTING_WIND::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x00623D20
// FUNCTION: fn_04540
void CSkillAction_DR_CUTTING_WIND::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target || !source) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("cutting_wind", 0x4F1791FB1503C713ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x4F1791FB1503C713ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetAngle(ScreenAngle(&target->render.position.x, &source->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values));
  }
}

// OFFSET: 0x00623F70
// FUNCTION: fn_04541
void CSkillAction_DR_CUTTING_WIND::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
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
int CSkillAction_DR_WIND_BOMB::GetSkillCastMotion() {
  return 61;
}

// OFFSET: 0x00624180
// FUNCTION: fn_04543
int CSkillAction_DR_WIND_BOMB::GetSkillAttackMotion() {
  return 64;
}

// OFFSET: 0x00624190
// FUNCTION: fn_04544
void CSkillAction_DR_WIND_BOMB::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x006241C0
// FUNCTION: fn_04545
void CSkillAction_DR_WIND_BOMB::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 2468, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    static_cast<CBezierArcThrowEffect*>(effect)->SetTarget(target->render.position);
  }

  constexpr auto effectSourceText = CRYPT_NAME("wind_bomb_main", 0xD1538513DF975701ULL);
  const auto& effectSource = effectSourceText.data;
  thread_local EncryptedName<sizeof(effectSource), 0xD1538513DF975701ULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetDelay(300);
  }
}

// OFFSET: 0x00624430
// FUNCTION: fn_04546
void CSkillAction_DR_WIND_BOMB::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("wind_bomb_hit", 0x91E147EBBD47C39FULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x91E147EBBD47C39FULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00624640
// FUNCTION: fn_04547
void CSkillAction_DR_AROUND_FLOWER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
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
void CSkillAction_DR_EARTH_FLOWER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 13> bytes = CRYPT_NAME("earth_flower", 0x1BF915E1F37DFDFDULL);
    thread_local EncryptedName<sizeof(bytes), 0x1BF915E1F37DFDFDULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 20> bytes = CRYPT_NAME("earth_flower_bottom", 0x01FF9313695DD9DFULL);
    thread_local EncryptedName<sizeof(bytes), 0x01FF9313695DD9DFULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00624D80
// FUNCTION: fn_04549
void CSkillAction_DR_ICE_CLOUD::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  constexpr auto effectSourceText = CRYPT_NAME("ice_cloud", 0x474DC581D5E98357ULL);
  const auto& effectSource = effectSourceText.data;
  thread_local EncryptedName<sizeof(effectSource), 0x474DC581D5E98357ULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00624F80
// FUNCTION: fn_04550
void CSkillAction_DR_ICE_TOTEM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  constexpr auto effectSourceText = CRYPT_NAME("ice_totem", 0x3F557BE333F95F6DULL);
  const auto& effectSource = effectSourceText.data;
  thread_local EncryptedName<sizeof(effectSource), 0x3F557BE333F95F6DULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00625180
// FUNCTION: fn_04551
int CSkillAction_DR_WEREWOLF::GetSkillAttackMotion() {
  return 64;
}

// OFFSET: 0x00625190
// FUNCTION: fn_04552
void CSkillAction_DR_WEREWOLF::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }
  if (this->IsInForm(source) == true) {
    return;
  }

  constexpr auto effectSourceText = CRYPT_NAME("werewolf", 0x490FFB43A12F0DB9ULL);
  const auto& effectSource = effectSourceText.data;
  thread_local EncryptedName<sizeof(effectSource), 0x490FFB43A12F0DB9ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006253A0
// FUNCTION: fn_04553
EfstActionMessage CEfstAction_WEREWOLF::GetEfstBeginMessage() {
  return {LookupMessage(4642), 0xFFFF00};
}

// OFFSET: 0x006253D0
// FUNCTION: fn_04554
EfstActionMessage CEfstAction_WEREWOLF::GetEfstEndMessage() {
  return {LookupMessage(4643), 0xFF};
}

// OFFSET: 0x00620270
// FUNCTION: fn_04466
void CSkillAction_DR_ENRAGE_WOLF::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("enrage_wolf", 0x31D7C1EDA345E36DULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x31D7C1EDA345E36DULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00620530
// FUNCTION: fn_04472
void CSkillAction_DR_BLOOD_HOWLING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  constexpr auto effectSourceText = CRYPT_NAME("blood_howling", 0xD98F03A52D35636DULL);
  const auto& effectSource = effectSourceText.data;
  thread_local EncryptedName<sizeof(effectSource), 0xD98F03A52D35636DULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006207B0
// FUNCTION: fn_04476
void CSkillAction_DR_NOMERCY_CLAW::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  constexpr auto effectSourceText = CRYPT_NAME("nomercy_claw", 0x0F0F8B6B8F9D19D1ULL);
  const auto& effectSource = effectSourceText.data;
  thread_local EncryptedName<sizeof(effectSource), 0x0F0F8B6B8F9D19D1ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006210B0
// FUNCTION: fn_04485
void CSkillAction_DR_ENRAGE_RAPTOR::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  constexpr auto effectSourceText = CRYPT_NAME("enrage_raptor", 0x812BE72BCD5707B7ULL);
  const auto& effectSource = effectSourceText.data;
  thread_local EncryptedName<sizeof(effectSource), 0x812BE72BCD5707B7ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00621370
// FUNCTION: fn_04491
void CSkillAction_DR_PREENING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  constexpr auto effectSourceText = CRYPT_NAME("preening", 0xC52FB5FBB94FF917ULL);
  const auto& effectSource = effectSourceText.data;
  thread_local EncryptedName<sizeof(effectSource), 0xC52FB5FBB94FF917ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00620DF0
// FUNCTION: fn_04481
void CSkillAction_DR_CRUEL_BITE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target || !this->CanUse(source)) {
    return;
  }

  if (CRagEffect* effect = source->LaunchEffect(2439, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("cruel_bite", 0x59278F63D79F3DA1ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x59278F63D79F3DA1ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x006215E0
// FUNCTION: fn_04495
void CSkillAction_DR_SHOOTING_FEATHER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target || !this->CanUse(source)) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0xB6, 0xA5, 0xE8, 0x9A, 0xD5, 0xCA, 0x89, 0xF4, 0x9A, 0xAB, 0xE2, 0x94, 0xD5, 0xCB, 0x82, 0xE1, 0xC5};
    thread_local EncryptedName<sizeof(bytes), 0x93E7A3A1F587CDC5ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetAngle(ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values));
  }
}

// OFFSET: 0x00621830
// FUNCTION: fn_04496
void CSkillAction_DR_SHOOTING_FEATHER::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  this->SpawnAttack00Effect(level, source);
}

// OFFSET: 0x0061FF40
// FUNCTION: fn_04457
void CEfstAction_WEREWOLF::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    this->ApplyAnimalForm(actor, 1);
    static_cast<CGameActor*>(actor)->SetBodyPaletteName();
  }
}

// OFFSET: 0x006201F0
// FUNCTION: fn_04462
void CEfstAction_WERERAPTOR::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    this->ApplyAnimalForm(actor, 2);
    static_cast<CGameActor*>(actor)->SetBodyPaletteName();
  }
}
