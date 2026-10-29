#include "Contents/Actors/GameActor.h"
#include "Contents/EffectResourceSwitcher.h"
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

class CSkillAction_Icing : public CSkillAction {
};

class CSkillAction_Thundering : public CSkillAction {
};

class CSkillAction_ThunderingBasic : public CSkillAction_Thundering {
};

class CSkillAction_ThunderingExpand : public CSkillAction_Thundering {
};

class CSkillAction_AT_PULSE_OF_MADNESS : public CSkillAction_DamageWereWolfForm {
public:
  int Slot02() override;
};

class CEfstAction_PULSE_OF_MADNESS : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_AT_FRENZY_FANG : public CSkillAction_DamageWereWolfForm {
public:
  int Slot02() override;
};

class CSkillAction_AT_PINION_SHOT : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
};

class CSkillAction_AT_QUILL_SPEAR : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
};

class CSkillAction_AT_NATURE_HARMONY : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_NATURE_HARMONY : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_AT_GRAVITY_HOLE : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_CHILLING_BLAST : public CSkillAction_Icing {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_GLACIER_MONOLITH : public CSkillAction_Icing {
public:
  int Slot02() override;
  int Slot14() override;
};

class CSkillObjectAction_AT_GLACIER_MONOLITH : public CSkillObjectAction {
public:
  void Slot00(int argument, CRenderObject* object, CGameActor* source) override;
  void Slot01(CRenderObject* object) override;
};

class CSkillAction_AT_GLACIER_NOVA : public CSkillAction_Icing {
public:
  int Slot02() override;
  void Slot12(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_AT_GLACIER_SHARD : public CSkillAction_Icing {
public:
  void Slot10(int level, CGameActor* source, int argument3, int argument4, int argument5) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_GLACIER_STOMP : public CSkillAction_Icing {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_ROARING_CHARGE : public CSkillAction_ThunderingBasic {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_ROARING_CHARGE_S : public CSkillAction_ThunderingExpand {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_FURIOS_STORM : public CSkillAction_Thundering {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

// OFFSET: 0x005A69C0
// FUNCTION: fn_03190
int CSkillAction_AT_PULSE_OF_MADNESS::Slot02() {
  return 68;
}

// OFFSET: 0x005A6C00

// OFFSET: 0x005A6C30

// OFFSET: 0x005A6C60
// FUNCTION: fn_03195
int CSkillAction_AT_FRENZY_FANG::Slot02() {
  return 70;
}

// OFFSET: 0x005A6EE0
// FUNCTION: fn_03197
int CSkillAction_AT_PINION_SHOT::Slot01() {
  return 60;
}

// OFFSET: 0x005A6EF0
// FUNCTION: fn_03198
int CSkillAction_AT_PINION_SHOT::Slot02() {
  return 78;
}

// OFFSET: 0x005A71A0
// FUNCTION: fn_03202
int CSkillAction_AT_QUILL_SPEAR::Slot01() {
  return 60;
}

// OFFSET: 0x005A71B0
// FUNCTION: fn_03203
int CSkillAction_AT_QUILL_SPEAR::Slot02() {
  return 79;
}

// OFFSET: 0x005A7750
// FUNCTION: fn_03207
int CSkillAction_AT_NATURE_HARMONY::Slot01() {
  return 61;
}

// OFFSET: 0x005A7760
// FUNCTION: fn_03208
int CSkillAction_AT_NATURE_HARMONY::Slot02() {
  return 64;
}

// OFFSET: 0x005A77A0

// OFFSET: 0x005A79B0

// OFFSET: 0x005A79E0

// OFFSET: 0x005A7A10
// FUNCTION: fn_03213
int CSkillAction_AT_GRAVITY_HOLE::Slot01() {
  return 61;
}

// OFFSET: 0x005A7A20
// FUNCTION: fn_03214
int CSkillAction_AT_GRAVITY_HOLE::Slot02() {
  return 64;
}

// OFFSET: 0x005A7A60

// OFFSET: 0x005A7AD0

// OFFSET: 0x005A7E70
// FUNCTION: fn_03218
int CSkillAction_AT_CHILLING_BLAST::Slot02() {
  return 65;
}

// OFFSET: 0x005A7E80

// OFFSET: 0x005A8230
// FUNCTION: fn_03220
int CSkillAction_AT_GLACIER_MONOLITH::Slot14() {
  return 9;
}

// OFFSET: 0x005A8240
// FUNCTION: fn_03221
int CSkillAction_AT_GLACIER_MONOLITH::Slot02() {
  return 65;
}

// OFFSET: 0x005A8250

// OFFSET: 0x005A8960

// OFFSET: 0x005A8FC0
// FUNCTION: fn_03224
int CSkillAction_AT_GLACIER_NOVA::Slot02() {
  return 65;
}

// OFFSET: 0x005A8FD0

// OFFSET: 0x005A91A0

// OFFSET: 0x005A93B0

// OFFSET: 0x005A95C0
// FUNCTION: fn_03228
int CSkillAction_AT_GLACIER_STOMP::Slot02() {
  return 65;
}

// OFFSET: 0x005A95D0

// OFFSET: 0x005A97E0
// FUNCTION: fn_03230
int CSkillAction_AT_ROARING_CHARGE::Slot02() {
  return 65;
}

// OFFSET: 0x005A97F0

// OFFSET: 0x005A9BA0
// FUNCTION: fn_03232
int CSkillAction_AT_ROARING_CHARGE_S::Slot02() {
  return 65;
}

// OFFSET: 0x005A9BB0

// OFFSET: 0x005AA3B0
// FUNCTION: fn_03236
int CSkillAction_AT_FURIOS_STORM::Slot02() {
  return 65;
}

// OFFSET: 0x005AA3C0
