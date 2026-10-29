#include "Contents/Actors/GameActor.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Skills/SkillInfo.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
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
};

class CSkillAction_Direction {
};

class CSkillAction_DamageWereRaptorForm : public CSkillAction_AnimalForm, public CSkillAction_Direction {
};

class CSkillAction_DamageWereWolfForm : public CSkillAction_AnimalForm {
};

class CSkillAction_KR_FEATHER_SPRINKLE : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot02() override;
};

class CSkillAction_KR_SHARPEN_GUST : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
};

class CSkillAction_KR_SHARPEN_HAIL : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_IRON_HOWLING : public CSkillAction_DamageWereWolfForm {
public:
  int Slot01() override;
  int Slot02() override;
};

// OFFSET: 0x005B3E90
// FUNCTION: fn_03368
int CSkillAction_KR_FEATHER_SPRINKLE::Slot02() {
  return 68;
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

// OFFSET: 0x005B4AF0

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
