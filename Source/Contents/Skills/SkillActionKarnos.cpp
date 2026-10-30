#include "Client/Mode/CGameMode.h"
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
};

class CSkillAction_Direction {
};

class CSkillAction_DamageWereRaptorForm : public CSkillAction_AnimalForm, public CSkillAction_Direction {
public:
  void sub_5A4930(int level, CGameActor* source);
  void sub_5A4B00(int level, CGameActor* source);
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
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_NASTY_SLASH : public CSkillAction_DamageWereWolfForm {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_CLAW_WAVE : public CSkillAction_DamageWereWolfForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot12(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_KR_EARTH_DRILL : public CSkillAction_Earthing {
public:
  int Slot02() override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_EARTH_STAMP : public CSkillAction_Earthing {
public:
  int Slot02() override;
  void Slot12(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_KR_GROUND_BLOOM : public CSkillAction {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_THUNDERING_CALL : public CSkillAction_ThunderingBasic {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_THUNDERING_CALL_S : public CSkillAction_ThunderingExpand {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_THUNDERING_FOCUS : public CSkillAction_ThunderingBasic {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_THUNDERING_FOCUS_S : public CSkillAction_ThunderingExpand {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_THUNDERING_ORB : public CSkillAction_ThunderingBasic {
public:
  void Slot12(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_KR_THUNDERING_ORB_S : public CSkillAction_ThunderingExpand {
public:
  void Slot12(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_KR_ICE_SPLASH : public CSkillAction_Icing {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_ICE_PILLAR : public CSkillAction_Icing {
public:
  int Slot02() override;
  int Slot14() override;
};

class CSkillObjectAction_KR_ICE_PILLAR : public CSkillObjectAction {
public:
  void Slot00(int argument, CRenderObject* object, CGameActor* source) override;
  void Slot01(CRenderObject* object) override;
};

class CSkillAction_KR_FEATHER_SPRINKLE : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_SHARPEN_GUST : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_SHARPEN_HAIL : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_IRON_HOWLING : public CSkillAction_DamageWereWolfForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_IRON_HOWLING : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_KR_TYPHOON_WING : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot12(vector3d position, int level, int argument5) override;
};

class CSkillAction_KR_WIND_VEIL : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
};

class CEfstAction_WIND_VEIL : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_SKID_KR_CHOP_CHOP : public CSkillAction_DamageWereWolfForm {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_CHOP_CHOP : public CEfstAction {
public:
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
};

// OFFSET: 0x005B0800
// FUNCTION: fn_03340
int CSkillAction_KR_DOUBLE_SLASH::Slot02() {
  return 72;
}

// OFFSET: 0x005B0810
// FUNCTION: fn_03341
void CSkillAction_KR_DOUBLE_SLASH::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const std::uint8_t effectSource[] = {0x7B, 0x5E, 0xB8, 0x85, 0x81, 0x10, 0x32, 0x6E, 0x73, 0x50, 0xBE, 0x8F, 0xED};
  thread_local EncryptedName<sizeof(effectSource), 0x1D6D75EDE7CD311FULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B0A10
// FUNCTION: fn_03342
int CSkillAction_KR_NASTY_SLASH::Slot02() {
  return 77;
}

// OFFSET: 0x005B0C80
// FUNCTION: fn_03344
int CSkillAction_KR_CLAW_WAVE::Slot01() {
  return 60;
}

// OFFSET: 0x005B0C90
// FUNCTION: fn_03345
int CSkillAction_KR_CLAW_WAVE::Slot02() {
  return 71;
}

// OFFSET: 0x005B0CA0
// FUNCTION: fn_03346
void CSkillAction_KR_CLAW_WAVE::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x005B0CD0
// FUNCTION: fn_03347
void CSkillAction_KR_CLAW_WAVE::Slot12(vector3d position, int argument2, int argument3) {
  const std::uint8_t effectSource[] = {0xD0, 0x0B, 0xA2, 0x24, 0xA6, 0xEC, 0x54, 0x61, 0xD6, 0x67};
  thread_local EncryptedName<sizeof(effectSource), 0x17359BF953C367B3ULL> effectName(effectSource);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005B0EA0
// FUNCTION: fn_03348
int CSkillAction_KR_EARTH_DRILL::Slot02() {
  return 62;
}

// OFFSET: 0x005B0EB0
// FUNCTION: fn_03349
void CSkillAction_KR_EARTH_DRILL::Slot15(int level, CGameActor* source, CGameActor* target) {
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
int CSkillAction_KR_EARTH_STAMP::Slot02() {
  return 63;
}

// OFFSET: 0x005B1250
// FUNCTION: fn_03351
void CSkillAction_KR_EARTH_STAMP::Slot12(vector3d position, int argument2, int argument3) {
  const std::uint8_t effectSource[] = {0xB2, 0x2E, 0x71, 0xE1, 0xB1, 0xDA, 0xDC, 0x4B, 0xB6, 0x22, 0x73, 0x95};
  thread_local EncryptedName<sizeof(effectSource), 0x3FAF85D995034FD7ULL> effectName(effectSource);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), position.x, position.y, position.z, 0, -1, 0, 0);

  const std::uint8_t bottomSource[] = {0xDC, 0x6C, 0x5D, 0xD5, 0x2B, 0xA4, 0x7C, 0x3D, 0xD8, 0x60, 0x5F, 0xFE, 0x21, 0x94, 0x7B, 0x3D, 0xD6, 0x60, 0x2F};
  thread_local EncryptedName<sizeof(bottomSource), 0x490FFB43A12F0DB9ULL> bottomName(bottomSource);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005B1590
// FUNCTION: fn_03352
int CSkillAction_KR_GROUND_BLOOM::Slot02() {
  return 0;
}

// OFFSET: 0x005B15A0
// FUNCTION: fn_03353
void CSkillAction_KR_GROUND_BLOOM::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  if (CRagEffect* effect = source->LaunchEffect(0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&source->render.position), 0, 0, 0, 0);
  }
}

// OFFSET: 0x005B1610
// FUNCTION: fn_03354
void CSkillAction_KR_GROUND_BLOOM::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const std::uint8_t effectSource[] = {0x16, 0x8B, 0x36, 0x80, 0xEF, 0x79, 0x3A, 0xF3, 0x1D, 0x96, 0x36, 0x98, 0x81};
  thread_local EncryptedName<sizeof(effectSource), 0x91651D81F559F971ULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  const std::uint8_t bottomSource[] = {0x6C, 0x09, 0xB0, 0x14, 0x25, 0x03, 0x8E, 0x11, 0x67, 0x14, 0xB0, 0x0C, 0x14, 0x05, 0xBE, 0x07,
                                       0x7F, 0x14, 0xB2, 0x61};
  thread_local EncryptedName<sizeof(bottomSource), 0x73D1674B61DF7B0BULL> bottomName(bottomSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B19B0
// FUNCTION: fn_03355
void CSkillAction_KR_THUNDERING_CALL::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  int effectId;
  int bottomEffectId;
  if (level < 7) {
    const std::uint8_t effectSource[] = {0x19, 0x0B, 0x40, 0x43, 0xC1, 0x66, 0xFD, 0xB0, 0x03, 0x04, 0x6A, 0x4E, 0xC4, 0x6F, 0xE3, 0x86,
                                         0x0A, 0x3C, 0x06, 0x55, 0x96, 0x03};
    thread_local EncryptedName<sizeof(effectSource), 0xD98F03A52D35636DULL> effectName(effectSource);
    effectId = CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text());

    const std::uint8_t bottomSource[] = {0xA5, 0x71, 0xE8, 0xE1, 0x0F, 0xEE, 0x7D, 0x66, 0xBF, 0x7E, 0xC2, 0xEC, 0x0A, 0xE7, 0x63, 0x50,
                                         0xB6, 0x46, 0xAE, 0xF7, 0x58, 0xD4, 0x6D, 0x60, 0xA5, 0x6D, 0xF2, 0xE2, 0x6B};
    thread_local EncryptedName<sizeof(bottomSource), 0x0F0F8B6B8F9D19D1ULL> bottomName(bottomSource);
    bottomEffectId = CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text());
  } else {
    const std::uint8_t effectSource[] = {0x7D, 0xC1, 0x32, 0x31, 0xBD, 0x38, 0x07, 0xBA, 0x67, 0xCE, 0x18, 0x3C, 0xB8, 0x31, 0x19, 0x8C,
                                         0x6E, 0xF6, 0x72, 0x27, 0xEC, 0x5D};
    thread_local EncryptedName<sizeof(effectSource), 0xD3755DD95F47A909ULL> effectName(effectSource);
    effectId = CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text());

    const std::uint8_t bottomSource[] = {0x19, 0x8B, 0x30, 0xCD, 0x89, 0xA4, 0xA5, 0x58, 0x03, 0x84, 0x1A, 0xC0, 0x8C, 0xAD, 0xBB, 0x6E,
                                         0x0A, 0xBC, 0x70, 0xDB, 0xD8, 0x9E, 0xB5, 0x5E, 0x19, 0x97, 0x2A, 0xCE, 0xED};
    thread_local EncryptedName<sizeof(bottomSource), 0x31D7C1EDA345E36DULL> bottomName(bottomSource);
    bottomEffectId = CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text());
  }

  g_effectMgr->Spawn(target->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(target->render.accountId, bottomEffectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B2010
// FUNCTION: fn_03356
void CSkillAction_KR_THUNDERING_CALL_S::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  int effectId;
  int bottomEffectId;
  if (level < 7) {
    const std::uint8_t effectSource[] = {0x05, 0x73, 0x86, 0x91, 0xE3, 0x40, 0xE3, 0xAE, 0x1F, 0x7C, 0xAC, 0x9C, 0xE6, 0x49, 0xFD, 0x98,
                                         0x08, 0x44, 0xC0, 0x87, 0xB4, 0x25};
    thread_local EncryptedName<sizeof(effectSource), 0xC7912587FFF31B71ULL> effectName(effectSource);
    effectId = CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text());

    const std::uint8_t bottomSource[] = {0x19, 0x5B, 0x7A, 0xFB, 0xBF, 0x20, 0xA1, 0x00, 0x03, 0x54, 0x50, 0xF6, 0xBA, 0x29, 0xBF, 0x36,
                                         0x14, 0x6C, 0x3C, 0xED, 0xE8, 0x1A, 0xB1, 0x06, 0x19, 0x47, 0x60, 0xF8, 0xDB};
    thread_local EncryptedName<sizeof(bottomSource), 0x69D345DB950F336DULL> bottomName(bottomSource);
    bottomEffectId = CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text());
  } else {
    const std::uint8_t effectSource[] = {0x5B, 0xF7, 0xC6, 0xB5, 0x8B, 0xAC, 0x65, 0xF2, 0x41, 0xF8, 0xEC, 0xB8, 0x8E, 0xA5, 0x7B, 0xC4,
                                         0x56, 0xC0, 0x86, 0xA3, 0xDA, 0xC9};
    thread_local EncryptedName<sizeof(effectSource), 0x9B17C9EFDBB39F2FULL> effectName(effectSource);
    effectId = CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text());

    const std::uint8_t bottomSource[] = {0xA9, 0xBD, 0x3C, 0x2F, 0x13, 0x70, 0x49, 0x44, 0xB3, 0xB2, 0x16, 0x22, 0x16, 0x79, 0x57, 0x72,
                                         0xA4, 0x8A, 0x7C, 0x39, 0x42, 0x4A, 0x59, 0x42, 0xA9, 0xA1, 0x26, 0x2C, 0x77};
    thread_local EncryptedName<sizeof(bottomSource), 0x2D3B15774149D5DDULL> bottomName(bottomSource);
    bottomEffectId = CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text());
  }

  g_effectMgr->Spawn(target->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(target->render.accountId, bottomEffectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B2670
// FUNCTION: fn_03357
int CSkillAction_KR_THUNDERING_FOCUS::Slot02() {
  return 65;
}

// OFFSET: 0x005B2680
// FUNCTION: fn_03358
void CSkillAction_KR_THUNDERING_FOCUS::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const std::uint8_t effectSource[] = {0x95, 0xF3, 0x86, 0x15, 0x89, 0xE2, 0x8D, 0x08, 0x8F, 0xFC, 0xAC, 0x1D, 0x82, 0xE4, 0x8A, 0x12, 0xBE, 0xFC, 0xF3};
  thread_local EncryptedName<sizeof(effectSource), 0x61FF87ED7BF39BE1ULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B2880
// FUNCTION: fn_03359
int CSkillAction_KR_THUNDERING_FOCUS_S::Slot02() {
  return 65;
}

// OFFSET: 0x005B2890
// FUNCTION: fn_03360
void CSkillAction_KR_THUNDERING_FOCUS_S::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const std::uint8_t effectSource[] = {0x8B, 0xED, 0x52, 0x63, 0x01, 0x5A, 0xEB, 0x14, 0x91, 0xE2, 0x78, 0x6B, 0x0A, 0x5C, 0xEC, 0x0E, 0xA0, 0xFC, 0x27};
  thread_local EncryptedName<sizeof(effectSource), 0x7D993F650D2785FFULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B2A90
// FUNCTION: fn_03361
void CSkillAction_KR_THUNDERING_ORB::Slot12(vector3d position, int argument2, int argument3) {
  CGameActor* const target = reinterpret_cast<CGameActor*>(argument3);
  if (!target) {
    return;
  }

  if (CEffect* orb = g_effectMgr->Spawn(target->render.accountId, 2441, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    static_cast<CBezierArcThrowEffect*>(orb)->SetTarget(position);
  }

  const std::uint8_t effectSource[] = {0x47, 0x4F, 0x46, 0x61, 0xD9, 0x28, 0x4B, 0x44, 0x5D, 0x40, 0x6C, 0x60, 0xCF, 0x2F, 0x66, 0x4A, 0x33};
  thread_local EncryptedName<sizeof(effectSource), 0x2D394DBD0F332733ULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), position.x, position.y, position.z, 0, -1, 0, 0)) {
    effect->Slot16(300);
  }

  const std::uint8_t bottomSource[] = {0x33, 0x49, 0xCE, 0x85, 0x8F, 0x12, 0x6D, 0x7C, 0x29, 0x46, 0xE4, 0x84, 0x99, 0x15, 0x40, 0x72,
                                       0x18, 0x43, 0xD4, 0x9F, 0x9F, 0x18, 0x72, 0x15};
  thread_local EncryptedName<sizeof(bottomSource), 0x151F77EBEBBB2147ULL> bottomName(bottomSource);
  if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), position.x, position.y, position.z, 0, -1, 0, 0)) {
    effect->Slot16(300);
  }
}

// OFFSET: 0x005B2E80
// FUNCTION: fn_03362
void CSkillAction_KR_THUNDERING_ORB_S::Slot12(vector3d position, int argument2, int argument3) {
  CGameActor* const target = reinterpret_cast<CGameActor*>(argument3);
  if (!target) {
    return;
  }

  if (CEffect* orb = g_effectMgr->Spawn(target->render.accountId, 2441, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    static_cast<CBezierArcThrowEffect*>(orb)->SetTarget(position);
  }

  const std::uint8_t effectSource[] = {0xB5, 0xA7, 0xA6, 0x5D, 0xF3, 0x78, 0x8F, 0x00, 0xAF, 0xA8, 0x8C, 0x5C, 0xE5, 0x7F, 0xA2, 0x10, 0xC1};
  thread_local EncryptedName<sizeof(effectSource), 0x69FD1D9733D3CFC1ULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), position.x, position.y, position.z, 0, -1, 0, 0)) {
    effect->Slot16(300);
  }

  const std::uint8_t bottomSource[] = {0xB3, 0xA5, 0x90, 0x53, 0x7B, 0x18, 0x17, 0x9C, 0xA9, 0xAA, 0xBA, 0x52, 0x6D, 0x1F, 0x3A, 0x8C,
                                       0x98, 0xAF, 0x8A, 0x49, 0x6B, 0x12, 0x08, 0xF5};
  thread_local EncryptedName<sizeof(bottomSource), 0xF5657D1F3DE5CDC7ULL> bottomName(bottomSource);
  if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), position.x, position.y, position.z, 0, -1, 0, 0)) {
    effect->Slot16(300);
  }
}

// OFFSET: 0x005B3270
// FUNCTION: fn_03363
void CSkillAction_KR_ICE_SPLASH::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const std::uint8_t effectSource[] = {0x58, 0x3A, 0x4E, 0x5E, 0x12, 0xBD, 0x09, 0xEA, 0x42, 0x31, 0x2B};
  thread_local EncryptedName<sizeof(effectSource), 0x8B65CD61012B5931ULL> effectName(effectSource);
  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B3470
// FUNCTION: fn_03364
int CSkillAction_KR_ICE_PILLAR::Slot14() {
  return 7;
}

// OFFSET: 0x005B3480
// FUNCTION: fn_03365
int CSkillAction_KR_ICE_PILLAR::Slot02() {
  return 65;
}

// OFFSET: 0x005B3490
// FUNCTION: fn_03366
void CSkillObjectAction_KR_ICE_PILLAR::Slot00(int argument, CRenderObject* object, CGameActor* source) {
  if (!object) {
    return;
  }

  g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x8C, 0xEC, 0x28, 0x4A, 0x47, 0x68, 0x3F, 0x9B, 0x84, 0xFD, 0x12, 0x66, 0x43, 0x60, 0x21, 0x83, 0xE5};
    thread_local EncryptedName<sizeof(bytes), 0xF7530137154D8FE5ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0xAA, 0xA0, 0xB0, 0xD4, 0x2B, 0x82, 0x53, 0xE7, 0xA2, 0xB1, 0x8A, 0xF8, 0x2F, 0x8A, 0x4D, 0xFF,
                                  0x9C, 0xA1, 0xBA, 0xFF, 0x2F, 0x84, 0x52, 0x8B};
    thread_local EncryptedName<sizeof(bytes), 0x8B3FEB5B8BD5C3C3ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  if (CEffect* effect = g_effectMgr->Spawn(object->render.accountId, 2472, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    static_cast<EffectResourceSwitcher*>(effect)->SetNames([] {
      const std::uint8_t bytes[] = {0xCE, 0xD0, 0x60, 0x96, 0xD5, 0x0A, 0x87, 0x67, 0xC6, 0xC1, 0x5A, 0xA5, 0xCA, 0x0C, 0x9B, 0x0B};
      thread_local EncryptedName<sizeof(bytes), 0x0BEB63A5C905B3A7ULL> name(bytes);
      return name.Text();
    }(), [] {
      const std::uint8_t bytes[] = {0xA0, 0xD4, 0x40, 0xC8, 0xCB, 0x5A, 0xEF, 0x6B, 0xA8, 0xC5, 0x7A, 0xF2, 0xD5, 0x57, 0x83};
      thread_local EncryptedName<sizeof(bytes), 0x078333BB9725B7C9ULL> name(bytes);
      return name.Text();
    }());
    effect->Slot16(990);
  }
  if (CEffect* effect = g_effectMgr->Spawn(object->render.accountId, 2473, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    static_cast<EffectResourceSwitcher*>(effect)->SetNames([] {
      const std::uint8_t bytes[] = {0xDC, 0x16, 0x04, 0x00, 0x8D, 0x32, 0x77, 0xE3, 0xD4, 0x07, 0x3E, 0x33, 0x92, 0x34, 0x6B, 0xD0,
                                    0xD7, 0x1A, 0x15, 0x2B, 0x92, 0x36, 0x1B};
      thread_local EncryptedName<sizeof(bytes), 0x8F1B5BFD5F6175B5ULL> name(bytes);
      return name.Text();
    }(), [] {
      const std::uint8_t bytes[] = {0x72, 0x6E, 0x5C, 0x56, 0xDF, 0x20, 0xE3, 0x37, 0x7A, 0x7F, 0x66, 0x6C, 0xC1, 0x2D, 0xD0, 0x39,
                                    0x74, 0x79, 0x4D, 0x66, 0xC2, 0x49};
      thread_local EncryptedName<sizeof(bytes), 0x5B8F49AF09390D1BULL> name(bytes);
      return name.Text();
    }());
    effect->Slot16(990);
  }
}

// OFFSET: 0x005B3E30
// FUNCTION: fn_03367
void CSkillObjectAction_KR_ICE_PILLAR::Slot01(CRenderObject* object) {
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
int CSkillAction_KR_FEATHER_SPRINKLE::Slot02() {
  return 68;
}

// OFFSET: 0x005B4240
// FUNCTION: fn_03370
void CSkillAction_KR_FEATHER_SPRINKLE::Slot15(int level, CGameActor* source, CGameActor* target) {
  this->sub_5A4930(level, source);
}

// OFFSET: 0x005B4260
// FUNCTION: fn_03371
int CSkillAction_KR_SHARPEN_GUST::Slot01() {
  return 60;
}

// OFFSET: 0x005B4270
// FUNCTION: fn_03372
int CSkillAction_KR_SHARPEN_GUST::Slot02() {
  return 78;
}

// OFFSET: 0x005B4280
// FUNCTION: fn_03373
void CSkillAction_KR_SHARPEN_GUST::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x005B46D0
// FUNCTION: fn_03375
void CSkillAction_KR_SHARPEN_GUST::Slot15(int level, CGameActor* source, CGameActor* target) {
  this->sub_5A4930(level, source);
  this->sub_5A4B00(level, source);
}

// OFFSET: 0x005B4700
// FUNCTION: fn_03376
int CSkillAction_KR_SHARPEN_HAIL::Slot01() {
  return 60;
}

// OFFSET: 0x005B4710
// FUNCTION: fn_03377
int CSkillAction_KR_SHARPEN_HAIL::Slot02() {
  return 79;
}

// OFFSET: 0x005B4720
// FUNCTION: fn_03378
void CSkillAction_KR_SHARPEN_HAIL::Slot08(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  static_cast<EffectActor*>(source)->LaunchEffect(56, level / 16);

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0x6A, 0x05, 0xAE, 0x8D, 0x2D, 0x64, 0xAF, 0x20, 0x71, 0x0C, 0xA6, 0x93, 0x5D};
        thread_local EncryptedName<sizeof(bytes), 0x7FC1015DFFCF6D19ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot14(level);
  }
  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0x50, 0xEB, 0xBA, 0xF1, 0x31, 0xD8, 0x51, 0x94, 0x4B, 0xE2, 0xB2, 0xEF, 0x1E, 0xDF, 0x50, 0xBF,
                                      0x57, 0xEC, 0xB6, 0x83};
        thread_local EncryptedName<sizeof(bytes), 0xCB3FBD4183DB8323ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot14(level);
  }
}

// OFFSET: 0x005B4AF0
// FUNCTION: fn_03379
void CSkillAction_KR_SHARPEN_HAIL::Slot11(int level, CGameActor* source, CGameActor* target) {
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
        shot->Slot16(startTime);
      }

      CEffect* bottom = g_effectMgr->Spawn(0, bottomEffect, position.x, position.y, position.z, 0, -1, 0, 0);
      if (bottom) {
        bottom->Slot16(startTime);
      }
    }
  }
}

// OFFSET: 0x005B4F70
// FUNCTION: fn_03380
void CSkillAction_KR_SHARPEN_HAIL::Slot15(int level, CGameActor* source, CGameActor* target) {
  this->sub_5A4930(level, source);
}

// OFFSET: 0x005B4F90
// FUNCTION: fn_03381
int CSkillAction_KR_IRON_HOWLING::Slot01() {
  return 66;
}

// OFFSET: 0x005B4FA0
// FUNCTION: fn_03382
int CSkillAction_KR_IRON_HOWLING::Slot02() {
  return 68;
}

// OFFSET: 0x005B4FB0
// FUNCTION: fn_03383
void CSkillAction_KR_IRON_HOWLING::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x005B51F0
// FUNCTION: fn_03385
EfstActionMessage CEfstAction_IRON_HOWLING::Slot03() {
  return {LookupMessage(4662), 0xFFFF00};
}

// OFFSET: 0x005B5220
// FUNCTION: fn_03386
EfstActionMessage CEfstAction_IRON_HOWLING::Slot04() {
  return {LookupMessage(4663), 0xFF};
}

// OFFSET: 0x005B5250
// FUNCTION: fn_03387
int CSkillAction_KR_TYPHOON_WING::Slot01() {
  return 66;
}

// OFFSET: 0x005B5260
// FUNCTION: fn_03388
int CSkillAction_KR_TYPHOON_WING::Slot02() {
  return 67;
}

// OFFSET: 0x005B5270
// FUNCTION: fn_03389
void CSkillAction_KR_TYPHOON_WING::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x005B52A0
// FUNCTION: fn_03390
void CSkillAction_KR_TYPHOON_WING::Slot12(vector3d position, int level, int argument5) {
  const int effectId = CEZ2STREffectMgr::GetInstance()->EffectId(
    level < 6 ? std::string([] {
      const std::uint8_t source[] = {0x67, 0xBE, 0x73, 0x7D, 0x94, 0xFE, 0x79, 0x10, 0x64, 0xAE, 0x6D, 0x72, 0xA4, 0xA4, 0x6F, 0x7A, 0x13};
      thread_local EncryptedName<sizeof(source), 0x4F1791FB1503C713ULL> name(source);
      return name.Text();
    }())
              : std::string([] {
      const std::uint8_t source[] = {0xA7, 0x94, 0xE7, 0x7D, 0x22, 0x34, 0xCB, 0xFE, 0xA4, 0x84, 0xF9, 0x72, 0x12, 0x6C, 0xDD, 0x96, 0xD3};
      thread_local EncryptedName<sizeof(source), 0xA1A55B4D1597EDD3ULL> name(source);
      return name.Text();
    }())
  );
  const int bottomId = CEZ2STREffectMgr::GetInstance()->EffectId(
    level < 6 ? std::string([] {
      const std::uint8_t source[] = {0x89, 0x84, 0x0D, 0x9B, 0x8E, 0x7A, 0x97, 0x44, 0x8A, 0x94, 0x13, 0x94,
                                     0xBE, 0x20, 0x81, 0x2E, 0xA2, 0x9F, 0x12, 0x87, 0x95, 0x7A, 0x94, 0x1B};
      thread_local EncryptedName<sizeof(source), 0x1BF915E1F37DFDFDULL> name(source);
      return name.Text();
    }())
              : std::string([] {
      const std::uint8_t source[] = {0xAB, 0xA0, 0x2D, 0x01, 0x7C, 0xFC, 0x91, 0x5E, 0xA8, 0xB0, 0x33, 0x0E,
                                     0x4C, 0xA4, 0x87, 0x36, 0x80, 0xBB, 0x32, 0x1D, 0x67, 0xFC, 0x92, 0x01};
      thread_local EncryptedName<sizeof(source), 0x01FF9313695DD9DFULL> name(source);
      return name.Text();
    }())
  );

  g_effectMgr->Spawn(0, effectId, position.x, position.y, position.z, 0, -1, 0, 0);
  g_effectMgr->Spawn(0, bottomId, position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005B58A0
// FUNCTION: fn_03391
int CSkillAction_KR_WIND_VEIL::Slot01() {
  return 66;
}

// OFFSET: 0x005B58B0
// FUNCTION: fn_03392
int CSkillAction_KR_WIND_VEIL::Slot02() {
  return 68;
}

// OFFSET: 0x005B58C0
// FUNCTION: fn_03393
void CSkillAction_KR_WIND_VEIL::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x005B58F0
// FUNCTION: fn_03394
EfstActionMessage CEfstAction_WIND_VEIL::Slot03() {
  return {LookupMessage(4664), 0xFFFF00};
}

// OFFSET: 0x005B5920
// FUNCTION: fn_03395
EfstActionMessage CEfstAction_WIND_VEIL::Slot04() {
  return {LookupMessage(4665), 0xFF};
}

// OFFSET: 0x005B5950
// FUNCTION: fn_03396
void CEfstAction_WIND_VEIL::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (!actor) {
    return;
  }

  g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t source[] = {0xEC, 0xC4, 0x19, 0xAB, 0x8E, 0x4B, 0x88, 0x04, 0xF7, 0xAD};
    thread_local EncryptedName<sizeof(source), 0x6DED3DD1CF75AD9BULL> name(source);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t source[] = {0xB6, 0xBA, 0xB7, 0x13, 0xC4, 0x03, 0xA6, 0x20, 0xAD, 0x8C, 0xB9, 0x18, 0xEF, 0x01, 0xAC, 0x24, 0xC1};
    thread_local EncryptedName<sizeof(source), 0x49C3759B77DBD3C1ULL> name(source);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B5CE0
// FUNCTION: fn_03397
void CEfstAction_WIND_VEIL::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t source[] = {0xD8, 0x26, 0x47, 0x87, 0x80, 0x63, 0x36, 0x56, 0xC3, 0x4F};
    thread_local EncryptedName<sizeof(source), 0x3F5315DFE32B4FAFULL> name(source);
    return name.Text();
  }()));
  g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t source[] = {0x9E, 0xBA, 0x51, 0x63, 0x6A, 0x5F, 0x6E, 0x8C, 0x85, 0x8C, 0x5F, 0x68, 0x41, 0x5D, 0x64, 0x88, 0xE9};
    thread_local EncryptedName<sizeof(source), 0xE50B2935073DD3E9ULL> name(source);
    return name.Text();
  }()));
}

// OFFSET: 0x005B6010
// FUNCTION: fn_03398
void CEfstAction_WIND_VEIL::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    this->Slot00(efstId, value, 0, 0, actor, false);
  }
}

// OFFSET: 0x005B6030
// FUNCTION: fn_03399
int CSkillAction_SKID_KR_CHOP_CHOP::Slot02() {
  return 75;
}

// OFFSET: 0x005B6330
// FUNCTION: fn_03401
void CEfstAction_CHOP_CHOP::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  while (CEffect* effect = g_effectMgr->Find(actor->render.accountId, 2466)) {
    g_effectMgr->Retire(effect, 1);
  }
  while (CEffect* effect = g_effectMgr->Find(actor->render.accountId, 2444)) {
    g_effectMgr->Retire(effect, 1);
  }
  while (CEffect* effect = g_effectMgr->Find(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
           const std::uint8_t source[] = {0xDC, 0xD5, 0xB0, 0x4F, 0x12, 0x4E, 0xEB, 0x5C, 0xCF, 0xE2, 0xAC, 0x4A, 0x2F, 0x2D};
           thread_local EncryptedName<sizeof(source), 0x33832D4D3FDFBDBFULL> name(source);
           return name.Text();
         }()))) {
    g_effectMgr->Retire(effect, 1);
  }
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
  const int angle = static_cast<int>(parent->GetInstant(25));
  parent->render.angle = static_cast<float>(angle) / 1000.0f;
  return false;
}

// OFFSET: 0x005B6D00
// FUNCTION: fn_03405
void CWhirlwindEffect::SetLifetime(int milliseconds) {
  this->endTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
}

// OFFSET: 0x005B6D50
// FUNCTION: fn_03406
void CWhirlwindEffect::SetDuration(int duration) {
  this->duration = duration;
}

// OFFSET: 0x005B0A20
// FUNCTION: fn_03343
void CSkillAction_KR_NASTY_SLASH::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source) || !target) {
    return;
  }

  const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
  const std::uint8_t effectSource[] = {0x49, 0x60, 0x74, 0x19, 0x7A, 0x9A, 0x80, 0x43, 0x46, 0x72, 0x6F, 0x6D};
  thread_local EncryptedName<sizeof(effectSource), 0x2FF3C5036D070127ULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot17(angle);
    effect->Slot18(25.0f);
  }
}

// OFFSET: 0x005B3EA0
// FUNCTION: fn_03369
void CSkillAction_KR_FEATHER_SPRINKLE::Slot11(int level, CGameActor* source, CGameActor* target) {
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
void CSkillAction_KR_SHARPEN_GUST::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source) || !target) {
    return;
  }

  const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
  const std::uint8_t effectSource[] = {0x1E, 0x37, 0x98, 0x41, 0x93, 0x1E, 0x3B, 0x60, 0x0A, 0x2A, 0x8A, 0x47, 0xE3};
  thread_local EncryptedName<sizeof(effectSource), 0x3F557BE333F95F6DULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot17(angle);
    effect->Slot18(30.0f);
  }

  const std::uint8_t bottomSource[] = {0x4A, 0x89, 0xC8, 0x9D, 0x91, 0xC2, 0x3D, 0x58, 0x5E, 0x94, 0xDA, 0x9B, 0xBE, 0xC5, 0x3C, 0x73, 0x4D, 0x8E, 0xC4, 0xEF};
  thread_local EncryptedName<sizeof(bottomSource), 0x0753A7E1EFA9E139ULL> bottomName(bottomSource);
  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(bottomName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot17(angle);
    effect->Slot18(30.0f);
  }
}

// OFFSET: 0x005B4FE0
// FUNCTION: fn_03384
void CSkillAction_KR_IRON_HOWLING::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  const std::uint8_t effectSource[] = {0xF6, 0xB1, 0x28, 0xD3, 0xB4, 0x2F, 0x8E, 0xE6, 0xF3, 0xAA, 0x29, 0xDA, 0xEB};
  thread_local EncryptedName<sizeof(effectSource), 0x91E147EBBD47C39FULL> effectName(effectSource);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005B6040
// FUNCTION: fn_03400
void CSkillAction_SKID_KR_CHOP_CHOP::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source)) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 2466, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->unknown_002C = 1;
  }

  const std::uint8_t effectSource[] = {0x92, 0xAB, 0x86, 0xCD, 0xF2, 0x6E, 0x1D, 0x12, 0x81, 0x9C, 0x9A, 0xC8, 0xCF, 0x0D};
  thread_local EncryptedName<sizeof(effectSource), 0x7D750DADBDE9C3F1ULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot17(static_cast<float>(std::max(0, rand() % 360)));
  }

  if (CRagEffect* effect = source->LaunchEffect(2444, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    static_cast<CWhirlwindEffect*>(effect)->SetLifetime(1000);
    static_cast<CWhirlwindEffect*>(effect)->SetDuration(500);
  }
}
