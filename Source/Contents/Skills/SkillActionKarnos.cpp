#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Camera/Camera.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/BezierArcThrowEffect.h"
#include "Contents/Effects/EZ2STREffectEx.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Effects/WhirlwindEffect.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Math/Vector3d.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <windows.h>

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

  // INLINED
  template <typename Byte>
  explicit EncryptedName(const std::array<Byte, Length>& source) : m_encrypted(true) {
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
};

class CSkillAction_Direction {
};

class CSkillAction_DamageWereRaptorForm : public CSkillAction_AnimalForm, public CSkillAction_Direction {
public:
  void SpawnAttack01Effect(int level, CGameActor* source);
  void SpawnAttack02Effect(int level, CGameActor* source);
};

class CSkillAction_DamageWereWolfForm : public CSkillAction_AnimalForm {
};

class CSkillAction_Earthing : public CSkillAction {
};

class CSkillAction_Thundering : public CSkillAction {
};

class CSkillAction_ThunderingBasic : public CSkillAction_Thundering {
};

class CSkillAction_ThunderingExpand : public CSkillAction_Thundering {
};

class CSkillAction_Icing : public CSkillAction {
};

class CSkillAction_KR_DOUBLE_SLASH : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_NASTY_SLASH : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_CLAW_WAVE : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_KR_EARTH_DRILL : public CSkillAction_Earthing {
public:
  int GetSkillAttackMotion() override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_EARTH_STAMP : public CSkillAction_Earthing {
public:
  int GetSkillAttackMotion() override;
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_KR_GROUND_BLOOM : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_THUNDERING_CALL : public CSkillAction_ThunderingBasic {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_THUNDERING_CALL_S : public CSkillAction_ThunderingExpand {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_THUNDERING_FOCUS : public CSkillAction_ThunderingBasic {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_THUNDERING_FOCUS_S : public CSkillAction_ThunderingExpand {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_THUNDERING_ORB : public CSkillAction_ThunderingBasic {
public:
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_KR_THUNDERING_ORB_S : public CSkillAction_ThunderingExpand {
public:
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_KR_ICE_SPLASH : public CSkillAction_Icing {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_ICE_PILLAR : public CSkillAction_Icing {
public:
  int GetSkillAttackMotion() override;
  int GetSkillScopeType() override;
};

class CSkillObjectAction_KR_ICE_PILLAR : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
  void OnSkillObjectRemove(CRenderObject* object) override;
};

class CSkillAction_KR_FEATHER_SPRINKLE : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_SHARPEN_GUST : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_SHARPEN_HAIL : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_IRON_HOWLING : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IRON_HOWLING : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_KR_TYPHOON_WING : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnGroundSkillEffect(vector3d position, int level, int argument5) override;
};

class CSkillAction_KR_WIND_VEIL : public CSkillAction_DamageWereRaptorForm {
public:
  int GetSkillCastMotion() override;
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
};

class CEfstAction_WIND_VEIL : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_SKID_KR_CHOP_CHOP : public CSkillAction_DamageWereWolfForm {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_CHOP_CHOP : public CEfstAction {
public:
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
};

// OFFSET: 0x005B0800
// FUNCTION: fn_03340
int CSkillAction_KR_DOUBLE_SLASH::GetSkillAttackMotion() {
  return 72;
}

// OFFSET: 0x005B0810
// FUNCTION: fn_03341
void CSkillAction_KR_DOUBLE_SLASH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const std::uint8_t effectSource[] = {0x7B, 0x5E, 0xB8, 0x85, 0x81, 0x10, 0x32, 0x6E, 0x73, 0x50, 0xBE, 0x8F, 0xED};
  thread_local EncryptedName<sizeof(effectSource), 0x1D6D75EDE7CD311FULL> effectName(effectSource);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B0A10
// FUNCTION: fn_03342
int CSkillAction_KR_NASTY_SLASH::GetSkillAttackMotion() {
  return 77;
}

// OFFSET: 0x005B0C80
// FUNCTION: fn_03344
int CSkillAction_KR_CLAW_WAVE::GetSkillCastMotion() {
  return 60;
}

// OFFSET: 0x005B0C90
// FUNCTION: fn_03345
int CSkillAction_KR_CLAW_WAVE::GetSkillAttackMotion() {
  return 71;
}

// OFFSET: 0x005B0CA0
// FUNCTION: fn_03346
void CSkillAction_KR_CLAW_WAVE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x005B0CD0
// FUNCTION: fn_03347
void CSkillAction_KR_CLAW_WAVE::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  const std::uint8_t effectSource[] = {0xD0, 0x0B, 0xA2, 0x24, 0xA6, 0xEC, 0x54, 0x61, 0xD6, 0x67};
  thread_local EncryptedName<sizeof(effectSource), 0x17359BF953C367B3ULL> effectName(effectSource);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005B0EA0
// FUNCTION: fn_03348
int CSkillAction_KR_EARTH_DRILL::GetSkillAttackMotion() {
  return 62;
}

// OFFSET: 0x005B0EB0
// FUNCTION: fn_03349
void CSkillAction_KR_EARTH_DRILL::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  const std::uint8_t effectSource[] = {0x72, 0x98, 0x3D, 0xCD, 0x93, 0xEA, 0x4B, 0xB7, 0x7E, 0x95, 0x23, 0xB9};
  thread_local EncryptedName<sizeof(effectSource), 0xC52FB5FBB94FF917ULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const std::uint8_t bottomSource[] = {0xA0, 0xAC, 0xF5, 0x81, 0xC9, 0xFC, 0x83, 0xE1, 0xAC, 0xA1, 0xEB, 0xAA, 0xC3, 0xCC, 0x93, 0xE7, 0xAA, 0xA0, 0x87};
  thread_local EncryptedName<sizeof(bottomSource), 0x93E7A3A1F587CDC5ULL> bottomName(bottomSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B1240
// FUNCTION: fn_03350
int CSkillAction_KR_EARTH_STAMP::GetSkillAttackMotion() {
  return 63;
}

// OFFSET: 0x005B1250
// FUNCTION: fn_03351
void CSkillAction_KR_EARTH_STAMP::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  const char effectSource[] = {'\xB2', '\x2E', '\x71', '\xE1', '\xB1', '\xDA', '\xDC', '\x4B', '\xB6', '\x22', '\x73', '\x95'};
  thread_local EncryptedName<sizeof(effectSource), 0x3FAF85D995034FD7ULL> effectName(effectSource);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), position.x, position.y, position.z, 0, -1, 0, 0);

  const char bottomSource[] = {'\xDC', '\x6C', '\x5D', '\xD5', '\x2B', '\xA4', '\x7C', '\x3D', '\xD8', '\x60', '\x5F', '\xFE', '\x21', '\x94', '\x7B', '\x3D', '\xD6', '\x60', '\x2F'};
  thread_local EncryptedName<sizeof(bottomSource), 0x490FFB43A12F0DB9ULL> bottomName(bottomSource);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005B1590
// FUNCTION: fn_03352
int CSkillAction_KR_GROUND_BLOOM::GetSkillAttackMotion() {
  return 0;
}

// OFFSET: 0x005B15A0
// FUNCTION: fn_03353
void CSkillAction_KR_GROUND_BLOOM::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  if (CRagEffect* effect = source->LaunchEffect(0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&source->render.position), 0, 0, 0, 0);
  }
}

// OFFSET: 0x005B1610
// FUNCTION: fn_03354
void CSkillAction_KR_GROUND_BLOOM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const char effectSource[] = {'\x16', '\x8B', '\x36', '\x80', '\xEF', '\x79', '\x3A', '\xF3', '\x1D', '\x96', '\x36', '\x98', '\x81'};
  thread_local EncryptedName<sizeof(effectSource), 0x91651D81F559F971ULL> effectName(effectSource);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const char bottomSource[] = {'\x6C', '\x09', '\xB0', '\x14', '\x25', '\x03', '\x8E', '\x11', '\x67', '\x14', '\xB0', '\x0C', '\x14', '\x05', '\xBE', '\x07',
                                       '\x7F', '\x14', '\xB2', '\x61'};
  thread_local EncryptedName<sizeof(bottomSource), 0x73D1674B61DF7B0BULL> bottomName(bottomSource);
  effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B19B0
// FUNCTION: fn_03355
void CSkillAction_KR_THUNDERING_CALL::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  int effectId;
  int bottomEffectId;
  if (level < 7) {
    const char effectSource[] = {'\x19', '\x0B', '\x40', '\x43', '\xC1', '\x66', '\xFD', '\xB0', '\x03', '\x04', '\x6A', '\x4E', '\xC4', '\x6F', '\xE3', '\x86',
                                         '\x0A', '\x3C', '\x06', '\x55', '\x96', '\x03'};
    thread_local EncryptedName<sizeof(effectSource), 0xD98F03A52D35636DULL> effectName(effectSource);
    effectId = CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text());

    const char bottomSource[] = {'\xA5', '\x71', '\xE8', '\xE1', '\x0F', '\xEE', '\x7D', '\x66', '\xBF', '\x7E', '\xC2', '\xEC', '\x0A', '\xE7', '\x63', '\x50',
                                         '\xB6', '\x46', '\xAE', '\xF7', '\x58', '\xD4', '\x6D', '\x60', '\xA5', '\x6D', '\xF2', '\xE2', '\x6B'};
    thread_local EncryptedName<sizeof(bottomSource), 0x0F0F8B6B8F9D19D1ULL> bottomName(bottomSource);
    bottomEffectId = CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text());
  } else {
    const char effectSource[] = {'\x7D', '\xC1', '\x32', '\x31', '\xBD', '\x38', '\x07', '\xBA', '\x67', '\xCE', '\x18', '\x3C', '\xB8', '\x31', '\x19', '\x8C',
                                         '\x6E', '\xF6', '\x72', '\x27', '\xEC', '\x5D'};
    thread_local EncryptedName<sizeof(effectSource), 0xD3755DD95F47A909ULL> effectName(effectSource);
    effectId = CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text());

    const char bottomSource[] = {'\x19', '\x8B', '\x30', '\xCD', '\x89', '\xA4', '\xA5', '\x58', '\x03', '\x84', '\x1A', '\xC0', '\x8C', '\xAD', '\xBB', '\x6E',
                                         '\x0A', '\xBC', '\x70', '\xDB', '\xD8', '\x9E', '\xB5', '\x5E', '\x19', '\x97', '\x2A', '\xCE', '\xED'};
    thread_local EncryptedName<sizeof(bottomSource), 0x31D7C1EDA345E36DULL> bottomName(bottomSource);
    bottomEffectId = CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text());
  }

  g_effectMgr->Spawn(target->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(target->render.accountId, bottomEffectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B2010
// FUNCTION: fn_03356
void CSkillAction_KR_THUNDERING_CALL_S::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  int effectId;
  int bottomEffectId;
  if (level < 7) {
    const char effectSource[] = {'\x05', '\x73', '\x86', '\x91', '\xE3', '\x40', '\xE3', '\xAE', '\x1F', '\x7C', '\xAC', '\x9C', '\xE6', '\x49', '\xFD', '\x98',
                                         '\x08', '\x44', '\xC0', '\x87', '\xB4', '\x25'};
    thread_local EncryptedName<sizeof(effectSource), 0xC7912587FFF31B71ULL> effectName(effectSource);
    effectId = CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text());

    const char bottomSource[] = {'\x19', '\x5B', '\x7A', '\xFB', '\xBF', '\x20', '\xA1', '\x00', '\x03', '\x54', '\x50', '\xF6', '\xBA', '\x29', '\xBF', '\x36',
                                         '\x14', '\x6C', '\x3C', '\xED', '\xE8', '\x1A', '\xB1', '\x06', '\x19', '\x47', '\x60', '\xF8', '\xDB'};
    thread_local EncryptedName<sizeof(bottomSource), 0x69D345DB950F336DULL> bottomName(bottomSource);
    bottomEffectId = CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text());
  } else {
    const char effectSource[] = {'\x5B', '\xF7', '\xC6', '\xB5', '\x8B', '\xAC', '\x65', '\xF2', '\x41', '\xF8', '\xEC', '\xB8', '\x8E', '\xA5', '\x7B', '\xC4',
                                         '\x56', '\xC0', '\x86', '\xA3', '\xDA', '\xC9'};
    thread_local EncryptedName<sizeof(effectSource), 0x9B17C9EFDBB39F2FULL> effectName(effectSource);
    effectId = CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text());

    const char bottomSource[] = {'\xA9', '\xBD', '\x3C', '\x2F', '\x13', '\x70', '\x49', '\x44', '\xB3', '\xB2', '\x16', '\x22', '\x16', '\x79', '\x57', '\x72',
                                         '\xA4', '\x8A', '\x7C', '\x39', '\x42', '\x4A', '\x59', '\x42', '\xA9', '\xA1', '\x26', '\x2C', '\x77'};
    thread_local EncryptedName<sizeof(bottomSource), 0x2D3B15774149D5DDULL> bottomName(bottomSource);
    bottomEffectId = CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text());
  }

  g_effectMgr->Spawn(target->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(target->render.accountId, bottomEffectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B2670
// FUNCTION: fn_03357
int CSkillAction_KR_THUNDERING_FOCUS::GetSkillAttackMotion() {
  return 65;
}

// OFFSET: 0x005B2680
// FUNCTION: fn_03358
void CSkillAction_KR_THUNDERING_FOCUS::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const char effectSource[] = {'\x95', '\xF3', '\x86', '\x15', '\x89', '\xE2', '\x8D', '\x08', '\x8F', '\xFC', '\xAC', '\x1D', '\x82', '\xE4', '\x8A', '\x12', '\xBE', '\xFC', '\xF3'};
  thread_local EncryptedName<sizeof(effectSource), 0x61FF87ED7BF39BE1ULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B2880
// FUNCTION: fn_03359
int CSkillAction_KR_THUNDERING_FOCUS_S::GetSkillAttackMotion() {
  return 65;
}

// OFFSET: 0x005B2890
// FUNCTION: fn_03360
void CSkillAction_KR_THUNDERING_FOCUS_S::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const char effectSource[] = {'\x8B', '\xED', '\x52', '\x63', '\x01', '\x5A', '\xEB', '\x14', '\x91', '\xE2', '\x78', '\x6B', '\x0A', '\x5C', '\xEC', '\x0E', '\xA0', '\xFC', '\x27'};
  thread_local EncryptedName<sizeof(effectSource), 0x7D993F650D2785FFULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B2A90
// FUNCTION: fn_03361
void CSkillAction_KR_THUNDERING_ORB::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  CGameActor* const target = reinterpret_cast<CGameActor*>(argument3);
  if (!target) {
    return;
  }

  if (CEffect* orb = g_effectMgr->Spawn(target->render.accountId, 2441, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    static_cast<CBezierArcThrowEffect*>(orb)->SetTarget(position);
  }

  const char effectSource[] = {'\x47', '\x4F', '\x46', '\x61', '\xD9', '\x28', '\x4B', '\x44', '\x5D', '\x40', '\x6C', '\x60', '\xCF', '\x2F', '\x66', '\x4A', '\x33'};
  thread_local EncryptedName<sizeof(effectSource), 0x2D394DBD0F332733ULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), position.x, position.y, position.z, 0, -1, 0, 0)) {
    effect->SetDelay(300);
  }

  const char bottomSource[] = {'\x33', '\x49', '\xCE', '\x85', '\x8F', '\x12', '\x6D', '\x7C', '\x29', '\x46', '\xE4', '\x84', '\x99', '\x15', '\x40', '\x72',
                                       '\x18', '\x43', '\xD4', '\x9F', '\x9F', '\x18', '\x72', '\x15'};
  thread_local EncryptedName<sizeof(bottomSource), 0x151F77EBEBBB2147ULL> bottomName(bottomSource);
  if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), position.x, position.y, position.z, 0, -1, 0, 0)) {
    effect->SetDelay(300);
  }
}

// OFFSET: 0x005B2E80
// FUNCTION: fn_03362
void CSkillAction_KR_THUNDERING_ORB_S::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  CGameActor* const target = reinterpret_cast<CGameActor*>(argument3);
  if (!target) {
    return;
  }

  if (CEffect* orb = g_effectMgr->Spawn(target->render.accountId, 2441, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    static_cast<CBezierArcThrowEffect*>(orb)->SetTarget(position);
  }

  const char effectSource[] = {'\xB5', '\xA7', '\xA6', '\x5D', '\xF3', '\x78', '\x8F', '\x00', '\xAF', '\xA8', '\x8C', '\x5C', '\xE5', '\x7F', '\xA2', '\x10', '\xC1'};
  thread_local EncryptedName<sizeof(effectSource), 0x69FD1D9733D3CFC1ULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), position.x, position.y, position.z, 0, -1, 0, 0)) {
    effect->SetDelay(300);
  }

  const char bottomSource[] = {'\xB3', '\xA5', '\x90', '\x53', '\x7B', '\x18', '\x17', '\x9C', '\xA9', '\xAA', '\xBA', '\x52', '\x6D', '\x1F', '\x3A', '\x8C',
                                       '\x98', '\xAF', '\x8A', '\x49', '\x6B', '\x12', '\x08', '\xF5'};
  thread_local EncryptedName<sizeof(bottomSource), 0xF5657D1F3DE5CDC7ULL> bottomName(bottomSource);
  if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), position.x, position.y, position.z, 0, -1, 0, 0)) {
    effect->SetDelay(300);
  }
}

// OFFSET: 0x005B3270
// FUNCTION: fn_03363
void CSkillAction_KR_ICE_SPLASH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const std::uint8_t effectSource[] = {0x58, 0x3A, 0x4E, 0x5E, 0x12, 0xBD, 0x09, 0xEA, 0x42, 0x31, 0x2B};
  thread_local EncryptedName<sizeof(effectSource), 0x8B65CD61012B5931ULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B3470
// FUNCTION: fn_03364
int CSkillAction_KR_ICE_PILLAR::GetSkillScopeType() {
  return 7;
}

// OFFSET: 0x005B3480
// FUNCTION: fn_03365
int CSkillAction_KR_ICE_PILLAR::GetSkillAttackMotion() {
  return 65;
}

// OFFSET: 0x005B3490
// FUNCTION: fn_03366
void CSkillObjectAction_KR_ICE_PILLAR::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  if (!object) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\x8C', '\xEC', '\x28', '\x4A', '\x47', '\x68', '\x3F', '\x9B', '\x84', '\xFD', '\x12', '\x66', '\x43', '\x60', '\x21', '\x83', '\xE5'};
    thread_local EncryptedName<sizeof(bytes), 0xF7530137154D8FE5ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = {'\xAA', '\xA0', '\xB0', '\xD4', '\x2B', '\x82', '\x53', '\xE7', '\xA2', '\xB1', '\x8A', '\xF8', '\x2F', '\x8A', '\x4D', '\xFF',
                                  '\x9C', '\xA1', '\xBA', '\xFF', '\x2F', '\x84', '\x52', '\x8B'};
    thread_local EncryptedName<sizeof(bytes), 0x8B3FEB5B8BD5C3C3ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  if (CEffect* effect = g_effectMgr->Spawn(object->render.accountId, 2472, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    static_cast<EffectResourceSwitcher*>(effect)->SetNames([] {
      const char bytes[] = {'\xCE', '\xD0', '\x60', '\x96', '\xD5', '\x0A', '\x87', '\x67', '\xC6', '\xC1', '\x5A', '\xA5', '\xCA', '\x0C', '\x9B', '\x0B'};
      thread_local EncryptedName<sizeof(bytes), 0x0BEB63A5C905B3A7ULL> name(bytes);
      return name.Text();
    }(), [] {
      const char bytes[] = {'\xA0', '\xD4', '\x40', '\xC8', '\xCB', '\x5A', '\xEF', '\x6B', '\xA8', '\xC5', '\x7A', '\xF2', '\xD5', '\x57', '\x83'};
      thread_local EncryptedName<sizeof(bytes), 0x078333BB9725B7C9ULL> name(bytes);
      return name.Text();
    }());
    effect->SetDelay(990);
  }
  if (CEffect* effect = g_effectMgr->Spawn(object->render.accountId, 2473, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    static_cast<EffectResourceSwitcher*>(effect)->SetNames([] {
      const char bytes[] = {'\xDC', '\x16', '\x04', '\x00', '\x8D', '\x32', '\x77', '\xE3', '\xD4', '\x07', '\x3E', '\x33', '\x92', '\x34', '\x6B', '\xD0',
                                    '\xD7', '\x1A', '\x15', '\x2B', '\x92', '\x36', '\x1B'};
      thread_local EncryptedName<sizeof(bytes), 0x8F1B5BFD5F6175B5ULL> name(bytes);
      return name.Text();
    }(), [] {
      const char bytes[] = {'\x72', '\x6E', '\x5C', '\x56', '\xDF', '\x20', '\xE3', '\x37', '\x7A', '\x7F', '\x66', '\x6C', '\xC1', '\x2D', '\xD0', '\x39',
                                    '\x74', '\x79', '\x4D', '\x66', '\xC2', '\x49'};
      thread_local EncryptedName<sizeof(bytes), 0x5B8F49AF09390D1BULL> name(bytes);
      return name.Text();
    }());
    effect->SetDelay(990);
  }
}

// OFFSET: 0x005B3E30
// FUNCTION: fn_03367
void CSkillObjectAction_KR_ICE_PILLAR::OnSkillObjectRemove(CRenderObject* object) {
  if (!object) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Find(object->render.accountId, 2472)) {
    static_cast<EffectResourceSwitcher*>(effect)->Start();
  }
  if (CEffect* effect = g_effectMgr->Find(object->render.accountId, 2473)) {
    static_cast<EffectResourceSwitcher*>(effect)->Start();
  }
}

// OFFSET: 0x005B3E90
// FUNCTION: fn_03368
int CSkillAction_KR_FEATHER_SPRINKLE::GetSkillAttackMotion() {
  return 68;
}

// OFFSET: 0x005B4240
// FUNCTION: fn_03370
void CSkillAction_KR_FEATHER_SPRINKLE::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  this->SpawnAttack01Effect(level, source);
}

// OFFSET: 0x005B4260
// FUNCTION: fn_03371
int CSkillAction_KR_SHARPEN_GUST::GetSkillCastMotion() {
  return 60;
}

// OFFSET: 0x005B4270
// FUNCTION: fn_03372
int CSkillAction_KR_SHARPEN_GUST::GetSkillAttackMotion() {
  return 78;
}

// OFFSET: 0x005B4280
// FUNCTION: fn_03373
void CSkillAction_KR_SHARPEN_GUST::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x005B46D0
// FUNCTION: fn_03375
void CSkillAction_KR_SHARPEN_GUST::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  this->SpawnAttack01Effect(level, source);
  this->SpawnAttack02Effect(level, source);
}

// OFFSET: 0x005B4700
// FUNCTION: fn_03376
int CSkillAction_KR_SHARPEN_HAIL::GetSkillCastMotion() {
  return 60;
}

// OFFSET: 0x005B4710
// FUNCTION: fn_03377
int CSkillAction_KR_SHARPEN_HAIL::GetSkillAttackMotion() {
  return 79;
}

// OFFSET: 0x005B4720
// FUNCTION: fn_03378
void CSkillAction_KR_SHARPEN_HAIL::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  static_cast<EffectActor*>(source)->LaunchEffect(56, level / 16);

  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const char bytes[] = {'\x6A', '\x05', '\xAE', '\x8D', '\x2D', '\x64', '\xAF', '\x20', '\x71', '\x0C', '\xA6', '\x93', '\x5D'};
        thread_local EncryptedName<sizeof(bytes), 0x7FC1015DFFCF6D19ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetLifeTime(level);
  }
  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const char bytes[] = {'\x50', '\xEB', '\xBA', '\xF1', '\x31', '\xD8', '\x51', '\x94', '\x4B', '\xE2', '\xB2', '\xEF', '\x1E', '\xDF', '\x50', '\xBF',
                                      '\x57', '\xEC', '\xB6', '\x83'};
        thread_local EncryptedName<sizeof(bytes), 0xCB3FBD4183DB8323ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x005B4AF0
// FUNCTION: fn_03379
void CSkillAction_KR_SHARPEN_HAIL::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const int shotEffect = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t encrypted[] = {0x3E, 0xB9, 0xAA, 0xB5, 0x89, 0x12, 0xA3, 0x20, 0x25, 0xB0, 0xA2, 0xAB, 0xA6, 0x04, 0xA5, 0x10, 0x39, 0xD1};
    thread_local EncryptedName<sizeof(encrypted), 0x7FCD77F9C7CBD14DULL> name(encrypted);
    return name.Text();
  }());
  const int bottomEffect = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t encrypted[] = {0x72, 0x3F, 0xF6, 0xAD, 0x63, 0xE0, 0x3D, 0x8E, 0x69, 0x36, 0xFE, 0xB3, 0x4C,
                                      0xF6, 0x3B, 0xBE, 0x75, 0x08, 0xF5, 0xB0, 0x67, 0xF1, 0x3C, 0xBC, 0x01};
    thread_local EncryptedName<sizeof(encrypted), 0xD1538513DF975701ULL> name(encrypted);
    return name.Text();
  }());

  for (int delay = 0; delay < 1000; delay += 200) {
    int count = rand() % 8;
    if (count < 5) {
      count = 5;
    }

    for (int index = 0; index < count; ++index) {
      const int startTime = MulDiv(index, 200, count) + delay;
      int flip = rand() % 2;
      if (flip < 0) {
        flip = 0;
      }

      int offsetY = rand() % 6;
      if (offsetY < 0) {
        offsetY = 0;
      }

      int offsetX = rand() % 6;
      if (offsetX < 0) {
        offsetX = 0;
      }

      if (flip) {
        offsetX = -offsetX;
        offsetY = -offsetY;
      }

      const vector3d position{
        target->render.position.x + static_cast<float>(offsetX * 4),
        target->render.position.y + static_cast<float>(offsetY * 4),
        target->render.position.z + 0.0f,
      };

      CEffect* shot = g_effectMgr->Spawn(0, shotEffect, position.x, position.y, position.z, 0, -1, 0, 0);
      if (shot) {
        shot->SetDelay(startTime);
      }

      CEffect* bottom = g_effectMgr->Spawn(0, bottomEffect, position.x, position.y, position.z, 0, -1, 0, 0);
      if (bottom) {
        bottom->SetDelay(startTime);
      }
    }
  }
}

// OFFSET: 0x005B4F70
// FUNCTION: fn_03380
void CSkillAction_KR_SHARPEN_HAIL::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  this->SpawnAttack01Effect(level, source);
}

// OFFSET: 0x005B4F90
// FUNCTION: fn_03381
int CSkillAction_KR_IRON_HOWLING::GetSkillCastMotion() {
  return 66;
}

// OFFSET: 0x005B4FA0
// FUNCTION: fn_03382
int CSkillAction_KR_IRON_HOWLING::GetSkillAttackMotion() {
  return 68;
}

// OFFSET: 0x005B4FB0
// FUNCTION: fn_03383
void CSkillAction_KR_IRON_HOWLING::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x005B51F0
// FUNCTION: fn_03385
EfstActionMessage CEfstAction_IRON_HOWLING::GetEfstBeginMessage() {
  return {LookupMessage(4662), 0xFFFF00};
}

// OFFSET: 0x005B5220
// FUNCTION: fn_03386
EfstActionMessage CEfstAction_IRON_HOWLING::GetEfstEndMessage() {
  return {LookupMessage(4663), 0xFF};
}

// OFFSET: 0x005B5250
// FUNCTION: fn_03387
int CSkillAction_KR_TYPHOON_WING::GetSkillCastMotion() {
  return 66;
}

// OFFSET: 0x005B5260
// FUNCTION: fn_03388
int CSkillAction_KR_TYPHOON_WING::GetSkillAttackMotion() {
  return 67;
}

// OFFSET: 0x005B5270
// FUNCTION: fn_03389
void CSkillAction_KR_TYPHOON_WING::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x005B52A0
// FUNCTION: fn_03390
void CSkillAction_KR_TYPHOON_WING::OnGroundSkillEffect(vector3d position, int level, int argument5) {
  const int effectId = CEZ2STREffectMgr::GetInstance()->EffectId(
    level < 6 ? std::string([] {
      const char source[] = {'\x67', '\xBE', '\x73', '\x7D', '\x94', '\xFE', '\x79', '\x10', '\x64', '\xAE', '\x6D', '\x72', '\xA4', '\xA4', '\x6F', '\x7A', '\x13'};
      thread_local EncryptedName<sizeof(source), 0x4F1791FB1503C713ULL> name(source);
      return name.Text();
    }())
              : std::string([] {
      const char source[] = {'\xA7', '\x94', '\xE7', '\x7D', '\x22', '\x34', '\xCB', '\xFE', '\xA4', '\x84', '\xF9', '\x72', '\x12', '\x6C', '\xDD', '\x96', '\xD3'};
      thread_local EncryptedName<sizeof(source), 0xA1A55B4D1597EDD3ULL> name(source);
      return name.Text();
    }())
  );
  const int bottomId = CEZ2STREffectMgr::GetInstance()->EffectId(
    level < 6 ? std::string([] {
      const char source[] = {'\x89', '\x84', '\x0D', '\x9B', '\x8E', '\x7A', '\x97', '\x44', '\x8A', '\x94', '\x13', '\x94',
                                     '\xBE', '\x20', '\x81', '\x2E', '\xA2', '\x9F', '\x12', '\x87', '\x95', '\x7A', '\x94', '\x1B'};
      thread_local EncryptedName<sizeof(source), 0x1BF915E1F37DFDFDULL> name(source);
      return name.Text();
    }())
              : std::string([] {
      const char source[] = {'\xAB', '\xA0', '\x2D', '\x01', '\x7C', '\xFC', '\x91', '\x5E', '\xA8', '\xB0', '\x33', '\x0E',
                                     '\x4C', '\xA4', '\x87', '\x36', '\x80', '\xBB', '\x32', '\x1D', '\x67', '\xFC', '\x92', '\x01'};
      thread_local EncryptedName<sizeof(source), 0x01FF9313695DD9DFULL> name(source);
      return name.Text();
    }())
  );

  g_effectMgr->Spawn(0, effectId, position.x, position.y, position.z, 0, -1, 0, 0);
  g_effectMgr->Spawn(0, bottomId, position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005B58A0
// FUNCTION: fn_03391
int CSkillAction_KR_WIND_VEIL::GetSkillCastMotion() {
  return 66;
}

// OFFSET: 0x005B58B0
// FUNCTION: fn_03392
int CSkillAction_KR_WIND_VEIL::GetSkillAttackMotion() {
  return 68;
}

// OFFSET: 0x005B58C0
// FUNCTION: fn_03393
void CSkillAction_KR_WIND_VEIL::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x005B58F0
// FUNCTION: fn_03394
EfstActionMessage CEfstAction_WIND_VEIL::GetEfstBeginMessage() {
  return {LookupMessage(4664), 0xFFFF00};
}

// OFFSET: 0x005B5920
// FUNCTION: fn_03395
EfstActionMessage CEfstAction_WIND_VEIL::GetEfstEndMessage() {
  return {LookupMessage(4665), 0xFF};
}

// OFFSET: 0x005B5950
// FUNCTION: fn_03396
void CEfstAction_WIND_VEIL::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (!actor) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char source[] = {'\xEC', '\xC4', '\x19', '\xAB', '\x8E', '\x4B', '\x88', '\x04', '\xF7', '\xAD'};
    thread_local EncryptedName<sizeof(source), 0x6DED3DD1CF75AD9BULL> name(source);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char source[] = {'\xB6', '\xBA', '\xB7', '\x13', '\xC4', '\x03', '\xA6', '\x20', '\xAD', '\x8C', '\xB9', '\x18', '\xEF', '\x01', '\xAC', '\x24', '\xC1'};
    thread_local EncryptedName<sizeof(source), 0x49C3759B77DBD3C1ULL> name(source);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B5CE0
// FUNCTION: fn_03397
void CEfstAction_WIND_VEIL::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::array<char, 10> source = {'\xD8', '\x26', '\x47', '\x87', '\x80', '\x63', '\x36', '\x56', '\xC3', '\x4F'};
    thread_local EncryptedName<sizeof(source), 0x3F5315DFE32B4FAFULL> name(source);
    return name.Text();
  }()));
  effects = g_effectMgr;
  effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::array<char, 17> source = {'\x9E', '\xBA', '\x51', '\x63', '\x6A', '\x5F', '\x6E', '\x8C', '\x85', '\x8C', '\x5F', '\x68', '\x41', '\x5D', '\x64', '\x88', '\xE9'};
    thread_local EncryptedName<sizeof(source), 0xE50B2935073DD3E9ULL> name(source);
    return name.Text();
  }()));
}

// OFFSET: 0x005B6010
// FUNCTION: fn_03398
void CEfstAction_WIND_VEIL::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    this->OnEfstBegin(efstId, value, 0, 0, actor, false);
  }
}

// OFFSET: 0x005B6030
// FUNCTION: fn_03399
int CSkillAction_SKID_KR_CHOP_CHOP::GetSkillAttackMotion() {
  return 75;
}

// OFFSET: 0x005B6330
// FUNCTION: fn_03401
void CEfstAction_CHOP_CHOP::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  while (CEffect* effect = g_effectMgr->Find(actor->render.accountId, 2466)) {
    g_effectMgr->Retire(effect, 1);
  }
  while (CEffect* effect = g_effectMgr->Find(actor->render.accountId, 2444)) {
    g_effectMgr->Retire(effect, 1);
  }
  for (;;) {
    EffectLists* effects = g_effectMgr;
    CEffect* effect = effects->Find(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t source[] = {0xDC, 0xD5, 0xB0, 0x4F, 0x12, 0x4E, 0xEB, 0x5C, 0xCF, 0xE2, 0xAC, 0x4A, 0x2F, 0x2D};
      thread_local EncryptedName<sizeof(source), 0x33832D4D3FDFBDBFULL> name(source);
      return name.Text();
    }()));
    if (!effect) {
      break;
    }
    g_effectMgr->Retire(effect, 1);
  }
}

// OFFSET: 0x005B6A70
// FUNCTION: fn_03403
void CWhirlwindEffect::Init(EffectActor* parent, int id, vector3d deltaPos) {
  if (parent && !parent->HasId(0x98C)) {
    parent->render.instant.Values()[25] = static_cast<std::int64_t>(parent->render.angle * 1000.0f);
  }

  this->CRagEffect::Init(parent, id, deltaPos);
}

// OFFSET: 0x005B6BD0
// FUNCTION: fn_03404
bool CWhirlwindEffect::OnProcess() {
  if (!this->state.parent) {
    return false;
  }

  this->render.position.x = this->state.parent->render.position.x;
  this->render.position.y = this->state.parent->render.position.y;
  this->render.position.z = this->state.parent->render.position.z;

  const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  const int remaining = std::max(static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(this->endTime - now).count()), 0);
  this->state.parent->render.angle = static_cast<float>(MulDiv(remaining % this->duration, 360, this->duration));
  if (now < this->endTime) {
    return true;
  }

  EffectActor* parent = this->state.parent;
  const std::map<int, std::int64_t>& values = parent->render.instant.Values();
  const auto entry = values.find(25);
  const std::int64_t angle = entry == values.end() ? 0 : entry->second;
  parent->render.angle = static_cast<float>(static_cast<int>(angle)) / 1000.0f;
  return false;
}

// OFFSET: 0x005B6D00
// FUNCTION: fn_03405
__declspec(noinline) void CWhirlwindEffect::SetLifetime(int milliseconds) {
  this->endTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
}

// OFFSET: 0x005B6D50
// FUNCTION: fn_03406
__declspec(noinline) void CWhirlwindEffect::SetDuration(int duration) {
  this->duration = duration;
}

// OFFSET: 0x005B0A20
// FUNCTION: fn_03343
void CSkillAction_KR_NASTY_SLASH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source) || !target) {
    return;
  }

  const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
  const std::uint8_t effectSource[] = {0x49, 0x60, 0x74, 0x19, 0x7A, 0x9A, 0x80, 0x43, 0x46, 0x72, 0x6F, 0x6D};
  thread_local EncryptedName<sizeof(effectSource), 0x2FF3C5036D070127ULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetAngle(angle);
    effect->SetPitch(25.0f);
  }
}

// OFFSET: 0x005B3EA0
// FUNCTION: fn_03369
void CSkillAction_KR_FEATHER_SPRINKLE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  const std::uint8_t effectSource[] = {0x89, 0x46, 0xB4, 0x9B, 0x71, 0x34, 0xE3, 0x64, 0x9C, 0x53, 0xA7, 0x86, 0x77, 0x3A, 0xFD, 0x5E, 0xEF};
  thread_local EncryptedName<sizeof(effectSource), 0x3B915119EFD523EFULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const std::uint8_t bottomSource[] = {0x31, 0xE6, 0x88, 0xA1, 0xE9, 0xA0, 0x3F, 0x18, 0x24, 0xF3, 0x9B, 0xBC, 0xEF, 0xAE, 0x21, 0x22, 0x08, 0xE1, 0x86, 0xA1, 0xF5, 0xAA, 0x20, 0x47};
  thread_local EncryptedName<sizeof(bottomSource), 0x474DC581D5E98357ULL> bottomName(bottomSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B42B0
// FUNCTION: fn_03374
void CSkillAction_KR_SHARPEN_GUST::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source) || !target) {
    return;
  }

  const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
  const char effectSource[] = {'\x1E', '\x37', '\x98', '\x41', '\x93', '\x1E', '\x3B', '\x60', '\x0A', '\x2A', '\x8A', '\x47', '\xE3'};
  thread_local EncryptedName<sizeof(effectSource), 0x3F557BE333F95F6DULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetAngle(angle);
    effect->SetPitch(30.0f);
  }

  const char bottomSource[] = {'\x4A', '\x89', '\xC8', '\x9D', '\x91', '\xC2', '\x3D', '\x58', '\x5E', '\x94', '\xDA', '\x9B', '\xBE', '\xC5', '\x3C', '\x73', '\x4D', '\x8E', '\xC4', '\xEF'};
  thread_local EncryptedName<sizeof(bottomSource), 0x0753A7E1EFA9E139ULL> bottomName(bottomSource);
  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetAngle(angle);
    effect->SetPitch(30.0f);
  }
}

// OFFSET: 0x005B4FE0
// FUNCTION: fn_03384
void CSkillAction_KR_IRON_HOWLING::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  const std::uint8_t effectSource[] = {0xF6, 0xB1, 0x28, 0xD3, 0xB4, 0x2F, 0x8E, 0xE6, 0xF3, 0xAA, 0x29, 0xDA, 0xEB};
  thread_local EncryptedName<sizeof(effectSource), 0x91E147EBBD47C39FULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B6040
// FUNCTION: fn_03400
void CSkillAction_SKID_KR_CHOP_CHOP::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 2466, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->effectLifetime = 1;
  }

  const std::uint8_t effectSource[] = {0x92, 0xAB, 0x86, 0xCD, 0xF2, 0x6E, 0x1D, 0x12, 0x81, 0x9C, 0x9A, 0xC8, 0xCF, 0x0D};
  thread_local EncryptedName<sizeof(effectSource), 0x7D750DADBDE9C3F1ULL> effectName(effectSource);
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetAngle(static_cast<float>(std::max(0, rand() % 360)));
  }

  if (CRagEffect* effect = source->LaunchEffect(2444, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    static_cast<CWhirlwindEffect*>(effect)->SetLifetime(1000);
    static_cast<CWhirlwindEffect*>(effect)->SetDuration(500);
  }
}
