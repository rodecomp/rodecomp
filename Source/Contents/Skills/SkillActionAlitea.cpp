#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/EZ2STREffectEx.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Math/Vector3d.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <string_view>

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
  virtual bool IsInForm(CGameActor* actor) = 0;
  virtual void PlayFormAttackEffect(CGameActor* actor) = 0;
};

class CSkillAction_Direction {
public:
  void SpawnDirectionalEffect(CGameActor* source, CGameActor* target, std::string_view name);
  int GetDirection(float angle);
  bool IsValidDirection(unsigned int direction);
};

class CSkillAction_DamageWereRaptorForm : public CSkillAction_AnimalForm, public CSkillAction_Direction {
public:
  void SpawnAttack00Effect(int sourceActor, CGameActor* target);
  __declspec(noinline) void SpawnAttack01Effect(int level, CGameActor* source);
  __declspec(noinline) void SpawnAttack02Effect(int level, CGameActor* source);
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

class CSkillAction_AT_ROARING_PIERCEER_S : public CSkillAction_ThunderingExpand {
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
      const char bytes[] = {'\x7B', '\x43', '\x92', '\x8F', '\x84', '\x01', '\x32', '\x6A', '\x7A', '\x43', '\xA8', '\x95', '\x8C', '\x05', '\x19', '\x72',
                                    '\x6D', '\x31'};
      thread_local EncryptedName<sizeof(bytes), 0x1D6D75EDE7CD311FULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A52B0
// FUNCTION: fn_03156
void CSkillAction_DamageWereWolfForm::PlayFormAttackEffect(CGameActor* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\x31', '\x8F', '\xEA', '\xB3', '\xAE', '\xC5', '\x2A', '\x44', '\x30', '\x8F', '\xD0', '\xAC', '\xA8', '\xDD', '\x13', '\x33'};
      thread_local EncryptedName<sizeof(bytes), 0x3375B1C7DBB5FD55ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A54B0
// FUNCTION: fn_03157
void CSkillAction_Icing::SpawnIcing(CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
          const char bytes[] = {'\x18', '\x9A', '\x3C', '\xAA', '\xE9', '\x74', '\x11', '\x91'};
          thread_local EncryptedName<sizeof(bytes), 0x91651D81F559F971ULL> name(bytes);
          return name.Text();
        }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetScale(0.9f);
    }
  }
}

// OFFSET: 0x005A56C0
// FUNCTION: fn_03158
void CSkillAction_ThunderingBasic::PlayThunderEffect(CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\xA3', '\x27', '\x76', '\xFB', '\xBD', '\xE0', '\xDD', '\x56', '\xB9', '\x28', '\x5C', '\xF4', '\xAD', '\xF1', '\xCE', '\x5C',
                                    '\xBC', '\x10', '\x64', '\x95'};
      thread_local EncryptedName<sizeof(bytes), 0x3FAF85D995034FD7ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A58C0
// FUNCTION: fn_03159
void CSkillAction_ThunderingExpand::PlayThunderEffect(CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xCD, 0x65, 0x5A, 0xCF, 0x27, 0x9E, 0x7D, 0x20, 0xD7, 0x6A, 0x70, 0xC0, 0x37, 0x8F, 0x6E, 0x2A,
                                    0xD2, 0x52, 0x56, 0xA1};
      thread_local EncryptedName<sizeof(bytes), 0x490FFB43A12F0DB9ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A5AC0
// FUNCTION: fn_03160
void CSkillAction_Direction::SpawnDirectionalEffect(CGameActor* source, CGameActor* target, std::string_view name) {
  const float angle = ScreenAngle(
    &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
  );
  const int direction = this->GetDirection(angle);
  if (!this->IsValidDirection(direction)) {
    return;
  }

  const bool flipped = direction == 1 || direction == 2;
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(name.data()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
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
      g_effectMgr->Spawn(actor->render.accountId, id, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ice_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect =
            effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ice_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x005A6260
// FUNCTION: fn_03180
void CSkillAction_Thundering::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
          const char bytes[] = {'\xB1', '\xA5', '\xF2', '\x9B', '\xC5', '\xC6', '\x95', '\xFA', '\xAB', '\xAA', '\xD8', '\x96', '\xC0', '\xD0', '\x93', '\x93'};
          thread_local EncryptedName<sizeof(bytes), 0x93E7A3A1F587CDC5ULL> name(bytes);
          return name.Text();
        }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
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
  EffectLists* effects = g_effectMgr;
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\xC5', '\x62', '\x23', '\xB8', '\x59', '\x89', '\x2B'};
    thread_local EncryptedName<sizeof(bytes), 0x812BE72BCD5707B7ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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

  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\x7E', '\x5D', '\xBD', '\x8F', '\x8C', '\x2A', '\x0E', '\x71', '\x7E', '\x46', '\xCD'};
    thread_local EncryptedName<sizeof(bytes), 0x1D6D75EDE7CD311FULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
  if (source && source->GetShapeShift() != 0) {
    this->IsInForm(source);
  }

  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x57, 0x74, 0x6B, 0x1E, 0x66, 0x9A, 0x9C, 0x49, 0x78, 0x6C, 0x66, 0x09, 0x6D, 0xA0, 0x80, 0x5C, 0x27};
    thread_local EncryptedName<sizeof(bytes), 0x2FF3C5036D070127ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const char bytes[] = {'\xD5', '\x15', '\xA6', '\x3D', '\x83', '\xE2', '\x6A', '\x71', '\xD2', '\x09', '\xA4', '\x53'};
        thread_local EncryptedName<sizeof(bytes), 0x17359BF953C367B3ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
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

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const char bytes[] = {'\x67', '\x90', '\x21', '\xD0', '\x94', '\xDB', '\x70', '\xB6', '\x7F', '\x96', '\x3B', '\xB9'};
        thread_local EncryptedName<sizeof(bytes), 0xC52FB5FBB94FF917ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
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
    const char bytes[] = {'\xB4', '\xB8', '\xEE', '\x99', '\xCD', '\xFC', '\x94', '\xE3', '\xA0', '\xAC', '\xF5', '\xAA', '\x92', '\xDB', '\xD0', '\x93'};
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
    const char bytes[] = {'\xA6', '\x3A', '\x6A', '\xF9', '\xB5', '\xDA', '\xDC', '\x4F', '\xB2', '\x2E', '\x71', '\xCA', '\xEC', '\xFD', '\x98', '\x3F'};
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
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\xD7', '\x6C', '\x5B', '\xD4', '\x31', '\x9E', '\x50', '\x21', '\xD8', '\x7F', '\x42', '\xCE', '\x2D', '\x82', '\x0F'};
      thread_local EncryptedName<sizeof(bytes), 0x490FFB43A12F0DB9ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
      const char bytes[] = {'\x16', '\x8B', '\x38', '\x83', '\xE8', '\x69', '\x1C', '\xCE', '\x19', '\x96', '\x35', '\x90', '\x81'};
      thread_local EncryptedName<sizeof(bytes), 0x91651D81F559F971ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\x6C', '\x09', '\xBE', '\x17', '\x22', '\x13', '\xA8', '\x2C', '\x63', '\x14', '\xB3', '\x04', '\x14', '\x05', '\xBE', '\x07', '\x7F', '\x14', '\xB2', '\x61'};
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
      const char bytes[] = {'\x0E', '\x0B', '\x5C', '\x41', '\xC9', '\x6A', '\xE1', '\xBE', '\x32', '\x01', '\x59', '\x4C', '\xD6', '\x77', '\x8F'};
      thread_local EncryptedName<sizeof(bytes), 0xD98F03A52D35636DULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\xB2', '\x71', '\xF4', '\xE3', '\x07', '\xE2', '\x61', '\x68', '\x8E', '\x7B', '\xF1', '\xEE', '\x18', '\xFF', '\x50', '\x6D',
                                    '\xBE', '\x6D', '\xE9', '\xE0', '\x06', '\x8B'};
      thread_local EncryptedName<sizeof(bytes), 0x0F0F8B6B8F9D19D1ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
    const char bytes[] = {'\x6E', '\xC5', '\x26', '\x3C', '\xB0', '\x38', '\x07', '\x8C', '\x64', '\xC6', '\x29', '\x30', '\xB5', '\x34', '\x01', '\xBB',
                                  '\x56', '\xDA', '\x33', '\x3E', '\xAB', '\x29', '\x75'};
    thread_local EncryptedName<sizeof(bytes), 0xD3755DD95F47A909ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\x0A', '\x8F', '\x24', '\xC0', '\x84', '\xA4', '\xA5', '\x6E', '\x00', '\x8C', '\x2B', '\xCC', '\x81', '\xA8', '\xA3', '\x59',
                                  '\x32', '\x90', '\x31', '\xC2', '\x9F', '\xB5', '\x88', '\x53', '\x02', '\x97', '\x31', '\xCC', '\x80', '\xC1'};
    thread_local EncryptedName<sizeof(bytes), 0x31D7C1EDA345E36DULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const char bytes[] = {'\x16', '\x77', '\x92', '\x9C', '\xEE', '\x40', '\xE3', '\x98', '\x1C', '\x74', '\x9D', '\x90', '\xEB', '\x4C', '\xE5', '\xAF',
                                      '\x2E', '\x77', '\x9C', '\x90', '\xF7', '\x25'};
        thread_local EncryptedName<sizeof(bytes), 0xC7912587FFF31B71ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetDelay(990);
  }
  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const char bytes[] = {'\x0A', '\x5F', '\x6E', '\xF6', '\xB2', '\x20', '\xA1', '\x36', '\x00', '\x5C', '\x61', '\xFA', '\xB7', '\x2C', '\xA7', '\x01',
                                      '\x32', '\x5F', '\x60', '\xFA', '\xAB', '\x1A', '\xB1', '\x06', '\x19', '\x47', '\x60', '\xF8', '\xDB'};
        thread_local EncryptedName<sizeof(bytes), 0x69D345DB950F336DULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
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
    const char bytes[] = {'\x48', '\xF3', '\xD2', '\xB8', '\x86', '\xAC', '\x65', '\xC4', '\x42', '\xF0', '\xDD', '\xB4', '\x83', '\xA0', '\x63', '\xF3',
                                  '\x70', '\xF3', '\xDC', '\xB4', '\x9F', '\xC9'};
    thread_local EncryptedName<sizeof(bytes), 0x9B17C9EFDBB39F2FULL> name(bytes);
    return name.Text();
  }()));
  effects = g_effectMgr;
  effects->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\xBA', '\xB9', '\x28', '\x22', '\x1E', '\x70', '\x49', '\x72', '\xB0', '\xBA', '\x27', '\x2E', '\x1B', '\x7C', '\x4F', '\x45',
                                  '\x82', '\xB9', '\x26', '\x2E', '\x07', '\x4A', '\x59', '\x42', '\xA9', '\xA1', '\x26', '\x2C', '\x77'};
    thread_local EncryptedName<sizeof(bytes), 0x2D3B15774149D5DDULL> name(bytes);
    return name.Text();
  }()));

  effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\x86', '\xF7', '\x92', '\x18', '\x84', '\xE2', '\x8D', '\x3E', '\x8C', '\xF4', '\x9D', '\x14', '\x81', '\xEE', '\x8B', '\x09', '\xBE', '\xFE', '\x9D', '\x1F', '\xED'};
    thread_local EncryptedName<sizeof(bytes), 0x61FF87ED7BF39BE1ULL> name(bytes);
    return name.Text();
  }()), object->render.position.x, object->render.position.y, object->render.position.z, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\x98', '\xE9', '\x46', '\x6E', '\x0C', '\x5A', '\xEB', '\x22', '\x92', '\xEA', '\x49', '\x62', '\x09', '\x56', '\xED', '\x15',
                                  '\xA0', '\xE0', '\x49', '\x69', '\x3A', '\x5D', '\xF6', '\x09', '\x8B', '\xEA', '\x4A', '\x0D'};
    thread_local EncryptedName<sizeof(bytes), 0x7D993F650D2785FFULL> name(bytes);
    return name.Text();
  }()), object->render.position.x, object->render.position.y, object->render.position.z, 0, -1, 0, 0);
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
    const char bytes[] = {'\x54', '\x4B', '\x52', '\x6C', '\xD4', '\x28', '\x4B', '\x72', '\x5D', '\x48', '\x45', '\x6E', '\xBD'};
    thread_local EncryptedName<sizeof(bytes), 0x2D394DBD0F332733ULL> name(bytes);
    return name.Text();
  }()), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005A91A0
// FUNCTION: fn_03226
void CSkillAction_AT_GLACIER_SHARD::OnGroundSkillCast(int level, CGameActor* source, int argument3, int argument4, int argument5) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
          const char bytes[] = {'\x20', '\x4D', '\xDA', '\x88', '\x82', '\x12', '\x6D', '\x4A', '\x34', '\x49', '\xDA', '\x99', '\x8F', '\x28', '\x6B', '\x74',
                                        '\x35', '\x46', '\xDE', '\x9F', '\xEB'};
          thread_local EncryptedName<sizeof(bytes), 0x151F77EBEBBB2147ULL> name(bytes);
          return name.Text();
        }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
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
      const char bytes[] = {'\xA6', '\xA3', '\xB2', '\x50', '\xFE', '\x78', '\x8F', '\x36', '\xB2', '\xA7', '\xB2', '\x41', '\xF3', '\x1D'};
      thread_local EncryptedName<sizeof(bytes), 0x69FD1D9733D3CFC1ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
      const char bytes[] = {'\xA0', '\xA1', '\x84', '\x5E', '\x76', '\x18', '\x17', '\xAA', '\xB4', '\xB9', '\x8A', '\x50', '\x6F', '\x7D'};
      thread_local EncryptedName<sizeof(bytes), 0xF5657D1F3DE5CDC7ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
      const char bytes[] = {'\x43', '\x36', '\x4A', '\x73', '\x08', '\xA3', '\x02', '\xD4', '\x52', '\x31', '\x4A', '\x73', '\x06', '\xA8', '\x65'};
      thread_local EncryptedName<sizeof(bytes), 0x8B65CD61012B5931ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\x97', '\xE0', '\x2C', '\x67', '\x5E', '\x6F', '\x34', '\xA8', '\x86', '\xE7', '\x2C', '\x67', '\x50', '\x64', '\x0C', '\x95',
                                    '\x8A', '\xFB', '\x39', '\x7A', '\x5A', '\x01'};
      thread_local EncryptedName<sizeof(bytes), 0xF7530137154D8FE5ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
      const char bytes[] = {'\xB1', '\xAC', '\xB4', '\xF9', '\x32', '\x85', '\x58', '\xD4', '\xA0', '\xAB', '\xB4', '\xF9', '\x3C', '\x8E', '\x60', '\xF8', '\xC3'};
      thread_local EncryptedName<sizeof(bytes), 0x8B3FEB5B8BD5C3C3ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\xD5', '\xDC', '\x64', '\xBB', '\xCC', '\x0D', '\x8C', '\x54', '\xC4', '\xDB', '\x64', '\xBB', '\xC2', '\x06', '\xB4', '\x78',
                                    '\xF8', '\xD1', '\x6A', '\xBD', '\xD1', '\x0C', '\x86', '\x0B'};
      thread_local EncryptedName<sizeof(bytes), 0x0BEB63A5C905B3A7ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0xBB, 0xD8, 0x44, 0xE5, 0xD2, 0x5D, 0xE4, 0x58, 0xB9, 0xDE, 0x40, 0xE5, 0xD8, 0x56, 0xF1, 0x58,
                                      0xAE, 0xB7};
        thread_local EncryptedName<sizeof(bytes), 0x078333BB9725B7C9ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(
      &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
    );
    effect->SetAngle(angle);
  }
}

// OFFSET: 0x005AA180
// FUNCTION: fn_03235
void CSkillAction_AT_ROARING_PIERCEER_S::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  if (!target) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0xC7, 0x1A, 0x00, 0x2D, 0x94, 0x35, 0x7C, 0xD0, 0xC5, 0x1C, 0x04, 0x2D, 0x9E, 0x3E, 0x69, 0xD0,
                                      0xCC, 0x75};
        thread_local EncryptedName<sizeof(bytes), 0x8F1B5BFD5F6175B5ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(
      &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
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
        const char bytes[] = {'\x6F', '\x65', '\x4C', '\x67', '\xCB', '\x2C', '\xFD', '\x32', '\x75', '\x6A', '\x66', '\x6A', '\xCE', '\x3A', '\xFB', '\x04',
                                      '\x76', '\x78', '\x4D', '\x6C', '\xAF'};
        thread_local EncryptedName<sizeof(bytes), 0x5B8F49AF09390D1BULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetLifeTime(1000);
  }

  const int stormA = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\x89', '\x56', '\xA7', '\x86', '\x76', '\x22', '\xCE', '\x48', '\x9B', '\x4C', '\xA7', '\x82', '\x46', '\x30', '\x91'};
    thread_local EncryptedName<sizeof(bytes), 0x3B915119EFD523EFULL> name(bytes);
    return name.Text();
  }());
  const int stormABottom = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\x31', '\xF6', '\x9B', '\xBC', '\xEE', '\xB6', '\x12', '\x34', '\x23', '\xEC', '\x9B', '\xB8', '\xDE', '\xA4', '\x12', '\x25',
                                  '\x38', '\xF7', '\x9D', '\xBA', '\xEC', '\xC5'};
    thread_local EncryptedName<sizeof(bytes), 0x474DC581D5E98357ULL> name(bytes);
    return name.Text();
  }());
  const int stormB = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\x0B', '\x2A', '\x8B', '\x5A', '\x8C', '\x08', '\x0A', '\x4C', '\x19', '\x30', '\x8B', '\x5E', '\xBC', '\x19', '\x55'};
    thread_local EncryptedName<sizeof(bytes), 0x3F557BE333F95F6DULL> name(bytes);
    return name.Text();
  }());
  const int stormBBottom = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\x5F', '\x94', '\xDB', '\x86', '\x8E', '\xD4', '\x0C', '\x74', '\x4D', '\x8E', '\xDB', '\x82', '\xBE', '\xC5', '\x0C', '\x65',
                                  '\x56', '\x95', '\xDD', '\x80', '\x8C', '\xA7'};
    thread_local EncryptedName<sizeof(bytes), 0x0753A7E1EFA9E139ULL> name(bytes);
    return name.Text();
  }());

  for (int delay = 0; delay < 1000; delay += 250) {
    const int variant = std::max(std::rand() % 2, 0);
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, variant ? stormA : stormB, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetDelay(delay);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, variant ? stormABottom : stormBBottom, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
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
      const std::uint8_t bytes[] = {0x6D, 0x05, 0xBA, 0x91, 0x39, 0x64, 0xB3, 0x16, 0x77, 0x0A, 0x90, 0x9C, 0x3C, 0x72, 0xB5, 0x7F};
      thread_local EncryptedName<sizeof(bytes), 0x7FC1015DFFCF6D19ULL> name(bytes);
      return name.Text();
    }()));
    for (int index = 0; index < 4; ++index) {
      g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0x45, 0xF6, 0xA9, 0xEA, 0x2E, 0xCE, 0x60, 0xB8, 0x57, 0xEC, 0xA9, 0xEE, 0x1E, 0xDC, 0x3F};
        thread_local EncryptedName<sizeof(bytes), 0xCB3FBD4183DB8323ULL> name(bytes);
        return name.Text();
      }()));
      g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0x2B, 0xA4, 0xB9, 0xAE, 0x96, 0x04, 0x92, 0x0C, 0x39, 0xBE, 0xB9, 0xAA, 0xA6, 0x16, 0x92, 0x1D,
                                      0x22, 0xA5, 0xBF, 0xA8, 0x94, 0x77};
        thread_local EncryptedName<sizeof(bytes), 0x7FCD77F9C7CBD14DULL> name(bytes);
        return name.Text();
      }()));
      g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0x67, 0x22, 0xE5, 0xB6, 0x7C, 0xF6, 0x0C, 0xA2, 0x75, 0x38, 0xE5, 0xB2, 0x4C, 0xE7, 0x53};
        thread_local EncryptedName<sizeof(bytes), 0xD1538513DF975701ULL> name(bytes);
        return name.Text();
      }()));
      g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0xF9, 0xB6, 0x35, 0xD4, 0x84, 0x34, 0xBE, 0xE2, 0xEB, 0xAC, 0x35, 0xD0, 0xB4, 0x25, 0xBE, 0xF3,
                                      0xF0, 0xB7, 0x33, 0xD2, 0x86, 0x47};
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
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\x72', '\xAB', '\x73', '\x7D', '\x9A', '\xCE', '\x67', '\x27', '\x72', '\xB4', '\x66', '\x15'};
      thread_local EncryptedName<sizeof(bytes), 0x4F1791FB1503C713ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\xB2', '\x9D', '\xF2', '\x6D', '\x12', '\x2B', '\xCD', '\xC0', '\xA0', '\x88', '\x97'};
      thread_local EncryptedName<sizeof(bytes), 0xA1A55B4D1597EDD3ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\x9B', '\x91', '\x14', '\x83', '\xBE', '\x73', '\x95', '\x7A', '\x8D', '\xFD'};
      thread_local EncryptedName<sizeof(bytes), 0x1BF915E1F37DFDFDULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
      const char bytes[] = {'\xBE', '\xBC', '\x2F', '\x06', '\x4C', '\xE0', '\x86', '\x6F', '\xBC', '\x86', '\x2E', '\x10', '\x7D', '\xF0', '\xFF'};
      thread_local EncryptedName<sizeof(bytes), 0x01FF9313695DD9DFULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\xFA', '\xC8', '\x07', '\xA0', '\x8E', '\x4E', '\x94', '\x03', '\xF8', '\xF2', '\x18', '\xAE', '\xB8', '\x53', '\xED'};
      thread_local EncryptedName<sizeof(bytes), 0x6DED3DD1CF75AD9BULL> name(bytes);
      return name.Text();
    }()), source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\xA0', '\xB6', '\xA9', '\x18', '\xC4', '\x06', '\xBA', '\x27', '\xA2', '\x8C', '\xB6', '\x16', '\xF2', '\x1B', '\x9C', '\x2B',
                                    '\xAE', '\xA7', '\xAF', '\x18', '\xF6', '\x75'};
      thread_local EncryptedName<sizeof(bytes), 0x49C3759B77DBD3C1ULL> name(bytes);
      return name.Text();
    }()), source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
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
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\xCE', '\x2A', '\x59', '\x8C', '\x80', '\x66', '\x2A', '\x51', '\xCC', '\x10', '\x58', '\x9A', '\xB1', '\x76', '\x53'};
      thread_local EncryptedName<sizeof(bytes), 0x3F5315DFE32B4FAFULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
      const char bytes[] = {'\x15', '\xCA', '\xE2', '\x49', '\xEE', '\xBA', '\x8B', '\x48', '\x07', '\xC3', '\xEE', '\x49', '\x8B'};
      thread_local EncryptedName<sizeof(bytes), 0x17FFC98B398FAF61ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\x47', '\xF6', '\x20', '\xE5', '\xFE', '\xE6', '\x2F', '\xC4', '\x55', '\xFF', '\x2C', '\xE5', '\xC4', '\xF7', '\x34', '\xEF',
                                    '\x47', '\xFC', '\x20', '\x95'};
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

  const int variant = std::max(std::rand() % 2, 0);
  const int effectId = CEZ2STREffectMgr::GetInstance()->EffectId(variant ? std::string([] {
    const char bytes[] = {'\xF1', '\x04', '\xF9', '\x65', '\x1E', '\x3E', '\xCD', '\xCE', '\xF7', '\x17', '\xEE', '\x64', '\x0B', '\x3E', '\xD7', '\xCA',
                                  '\xE1', '\x61'};
    thread_local EncryptedName<sizeof(bytes), 0xAFA5617F178B6185ULL> name(bytes);
    return name.Text();
  }()) : std::string([] {
    const char bytes[] = {'\x39', '\x40', '\xEB', '\x3B', '\x32', '\x1C', '\xE9', '\x66', '\x3F', '\x53', '\xFC', '\x3A', '\x27', '\x1C', '\xF8', '\x62',
                                  '\x21', '\x49', '\xF6', '\x3E', '\x53'};
    thread_local EncryptedName<sizeof(bytes), 0x078143534999254DULL> name(bytes);
    return name.Text();
  }()));
  const int bottomId = CEZ2STREffectMgr::GetInstance()->EffectId(variant ? std::string([] {
    const char bytes[] = {'\x3B', '\x2E', '\xC1', '\xFD', '\x4C', '\x4A', '\x29', '\xE6', '\x3D', '\x3D', '\xD6', '\xFC', '\x59', '\x4A', '\x33', '\xE2',
                                  '\x2B', '\x14', '\xD1', '\xE0', '\x59', '\x61', '\x2E', '\xEA', '\x4F'};
    thread_local EncryptedName<sizeof(bytes), 0x8741152D8FB34B4FULL> name(bytes);
    return name.Text();
  }()) : std::string([] {
    const char bytes[] = {'\x73', '\xDE', '\x9B', '\x3F', '\x98', '\x12', '\x35', '\xF4', '\x75', '\xCD', '\x8C', '\x3E', '\x8D', '\x12', '\x24', '\xF0',
                                  '\x6B', '\xD7', '\x86', '\x3A', '\xA6', '\x2F', '\x32', '\xE1', '\x73', '\xD4', '\x84', '\x4D'};
    thread_local EncryptedName<sizeof(bytes), 0x955D4DF94DE9BB07ULL> name(bytes);
    return name.Text();
  }()));
  g_effectMgr->Spawn(target->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(target->render.accountId, bottomId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005AD630
// FUNCTION: fn_03268
void CSkillAction_AT_TERRA_WAVE::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\x7F', '\xD0', '\xFB', '\x67', '\x68', '\xE8', '\xEE', '\xA2', '\x7D', '\xD0', '\x89'};
    thread_local EncryptedName<sizeof(bytes), 0xC399B7091589B50BULL> name(bytes);
    return name.Text();
  }()), position.x, position.y, position.z, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\x1B', '\xC8', '\x53', '\x9B', '\x5E', '\x58', '\x4E', '\xD6', '\x19', '\xC8', '\x7E', '\x8B', '\x50', '\x73', '\x4D', '\xD8',
                                  '\x02', '\xAD'};
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
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\xAA', '\x56', '\x7F', '\x10', '\xAF', '\xDC', '\x0E', '\x8B', '\xB6', '\x54', '\x63', '\x79'};
      thread_local EncryptedName<sizeof(bytes), 0xFF7D83CB791339D9ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\x66', '\xFC', '\xDF', '\xA8', '\x83', '\x1C', '\x22', '\x99', '\x7A', '\xFE', '\xC3', '\x9E', '\x85', '\x2C', '\x25', '\x99',
                                    '\x7A', '\xFE', '\xB3'};
      thread_local EncryptedName<sizeof(bytes), 0xED5143E7C1B39315ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
      const char bytes[] = {'\xEB', '\xA6', '\xCD', '\x09', '\x1C', '\xE3', '\x62', '\x55', '\xF8', '\xAD', '\xD6', '\x61'};
      thread_local EncryptedName<sizeof(bytes), 0x393D916561BDC391ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\xEF', '\x98', '\x61', '\xDB', '\x28', '\x17', '\xCA', '\x5D', '\xFC', '\x93', '\x7A', '\xEC', '\x33', '\x0A', '\xE1', '\x45',
                                    '\xFA', '\x90', '\x11'};
      thread_local EncryptedName<sizeof(bytes), 0x31956551B311FD95ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005AE130
// FUNCTION: fn_03275
void CEfstAction_ZEPHYR_LINK::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\xF3', '\x58', '\x1D', '\x8D', '\xDE', '\x73', '\x78', '\x43', '\xE0', '\x53', '\x06', '\xE5'};
      thread_local EncryptedName<sizeof(bytes), 0x2F2701A7E56D3D89ULL> name(bytes);
      return name.Text();
    }()));
    effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\x5F', '\xF4', '\x77', '\xCB', '\x98', '\x73', '\x6C', '\x07', '\x4C', '\xFF', '\x6C', '\xFC', '\x83', '\x6E', '\x47', '\x1F',
                                    '\x4A', '\xFC', '\x07'};
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
      const char bytes[] = {'\xCA', '\xC7', '\xB6', '\x43', '\x48', '\x00', '\xD9', '\x02', '\xCE', '\xCF', '\xA0', '\x68', '\x27', '\x58', '\xB7'};
      thread_local EncryptedName<sizeof(bytes), 0x71B7691737C5A1AFULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\x06', '\x5F', '\x0E', '\xD5', '\x06', '\xDC', '\xF1', '\xB0', '\x02', '\x57', '\x18', '\xFE', '\x3B', '\xDA', '\xEB', '\xB7',
                                    '\x0C', '\x54', '\x22', '\x91', '\x68', '\xB5'};
      thread_local EncryptedName<sizeof(bytes), 0xC39FB559A17D3963ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
      const char bytes[] = {'\x32', '\x97', '\xC6', '\xBF', '\x5C', '\x98', '\x8D', '\x36', '\x36', '\x9F', '\xD0', '\x94', '\x33', '\xC3', '\xE3'};
      thread_local EncryptedName<sizeof(bytes), 0x45E3F103CBB5F157ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\xCE', '\x29', '\xEC', '\xD7', '\xDA', '\x46', '\x7F', '\x74', '\xCA', '\x21', '\xFA', '\xFC', '\xE7', '\x40', '\x65', '\x73',
                                    '\xC4', '\x22', '\xC0', '\x93', '\xB7', '\x2F'};
      thread_local EncryptedName<sizeof(bytes), 0x07112F85A39F4FABULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
      const char bytes[] = {'\xBA', '\x21', '\xDE', '\x13', '\x40', '\x10', '\x5D', '\x5E', '\xBE', '\x29', '\xC8', '\x38', '\x2F', '\x4A', '\x33'};
      thread_local EncryptedName<sizeof(bytes), 0x2D33791F67AD47DFULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\x90', '\xE1', '\x8E', '\x89', '\x70', '\x54', '\x01', '\xE0', '\x94', '\xE9', '\x98', '\xA2', '\x4D', '\x52', '\x1B', '\xE7',
                                    '\x9A', '\xEA', '\xA2', '\xCD', '\x1C', '\x3D'};
      thread_local EncryptedName<sizeof(bytes), 0x936F3D2FFDFD87F5ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
    const char bytes[] = {'\x9A', '\xB2', '\x4B', '\x66', '\x52', '\x4C', '\x54', '\x89', '\x9C', '\xBD', '\x5A', '\x62', '\x35'};
    thread_local EncryptedName<sizeof(bytes), 0xE50B2935073DD3E9ULL> name(bytes);
    return name.Text();
  }()), target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\x82', '\xA2', '\x9F', '\xDC', '\xCA', '\x68', '\x2A', '\x11', '\x84', '\xAD', '\x8E', '\xD8', '\xF2', '\x6F', '\x1A', '\x09',
                                  '\x85', '\xAC', '\x84', '\xBD'};
    thread_local EncryptedName<sizeof(bytes), 0x7D750DADBDE9C3F1ULL> name(bytes);
    return name.Text();
  }()), target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005AC990
// FUNCTION: fn_03264
void CSkillAction_AT_TEMPEST_FLAP::OnSkillCast(int level, CGameActor* source) {
  if (this->CanUse(source)) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const char bytes[] = {'\xCB', '\xD8', '\xB2', '\x4F', '\x28', '\x5E', '\xF7', '\x6C', '\xD9', '\xD1', '\xBE', '\x4F', '\x12', '\x4E', '\xE2', '\x40',
                                    '\xCB', '\xBD'};
      thread_local EncryptedName<sizeof(bytes), 0x33832D4D3FDFBDBFULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
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
    const char bytes[] = {'\xC7', '\x75', '\x3E', '\xA0', '\x4A', '\x8B', '\x74', '\xE2', '\xDB', '\x66', '\x20', '\xCD'};
    thread_local EncryptedName<sizeof(bytes), 0x812BE72BCD5707B7ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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

  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\x33', '\x98', '\xC7', '\xBA', '\xAB', '\xEE', '\x16', '\x5F', '\x34', '\x8A', '\xB5'};
    thread_local EncryptedName<sizeof(bytes), 0x3375B1C7DBB5FD55ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// GLOBAL: gl_25058
const float g_featherOffsets[3] = {0.0f, 0.1f, -0.1f};

// OFFSET: 0x005A4760
// FUNCTION: fn_03152
void CSkillAction_DamageWereRaptorForm::SpawnAttack00Effect(int sourceActor, CGameActor* target) {
  CGameActor* source = reinterpret_cast<CGameActor*>(sourceActor);
  if (source && target) {
    const char bytes[] = {'\x46', '\x75', '\x73', '\x0C', '\x60', '\xAE', '\xAC', '\x1F', '\x17', '\x01'};
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
    const char bytes[] = {'\xD2', '\x13', '\xB7', '\x32', '\x9A', '\xF0', '\x6A', '\x27', '\x82', '\x67'};
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
    const char bytes[] = {'\x76', '\x8D', '\x3B', '\xD8', '\x98', '\xDE', '\x70', '\xF5', '\x25', '\xF9'};
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
      if (std::max(std::rand() % 2, 0) != 0) {
        continue;
      }

      const float offset = g_featherOffsets[std::max(std::rand() % 3, 0)];
      const float step = static_cast<float>(column);
      const float x = rightX * step + baseX + offset * forwardX;
      const float y = rightY * step + baseY + offset * forwardY;
      const float z = rightZ * step + baseZ + offset * forwardZ;
      const int fade = std::max(std::rand() % 101, 0) + alpha;
      const int scale = std::max(std::rand() % 11, 7);
      if (CEffect* effect = g_effectMgr->Spawn(0, effectId, x, y, z, 0, -1, 0, 0)) {
        effect->SetFlipX(flipped);
        effect->SetDelay(fade);
        effect->SetScale(static_cast<float>(scale) * 0.1f);
      }
    }

    count += 2;
    ++row;
  }
}
