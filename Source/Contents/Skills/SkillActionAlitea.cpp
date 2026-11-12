#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session/Session.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
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

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <string_view>

class CSkillAction_AnimalForm : public CSkillAction {
public:
  __declspec(noinline) bool CanUse(CGameActor* actor);
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
  __declspec(noinline) void SpawnAttack01Effect(int level, CGameActor* source);
  __declspec(noinline) void SpawnAttack02Effect(int level, CGameActor* source);
  void SpawnDirectionalEffect(CGameActor* source, CGameActor* target, std::string_view name);
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
  void SpawnIcing(CGameActor* target);
};

class CSkillAction_Thundering : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
  virtual void PlayThunderEffect(CGameActor* source) = 0;
};

class CSkillAction_ThunderingBasic : public CSkillAction_Thundering {
public:
  void PlayThunderEffect(CGameActor* source) override;
};

class CSkillAction_ThunderingExpand : public CSkillAction_Thundering {
public:
  void PlayThunderEffect(CGameActor* source) override;
};

class CEfstAction_BasicAction : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  virtual int GetEffectId(int efstId, int argument4) = 0;
};

class CEfstAction_AnimalForm : public CEfstAction {
public:
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void ResetAnimalForm(CRenderObject* actor);
  void ApplyAnimalForm(CRenderObject* actor, int form);
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_AT_ALPHA_CLAW : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_PULSE_OF_MADNESS : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_PULSE_OF_MADNESS : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_AT_PRIMAL_CLAW : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_FERAL_CLAW : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_FRENZY_FANG : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_PINION_SHOT : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_QUILL_SPEAR : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_QUILL_SPEAR_S : public CSkillAction_DamageWereRaptorForm {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_NATURE_HARMONY : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_NATURE_HARMONY : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_AT_GRAVITY_HOLE : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_CHILLING_BLAST : public CSkillAction_Icing {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_GLACIER_MONOLITH : public CSkillAction_Icing {
public:
  int GetSkillAttackMotion() override;
  int GetSkillScopeType() override;
};

class CSkillObjectAction_AT_GLACIER_MONOLITH : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
  void OnSkillObjectRemove(CRenderObject* object) override;
};

class CSkillAction_AT_GLACIER_NOVA : public CSkillAction_Icing {
public:
  int GetSkillAttackMotion() override;
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_AT_GLACIER_SHARD : public CSkillAction_Icing {
public:
  void OnGroundSkillCast(int level, CGameActor* source, int argument3, int argument4, int argument5) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_GLACIER_STOMP : public CSkillAction_Icing {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_ROARING_CHARGE : public CSkillAction_ThunderingBasic {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_ROARING_CHARGE_S : public CSkillAction_ThunderingExpand {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_ROARING_PIERCER : public CSkillAction_ThunderingBasic {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_ROARING_PIERCER_S : public CSkillAction_ThunderingExpand {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_FURIOS_STORM : public CSkillAction_Thundering {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void PlayThunderEffect(CGameActor* source) override;
};

class CEfstAction_FURIOS_STORM : public CEfstAction {
public:
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_AT_ALPHA_PHASE : public CSkillAction {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_ALPHA_PHASE : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_AT_APEX_PHASE : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_AT_APEX_PHASE : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_AT_FLIP_FLAP : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_FLIP_FLAP : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_AT_AERO_SYNC : public CSkillAction {
public:
  bool IsSupport() override;
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_FLIP_FLAP_TARGET : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_AT_SAVAGE_LUNGE : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_TEMPEST_FLAP : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_TERRA_HARVEST : public CSkillAction_Earthing {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_TERRA_WAVE : public CSkillAction_Earthing {
public:
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_AT_SOLID_STOMP : public CSkillAction_Earthing {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_SOLID_STOMP : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_AT_ZEPHYR_LINK : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
};

class CEfstAction_ZEPHYR_LINK : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_INSANE : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_INSANE2 : public CEfstAction_INSANE {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
};

class CEfstAction_INSANE3 : public CEfstAction_INSANE {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
};

// OFFSET: 0x005A50B0
// FUNCTION: fn_03155
void CSkillAction_DamageWereRaptorForm::PlayFormAttackEffect(CGameActor* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 18> bytes = CRYPT_NAME("dr_hit_wereraptor", 0x1D6D75EDE7CD311FULL);
      thread_local EncryptedName<sizeof(bytes), 0x1D6D75EDE7CD311FULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A52B0
// FUNCTION: fn_03156
void CSkillAction_DamageWereWolfForm::PlayFormAttackEffect(CGameActor* actor) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 16> bytes = CRYPT_NAME("dr_hit_werewolf", 0x3375B1C7DBB5FD55ULL);
      thread_local EncryptedName<sizeof(bytes), 0x3375B1C7DBB5FD55ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A54B0
// FUNCTION: fn_03157
void CSkillAction_Icing::SpawnIcing(CGameActor* target) {
  if (target) {
    if (CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
          constexpr auto bytesText = CRYPT_NAME("ice_hit", 0x91651D81F559F971ULL);
          const auto& bytes = bytesText.data;
          thread_local EncryptedName<sizeof(bytes), 0x91651D81F559F971ULL> name(bytes);
          return name.Text();
        }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetScale(0.9f);
    }
  }
}

// OFFSET: 0x005A56C0
// FUNCTION: fn_03158
void CSkillAction_ThunderingBasic::PlayThunderEffect(CGameActor* source) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 20> bytes = CRYPT_NAME("thundering_attack_g", 0x3FAF85D995034FD7ULL);
      thread_local EncryptedName<sizeof(bytes), 0x3FAF85D995034FD7ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A58C0
// FUNCTION: fn_03159
void CSkillAction_ThunderingExpand::PlayThunderEffect(CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<std::uint8_t, 20> bytesArray = CRYPT_NAME("thundering_attack_y", 0x490FFB43A12F0DB9ULL);
      const std::uint8_t (&bytes)[20] = bytesArray._Elems;
      thread_local EncryptedName<sizeof(bytes), 0x490FFB43A12F0DB9ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A5AC0
// FUNCTION: fn_03160
void CSkillAction_DamageWereRaptorForm::SpawnDirectionalEffect(CGameActor* source, CGameActor* target, std::string_view name) {
  const float angle = ScreenAngle(
    &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
  );
  const int direction = this->GetDirection(angle);
  if (!this->IsValidDirection(direction)) {
    return;
  }

  const bool flipped = direction == 1 || direction == 2;
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(name.data()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetFlipX(flipped);
    effect->SetAngle(static_cast<float>((static_cast<int>(angle + (flipped ? -180.0f : 0.0f)) + 360) % 360));
  }
}

// OFFSET: 0x005A5D20
// FUNCTION: fn_03161
int CSkillAction_Earthing::GetSkillCastMotion() {
  return 60;
}

// OFFSET: 0x005A5D30
// FUNCTION: fn_03162
int CSkillAction_Icing::GetSkillCastMotion() {
  return 61;
}

// OFFSET: 0x005A5D40
// FUNCTION: fn_03163
int CSkillAction_Thundering::GetSkillCastMotion() {
  return 61;
}

// OFFSET: 0x005A5D50
// FUNCTION: fn_03164
int CSkillAction_Direction::GetDirection(float angle) {
  if (angle > 60.0f && angle <= 120.0f) {
    return 0;
  }
  if (angle > 120.0f && angle <= 240.0f) {
    return 1;
  }
  if (angle > 240.0f && angle <= 300.0f) {
    return 2;
  }
  if ((angle > 300.0f && angle <= 360.0f) || (angle >= 0.0f && angle <= 60.0f)) {
    return 3;
  }
  return -1;
}

// OFFSET: 0x005A5DE0
// FUNCTION: fn_03165
int CSkillAction_Earthing::GetSkillAttackMotion() {
  return 62;
}

// OFFSET: 0x005A5DF0
// FUNCTION: fn_03166
int CSkillAction_Icing::GetSkillAttackMotion() {
  return 64;
}

// OFFSET: 0x005A5E00
// FUNCTION: fn_03167
int CSkillAction_Thundering::GetSkillAttackMotion() {
  return 64;
}

// OFFSET: 0x005A5E10
// FUNCTION: fn_03168
bool CSkillAction_DamageWereRaptorForm::IsInForm(CGameActor* actor) {
  return actor->GetShapeShift() == 2;
}

// OFFSET: 0x005A5E30
// FUNCTION: fn_03169
bool CSkillAction_DamageWereWolfForm::IsInForm(CGameActor* actor) {
  return actor->GetShapeShift() == 1;
}

// OFFSET: 0x005A5E90
// FUNCTION: fn_03171
bool CSkillAction_Direction::IsValidDirection(unsigned int direction) {
  return direction <= 3;
}

// OFFSET: 0x005A5EA0
// FUNCTION: fn_03172
void CSkillAction_DamageWereRaptorForm::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  this->PlayFormAttackEffect(source);
}

// OFFSET: 0x005A5EB0
// FUNCTION: fn_03173
void CSkillAction_DamageWereWolfForm::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  this->PlayFormAttackEffect(source);
}

// OFFSET: 0x005A5EC0
// FUNCTION: fn_03174
void CSkillAction_Earthing::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  if (CRagEffect* effect = source->LaunchEffect(0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&source->render.position), 0, 0, 0, 0);
  }
}

// OFFSET: 0x005A5F30
// FUNCTION: fn_03175
void CSkillAction_Icing::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  this->SpawnIcing(source);
}

// OFFSET: 0x005A5F40
// FUNCTION: fn_03176
void CSkillAction_Thundering::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  this->PlayThunderEffect(source);
}

// OFFSET: 0x005A5F50
// FUNCTION: fn_03177
void CEfstAction_BasicAction::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    const int id = this->GetEffectId(efstId, argument4);
    if (id >= 0) {
      g_effectMgr->Spawn(actor->render.accountId, id, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    }
  }
}

// OFFSET: 0x005A5FB0
// FUNCTION: fn_03178
void CSkillAction_Earthing::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(57, level / 16);
  }
}

// OFFSET: 0x005A5FE0
// FUNCTION: fn_03179
void CSkillAction_Icing::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ice_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect =
            effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ice_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x005A6260
// FUNCTION: fn_03180
void CSkillAction_Thundering::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
          constexpr std::array<char, 16> bytes = CRYPT_NAME("thundering_cast", 0x93E7A3A1F587CDC5ULL);
          thread_local EncryptedName<sizeof(bytes), 0x93E7A3A1F587CDC5ULL> name(bytes);
          return name.Text();
        }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x005A6470
// FUNCTION: fn_03181
void CEfstAction_AnimalForm::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  this->ResetAnimalForm(actor);
}

// OFFSET: 0x005A6480
// FUNCTION: fn_03182
void CEfstAction_BasicAction::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    const int id = this->GetEffectId(efstId, argument4);
    if (id >= 0) {
      g_effectMgr->Retire(actor->render.accountId, id);
    }
  }
}

// OFFSET: 0x005A64C0
// FUNCTION: fn_03183
void CEfstAction_AnimalForm::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x005A64E0
// FUNCTION: fn_03184
void CEfstAction_BasicAction::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    this->OnEfstBegin(efstId, value, argument3, argument4, actor, false);
  }
}

// OFFSET: 0x005A6510
// FUNCTION: fn_03185
void CEfstAction_AnimalForm::ResetAnimalForm(CRenderObject* actor) {
  if (!actor) {
    return;
  }

  CGameActor* gameActor = static_cast<CGameActor*>(actor);
  if (g_session.m_aid == actor->render.accountId) {
    g_session.SetPlayerSkillMode(0);
  }
  gameActor->SetShapeShift(0);
  gameActor->SetSprJob(gameActor->GetShapeShiftJob());
  g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("return", 0x812BE72BCD5707B7ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x812BE72BCD5707B7ULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  gameActor->SetBodyPaletteName();
}

// OFFSET: 0x005A6740
// FUNCTION: fn_03186
void CEfstAction_AnimalForm::ApplyAnimalForm(CRenderObject* actor, int form) {
  if (actor) {
    CGameActor* gameActor = static_cast<CGameActor*>(actor);
    if (g_session.m_aid == actor->render.accountId) {
      g_session.SetPlayerSkillMode(form);
    }

    gameActor->SetShapeShift(form);
    gameActor->SetSprJob(gameActor->GetShapeShiftJob());
  }
}

// OFFSET: 0x005A6790
// FUNCTION: fn_03187
int CSkillAction_AT_ALPHA_CLAW::GetSkillAttackMotion() {
  return 76;
}

// OFFSET: 0x005A67A0
// FUNCTION: fn_03188
void CSkillAction_AT_ALPHA_CLAW::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source) || !target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("alpha_claw", 0x1D6D75EDE7CD311FULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x1D6D75EDE7CD311FULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x005A69B0
// FUNCTION: fn_03189
int CSkillAction_AT_PULSE_OF_MADNESS::GetSkillCastMotion() {
  return 66;
}

// OFFSET: 0x005A69C0
// FUNCTION: fn_03190
int CSkillAction_AT_PULSE_OF_MADNESS::GetSkillAttackMotion() {
  return 68;
}

// OFFSET: 0x005A69D0
// FUNCTION: fn_03191
void CSkillAction_AT_PULSE_OF_MADNESS::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x005A6A00
// FUNCTION: fn_03192
void CSkillAction_AT_PULSE_OF_MADNESS::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  this->CanUse(source);

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<std::uint8_t, 17> bytesArray = CRYPT_NAME("pulse_of_madness", 0x2FF3C5036D070127ULL);
    const std::uint8_t (&bytes)[17] = bytesArray._Elems;
    thread_local EncryptedName<sizeof(bytes), 0x2FF3C5036D070127ULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x005A6C00
// FUNCTION: fn_03193
EfstActionMessage CEfstAction_PULSE_OF_MADNESS::GetEfstBeginMessage() {
  return {LookupMessage(4668), 0xFFFF00};
}

// OFFSET: 0x005A6C30
// FUNCTION: fn_03194
EfstActionMessage CEfstAction_PULSE_OF_MADNESS::GetEfstEndMessage() {
  return {LookupMessage(4669), 0xFF};
}

// OFFSET: 0x005A6C60
// FUNCTION: fn_03195
int CSkillAction_AT_FRENZY_FANG::GetSkillAttackMotion() {
  return 70;
}

// OFFSET: 0x005A6C70
// FUNCTION: fn_03196
void CSkillAction_AT_FRENZY_FANG::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  if (!target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        constexpr auto bytesText = CRYPT_NAME("frenzy_fang", 0x17359BF953C367B3ULL);
        const auto& bytes = bytesText.data;
        thread_local EncryptedName<sizeof(bytes), 0x17359BF953C367B3ULL> name(bytes);
        return name.Text();
      }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(
      &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
    );
    effect->SetFlipX(angle < 90.0f || angle > 270.0f);
  }
}

// OFFSET: 0x005A6EE0
// FUNCTION: fn_03197
int CSkillAction_AT_PINION_SHOT::GetSkillCastMotion() {
  return 60;
}

// OFFSET: 0x005A6EF0
// FUNCTION: fn_03198
int CSkillAction_AT_PINION_SHOT::GetSkillAttackMotion() {
  return 78;
}

// OFFSET: 0x005A6F00
// FUNCTION: fn_03199
void CSkillAction_AT_PINION_SHOT::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x005A6F30
// FUNCTION: fn_03200
void CSkillAction_AT_PINION_SHOT::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  if (!this->CanUse(source)) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        constexpr auto bytesText = CRYPT_NAME("pinion_shot", 0xC52FB5FBB94FF917ULL);
        const auto& bytes = bytesText.data;
        thread_local EncryptedName<sizeof(bytes), 0xC52FB5FBB94FF917ULL> name(bytes);
        return name.Text();
      }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(
      &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
    );
    effect->SetAngle(angle);
  }
}

// OFFSET: 0x005A7180
// FUNCTION: fn_03201
void CSkillAction_AT_PINION_SHOT::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  this->SpawnAttack01Effect(level, source);
}

// OFFSET: 0x005A71A0
// FUNCTION: fn_03202
int CSkillAction_AT_QUILL_SPEAR::GetSkillCastMotion() {
  return 60;
}

// OFFSET: 0x005A71B0
// FUNCTION: fn_03203
int CSkillAction_AT_QUILL_SPEAR::GetSkillAttackMotion() {
  return 79;
}

// OFFSET: 0x005A71C0
// FUNCTION: fn_03204
void CSkillAction_AT_QUILL_SPEAR::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x005A71F0
// FUNCTION: fn_03205
void CSkillAction_AT_QUILL_SPEAR::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target || !this->CanUse(source)) {
    return;
  }

  const int effectId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 16> bytes = CRYPT_NAME("quill_spear_3x7", 0x93E7A3A1F587CDC5ULL);
    thread_local EncryptedName<sizeof(bytes), 0x93E7A3A1F587CDC5ULL> name(bytes);
    return name.Text();
  }());
  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(
      &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
    );
    effect->SetAngle(angle);
    if (CRagEffect* ragEffect = source->LaunchEffect(2442, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
      ragEffect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x005A74A0
// FUNCTION: fn_03206
void CSkillAction_AT_QUILL_SPEAR_S::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target || !this->CanUse(source)) {
    return;
  }

  const int effectId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 16> bytes = CRYPT_NAME("quill_spear_5x7", 0x3FAF85D995034FD7ULL);
    thread_local EncryptedName<sizeof(bytes), 0x3FAF85D995034FD7ULL> name(bytes);
    return name.Text();
  }());
  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(
      &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
    );
    effect->SetAngle(angle);
    if (CRagEffect* ragEffect = source->LaunchEffect(2442, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
      ragEffect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x005A7750
// FUNCTION: fn_03207
int CSkillAction_AT_NATURE_HARMONY::GetSkillCastMotion() {
  return 61;
}

// OFFSET: 0x005A7760
// FUNCTION: fn_03208
int CSkillAction_AT_NATURE_HARMONY::GetSkillAttackMotion() {
  return 64;
}

// OFFSET: 0x005A7770
// FUNCTION: fn_03209
void CSkillAction_AT_NATURE_HARMONY::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x005A77A0
// FUNCTION: fn_03210
void CSkillAction_AT_NATURE_HARMONY::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("nature_harmony", 0x490FFB43A12F0DB9ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x490FFB43A12F0DB9ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A79B0
// FUNCTION: fn_03211
EfstActionMessage CEfstAction_NATURE_HARMONY::GetEfstBeginMessage() {
  return {LookupMessage(4684), 0xFFFF00};
}

// OFFSET: 0x005A79E0
// FUNCTION: fn_03212
EfstActionMessage CEfstAction_NATURE_HARMONY::GetEfstEndMessage() {
  return {LookupMessage(4685), 0xFF};
}

// OFFSET: 0x005A7A10
// FUNCTION: fn_03213
int CSkillAction_AT_GRAVITY_HOLE::GetSkillCastMotion() {
  return 61;
}

// OFFSET: 0x005A7A20
// FUNCTION: fn_03214
int CSkillAction_AT_GRAVITY_HOLE::GetSkillAttackMotion() {
  return 64;
}

// OFFSET: 0x005A7A30
// FUNCTION: fn_03215
void CSkillAction_AT_GRAVITY_HOLE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x005A7A60
// FUNCTION: fn_03216
void CSkillAction_AT_GRAVITY_HOLE::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    if (CRagEffect* effect = source->LaunchEffect(0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
      effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&source->render.position), 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x005A7AD0
// FUNCTION: fn_03217
void CSkillAction_AT_GRAVITY_HOLE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 13> bytes = CRYPT_NAME("gravity_hole", 0x91651D81F559F971ULL);
      thread_local EncryptedName<sizeof(bytes), 0x91651D81F559F971ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 20> bytes = CRYPT_NAME("gravity_hole_bottom", 0x73D1674B61DF7B0BULL);
      thread_local EncryptedName<sizeof(bytes), 0x73D1674B61DF7B0BULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A7E70
// FUNCTION: fn_03218
int CSkillAction_AT_CHILLING_BLAST::GetSkillAttackMotion() {
  return 65;
}

// OFFSET: 0x005A7E80
// FUNCTION: fn_03219
void CSkillAction_AT_CHILLING_BLAST::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("chilling_blast", 0xD98F03A52D35636DULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0xD98F03A52D35636DULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("chilling_blast_bottom", 0x0F0F8B6B8F9D19D1ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x0F0F8B6B8F9D19D1ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A8230
// FUNCTION: fn_03220
int CSkillAction_AT_GLACIER_MONOLITH::GetSkillScopeType() {
  return 9;
}

// OFFSET: 0x005A8240
// FUNCTION: fn_03221
int CSkillAction_AT_GLACIER_MONOLITH::GetSkillAttackMotion() {
  return 65;
}

// OFFSET: 0x005A8250
// FUNCTION: fn_03222
void CSkillObjectAction_AT_GLACIER_MONOLITH::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  if (!object) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 23> bytes = CRYPT_NAME("glacier_monolith_start", 0xD3755DD95F47A909ULL);
    thread_local EncryptedName<sizeof(bytes), 0xD3755DD95F47A909ULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 30> bytes = CRYPT_NAME("glacier_monolith_start_bottom", 0x31D7C1EDA345E36DULL);
    thread_local EncryptedName<sizeof(bytes), 0x31D7C1EDA345E36DULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        constexpr std::array<char, 22> bytes = CRYPT_NAME("glacier_monolith_loop", 0xC7912587FFF31B71ULL);
        thread_local EncryptedName<sizeof(bytes), 0xC7912587FFF31B71ULL> name(bytes);
        return name.Text();
      }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetDelay(990);
  }
  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        constexpr std::array<char, 29> bytes = CRYPT_NAME("glacier_monolith_loop_bottom", 0x69D345DB950F336DULL);
        thread_local EncryptedName<sizeof(bytes), 0x69D345DB950F336DULL> name(bytes);
        return name.Text();
      }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetDelay(990);
  }
}

// OFFSET: 0x005A8960
// FUNCTION: fn_03223
void CSkillObjectAction_AT_GLACIER_MONOLITH::OnSkillObjectRemove(CRenderObject* object) {
  if (!object) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const auto bytes = CRYPT_NAME("glacier_monolith_loop", 0x9B17C9EFDBB39F2FULL);
    thread_local EncryptedName<sizeof(bytes), 0x9B17C9EFDBB39F2FULL> name(bytes.data);
    return name.Text();
  }()));
  effects = g_effectMgr;
  effects->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = CRYPT_CHARS(29, "glacier_monolith_loop_bottom", 0x2D3B15774149D5DDULL);
    thread_local EncryptedName<sizeof(bytes), 0x2D3B15774149D5DDULL> name(bytes);
    return name.Text();
  }()));

  effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const auto bytes = CRYPT_NAME("glacier_monolith_end", 0x61FF87ED7BF39BE1ULL);
    thread_local EncryptedName<sizeof(bytes), 0x61FF87ED7BF39BE1ULL> name(bytes.data);
    return name.Text();
  }()), object->render.position, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const auto bytes = CRYPT_NAME("glacier_monolith_end_bottom", 0x7D993F650D2785FFULL);
    thread_local EncryptedName<sizeof(bytes), 0x7D993F650D2785FFULL> name(bytes.data);
    return name.Text();
  }()), object->render.position, 0, -1, 0, 0);
}

// OFFSET: 0x005A8FC0
// FUNCTION: fn_03224
int CSkillAction_AT_GLACIER_NOVA::GetSkillAttackMotion() {
  return 65;
}

// OFFSET: 0x005A8FD0
// FUNCTION: fn_03225
void CSkillAction_AT_GLACIER_NOVA::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("glacier_nova", 0x2D394DBD0F332733ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x2D394DBD0F332733ULL> name(bytes);
    return name.Text();
  }()), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005A91A0
// FUNCTION: fn_03226
void CSkillAction_AT_GLACIER_SHARD::OnGroundSkillCast(int level, CGameActor* source, int argument3, int argument4, int argument5) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
          constexpr std::array<char, 21> bytes = CRYPT_NAME("glacier_shard_target", 0x151F77EBEBBB2147ULL);
          thread_local EncryptedName<sizeof(bytes), 0x151F77EBEBBB2147ULL> name(bytes);
          return name.Text();
        }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x005A93B0
// FUNCTION: fn_03227
void CSkillAction_AT_GLACIER_SHARD::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("glacier_shard", 0x69FD1D9733D3CFC1ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x69FD1D9733D3CFC1ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A95C0
// FUNCTION: fn_03228
int CSkillAction_AT_GLACIER_STOMP::GetSkillAttackMotion() {
  return 65;
}

// OFFSET: 0x005A95D0
// FUNCTION: fn_03229
void CSkillAction_AT_GLACIER_STOMP::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("glacier_stomp", 0xF5657D1F3DE5CDC7ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0xF5657D1F3DE5CDC7ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A97E0
// FUNCTION: fn_03230
int CSkillAction_AT_ROARING_CHARGE::GetSkillAttackMotion() {
  return 65;
}

// OFFSET: 0x005A97F0
// FUNCTION: fn_03231
void CSkillAction_AT_ROARING_CHARGE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("roaring_charge", 0x8B65CD61012B5931ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x8B65CD61012B5931ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("roaring_charge_bottom", 0xF7530137154D8FE5ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0xF7530137154D8FE5ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A9BA0
// FUNCTION: fn_03232
int CSkillAction_AT_ROARING_CHARGE_S::GetSkillAttackMotion() {
  return 65;
}

// OFFSET: 0x005A9BB0
// FUNCTION: fn_03233
void CSkillAction_AT_ROARING_CHARGE_S::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 17> bytes = CRYPT_NAME("roaring_charge_s", 0x8B3FEB5B8BD5C3C3ULL);
      thread_local EncryptedName<sizeof(bytes), 0x8B3FEB5B8BD5C3C3ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 24> bytes = CRYPT_NAME("roaring_charge_s_bottom", 0x0BEB63A5C905B3A7ULL);
      thread_local EncryptedName<sizeof(bytes), 0x0BEB63A5C905B3A7ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A9F50
// FUNCTION: fn_03234
void CSkillAction_AT_ROARING_PIERCER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  if (!target) {
    return;
  }

  const int effectId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("roaring_piercer_g", 0x078333BB9725B7C9ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x078333BB9725B7C9ULL> name(bytes);
    return name.Text();
  }());
  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(
      &target->render.position.x, &source->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
    );
    effect->SetAngle(angle);
  }
}

// OFFSET: 0x005AA180
// FUNCTION: fn_03235
void CSkillAction_AT_ROARING_PIERCER_S::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  if (!target) {
    return;
  }

  const int effectId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("roaring_piercer_y", 0x8F1B5BFD5F6175B5ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x8F1B5BFD5F6175B5ULL> name(bytes);
    return name.Text();
  }());
  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(
      &target->render.position.x, &source->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
    );
    effect->SetAngle(angle);
  }
}

// OFFSET: 0x005AA3B0
// FUNCTION: fn_03236
int CSkillAction_AT_FURIOS_STORM::GetSkillAttackMotion() {
  return 65;
}

// OFFSET: 0x005AA3C0
// FUNCTION: fn_03237
void CSkillAction_AT_FURIOS_STORM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        constexpr auto bytesText = CRYPT_NAME("thundering_cast_mute", 0x5B8F49AF09390D1BULL);
        const auto& bytes = bytesText.data;
        thread_local EncryptedName<sizeof(bytes), 0x5B8F49AF09390D1BULL> name(bytes);
        return name.Text();
      }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetLifeTime(1000);
  }

  const int stormA = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("furios_storm_a", 0x3B915119EFD523EFULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x3B915119EFD523EFULL> name(bytes);
    return name.Text();
  }());
  const int stormABottom = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("furios_storm_a_bottom", 0x474DC581D5E98357ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x474DC581D5E98357ULL> name(bytes);
    return name.Text();
  }());
  const int stormB = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("furios_storm_b", 0x3F557BE333F95F6DULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x3F557BE333F95F6DULL> name(bytes);
    return name.Text();
  }());
  const int stormBBottom = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("furios_storm_b_bottom", 0x0753A7E1EFA9E139ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x0753A7E1EFA9E139ULL> name(bytes);
    return name.Text();
  }());

  for (int delay = 0; delay < 1000; delay += 250) {
    const int variant = std::max(std::rand() % 2, 0);
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, variant ? stormA : stormB, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetDelay(delay);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, variant ? stormABottom : stormBBottom, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetDelay(delay);
    }
  }
}

// OFFSET: 0x005AABF0
// FUNCTION: fn_03238
void CSkillAction_AT_FURIOS_STORM::PlayThunderEffect(CGameActor* source) {
}

// OFFSET: 0x005AAC00
// FUNCTION: fn_03239
void CEfstAction_FURIOS_STORM::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<std::uint8_t, 16> bytes = CRYPT_NAME("thundering_cast", 0x7FC1015DFFCF6D19ULL);
      thread_local EncryptedName<sizeof(bytes), 0x7FC1015DFFCF6D19ULL> name(bytes);
      return name.Text();
    }()));
    for (int index = 0; index < 4; ++index) {
      g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        constexpr std::array<std::uint8_t, 15> bytes = CRYPT_NAME("furios_storm_a", 0xCB3FBD4183DB8323ULL);
        thread_local EncryptedName<sizeof(bytes), 0xCB3FBD4183DB8323ULL> name(bytes);
        return name.Text();
      }()));
      g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        constexpr std::array<std::uint8_t, 22> bytes = CRYPT_NAME("furios_storm_a_bottom", 0x7FCD77F9C7CBD14DULL);
        thread_local EncryptedName<sizeof(bytes), 0x7FCD77F9C7CBD14DULL> name(bytes);
        return name.Text();
      }()));
      g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        constexpr std::array<std::uint8_t, 15> bytes = CRYPT_NAME("furios_storm_b", 0xD1538513DF975701ULL);
        thread_local EncryptedName<sizeof(bytes), 0xD1538513DF975701ULL> name(bytes);
        return name.Text();
      }()));
      g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        constexpr std::array<std::uint8_t, 22> bytes = CRYPT_NAME("furios_storm_b_bottom", 0x91E147EBBD47C39FULL);
        thread_local EncryptedName<sizeof(bytes), 0x91E147EBBD47C39FULL> name(bytes);
        return name.Text();
      }()));
    }
  }
}

// OFFSET: 0x005AB5F0
// FUNCTION: fn_03240
int CSkillAction_AT_ALPHA_PHASE::GetSkillCastMotion() {
  return 66;
}

// OFFSET: 0x005AB600
// FUNCTION: fn_03241
int CSkillAction_AT_ALPHA_PHASE::GetSkillAttackMotion() {
  return 68;
}

// OFFSET: 0x005AB610
// FUNCTION: fn_03242
void CSkillAction_AT_ALPHA_PHASE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x005AB640
// FUNCTION: fn_03243
void CSkillAction_AT_ALPHA_PHASE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("alpha_phase", 0x4F1791FB1503C713ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x4F1791FB1503C713ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005AB840
// FUNCTION: fn_03244
EfstActionMessage CEfstAction_ALPHA_PHASE::GetEfstBeginMessage() {
  return {LookupMessage(4670), 0xFFFF00};
}

// OFFSET: 0x005AB870
// FUNCTION: fn_03245
EfstActionMessage CEfstAction_ALPHA_PHASE::GetEfstEndMessage() {
  return {LookupMessage(4671), 0xFF};
}

// OFFSET: 0x005AB8A0
// FUNCTION: fn_03246
int CSkillAction_AT_APEX_PHASE::GetSkillAttackMotion() {
  return 80;
}

// OFFSET: 0x005AB8B0
// FUNCTION: fn_03247
void CSkillAction_AT_APEX_PHASE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("apex_phase", 0xA1A55B4D1597EDD3ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0xA1A55B4D1597EDD3ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005ABAB0
// FUNCTION: fn_03248
EfstActionMessage CEfstAction_AT_APEX_PHASE::GetEfstBeginMessage() {
  return {LookupMessage(4682), 0xFFFF00};
}

// OFFSET: 0x005ABAE0
// FUNCTION: fn_03249
EfstActionMessage CEfstAction_AT_APEX_PHASE::GetEfstEndMessage() {
  return {LookupMessage(4683), 0xFF};
}

// OFFSET: 0x005ABB10
// FUNCTION: fn_03250
int CSkillAction_AT_FLIP_FLAP::GetSkillAttackMotion() {
  return 80;
}

// OFFSET: 0x005ABB20
// FUNCTION: fn_03251
void CSkillAction_AT_FLIP_FLAP::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("flip_flap", 0x1BF915E1F37DFDFDULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x1BF915E1F37DFDFDULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005ABD20
// FUNCTION: fn_03252
EfstActionMessage CEfstAction_FLIP_FLAP::GetEfstBeginMessage() {
  return {LookupMessage(4678), 0xFFFF00};
}

// OFFSET: 0x005ABD50
// FUNCTION: fn_03253
EfstActionMessage CEfstAction_FLIP_FLAP::GetEfstEndMessage() {
  return {LookupMessage(4679), 0xFF};
}

// OFFSET: 0x005ABD80
// FUNCTION: fn_03254
bool CSkillAction_AT_AERO_SYNC::IsSupport() {
  return true;
}

// OFFSET: 0x005ABD90
// FUNCTION: fn_03255
int CSkillAction_AT_AERO_SYNC::GetSkillAttackMotion() {
  return 70;
}

// OFFSET: 0x005ABDA0
// FUNCTION: fn_03256
void CSkillAction_AT_AERO_SYNC::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = CRYPT_CHARS(15, "aero_sync_sync", 0x01FF9313695DD9DFULL);
      thread_local EncryptedName<sizeof(bytes), 0x01FF9313695DD9DFULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = CRYPT_CHARS(15, "aero_sync_main", 0x6DED3DD1CF75AD9BULL);
      thread_local EncryptedName<sizeof(bytes), 0x6DED3DD1CF75AD9BULL> name(bytes);
      return name.Text();
    }()), source->render.position, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = CRYPT_CHARS(22, "aero_sync_main_bottom", 0x49C3759B77DBD3C1ULL);
      thread_local EncryptedName<sizeof(bytes), 0x49C3759B77DBD3C1ULL> name(bytes);
      return name.Text();
    }()), source->render.position, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005AC300
// FUNCTION: fn_03257
EfstActionMessage CEfstAction_FLIP_FLAP_TARGET::GetEfstBeginMessage() {
  return {LookupMessage(4680), 0xFFFF00};
}

// OFFSET: 0x005AC330
// FUNCTION: fn_03258
EfstActionMessage CEfstAction_FLIP_FLAP_TARGET::GetEfstEndMessage() {
  return {LookupMessage(4681), 0xFF};
}

// OFFSET: 0x005AC360
// FUNCTION: fn_03259
void CEfstAction_FLIP_FLAP_TARGET::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("aero_sync_sync", 0x3F5315DFE32B4FAFULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x3F5315DFE32B4FAFULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005AC570
// FUNCTION: fn_03260
int CSkillAction_AT_SAVAGE_LUNGE::GetSkillAttackMotion() {
  return 73;
}

// OFFSET: 0x005AC970
// FUNCTION: fn_03262
int CSkillAction_AT_TEMPEST_FLAP::GetSkillCastMotion() {
  return 66;
}

// OFFSET: 0x005AC980
// FUNCTION: fn_03263
int CSkillAction_AT_TEMPEST_FLAP::GetSkillAttackMotion() {
  return 67;
}

// OFFSET: 0x005ACBA0
// FUNCTION: fn_03265
void CSkillAction_AT_TEMPEST_FLAP::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 13> bytes = CRYPT_NAME("tempest_flap", 0x17FFC98B398FAF61ULL);
      thread_local EncryptedName<sizeof(bytes), 0x17FFC98B398FAF61ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 20> bytes = CRYPT_NAME("tempest_flap_bottom", 0x9B5B959B954D9333ULL);
      thread_local EncryptedName<sizeof(bytes), 0x9B5B959B954D9333ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005ACF40
// FUNCTION: fn_03266
int CSkillAction_AT_TERRA_HARVEST::GetSkillAttackMotion() {
  return 62;
}

// OFFSET: 0x005ACF50
// FUNCTION: fn_03267
void CSkillAction_AT_TERRA_HARVEST::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const int roll = std::rand() % 2;
  const int variant = roll >= 0 ? roll : 0;
  const int effectId = CEZ2STREffectMgr::GetInstance()->EffectId(variant ? std::string([] {
    constexpr auto bytesText = CRYPT_NAME("terra_harvest_red", 0xAFA5617F178B6185ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0xAFA5617F178B6185ULL> name(bytes);
    return name.Text();
  }()) : std::string([] {
    constexpr auto bytesText = CRYPT_NAME("terra_harvest_yellow", 0x078143534999254DULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x078143534999254DULL> name(bytes);
    return name.Text();
  }()));
  const int bottomId = CEZ2STREffectMgr::GetInstance()->EffectId(variant ? std::string([] {
    constexpr auto bytesText = CRYPT_NAME("terra_harvest_red_bottom", 0x8741152D8FB34B4FULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x8741152D8FB34B4FULL> name(bytes);
    return name.Text();
  }()) : std::string([] {
    constexpr auto bytesText = CRYPT_NAME("terra_harvest_yellow_bottom", 0x955D4DF94DE9BB07ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x955D4DF94DE9BB07ULL> name(bytes);
    return name.Text();
  }()));
  g_effectMgr->Spawn(target->render.accountId, effectId, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  g_effectMgr->Spawn(target->render.accountId, bottomId, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x005AD630
// FUNCTION: fn_03268
void CSkillAction_AT_TERRA_WAVE::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 11> bytes = CRYPT_NAME("terra_wave", 0xC399B7091589B50BULL);
    thread_local EncryptedName<sizeof(bytes), 0xC399B7091589B50BULL> name(bytes);
    return name.Text();
  }()), position.x, position.y, position.z, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 18> bytes = CRYPT_NAME("terra_wave_bottom", 0xB739073FE921AD6FULL);
    thread_local EncryptedName<sizeof(bytes), 0xB739073FE921AD6FULL> name(bytes);
    return name.Text();
  }()), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005AD970
// FUNCTION: fn_03269
int CSkillAction_AT_SOLID_STOMP::GetSkillAttackMotion() {
  return 63;
}

// OFFSET: 0x005AD980
// FUNCTION: fn_03270
void CSkillAction_AT_SOLID_STOMP::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 12> bytes = CRYPT_NAME("solid_stomp", 0xFF7D83CB791339D9ULL);
      thread_local EncryptedName<sizeof(bytes), 0xFF7D83CB791339D9ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 19> bytes = CRYPT_NAME("solid_stomp_bottom", 0xED5143E7C1B39315ULL);
      thread_local EncryptedName<sizeof(bytes), 0xED5143E7C1B39315ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005ADD10
// FUNCTION: fn_03271
EfstActionMessage CEfstAction_SOLID_STOMP::GetEfstBeginMessage() {
  return {LookupMessage(4686), 0xFFFF00};
}

// OFFSET: 0x005ADD40
// FUNCTION: fn_03272
EfstActionMessage CEfstAction_SOLID_STOMP::GetEfstEndMessage() {
  return {LookupMessage(4687), 0xFF};
}

// OFFSET: 0x005ADD70
// FUNCTION: fn_03273
int CSkillAction_AT_ZEPHYR_LINK::GetSkillAttackMotion() {
  return 0;
}

// OFFSET: 0x005ADD80
// FUNCTION: fn_03274
void CEfstAction_ZEPHYR_LINK::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    this->OnEfstEnd(efstId, value, argument3, argument4, actor);
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 12> bytes = CRYPT_NAME("zephyr_link", 0x393D916561BDC391ULL);
      thread_local EncryptedName<sizeof(bytes), 0x393D916561BDC391ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 19> bytes = CRYPT_NAME("zephyr_link_bottom", 0x31956551B311FD95ULL);
      thread_local EncryptedName<sizeof(bytes), 0x31956551B311FD95ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005AE130
// FUNCTION: fn_03275
void CEfstAction_ZEPHYR_LINK::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 12> bytes = CRYPT_NAME("zephyr_link", 0x2F2701A7E56D3D89ULL);
      thread_local EncryptedName<sizeof(bytes), 0x2F2701A7E56D3D89ULL> name(bytes);
      return name.Text();
    }()));
    effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 19> bytes = CRYPT_NAME("zephyr_link_bottom", 0x6B3301E1A3079125ULL);
      thread_local EncryptedName<sizeof(bytes), 0x6B3301E1A3079125ULL> name(bytes);
      return name.Text();
    }()));
  }
}

// OFFSET: 0x005AE460
// FUNCTION: fn_03276
void CEfstAction_ZEPHYR_LINK::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    this->OnEfstBegin(efstId, value, 0, 0, actor, false);
  }
}

// OFFSET: 0x005AE480
// FUNCTION: fn_03277
void CEfstAction_INSANE::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    this->OnEfstEnd(efstId, value, argument3, argument4, actor);
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("efst_insane_01", 0x71B7691737C5A1AFULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x71B7691737C5A1AFULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("efst_insane_bottom_01", 0xC39FB559A17D3963ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0xC39FB559A17D3963ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    static_cast<CPc*>(actor)->outlineColor = static_cast<int>(0xFFFFF200);
  }
}

// OFFSET: 0x005AE840
// FUNCTION: fn_03278
void CEfstAction_INSANE::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    static_cast<CPc*>(actor)->outlineColor = 0;
  }
}

// OFFSET: 0x005AE860
// FUNCTION: fn_03279
void CEfstAction_INSANE::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    this->OnEfstBegin(efstId, value, 0, 0, actor, false);
  }
}

// OFFSET: 0x005AE880
// FUNCTION: fn_03280
void CEfstAction_INSANE2::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    this->OnEfstEnd(efstId, value, argument3, argument4, actor);
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("efst_insane_02", 0x45E3F103CBB5F157ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x45E3F103CBB5F157ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("efst_insane_bottom_02", 0x07112F85A39F4FABULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x07112F85A39F4FABULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    static_cast<CPc*>(actor)->outlineColor = static_cast<int>(0xFFFF7F27);
  }
}

// OFFSET: 0x005AEC40
// FUNCTION: fn_03281
void CEfstAction_INSANE3::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    this->OnEfstEnd(efstId, value, argument3, argument4, actor);
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("efst_insane_03", 0x2D33791F67AD47DFULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x2D33791F67AD47DFULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("efst_insane_bottom_03", 0x936F3D2FFDFD87F5ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x936F3D2FFDFD87F5ULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    static_cast<CPc*>(actor)->outlineColor = static_cast<int>(0xFFED1C24);
  }
}

// OFFSET: 0x005AF030
// FUNCTION: fn_03285
bool __stdcall KeepSkillAttackMotion(int) {
  return false;
}

// OFFSET: 0x005A5E50
// FUNCTION: fn_03170
bool CSkillAction_AnimalForm::CanUse(CGameActor* actor) {
  return actor && actor->HasShapeShift() && this->IsInForm(actor);
}

// OFFSET: 0x005AC580
// FUNCTION: fn_03261
void CSkillAction_AT_SAVAGE_LUNGE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target || !this->CanUse(source)) {
    return;
  }

  if (CRagEffect* effect = source->LaunchEffect(2439, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
  }
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = CRYPT_CHARS(13, "savage_lunge", 0xE50B2935073DD3E9ULL);
    thread_local EncryptedName<sizeof(bytes), 0xE50B2935073DD3E9ULL> name(bytes);
    return name.Text();
  }()), target->render.position, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = CRYPT_CHARS(20, "savage_lunge_bottom", 0x7D750DADBDE9C3F1ULL);
    thread_local EncryptedName<sizeof(bytes), 0x7D750DADBDE9C3F1ULL> name(bytes);
    return name.Text();
  }()), target->render.position, 0, -1, 0, 0);
}

// OFFSET: 0x005AC990
// FUNCTION: fn_03264
void CSkillAction_AT_TEMPEST_FLAP::OnSkillCast(int level, CGameActor* source) {
  if (this->CanUse(source)) {
        if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr std::array<char, 18> bytes = CRYPT_NAME("tempest_flap_cast", 0x33832D4D3FDFBDBFULL);
      thread_local EncryptedName<sizeof(bytes), 0x33832D4D3FDFBDBFULL> name(bytes);
      return name.Text();
    }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x005AF000
// FUNCTION: fn_03282
bool SkillActionIsSupport() {
  return false;
}

// OFFSET: 0x005AF010
// FUNCTION: fn_03283
int GetSkillCastMotion() {
  return 8;
}

// OFFSET: 0x005AF020
// FUNCTION: fn_03284
int GetSkillAttackMotion() {
  return 7;
}

// OFFSET: 0x005AF040
// FUNCTION: fn_03286
bool IsSkillWithoutMotion() {
  return false;
}

// OFFSET: 0x005AF050
// FUNCTION: fn_03287
bool KeepSkillAttackSpeed() {
  return false;
}

// OFFSET: 0x005AF060
// FUNCTION: fn_03288
bool SuppressSupportSkillArrow() {
  return false;
}

// OFFSET: 0x005AF070
// FUNCTION: fn_03289
bool SuppressAttackSkillArrow() {
  return false;
}

// OFFSET: 0x005AF080
// FUNCTION: fn_03290
void __stdcall OnSkillCast(int a, int b) {
}

// OFFSET: 0x005AF090
// FUNCTION: fn_03291
void __stdcall OnTargetSkillCast(int a, int b, int c) {
}

// OFFSET: 0x005AF0A0
// FUNCTION: fn_03292
void __stdcall OnGroundSkillCast(int a, int b, int c, int d, int e) {
}

// OFFSET: 0x005AF0B0
// FUNCTION: fn_03293
void __stdcall OnSkillEffect(int a, int b, int c) {
}

// OFFSET: 0x005AF0C0
// FUNCTION: fn_03294
void __stdcall OnGroundSkillEffect(vector3d a, int b, int c) {
}

// OFFSET: 0x005AF0D0
// FUNCTION: fn_03295
bool UseGroundSkillLastFrame() {
  return false;
}

// OFFSET: 0x005AF0E0
// FUNCTION: fn_03296
int GetSkillScopeType() {
  return 0;
}

// OFFSET: 0x005AF0F0
// FUNCTION: fn_03297
void __stdcall OnSkillHit(int a, int b, int c) {
}

// OFFSET: 0x005AF100
// FUNCTION: fn_03298
void __stdcall OnSkillDamage(int a, int b, int c) {
}

// OFFSET: 0x005AF110
// FUNCTION: fn_03299
void __stdcall OnEfstBegin(int a, int b, int c, int d, int e, int f) {
}

// OFFSET: 0x005AF120
// FUNCTION: fn_03300
void __stdcall OnEfstEnd(int a, int b, int c, int d, int e) {
}

// OFFSET: 0x005AF130
// FUNCTION: fn_03301
void __stdcall OnEfstUpdate(int a, int b, int c, int d, int e) {
}

// OFFSET: 0x005AF140
// FUNCTION: fn_03302
void __stdcall GetEfstBeginMessage(std::uint32_t* out) {
  out[0] = 0;
  out[1] = 0xFFFF00;
}

// OFFSET: 0x005AF160
// FUNCTION: fn_03303
void __stdcall GetEfstEndMessage(std::uint32_t* out) {
  out[0] = 0;
  out[1] = 0xFF;
}

// OFFSET: 0x005AF180
// FUNCTION: fn_03304
void __stdcall OnSkillObjectCreate(int a, int b, int c) {
}

// OFFSET: 0x005AF190
// FUNCTION: fn_03305
void __stdcall OnSkillObjectRemove(int a) {
}

// OFFSET: 0x005AF4E0
// FUNCTION: fn_03326
int CSkillAction_AT_PRIMAL_CLAW::GetSkillAttackMotion() {
  return 72;
}

// OFFSET: 0x005AF4F0
// FUNCTION: fn_03327
void CSkillAction_AT_PRIMAL_CLAW::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source) || !target) {
    return;
  }

    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("primal_claw", 0x812BE72BCD5707B7ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x812BE72BCD5707B7ULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x005AF700
// FUNCTION: fn_03328
int CSkillAction_AT_FERAL_CLAW::GetSkillAttackMotion() {
  return 71;
}

// OFFSET: 0x005AF710
// FUNCTION: fn_03329
void CSkillAction_AT_FERAL_CLAW::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source) || !target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("feral_claw", 0x3375B1C7DBB5FD55ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x3375B1C7DBB5FD55ULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// GLOBAL: gl_25058
const float g_featherOffsets[3] = {0.0f, 0.1f, -0.1f};

// OFFSET: 0x005A4760
// FUNCTION: fn_03152
void CSkillAction_DamageWereRaptorForm::SpawnAttack00Effect(int sourceActor, CGameActor* target) {
  CGameActor* source = reinterpret_cast<CGameActor*>(sourceActor);
  if (source && target) {
    constexpr auto bytesText = CRYPT_NAME("attack_00", 0x2FF3C5036D070127ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x2FF3C5036D070127ULL> name(bytes);
    const std::string effectName = name.Text();
    this->SpawnDirectionalEffect(source, target, effectName);
  }
}

// OFFSET: 0x005A4930
// FUNCTION: fn_03153
void CSkillAction_DamageWereRaptorForm::SpawnAttack01Effect(int sourceActor, CGameActor* target) {
  CGameActor* source = reinterpret_cast<CGameActor*>(sourceActor);
  if (source && target) {
    constexpr auto bytesText = CRYPT_NAME("attack_01", 0x17359BF953C367B3ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x17359BF953C367B3ULL> name(bytes);
    const std::string effectName = name.Text();
    this->SpawnDirectionalEffect(source, target, effectName);
  }
}

// OFFSET: 0x005A4B00
// FUNCTION: fn_03154
void CSkillAction_DamageWereRaptorForm::SpawnAttack02Effect(int sourceActor, CGameActor* target) {
  CGameActor* source = reinterpret_cast<CGameActor*>(sourceActor);
  if (!target || !source) {
    return;
  }

  const float angle = ScreenAngle(
    &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
  );
  const int direction = this->GetDirection(angle);
  if (!this->IsValidDirection(direction)) {
    return;
  }

  const int effectId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("attack_02", 0xC52FB5FBB94FF917ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0xC52FB5FBB94FF917ULL> name(bytes);
    return name.Text();
  }());
  const bool flipped = direction == 1 || direction == 2;

  const float deltaY = target->render.position.y - source->render.position.y;
  const float deltaX = target->render.position.x - source->render.position.x;
  const float deltaZ = target->render.position.z - source->render.position.z;
  const float inverseLength = 1.0f / SquareRoot(deltaY * deltaY + deltaX * deltaX + deltaZ * deltaZ);
  const float forwardY = inverseLength * deltaY * 35.0f;
  const float forwardZ = inverseLength * deltaZ * 35.0f;
  const float forwardX = inverseLength * deltaX * 35.0f;

  const float sideX = forwardY * 0.0f - forwardZ;
  const float sideY = forwardZ * 0.0f - forwardX * 0.0f;
  const float sideZ = forwardX - forwardY * 0.0f;
  const float inverseSide = 1.0f / SquareRoot(sideY * sideY + sideX * sideX + sideZ * sideZ);
  const float rightX = inverseSide * sideX * 7.5f;
  const float rightY = inverseSide * sideY * 7.5f;
  const float rightZ = inverseSide * sideZ * 7.5f;

  int count = 1;
  int row = 0;
  for (int alpha = 150; alpha < 525; alpha += 125) {
    const float advance = static_cast<float>(row + 1) / 3.0f;
    const float shift = static_cast<float>(row);
    const float baseX = (forwardX * advance + source->render.position.x) - rightX * shift;
    const float baseY = (forwardY * advance + source->render.position.y) - rightY * shift;
    const float baseZ = (forwardZ * advance + source->render.position.z) - rightZ * shift;

    for (int column = 0; column < count; ++column) {
      const int skip = std::rand() % 2;
      if ((skip >= 0 ? skip : 0) != 0) {
        continue;
      }

      const int pick = std::rand() % 3;
      const float offset = g_featherOffsets[pick >= 0 ? pick : 0];
      const float step = static_cast<float>(column);
      const vector3d position = {rightX * step + baseX + offset * forwardX, rightY * step + baseY + offset * forwardY, rightZ * step + baseZ + offset * forwardZ};
      const int jitter = std::rand() % 101;
      const int fade = (jitter >= 0 ? jitter : 0) + alpha;
      const int scale = std::max(std::rand() % 11, 7);
      if (CEffect* effect = g_effectMgr->Spawn(0, effectId, position, 0, -1, 0, 0)) {
        effect->SetFlipX(flipped);
        effect->SetDelay(fade);
        effect->SetScale(static_cast<float>(scale) * 0.1f);
      }
    }

    count += 2;
    ++row;
  }
}

// GLOBAL: gl_25618
static EfstActionRegistrar s_efstAction1705(1705, NewAction<CEfstAction_ALPHA_PHASE, CEfstAction>);
// GLOBAL: gl_25619
static EfstActionRegistrar s_efstAction1708(1708, NewAction<CEfstAction_AT_APEX_PHASE, CEfstAction>);
// GLOBAL: gl_25620
static EfstActionRegistrar s_efstAction1706(1706, NewAction<CEfstAction_FLIP_FLAP, CEfstAction>);
// GLOBAL: gl_25621
static EfstActionRegistrar s_efstAction1707(1707, NewAction<CEfstAction_FLIP_FLAP_TARGET, CEfstAction>);
// GLOBAL: gl_25622
static EfstActionRegistrar s_efstAction1722(1722, NewAction<CEfstAction_FURIOS_STORM, CEfstAction>);
// GLOBAL: gl_25623
static EfstActionRegistrar s_efstAction1703(1703, NewAction<CEfstAction_INSANE2, CEfstAction>);
// GLOBAL: gl_25624
static EfstActionRegistrar s_efstAction1704(1704, NewAction<CEfstAction_INSANE3, CEfstAction>);
// GLOBAL: gl_25625
static EfstActionRegistrar s_efstAction1702(1702, NewAction<CEfstAction_INSANE, CEfstAction>);
// GLOBAL: gl_25626
static EfstActionRegistrar s_efstAction1713(1713, NewAction<CEfstAction_NATURE_HARMONY, CEfstAction>);
// GLOBAL: gl_25627
static EfstActionRegistrar s_efstAction1715(1715, NewAction<CEfstAction_PULSE_OF_MADNESS, CEfstAction>);
// GLOBAL: gl_25628
static EfstActionRegistrar s_efstAction1712(1712, NewAction<CEfstAction_SOLID_STOMP, CEfstAction>);
// GLOBAL: gl_25629
static EfstActionRegistrar s_efstAction1710(1710, NewAction<CEfstAction_ZEPHYR_LINK, CEfstAction>);

// OFFSET: 0x0040D1D0
// FUNCTION: fn_45560
static bool s_skillObjectActionRegistered314 = [] {
  std::function<CSkillObjectAction*()> factory = NewAction<CSkillObjectAction_AT_GLACIER_MONOLITH, CSkillObjectAction>;
  CSkillActionMgr::GetInstance()->RegisterSkillObjectAction(314, factory);
  return true;
}();

// GLOBAL: gl_25630
static SkillActionRegistrar s_skillAction6591(6591, NewAction<CSkillAction_AT_AERO_SYNC, CSkillAction>);
// GLOBAL: gl_25631
static SkillActionRegistrar s_skillAction6580(6580, NewAction<CSkillAction_AT_ALPHA_CLAW, CSkillAction>);
// GLOBAL: gl_25632
static SkillActionRegistrar s_skillAction6577(6577, NewAction<CSkillAction_AT_ALPHA_PHASE, CSkillAction>);
// GLOBAL: gl_25633
static SkillActionRegistrar s_skillAction6585(6585, NewAction<CSkillAction_AT_APEX_PHASE, CSkillAction>);
// GLOBAL: gl_25634
static SkillActionRegistrar s_skillAction6596(6596, NewAction<CSkillAction_AT_CHILLING_BLAST, CSkillAction>);
// GLOBAL: gl_25635
static SkillActionRegistrar s_skillAction6579(6579, NewAction<CSkillAction_AT_FERAL_CLAW, CSkillAction>);
// GLOBAL: gl_25636
static SkillActionRegistrar s_skillAction6583(6583, NewAction<CSkillAction_AT_FLIP_FLAP, CSkillAction>);
// GLOBAL: gl_25637
static SkillActionRegistrar s_skillAction6582(6582, NewAction<CSkillAction_AT_FRENZY_FANG, CSkillAction>);
// GLOBAL: gl_25638
static SkillActionRegistrar s_skillAction6601(6601, NewAction<CSkillAction_AT_FURIOS_STORM, CSkillAction>);
// GLOBAL: gl_25639
static SkillActionRegistrar s_skillAction6592(6592, NewAction<CSkillAction_AT_GLACIER_MONOLITH, CSkillAction>);
// GLOBAL: gl_25640
static SkillActionRegistrar s_skillAction6593(6593, NewAction<CSkillAction_AT_GLACIER_NOVA, CSkillAction>);
// GLOBAL: gl_25641
static SkillActionRegistrar s_skillAction6594(6594, NewAction<CSkillAction_AT_GLACIER_SHARD, CSkillAction>);
// GLOBAL: gl_25642
static SkillActionRegistrar s_skillAction6595(6595, NewAction<CSkillAction_AT_GLACIER_STOMP, CSkillAction>);
// GLOBAL: gl_25643
static SkillActionRegistrar s_skillAction6605(6605, NewAction<CSkillAction_AT_GRAVITY_HOLE, CSkillAction>);
// GLOBAL: gl_25644
static SkillActionRegistrar s_skillAction6607(6607, NewAction<CSkillAction_AT_NATURE_HARMONY, CSkillAction>);
// GLOBAL: gl_25645
static SkillActionRegistrar s_skillAction6586(6586, NewAction<CSkillAction_AT_PINION_SHOT, CSkillAction>);
// GLOBAL: gl_25646
static SkillActionRegistrar s_skillAction6578(6578, NewAction<CSkillAction_AT_PRIMAL_CLAW, CSkillAction>);
// GLOBAL: gl_25647
static SkillActionRegistrar s_skillAction6576(6576, NewAction<CSkillAction_AT_PULSE_OF_MADNESS, CSkillAction>);
// GLOBAL: gl_25648
static SkillActionRegistrar s_skillAction6588(6588, NewAction<CSkillAction_AT_QUILL_SPEAR, CSkillAction>);
// GLOBAL: gl_25649
static SkillActionRegistrar s_skillAction6589(6589, NewAction<CSkillAction_AT_QUILL_SPEAR_S, CSkillAction>);
// GLOBAL: gl_25650
static SkillActionRegistrar s_skillAction6599(6599, NewAction<CSkillAction_AT_ROARING_CHARGE, CSkillAction>);
// GLOBAL: gl_25651
static SkillActionRegistrar s_skillAction6600(6600, NewAction<CSkillAction_AT_ROARING_CHARGE_S, CSkillAction>);
// GLOBAL: gl_25652
static SkillActionRegistrar s_skillAction6597(6597, NewAction<CSkillAction_AT_ROARING_PIERCER, CSkillAction>);
// GLOBAL: gl_25653
static SkillActionRegistrar s_skillAction6598(6598, NewAction<CSkillAction_AT_ROARING_PIERCER_S, CSkillAction>);
// GLOBAL: gl_25654
static SkillActionRegistrar s_skillAction6581(6581, NewAction<CSkillAction_AT_SAVAGE_LUNGE, CSkillAction>);
// GLOBAL: gl_25655
static SkillActionRegistrar s_skillAction6604(6604, NewAction<CSkillAction_AT_SOLID_STOMP, CSkillAction>);
// GLOBAL: gl_25656
static SkillActionRegistrar s_skillAction6590(6590, NewAction<CSkillAction_AT_TEMPEST_FLAP, CSkillAction>);
// GLOBAL: gl_25657
static SkillActionRegistrar s_skillAction6603(6603, NewAction<CSkillAction_AT_TERRA_HARVEST, CSkillAction>);
// GLOBAL: gl_25658
static SkillActionRegistrar s_skillAction6602(6602, NewAction<CSkillAction_AT_TERRA_WAVE, CSkillAction>);
// GLOBAL: gl_25659
static SkillActionRegistrar s_skillAction6587(6587, NewAction<CSkillAction_AT_ZEPHYR_LINK, CSkillAction>);
