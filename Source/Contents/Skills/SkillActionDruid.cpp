#include "Contents/Actors/ActorEnums.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Resource/MSGSTRINGID.h"
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
#include "Contents/Skills/SkillId.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

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
  void SpawnAttack00Effect(CGameActor* source, CGameActor* target);
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
  bool IsInForm(CGameActor* actor) override;
  void PlayFormAttackEffect(CGameActor* actor) override;
};

class CSkillAction_DamageWereWolfForm : public CSkillAction_AnimalForm {
public:
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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
  void OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) override;
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
  void OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) override;
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
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_Icing : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_DR_WIND_BOMB : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("wereraptor", 0xD3755DD95F47A909ULL, effectName)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x00620190
// FUNCTION: fn_04460
EfstActionMessage CEfstAction_WERERAPTOR::GetEfstBeginMessage() {
  return {LookupMessage(MSI_DR_WERERAPTOR_ON), 0xFFFF00};
}

// OFFSET: 0x006201C0
// FUNCTION: fn_04461
EfstActionMessage CEfstAction_WERERAPTOR::GetEfstEndMessage() {
  return {LookupMessage(MSI_DR_WERERAPTOR_OFF), 0xFF};
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
    source->LaunchEffect(EF_BEGINSPELL2, level / 16);
  }
}

// OFFSET: 0x00620480
// FUNCTION: fn_04467
EfstActionMessage CEfstAction_ENRAGE_WOLF::GetEfstBeginMessage() {
  return {LookupMessage(MSI_DR_ENRAGE_WOLF_ON), 0xFFFF00};
}

// OFFSET: 0x006204B0
// FUNCTION: fn_04468
EfstActionMessage CEfstAction_ENRAGE_WOLF::GetEfstEndMessage() {
  return {LookupMessage(MSI_DR_ENRAGE_WOLF_OFF), 0xFF};
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
    source->LaunchEffect(EF_BEGINSPELL2, level / 16);
  }
}

// OFFSET: 0x00620740
// FUNCTION: fn_04473
EfstActionMessage CEfstAction_BLOOD_HOWLING::GetEfstBeginMessage() {
  return {LookupMessage(MSI_DR_BLOOD_HOWLING_ON), 0xFFFF00};
}

// OFFSET: 0x00620770
// FUNCTION: fn_04474
EfstActionMessage CEfstAction_BLOOD_HOWLING::GetEfstEndMessage() {
  return {LookupMessage(MSI_DR_BLOOD_HOWLING_OFF), 0xFF};
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
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("hunger_target", 0x3375B1C7DBB5FD55ULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x00620BE0
// FUNCTION: fn_04479
void CSkillAction_DR_HUNGER::OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("hunger", 0x1D6D75EDE7CD311FULL, effectName)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    source->LaunchEffect(EF_BEGINSPELL4, level / 16);
  }
}

// OFFSET: 0x006212C0
// FUNCTION: fn_04486
EfstActionMessage CEfstAction_ENRAGE_RAPTOR::GetEfstBeginMessage() {
  return {LookupMessage(MSI_DR_ENRAGE_RAPTOR_ON), 0xFFFF00};
}

// OFFSET: 0x006212F0
// FUNCTION: fn_04487
EfstActionMessage CEfstAction_ENRAGE_RAPTOR::GetEfstEndMessage() {
  return {LookupMessage(MSI_DR_ENRAGE_RAPTOR_OFF), 0xFF};
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
    source->LaunchEffect(EF_BEGINSPELL4, level / 16);
  }
}

// OFFSET: 0x00621570
// FUNCTION: fn_04492
EfstActionMessage CEfstAction_PREENING::GetEfstBeginMessage() {
  return {LookupMessage(MSI_DR_PREENING_ON), 0xFFFF00};
}

// OFFSET: 0x006215A0
// FUNCTION: fn_04493
EfstActionMessage CEfstAction_PREENING::GetEfstEndMessage() {
  return {LookupMessage(MSI_DR_PREENING_OFF), 0xFF};
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

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT("flicking_tonado", 0x2FF3C5036D070127ULL)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("flicking_tonado_bottom", 0x17359BF953C367B3ULL, bottomName)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    &source->render.position.x, &target->render.position.x, g_modeMgr.GetGameMode()->camera->frame.view.values
  );
  const int direction = this->GetDirection(angle);
  if (!this->IsValidDirection(direction)) {
    return;
  }

  const float scale = source->attackMotion * source->render.speed * 16.0f;
  const bool flag = direction != 1 && direction != 2;
  const bool horizontal = (direction & 1) != 0;

  const std::string effectName(
    horizontal ? std::string(OBFUSCATE_TEXT_NAMED("low_flight_horizon", 0x8B65CD61012B5931ULL, horizonName)) : std::string(OBFUSCATE_TEXT_NAMED("low_flight_vertical", 0xF7530137154D8FE5ULL, verticalName))
  );

  EffectLists* effects = g_effectMgr;
  float angleOffset = 0.0f;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetDelay(static_cast<int>(scale));
    if (horizontal) {
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
    const auto rotated = [&angleOffset](const float& value) { return (static_cast<int>(value + angleOffset) + 360) % 360; };
    const int rotation = rotated(angle);
    effect->SetAngle(static_cast<float>(rotation));
  }

  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("low_flight_bottom", 0x69FD1D9733D3CFC1ULL, bottomName)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
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
    source->LaunchEffect(EF_BEGINSPELL, level / 16);
  }
}

// OFFSET: 0x00622240
// FUNCTION: fn_04504
void CSkillAction_DR_NATURE_SHIELD::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("nature_shield", 0xF5657D1F3DE5CDC7ULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x00622440
// FUNCTION: fn_04505
EfstActionMessage CEfstAction_NATURE_SHIELD::GetEfstBeginMessage() {
  return {LookupMessage(MSI_DR_NATURE_SHIELD_ON), 0xFFFF00};
}

// OFFSET: 0x00622470
// FUNCTION: fn_04506
EfstActionMessage CEfstAction_NATURE_SHIELD::GetEfstEndMessage() {
  return {LookupMessage(MSI_DR_NATURE_SHIELD_OFF), 0xFF};
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
    return OBFUSCATE_TEXT_NAMED("full", 0x078333BB9725B7C9ULL, fullName);
  };
  std::string suffix = step == 11 ? std::string(full()) : std::format("{:02}", step);

  return CEZ2STREffectMgr::GetInstance()->EffectId(std::string(OBFUSCATE_TEXT_NAMED("ground_grow_", 0x8F1B5BFD5F6175B5ULL, prefixName)) + suffix);
}

// OFFSET: 0x00622920
// FUNCTION: fn_04509
EfstActionMessage CEfstAction_NATURE_PROTECTION::GetEfstBeginMessage() {
  return {LookupMessage(MSI_KR_NATURE_PROTECTION_ON), 0xFFFF00};
}

// OFFSET: 0x00622950
// FUNCTION: fn_04510
EfstActionMessage CEfstAction_NATURE_PROTECTION::GetEfstEndMessage() {
  return {LookupMessage(MSI_KR_NATURE_PROTECTION_OFF), 0xFF};
}

// OFFSET: 0x00622980
// FUNCTION: fn_04511
int CEfstAction_NATURE_PROTECTION::GetEffectId(int efstId, int argument4) {
  return CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_ARRAY_TEXT("nature_protection", 0x8B3FEB5B8BD5C3C3ULL, char));
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
  if (step >= 0 && step < 6) {
    std::string suffix = std::format("{:02}", step);
    return CEZ2STREffectMgr::GetInstance()->EffectId(std::string(OBFUSCATE_TEXT_NAMED("thundering_rod_", 0x0BEB63A5C905B3A7ULL, prefixName)) + suffix);
  }

  return -1;
}

// OFFSET: 0x00622DD0
// FUNCTION: fn_04516
EfstActionMessage CEfstAction_THUNDERING_ROD_MAX::GetEfstBeginMessage() {
  return {LookupMessage(MSI_THUNDERING_ROD_MAX_ON), 0xFFFF00};
}

// OFFSET: 0x00622E00
// FUNCTION: fn_04517
EfstActionMessage CEfstAction_THUNDERING_ROD_MAX::GetEfstEndMessage() {
  return {LookupMessage(MSI_THUNDERING_ROD_MAX_OFF), 0xFF};
}

// OFFSET: 0x00622E30
// FUNCTION: fn_04518
int CEfstAction_THUNDERING_ROD_MAX::GetEffectId(int efstId, int argument4) {
  return CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_ARRAY_TEXT("thundering_rod_full", 0x9B17C9EFDBB39F2FULL, char));
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
    source->LaunchEffect(EF_BEGINSPELL5, level / 16);
  }
}

// OFFSET: 0x00623020
// FUNCTION: fn_04522
void CSkillAction_DR_TRUTH_OF_EARTH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("truth_of_earth", 0xC7912587FFF31B71ULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("truth_of_earth_bottom", 0x69D345DB950F336DULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x006233D0
// FUNCTION: fn_04523
EfstActionMessage CEfstAction_TRUTH_OF_EARTH::GetEfstBeginMessage() {
  return {LookupMessage(MSI_DR_TRUTH_OF_EARTH_ON), 0xFFFF00};
}

// OFFSET: 0x00623400
// FUNCTION: fn_04524
EfstActionMessage CEfstAction_TRUTH_OF_EARTH::GetEfstEndMessage() {
  return {LookupMessage(MSI_DR_TRUTH_OF_EARTH_OFF), 0xFF};
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
    source->LaunchEffect(EF_BEGINSPELL2, level / 16);
  }
}

// OFFSET: 0x00623480
// FUNCTION: fn_04528
void CSkillAction_DR_TRUTH_OF_ICE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("truth_of_ice", 0x2D394DBD0F332733ULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("truth_of_ice_bottom", 0x151F77EBEBBB2147ULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x00623820
// FUNCTION: fn_04529
EfstActionMessage CEfstAction_TRUTH_OF_ICE::GetEfstBeginMessage() {
  return {LookupMessage(MSI_DR_TRUTH_OF_ICE_ON), 0xFFFF00};
}

// OFFSET: 0x00623850
// FUNCTION: fn_04530
EfstActionMessage CEfstAction_TRUTH_OF_ICE::GetEfstEndMessage() {
  return {LookupMessage(MSI_DR_TRUTH_OF_ICE_OFF), 0xFF};
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
    source->LaunchEffect(EF_BEGINSPELL4, level / 16);
  }
}

// OFFSET: 0x006238D0
// FUNCTION: fn_04534
void CSkillAction_DR_TRUTH_OF_WIND::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("truth_of_wind", 0x61FF87ED7BF39BE1ULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("truth_of_wind_bottom", 0x7D993F650D2785FFULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x00623C70
// FUNCTION: fn_04535
EfstActionMessage CEfstAction_TRUTH_OF_WIND::GetEfstBeginMessage() {
  return {LookupMessage(MSI_DR_TRUTH_OF_WIND_ON), 0xFFFF00};
}

// OFFSET: 0x00623CA0
// FUNCTION: fn_04536
EfstActionMessage CEfstAction_TRUTH_OF_WIND::GetEfstEndMessage() {
  return {LookupMessage(MSI_DR_TRUTH_OF_WIND_OFF), 0xFF};
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
    source->LaunchEffect(EF_BEGINSPELL4, level / 16);
  }
}

// OFFSET: 0x00623D20
// FUNCTION: fn_04540
void CSkillAction_DR_CUTTING_WIND::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target || !source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("cutting_wind", 0x4F1791FB1503C713ULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(&target->render.position.x, &source->render.position.x, g_modeMgr.GetGameMode()->camera->frame.view.values);
    effect->SetAngle(angle);
  }
}

// OFFSET: 0x00623F70
// FUNCTION: fn_04541
void CSkillAction_DR_CUTTING_WIND::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (!target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("cutting_wind_hit", 0xA1A55B4D1597EDD3ULL, effectName)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    source->LaunchEffect(EF_BEGINSPELL4, level / 16);
  }
}

// OFFSET: 0x006241C0
// FUNCTION: fn_04545
void CSkillAction_DR_WIND_BOMB::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, EF_UNK_2468, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    static_cast<CBezierArcThrowEffect*>(effect)->SetTarget(target->render.position);
  }

  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("wind_bomb_main", 0xD1538513DF975701ULL, effectName)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetDelay(300);
  }
}

// OFFSET: 0x00624430
// FUNCTION: fn_04546
void CSkillAction_DR_WIND_BOMB::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (!target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("wind_bomb_hit", 0x91E147EBBD47C39FULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x00624640
// FUNCTION: fn_04547
void CSkillAction_DR_AROUND_FLOWER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("around_flower", 0x6DED3DD1CF75AD9BULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("around_flower_bottom", 0x49C3759B77DBD3C1ULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x006249E0
// FUNCTION: fn_04548
void CSkillAction_DR_EARTH_FLOWER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("earth_flower", 0x1BF915E1F37DFDFDULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("earth_flower_bottom", 0x01FF9313695DD9DFULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x00624D80
// FUNCTION: fn_04549
void CSkillAction_DR_ICE_CLOUD::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("ice_cloud", 0x474DC581D5E98357ULL, effectName)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x00624F80
// FUNCTION: fn_04550
void CSkillAction_DR_ICE_TOTEM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("ice_totem", 0x3F557BE333F95F6DULL, effectName)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("werewolf", 0x490FFB43A12F0DB9ULL, effectName)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x006253A0
// FUNCTION: fn_04553
EfstActionMessage CEfstAction_WEREWOLF::GetEfstBeginMessage() {
  return {LookupMessage(MSI_DR_WEREWOLF_ON), 0xFFFF00};
}

// OFFSET: 0x006253D0
// FUNCTION: fn_04554
EfstActionMessage CEfstAction_WEREWOLF::GetEfstEndMessage() {
  return {LookupMessage(MSI_DR_WEREWOLF_OFF), 0xFF};
}

// OFFSET: 0x00620270
// FUNCTION: fn_04466
void CSkillAction_DR_ENRAGE_WOLF::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("enrage_wolf", 0x31D7C1EDA345E36DULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x00620530
// FUNCTION: fn_04472
void CSkillAction_DR_BLOOD_HOWLING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("blood_howling", 0xD98F03A52D35636DULL, effectName)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x006207B0
// FUNCTION: fn_04476
void CSkillAction_DR_NOMERCY_CLAW::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("nomercy_claw", 0x0F0F8B6B8F9D19D1ULL, effectName)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x006210B0
// FUNCTION: fn_04485
void CSkillAction_DR_ENRAGE_RAPTOR::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("enrage_raptor", 0x812BE72BCD5707B7ULL, effectName)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x00621370
// FUNCTION: fn_04491
void CSkillAction_DR_PREENING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("preening", 0xC52FB5FBB94FF917ULL, effectName)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x00620DF0
// FUNCTION: fn_04481
void CSkillAction_DR_CRUEL_BITE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target || !this->CanUse(source)) {
    return;
  }

  if (CRagEffect* effect = source->LaunchEffect(EF_CRUEL_BITE_LINE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("cruel_bite", 0x59278F63D79F3DA1ULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x006215E0
// FUNCTION: fn_04495
void CSkillAction_DR_SHOOTING_FEATHER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target || !this->CanUse(source)) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_TEXT_NAMED("shooting_feather", 0x93E7A3A1F587CDC5ULL, name)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, g_modeMgr.GetGameMode()->camera->frame.view.values);
    effect->SetAngle(angle);
  }
}

// OFFSET: 0x00621830
// FUNCTION: fn_04496
void CSkillAction_DR_SHOOTING_FEATHER::OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) {
  this->SpawnAttack00Effect(source, target);
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

// GLOBAL: gl_25684
static EfstActionRegistrar s_efstActionBloodHowling{EFST_BLOOD_HOWLING, NewAction<CEfstAction_BLOOD_HOWLING, CEfstAction>};
// GLOBAL: gl_25685
static EfstActionRegistrar s_efstActionEnrageRaptor{EFST_ENRAGE_RAPTOR, NewAction<CEfstAction_ENRAGE_RAPTOR, CEfstAction>};
// GLOBAL: gl_25686
static EfstActionRegistrar s_efstActionEnrageWolf{EFST_ENRAGE_WOLF, NewAction<CEfstAction_ENRAGE_WOLF, CEfstAction>};
// GLOBAL: gl_25687
static EfstActionRegistrar s_efstActionGroundGrow{EFST_GROUND_GROW, NewAction<CEfstAction_GROUND_GROW, CEfstAction>};
// GLOBAL: gl_25688
static EfstActionRegistrar s_efstActionNatureProtection{EFST_NATURE_PROTECTION, NewAction<CEfstAction_NATURE_PROTECTION, CEfstAction>};
// GLOBAL: gl_25689
static EfstActionRegistrar s_efstActionNatureShield{EFST_NATURE_SHIELD, NewAction<CEfstAction_NATURE_SHIELD, CEfstAction>};
// GLOBAL: gl_25690
static EfstActionRegistrar s_efstActionPreening{EFST_PREENING, NewAction<CEfstAction_PREENING, CEfstAction>};
// GLOBAL: gl_25691
static EfstActionRegistrar s_efstActionThunderingRod{EFST_THUNDERING_ROD, NewAction<CEfstAction_THUNDERING_ROD, CEfstAction>};
// GLOBAL: gl_25692
static EfstActionRegistrar s_efstActionThunderingRodMax{EFST_THUNDERING_ROD_MAX, NewAction<CEfstAction_THUNDERING_ROD_MAX, CEfstAction>};
// GLOBAL: gl_25693
static EfstActionRegistrar s_efstActionTruthOfEarth{EFST_TRUTH_OF_EARTH, NewAction<CEfstAction_TRUTH_OF_EARTH, CEfstAction>};
// GLOBAL: gl_25694
static EfstActionRegistrar s_efstActionTruthOfIce{EFST_TRUTH_OF_ICE, NewAction<CEfstAction_TRUTH_OF_ICE, CEfstAction>};
// GLOBAL: gl_25695
static EfstActionRegistrar s_efstActionTruthOfWind{EFST_TRUTH_OF_WIND, NewAction<CEfstAction_TRUTH_OF_WIND, CEfstAction>};
// GLOBAL: gl_25696
static EfstActionRegistrar s_efstActionWereraptor{EFST_WERERAPTOR, NewAction<CEfstAction_WERERAPTOR, CEfstAction>};
// GLOBAL: gl_25697
static EfstActionRegistrar s_efstActionWerewolf{EFST_WEREWOLF, NewAction<CEfstAction_WEREWOLF, CEfstAction>};
// GLOBAL: gl_25698
static SkillActionRegistrar s_skillActionDrAroundFlower{DR_AROUND_FLOWER, NewAction<CSkillAction_DR_AROUND_FLOWER, CSkillAction>};
// GLOBAL: gl_25699
static SkillActionRegistrar s_skillActionDrBloodHowling{DR_BLOOD_HOWLING, NewAction<CSkillAction_DR_BLOOD_HOWLING, CSkillAction>};
// GLOBAL: gl_25700
static SkillActionRegistrar s_skillActionDrCruelBite{DR_CRUEL_BITE, NewAction<CSkillAction_DR_CRUEL_BITE, CSkillAction>};
// GLOBAL: gl_25701
static SkillActionRegistrar s_skillActionDrCuttingWind{DR_CUTTING_WIND, NewAction<CSkillAction_DR_CUTTING_WIND, CSkillAction>};
// GLOBAL: gl_25702
static SkillActionRegistrar s_skillActionDrEarthFlower{DR_EARTH_FLOWER, NewAction<CSkillAction_DR_EARTH_FLOWER, CSkillAction>};
// GLOBAL: gl_25703
static SkillActionRegistrar s_skillActionDrEnrageRaptor{DR_ENRAGE_RAPTOR, NewAction<CSkillAction_DR_ENRAGE_RAPTOR, CSkillAction>};
// GLOBAL: gl_25704
static SkillActionRegistrar s_skillActionDrEnrageWolf{DR_ENRAGE_WOLF, NewAction<CSkillAction_DR_ENRAGE_WOLF, CSkillAction>};
// GLOBAL: gl_25705
static SkillActionRegistrar s_skillActionDrFlickingTonado{DR_FLICKING_TONADO, NewAction<CSkillAction_DR_FLICKING_TONADO, CSkillAction>};
// GLOBAL: gl_25706
static SkillActionRegistrar s_skillActionDrHunger{DR_HUNGER, NewAction<CSkillAction_DR_HUNGER, CSkillAction>};
// GLOBAL: gl_25707
static SkillActionRegistrar s_skillActionDrIceCloud{DR_ICE_CLOUD, NewAction<CSkillAction_DR_ICE_CLOUD, CSkillAction>};
// GLOBAL: gl_25708
static SkillActionRegistrar s_skillActionDrIceTotem{DR_ICE_TOTEM, NewAction<CSkillAction_DR_ICE_TOTEM, CSkillAction>};
// GLOBAL: gl_25709
static SkillActionRegistrar s_skillActionDrLowFlight{DR_LOW_FLIGHT, NewAction<CSkillAction_DR_LOW_FLIGHT, CSkillAction>};
// GLOBAL: gl_25710
static SkillActionRegistrar s_skillActionDrNatureShield{DR_NATURE_SHIELD, NewAction<CSkillAction_DR_NATURE_SHIELD, CSkillAction>};
// GLOBAL: gl_25711
static SkillActionRegistrar s_skillActionDrNomercyClaw{DR_NOMERCY_CLAW, NewAction<CSkillAction_DR_NOMERCY_CLAW, CSkillAction>};
// GLOBAL: gl_25712
static SkillActionRegistrar s_skillActionDrPreening{DR_PREENING, NewAction<CSkillAction_DR_PREENING, CSkillAction>};
// GLOBAL: gl_25713
static SkillActionRegistrar s_skillActionDrShootingFeather{DR_SHOOTING_FEATHER, NewAction<CSkillAction_DR_SHOOTING_FEATHER, CSkillAction>};
// GLOBAL: gl_25714
static SkillActionRegistrar s_skillActionDrTruthOfEarth{DR_TRUTH_OF_EARTH, NewAction<CSkillAction_DR_TRUTH_OF_EARTH, CSkillAction>};
// GLOBAL: gl_25715
static SkillActionRegistrar s_skillActionDrTruthOfIce{DR_TRUTH_OF_ICE, NewAction<CSkillAction_DR_TRUTH_OF_ICE, CSkillAction>};
// GLOBAL: gl_25716
static SkillActionRegistrar s_skillActionDrTruthOfWind{DR_TRUTH_OF_WIND, NewAction<CSkillAction_DR_TRUTH_OF_WIND, CSkillAction>};
// GLOBAL: gl_25717
static SkillActionRegistrar s_skillActionDrWereraptor{DR_WERERAPTOR, NewAction<CSkillAction_DR_WERERAPTOR, CSkillAction>};
// GLOBAL: gl_25718
static SkillActionRegistrar s_skillActionDrWerewolf{DR_WEREWOLF, NewAction<CSkillAction_DR_WEREWOLF, CSkillAction>};
// GLOBAL: gl_25719
static SkillActionRegistrar s_skillActionDrWindBomb{DR_WIND_BOMB, NewAction<CSkillAction_DR_WIND_BOMB, CSkillAction>};
