#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Contents/Effects/Skills/BezierArcThrowEffect.h"
#include "Contents/Effects/Particles/EZ2STREffectEx.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Effects/Skills/WhirlwindEffect.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <windows.h>

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

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("double_slash", 0x1D6D75EDE7CD311FULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x1D6D75EDE7CD311FULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<std::uint8_t, 10> effectSourceArray = CRYPT_NAME("claw_wave", 0x17359BF953C367B3ULL);
    const std::uint8_t (&effectSource)[10] = effectSourceArray._Elems;
    thread_local EncryptedName<sizeof(effectSource), 0x17359BF953C367B3ULL> effectName(effectSource);
    return effectName.Text();
  }()), position, 0, -1, 0, 0);
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

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("earth_drill", 0xC52FB5FBB94FF917ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0xC52FB5FBB94FF917ULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("earth_drill_bottom", 0x93E7A3A1F587CDC5ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x93E7A3A1F587CDC5ULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x005B1240
// FUNCTION: fn_03350
int CSkillAction_KR_EARTH_STAMP::GetSkillAttackMotion() {
  return 63;
}

// OFFSET: 0x005B1250
// FUNCTION: fn_03351
void CSkillAction_KR_EARTH_STAMP::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const auto effectSource = CRYPT_NAME("earth_stamp", 0x3FAF85D995034FD7ULL);
    thread_local EncryptedName<sizeof(effectSource), 0x3FAF85D995034FD7ULL> effectName(effectSource.data);
    return effectName.Text();
  }()), position, 0, -1, 0, 0);

  effects = g_effectMgr;
  effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bottomSource[] = CRYPT_CHARS(19, "earth_stamp_bottom", 0x490FFB43A12F0DB9ULL);
    thread_local EncryptedName<sizeof(bottomSource), 0x490FFB43A12F0DB9ULL> bottomName(bottomSource);
    return bottomName.Text();
  }()), position, 0, -1, 0, 0);
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

  EffectLists* effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("ground_bloom", 0x91651D81F559F971ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x91651D81F559F971ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  effects = g_effectMgr;
  effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("ground_bloom_bottom", 0x73D1674B61DF7B0BULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x73D1674B61DF7B0BULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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
    effectId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("thundering_call_g_3x3", 0xD98F03A52D35636DULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0xD98F03A52D35636DULL> name(bytes);
      return name.Text();
    }());

    bottomEffectId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("thundering_call_g_3x3_bottom", 0x0F0F8B6B8F9D19D1ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x0F0F8B6B8F9D19D1ULL> name(bytes);
      return name.Text();
    }());
  } else {
    effectId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("thundering_call_g_5x5", 0xD3755DD95F47A909ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0xD3755DD95F47A909ULL> name(bytes);
      return name.Text();
    }());

    bottomEffectId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("thundering_call_g_5x5_bottom", 0x31D7C1EDA345E36DULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x31D7C1EDA345E36DULL> name(bytes);
      return name.Text();
    }());
  }

  g_effectMgr->Spawn(target->render.accountId, effectId, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  g_effectMgr->Spawn(target->render.accountId, bottomEffectId, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    effectId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("thundering_call_y_3x3", 0xC7912587FFF31B71ULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0xC7912587FFF31B71ULL> name(bytes);
      return name.Text();
    }());

    bottomEffectId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("thundering_call_y_3x3_bottom", 0x69D345DB950F336DULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x69D345DB950F336DULL> name(bytes);
      return name.Text();
    }());
  } else {
    effectId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("thundering_call_y_5x5", 0x9B17C9EFDBB39F2FULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x9B17C9EFDBB39F2FULL> name(bytes);
      return name.Text();
    }());

    bottomEffectId = CEZ2STREffectMgr::GetInstance()->EffectId([] {
      constexpr auto bytesText = CRYPT_NAME("thundering_call_y_5x5_bottom", 0x2D3B15774149D5DDULL);
      const auto& bytes = bytesText.data;
      thread_local EncryptedName<sizeof(bytes), 0x2D3B15774149D5DDULL> name(bytes);
      return name.Text();
    }());
  }

  g_effectMgr->Spawn(target->render.accountId, effectId, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  g_effectMgr->Spawn(target->render.accountId, bottomEffectId, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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

  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("thundering_focus_g", 0x61FF87ED7BF39BE1ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x61FF87ED7BF39BE1ULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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

  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("thundering_focus_y", 0x7D993F650D2785FFULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x7D993F650D2785FFULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x005B2A90
// FUNCTION: fn_03361
void CSkillAction_KR_THUNDERING_ORB::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  CGameActor* const target = reinterpret_cast<CGameActor*>(argument3);
  if (!target) {
    return;
  }

  if (CEffect* orb = g_effectMgr->Spawn(target->render.accountId, 2441, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    static_cast<CBezierArcThrowEffect*>(orb)->SetTarget(position);
  }

  if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 17> effectSource = CRYPT_NAME("thundering_orb_g", 0x2D394DBD0F332733ULL);
    thread_local EncryptedName<sizeof(effectSource), 0x2D394DBD0F332733ULL> effectName(effectSource);
    return effectName.Text();
  }()), position, 0, -1, 0, 0)) {
    effect->SetDelay(300);
  }

  if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 24> bottomSource = CRYPT_NAME("thundering_orb_g_bottom", 0x151F77EBEBBB2147ULL);
    thread_local EncryptedName<sizeof(bottomSource), 0x151F77EBEBBB2147ULL> bottomName(bottomSource);
    return bottomName.Text();
  }()), position, 0, -1, 0, 0)) {
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

  if (CEffect* orb = g_effectMgr->Spawn(target->render.accountId, 2441, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    static_cast<CBezierArcThrowEffect*>(orb)->SetTarget(position);
  }

  if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 17> effectSource = CRYPT_NAME("thundering_orb_y", 0x69FD1D9733D3CFC1ULL);
    thread_local EncryptedName<sizeof(effectSource), 0x69FD1D9733D3CFC1ULL> effectName(effectSource);
    return effectName.Text();
  }()), position, 0, -1, 0, 0)) {
    effect->SetDelay(300);
  }

  if (CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 24> bottomSource = CRYPT_NAME("thundering_orb_y_bottom", 0xF5657D1F3DE5CDC7ULL);
    thread_local EncryptedName<sizeof(bottomSource), 0xF5657D1F3DE5CDC7ULL> bottomName(bottomSource);
    return bottomName.Text();
  }()), position, 0, -1, 0, 0)) {
    effect->SetDelay(300);
  }
}

// OFFSET: 0x005B3270
// FUNCTION: fn_03363
void CSkillAction_KR_ICE_SPLASH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<std::uint8_t, 11> effectSourceArray = CRYPT_NAME("ice_splash", 0x8B65CD61012B5931ULL);
    const std::uint8_t (&effectSource)[11] = effectSourceArray._Elems;
    thread_local EncryptedName<sizeof(effectSource), 0x8B65CD61012B5931ULL> effectName(effectSource);
    return effectName.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    const char bytes[] = CRYPT_CHARS(17, "ice_pillar_start", 0xF7530137154D8FE5ULL);
    thread_local EncryptedName<sizeof(bytes), 0xF7530137154D8FE5ULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const char bytes[] = CRYPT_CHARS(24, "ice_pillar_start_bottom", 0x8B3FEB5B8BD5C3C3ULL);
    thread_local EncryptedName<sizeof(bytes), 0x8B3FEB5B8BD5C3C3ULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  if (CEffect* effect = g_effectMgr->Spawn(object->render.accountId, 2472, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    static_cast<EffectResourceSwitcher*>(effect)->SetNames([] {
      const char bytes[] = CRYPT_CHARS(16, "ice_pillar_loop", 0x0BEB63A5C905B3A7ULL);
      thread_local EncryptedName<sizeof(bytes), 0x0BEB63A5C905B3A7ULL> name(bytes);
      return name.Text();
    }(), [] {
      const char bytes[] = CRYPT_CHARS(15, "ice_pillar_end", 0x078333BB9725B7C9ULL);
      thread_local EncryptedName<sizeof(bytes), 0x078333BB9725B7C9ULL> name(bytes);
      return name.Text();
    }());
    effect->SetDelay(990);
  }
  if (CEffect* effect = g_effectMgr->Spawn(object->render.accountId, 2473, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    static_cast<EffectResourceSwitcher*>(effect)->SetNames([] {
      const char bytes[] = CRYPT_CHARS(23, "ice_pillar_loop_bottom", 0x8F1B5BFD5F6175B5ULL);
      thread_local EncryptedName<sizeof(bytes), 0x8F1B5BFD5F6175B5ULL> name(bytes);
      return name.Text();
    }(), [] {
      const char bytes[] = CRYPT_CHARS(22, "ice_pillar_end_bottom", 0x5B8F49AF09390D1BULL);
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
        constexpr std::array<char, 13> bytes = CRYPT_NAME("sharpen_hail", 0x7FC1015DFFCF6D19ULL);
        thread_local EncryptedName<sizeof(bytes), 0x7FC1015DFFCF6D19ULL> name(bytes);
        return name.Text();
      }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetLifeTime(level);
  }
  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        constexpr std::array<char, 20> bytes = CRYPT_NAME("sharpen_hail_bottom", 0xCB3FBD4183DB8323ULL);
        thread_local EncryptedName<sizeof(bytes), 0xCB3FBD4183DB8323ULL> name(bytes);
        return name.Text();
      }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
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
    constexpr auto bytesText = CRYPT_NAME("sharpen_hail_shot", 0x7FCD77F9C7CBD14DULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x7FCD77F9C7CBD14DULL> name(bytes);
    return name.Text();
  }());
  const int bottomEffect = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("sharpen_hail_shot_bottom", 0xD1538513DF975701ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0xD1538513DF975701ULL> name(bytes);
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

      CEffect* shot = g_effectMgr->Spawn(0, shotEffect, position, 0, -1, 0, 0);
      if (shot) {
        shot->SetDelay(startTime);
      }

      CEffect* bottom = g_effectMgr->Spawn(0, bottomEffect, position, 0, -1, 0, 0);
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
      constexpr auto sourceText = CRYPT_NAME("typhoon_wing_5x5", 0x4F1791FB1503C713ULL);
      const auto& source = sourceText.data;
      thread_local EncryptedName<sizeof(source), 0x4F1791FB1503C713ULL> name(source);
      return name.Text();
    }())
              : std::string([] {
      constexpr auto sourceText = CRYPT_NAME("typhoon_wing_7x7", 0xA1A55B4D1597EDD3ULL);
      const auto& source = sourceText.data;
      thread_local EncryptedName<sizeof(source), 0xA1A55B4D1597EDD3ULL> name(source);
      return name.Text();
    }())
  );
  const int bottomId = CEZ2STREffectMgr::GetInstance()->EffectId(
    level < 6 ? std::string([] {
      constexpr auto sourceText = CRYPT_NAME("typhoon_wing_5x5_bottom", 0x1BF915E1F37DFDFDULL);
      const auto& source = sourceText.data;
      thread_local EncryptedName<sizeof(source), 0x1BF915E1F37DFDFDULL> name(source);
      return name.Text();
    }())
              : std::string([] {
      constexpr auto sourceText = CRYPT_NAME("typhoon_wing_7x7_bottom", 0x01FF9313695DD9DFULL);
      const auto& source = sourceText.data;
      thread_local EncryptedName<sizeof(source), 0x01FF9313695DD9DFULL> name(source);
      return name.Text();
    }())
  );

  g_effectMgr->Spawn(0, effectId, position, 0, -1, 0, 0);
  g_effectMgr->Spawn(0, bottomId, position, 0, -1, 0, 0);
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
    constexpr std::array<char, 10> source = CRYPT_NAME("wild_veil", 0x6DED3DD1CF75AD9BULL);
    thread_local EncryptedName<sizeof(source), 0x6DED3DD1CF75AD9BULL> name(source);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 17> source = CRYPT_NAME("wild_veil_bottom", 0x49C3759B77DBD3C1ULL);
    thread_local EncryptedName<sizeof(source), 0x49C3759B77DBD3C1ULL> name(source);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x005B5CE0
// FUNCTION: fn_03397
void CEfstAction_WIND_VEIL::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 10> source = CRYPT_NAME("wild_veil", 0x3F5315DFE32B4FAFULL);
    thread_local EncryptedName<sizeof(source), 0x3F5315DFE32B4FAFULL> name(source);
    return name.Text();
  }()));
  effects = g_effectMgr;
  effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr std::array<char, 17> source = CRYPT_NAME("wild_veil_bottom", 0xE50B2935073DD3E9ULL);
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
      constexpr std::array<std::uint8_t, 14> sourceArray = CRYPT_NAME("chop_chop_sub", 0x33832D4D3FDFBDBFULL);
      const std::uint8_t (&source)[14] = sourceArray._Elems;
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
  constexpr std::array<std::uint8_t, 12> effectSourceArray = CRYPT_NAME("nasty_slash", 0x2FF3C5036D070127ULL);
  const std::uint8_t (&effectSource)[12] = effectSourceArray._Elems;
  thread_local EncryptedName<sizeof(effectSource), 0x2FF3C5036D070127ULL> effectName(effectSource);
  if (CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
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

  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("feather_sprinkle", 0x3B915119EFD523EFULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x3B915119EFD523EFULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);

  effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("feather_sprinkle_bottom", 0x474DC581D5E98357ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x474DC581D5E98357ULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x005B42B0
// FUNCTION: fn_03374
void CSkillAction_KR_SHARPEN_GUST::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!this->CanUse(source) || !target) {
    return;
  }

  const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("sharpen_gust", 0x3F557BE333F95F6DULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x3F557BE333F95F6DULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetAngle(angle);
    effect->SetPitch(30.0f);
  }

  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("sharpen_gust_bottom", 0x0753A7E1EFA9E139ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x0753A7E1EFA9E139ULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
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

  constexpr std::array<std::uint8_t, 13> effectSourceArray = CRYPT_NAME("iron_howling", 0x91E147EBBD47C39FULL);
  const std::uint8_t (&effectSource)[13] = effectSourceArray._Elems;
  thread_local EncryptedName<sizeof(effectSource), 0x91E147EBBD47C39FULL> effectName(effectSource);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(effectName.Text()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    constexpr auto bytesText = CRYPT_NAME("chop_chop_sub", 0x7D750DADBDE9C3F1ULL);
    const auto& bytes = bytesText.data;
    thread_local EncryptedName<sizeof(bytes), 0x7D750DADBDE9C3F1ULL> name(bytes);
    return name.Text();
  }()), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetAngle(static_cast<float>(std::max(0, rand() % 360)));
  }

  if (CRagEffect* effect = source->LaunchEffect(2444, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    static_cast<CWhirlwindEffect*>(effect)->SetLifetime(1000);
    static_cast<CWhirlwindEffect*>(effect)->SetDuration(500);
  }
}
