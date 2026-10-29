#include "Contents/Actors/GameActor.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
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
};

class CSkillAction_KR_CLAW_WAVE : public CSkillAction_DamageWereWolfForm {
public:
  int Slot01() override;
  int Slot02() override;
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

class CSkillAction_KR_ICE_SPLASH : public CSkillAction_Icing {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_KR_ICE_PILLAR : public CSkillAction_Icing {
public:
  int Slot02() override;
  int Slot14() override;
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

// OFFSET: 0x005B0800
// FUNCTION: fn_03340
int CSkillAction_KR_DOUBLE_SLASH::Slot02() {
  return 72;
}

// OFFSET: 0x005B0810

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

// OFFSET: 0x005B0CD0

// OFFSET: 0x005B0EA0
// FUNCTION: fn_03348
int CSkillAction_KR_EARTH_DRILL::Slot02() {
  return 62;
}

// OFFSET: 0x005B0EB0

// OFFSET: 0x005B1240
// FUNCTION: fn_03350
int CSkillAction_KR_EARTH_STAMP::Slot02() {
  return 63;
}

// OFFSET: 0x005B1250

// OFFSET: 0x005B1590
// FUNCTION: fn_03352
int CSkillAction_KR_GROUND_BLOOM::Slot02() {
  return 0;
}

// OFFSET: 0x005B15A0

// OFFSET: 0x005B1610

// OFFSET: 0x005B19B0

// OFFSET: 0x005B2010

// OFFSET: 0x005B2670
// FUNCTION: fn_03357
int CSkillAction_KR_THUNDERING_FOCUS::Slot02() {
  return 65;
}

// OFFSET: 0x005B2680

// OFFSET: 0x005B2880
// FUNCTION: fn_03359
int CSkillAction_KR_THUNDERING_FOCUS_S::Slot02() {
  return 65;
}

// OFFSET: 0x005B2890

// OFFSET: 0x005B3270

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
