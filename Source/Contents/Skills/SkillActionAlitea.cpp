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
public:
  virtual bool Slot17(CGameActor* actor) = 0;
  virtual void Slot18(CGameActor* actor) = 0;
};

class CSkillAction_Direction {
public:
  int GetDirection(float angle);
  bool IsValidDirection(unsigned int direction);
};

class CSkillAction_DamageWereRaptorForm : public CSkillAction_AnimalForm, public CSkillAction_Direction {
public:
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
  bool Slot17(CGameActor* actor) override;
  void Slot18(CGameActor* actor) override;
};

class CSkillAction_DamageWereWolfForm : public CSkillAction_AnimalForm {
public:
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
  bool Slot17(CGameActor* actor) override;
  void Slot18(CGameActor* actor) override;
};

class CSkillAction_Earthing : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_Icing : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
  void SpawnIcing(CGameActor* target);
};

class CSkillAction_Thundering : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
  virtual void Slot17(CGameActor* source) = 0;
};

class CSkillAction_ThunderingBasic : public CSkillAction_Thundering {
public:
  void Slot17(CGameActor* source) override;
};

class CSkillAction_ThunderingExpand : public CSkillAction_Thundering {
public:
  void Slot17(CGameActor* source) override;
};

class CEfstAction_BasicAction : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  virtual int Slot05(int efstId, int argument4) = 0;
};

class CEfstAction_AnimalForm : public CEfstAction {
public:
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_AT_ALPHA_CLAW : public CSkillAction_DamageWereWolfForm {
public:
  int Slot02() override;
};

class CSkillAction_AT_PULSE_OF_MADNESS : public CSkillAction_DamageWereWolfForm {
public:
  int Slot01() override;
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

// OFFSET: 0x005A50B0
// FUNCTION: fn_03155
void CSkillAction_DamageWereRaptorForm::Slot18(CGameActor* actor) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x7B, 0x43, 0x92, 0x8F, 0x84, 0x01, 0x32, 0x6A, 0x7A, 0x43, 0xA8, 0x95, 0x8C, 0x05, 0x19, 0x72,
                                    0x6D, 0x31};
      thread_local EncryptedName<sizeof(bytes), 0x1D6D75EDE7CD311FULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A52B0
// FUNCTION: fn_03156
void CSkillAction_DamageWereWolfForm::Slot18(CGameActor* actor) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x31, 0x8F, 0xEA, 0xB3, 0xAE, 0xC5, 0x2A, 0x44, 0x30, 0x8F, 0xD0, 0xAC, 0xA8, 0xDD, 0x13, 0x33};
      thread_local EncryptedName<sizeof(bytes), 0x3375B1C7DBB5FD55ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A54B0
// FUNCTION: fn_03157
void CSkillAction_Icing::SpawnIcing(CGameActor* target) {
  if (target) {
    if (CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
          const std::uint8_t bytes[] = {0x18, 0x9A, 0x3C, 0xAA, 0xE9, 0x74, 0x11, 0x91};
          thread_local EncryptedName<sizeof(bytes), 0x91651D81F559F971ULL> name(bytes);
          return name.Text();
        }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot22(0.9f);
    }
  }
}

// OFFSET: 0x005A56C0
// FUNCTION: fn_03158
void CSkillAction_ThunderingBasic::Slot17(CGameActor* source) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xA3, 0x27, 0x76, 0xFB, 0xBD, 0xE0, 0xDD, 0x56, 0xB9, 0x28, 0x5C, 0xF4, 0xAD, 0xF1, 0xCE, 0x5C,
                                    0xBC, 0x10, 0x64, 0x95};
      thread_local EncryptedName<sizeof(bytes), 0x3FAF85D995034FD7ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A58C0
// FUNCTION: fn_03159
void CSkillAction_ThunderingExpand::Slot17(CGameActor* source) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xCD, 0x65, 0x5A, 0xCF, 0x27, 0x9E, 0x7D, 0x20, 0xD7, 0x6A, 0x70, 0xC0, 0x37, 0x8F, 0x6E, 0x2A,
                                    0xD2, 0x52, 0x56, 0xA1};
      thread_local EncryptedName<sizeof(bytes), 0x490FFB43A12F0DB9ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A5D20
// FUNCTION: fn_03161
int CSkillAction_Earthing::Slot01() {
  return 60;
}

// OFFSET: 0x005A5D30
// FUNCTION: fn_03162
int CSkillAction_Icing::Slot01() {
  return 61;
}

// OFFSET: 0x005A5D40
// FUNCTION: fn_03163
int CSkillAction_Thundering::Slot01() {
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
int CSkillAction_Earthing::Slot02() {
  return 62;
}

// OFFSET: 0x005A5DF0
// FUNCTION: fn_03166
int CSkillAction_Icing::Slot02() {
  return 64;
}

// OFFSET: 0x005A5E00
// FUNCTION: fn_03167
int CSkillAction_Thundering::Slot02() {
  return 64;
}

// OFFSET: 0x005A5E10
// FUNCTION: fn_03168
bool CSkillAction_DamageWereRaptorForm::Slot17(CGameActor* actor) {
  return actor->GetUnknown0404() == 2;
}

// OFFSET: 0x005A5E30
// FUNCTION: fn_03169
bool CSkillAction_DamageWereWolfForm::Slot17(CGameActor* actor) {
  return actor->GetUnknown0404() == 1;
}

// OFFSET: 0x005A5E90
// FUNCTION: fn_03171
bool CSkillAction_Direction::IsValidDirection(unsigned int direction) {
  return direction <= 3;
}

// OFFSET: 0x005A5EA0
// FUNCTION: fn_03172
void CSkillAction_DamageWereRaptorForm::Slot16(int level, CGameActor* source, CGameActor* target) {
  this->Slot18(source);
}

// OFFSET: 0x005A5EB0
// FUNCTION: fn_03173
void CSkillAction_DamageWereWolfForm::Slot16(int level, CGameActor* source, CGameActor* target) {
  this->Slot18(source);
}

// OFFSET: 0x005A5EC0
// FUNCTION: fn_03174
void CSkillAction_Earthing::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  if (CRagEffect* effect = source->LaunchEffect(0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&source->render.position), 0, 0, 0, 0);
  }
}

// OFFSET: 0x005A5F30
// FUNCTION: fn_03175
void CSkillAction_Icing::Slot16(int level, CGameActor* source, CGameActor* target) {
  this->SpawnIcing(source);
}

// OFFSET: 0x005A5F40
// FUNCTION: fn_03176
void CSkillAction_Thundering::Slot16(int level, CGameActor* source, CGameActor* target) {
  this->Slot17(source);
}

// OFFSET: 0x005A5F50
// FUNCTION: fn_03177
void CEfstAction_BasicAction::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    const int id = this->Slot05(efstId, argument4);
    if (id >= 0) {
      g_effectMgr->Spawn(actor->render.accountId, id, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
  }
}

// OFFSET: 0x005A5FB0
// FUNCTION: fn_03178
void CSkillAction_Earthing::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(57, level / 16);
  }
}

// OFFSET: 0x005A5FE0
// FUNCTION: fn_03179
void CSkillAction_Icing::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ice_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
    if (CEffect* effect =
            g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ice_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x005A6260
// FUNCTION: fn_03180
void CSkillAction_Thundering::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
          const std::uint8_t bytes[] = {0xB1, 0xA5, 0xF2, 0x9B, 0xC5, 0xC6, 0x95, 0xFA, 0xAB, 0xAA, 0xD8, 0x96, 0xC0, 0xD0, 0x93, 0x93};
          thread_local EncryptedName<sizeof(bytes), 0x93E7A3A1F587CDC5ULL> name(bytes);
          return name.Text();
        }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x005A6480
// FUNCTION: fn_03182
void CEfstAction_BasicAction::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    const int id = this->Slot05(efstId, argument4);
    if (id >= 0) {
      g_effectMgr->Retire(actor->render.accountId, id);
    }
  }
}

// OFFSET: 0x005A64C0
// FUNCTION: fn_03183
void CEfstAction_AnimalForm::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x005A64E0
// FUNCTION: fn_03184
void CEfstAction_BasicAction::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    this->Slot00(efstId, value, argument3, argument4, actor, false);
  }
}

// OFFSET: 0x005A6790
// FUNCTION: fn_03187
int CSkillAction_AT_ALPHA_CLAW::Slot02() {
  return 76;
}

// OFFSET: 0x005A69B0
// FUNCTION: fn_03189
int CSkillAction_AT_PULSE_OF_MADNESS::Slot01() {
  return 66;
}

// OFFSET: 0x005A69C0
// FUNCTION: fn_03190
int CSkillAction_AT_PULSE_OF_MADNESS::Slot02() {
  return 68;
}

// OFFSET: 0x005A6C00
// FUNCTION: fn_03193
EfstActionMessage CEfstAction_PULSE_OF_MADNESS::Slot03() {
  return {LookupMessage(4668), 0xFFFF00};
}

// OFFSET: 0x005A6C30
// FUNCTION: fn_03194
EfstActionMessage CEfstAction_PULSE_OF_MADNESS::Slot04() {
  return {LookupMessage(4669), 0xFF};
}

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
// FUNCTION: fn_03210
void CSkillAction_AT_NATURE_HARMONY::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xD7, 0x6C, 0x5B, 0xD4, 0x31, 0x9E, 0x50, 0x21, 0xD8, 0x7F, 0x42, 0xCE, 0x2D, 0x82, 0x0F};
      thread_local EncryptedName<sizeof(bytes), 0x490FFB43A12F0DB9ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A79B0
// FUNCTION: fn_03211
EfstActionMessage CEfstAction_NATURE_HARMONY::Slot03() {
  return {LookupMessage(4684), 0xFFFF00};
}

// OFFSET: 0x005A79E0
// FUNCTION: fn_03212
EfstActionMessage CEfstAction_NATURE_HARMONY::Slot04() {
  return {LookupMessage(4685), 0xFF};
}

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
// FUNCTION: fn_03216
void CSkillAction_AT_GRAVITY_HOLE::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    if (CRagEffect* effect = source->LaunchEffect(0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&source->render.position), 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x005A7AD0
// FUNCTION: fn_03217
void CSkillAction_AT_GRAVITY_HOLE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x16, 0x8B, 0x38, 0x83, 0xE8, 0x69, 0x1C, 0xCE, 0x19, 0x96, 0x35, 0x90, 0x81};
      thread_local EncryptedName<sizeof(bytes), 0x91651D81F559F971ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x6C, 0x09, 0xBE, 0x17, 0x22, 0x13, 0xA8, 0x2C, 0x63, 0x14, 0xB3, 0x04, 0x14, 0x05, 0xBE, 0x07, 0x7F, 0x14, 0xB2, 0x61};
      thread_local EncryptedName<sizeof(bytes), 0x73D1674B61DF7B0BULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A7E70
// FUNCTION: fn_03218
int CSkillAction_AT_CHILLING_BLAST::Slot02() {
  return 65;
}

// OFFSET: 0x005A7E80
// FUNCTION: fn_03219
void CSkillAction_AT_CHILLING_BLAST::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x0E, 0x0B, 0x5C, 0x41, 0xC9, 0x6A, 0xE1, 0xBE, 0x32, 0x01, 0x59, 0x4C, 0xD6, 0x77, 0x8F};
      thread_local EncryptedName<sizeof(bytes), 0xD98F03A52D35636DULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xB2, 0x71, 0xF4, 0xE3, 0x07, 0xE2, 0x61, 0x68, 0x8E, 0x7B, 0xF1, 0xEE, 0x18, 0xFF, 0x50, 0x6D,
                                    0xBE, 0x6D, 0xE9, 0xE0, 0x06, 0x8B};
      thread_local EncryptedName<sizeof(bytes), 0x0F0F8B6B8F9D19D1ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

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
// FUNCTION: fn_03222
void CSkillObjectAction_AT_GLACIER_MONOLITH::Slot00(int argument, CRenderObject* object, CGameActor* source) {
  if (!object) {
    return;
  }

  g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x6E, 0xC5, 0x26, 0x3C, 0xB0, 0x38, 0x07, 0x8C, 0x64, 0xC6, 0x29, 0x30, 0xB5, 0x34, 0x01, 0xBB,
                                  0x56, 0xDA, 0x33, 0x3E, 0xAB, 0x29, 0x75};
    thread_local EncryptedName<sizeof(bytes), 0xD3755DD95F47A909ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x0A, 0x8F, 0x24, 0xC0, 0x84, 0xA4, 0xA5, 0x6E, 0x00, 0x8C, 0x2B, 0xCC, 0x81, 0xA8, 0xA3, 0x59,
                                  0x32, 0x90, 0x31, 0xC2, 0x9F, 0xB5, 0x88, 0x53, 0x02, 0x97, 0x31, 0xCC, 0x80, 0xC1};
    thread_local EncryptedName<sizeof(bytes), 0x31D7C1EDA345E36DULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);

  if (CEffect* effect = g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0x16, 0x77, 0x92, 0x9C, 0xEE, 0x40, 0xE3, 0x98, 0x1C, 0x74, 0x9D, 0x90, 0xEB, 0x4C, 0xE5, 0xAF,
                                      0x2E, 0x77, 0x9C, 0x90, 0xF7, 0x25};
        thread_local EncryptedName<sizeof(bytes), 0xC7912587FFF31B71ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot16(990);
  }
  if (CEffect* effect = g_effectMgr->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0x0A, 0x5F, 0x6E, 0xF6, 0xB2, 0x20, 0xA1, 0x36, 0x00, 0x5C, 0x61, 0xFA, 0xB7, 0x2C, 0xA7, 0x01,
                                      0x32, 0x5F, 0x60, 0xFA, 0xAB, 0x1A, 0xB1, 0x06, 0x19, 0x47, 0x60, 0xF8, 0xDB};
        thread_local EncryptedName<sizeof(bytes), 0x69D345DB950F336DULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot16(990);
  }
}

// OFFSET: 0x005A8960
// FUNCTION: fn_03223
void CSkillObjectAction_AT_GLACIER_MONOLITH::Slot01(CRenderObject* object) {
  if (!object) {
    return;
  }

  g_effectMgr->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x48, 0xF3, 0xD2, 0xB8, 0x86, 0xAC, 0x65, 0xC4, 0x42, 0xF0, 0xDD, 0xB4, 0x83, 0xA0, 0x63, 0xF3,
                                  0x70, 0xF3, 0xDC, 0xB4, 0x9F, 0xC9};
    thread_local EncryptedName<sizeof(bytes), 0x9B17C9EFDBB39F2FULL> name(bytes);
    return name.Text();
  }()));
  g_effectMgr->Retire(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0xBA, 0xB9, 0x28, 0x22, 0x1E, 0x70, 0x49, 0x72, 0xB0, 0xBA, 0x27, 0x2E, 0x1B, 0x7C, 0x4F, 0x45,
                                  0x82, 0xB9, 0x26, 0x2E, 0x07, 0x4A, 0x59, 0x42, 0xA9, 0xA1, 0x26, 0x2C, 0x77};
    thread_local EncryptedName<sizeof(bytes), 0x2D3B15774149D5DDULL> name(bytes);
    return name.Text();
  }()));

  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x86, 0xF7, 0x92, 0x18, 0x84, 0xE2, 0x8D, 0x3E, 0x8C, 0xF4, 0x9D, 0x14, 0x81, 0xEE, 0x8B, 0x09, 0xBE, 0xFE, 0x9D, 0x1F, 0xED};
    thread_local EncryptedName<sizeof(bytes), 0x61FF87ED7BF39BE1ULL> name(bytes);
    return name.Text();
  }()), object->render.position.x, object->render.position.y, object->render.position.z, 0, -1, 0, 0);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x98, 0xE9, 0x46, 0x6E, 0x0C, 0x5A, 0xEB, 0x22, 0x92, 0xEA, 0x49, 0x62, 0x09, 0x56, 0xED, 0x15,
                                  0xA0, 0xE0, 0x49, 0x69, 0x3A, 0x5D, 0xF6, 0x09, 0x8B, 0xEA, 0x4A, 0x0D};
    thread_local EncryptedName<sizeof(bytes), 0x7D993F650D2785FFULL> name(bytes);
    return name.Text();
  }()), object->render.position.x, object->render.position.y, object->render.position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005A8FC0
// FUNCTION: fn_03224
int CSkillAction_AT_GLACIER_NOVA::Slot02() {
  return 65;
}

// OFFSET: 0x005A8FD0
// FUNCTION: fn_03225
void CSkillAction_AT_GLACIER_NOVA::Slot12(vector3d position, int argument2, int argument3) {
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x54, 0x4B, 0x52, 0x6C, 0xD4, 0x28, 0x4B, 0x72, 0x5D, 0x48, 0x45, 0x6E, 0xBD};
    thread_local EncryptedName<sizeof(bytes), 0x2D394DBD0F332733ULL> name(bytes);
    return name.Text();
  }()), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005A91A0
// FUNCTION: fn_03226
void CSkillAction_AT_GLACIER_SHARD::Slot10(int level, CGameActor* source, int argument3, int argument4, int argument5) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
          const std::uint8_t bytes[] = {0x20, 0x4D, 0xDA, 0x88, 0x82, 0x12, 0x6D, 0x4A, 0x34, 0x49, 0xDA, 0x99, 0x8F, 0x28, 0x6B, 0x74,
                                        0x35, 0x46, 0xDE, 0x9F, 0xEB};
          thread_local EncryptedName<sizeof(bytes), 0x151F77EBEBBB2147ULL> name(bytes);
          return name.Text();
        }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x005A93B0
// FUNCTION: fn_03227
void CSkillAction_AT_GLACIER_SHARD::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xA6, 0xA3, 0xB2, 0x50, 0xFE, 0x78, 0x8F, 0x36, 0xB2, 0xA7, 0xB2, 0x41, 0xF3, 0x1D};
      thread_local EncryptedName<sizeof(bytes), 0x69FD1D9733D3CFC1ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A95C0
// FUNCTION: fn_03228
int CSkillAction_AT_GLACIER_STOMP::Slot02() {
  return 65;
}

// OFFSET: 0x005A95D0
// FUNCTION: fn_03229
void CSkillAction_AT_GLACIER_STOMP::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xA0, 0xA1, 0x84, 0x5E, 0x76, 0x18, 0x17, 0xAA, 0xB4, 0xB9, 0x8A, 0x50, 0x6F, 0x7D};
      thread_local EncryptedName<sizeof(bytes), 0xF5657D1F3DE5CDC7ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A97E0
// FUNCTION: fn_03230
int CSkillAction_AT_ROARING_CHARGE::Slot02() {
  return 65;
}

// OFFSET: 0x005A97F0
// FUNCTION: fn_03231
void CSkillAction_AT_ROARING_CHARGE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x43, 0x36, 0x4A, 0x73, 0x08, 0xA3, 0x02, 0xD4, 0x52, 0x31, 0x4A, 0x73, 0x06, 0xA8, 0x65};
      thread_local EncryptedName<sizeof(bytes), 0x8B65CD61012B5931ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x97, 0xE0, 0x2C, 0x67, 0x5E, 0x6F, 0x34, 0xA8, 0x86, 0xE7, 0x2C, 0x67, 0x50, 0x64, 0x0C, 0x95,
                                    0x8A, 0xFB, 0x39, 0x7A, 0x5A, 0x01};
      thread_local EncryptedName<sizeof(bytes), 0xF7530137154D8FE5ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005A9BA0
// FUNCTION: fn_03232
int CSkillAction_AT_ROARING_CHARGE_S::Slot02() {
  return 65;
}

// OFFSET: 0x005A9BB0
// FUNCTION: fn_03233
void CSkillAction_AT_ROARING_CHARGE_S::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xB1, 0xAC, 0xB4, 0xF9, 0x32, 0x85, 0x58, 0xD4, 0xA0, 0xAB, 0xB4, 0xF9, 0x3C, 0x8E, 0x60, 0xF8, 0xC3};
      thread_local EncryptedName<sizeof(bytes), 0x8B3FEB5B8BD5C3C3ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xD5, 0xDC, 0x64, 0xBB, 0xCC, 0x0D, 0x8C, 0x54, 0xC4, 0xDB, 0x64, 0xBB, 0xC2, 0x06, 0xB4, 0x78,
                                    0xF8, 0xD1, 0x6A, 0xBD, 0xD1, 0x0C, 0x86, 0x0B};
      thread_local EncryptedName<sizeof(bytes), 0x0BEB63A5C905B3A7ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005AA3B0
// FUNCTION: fn_03236
int CSkillAction_AT_FURIOS_STORM::Slot02() {
  return 65;
}

// OFFSET: 0x005AA3C0
// FUNCTION: fn_03237
void CSkillAction_AT_FURIOS_STORM::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0x6F, 0x65, 0x4C, 0x67, 0xCB, 0x2C, 0xFD, 0x32, 0x75, 0x6A, 0x66, 0x6A, 0xCE, 0x3A, 0xFB, 0x04,
                                      0x76, 0x78, 0x4D, 0x6C, 0xAF};
        thread_local EncryptedName<sizeof(bytes), 0x5B8F49AF09390D1BULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot14(1000);
  }

  const int stormA = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x89, 0x56, 0xA7, 0x86, 0x76, 0x22, 0xCE, 0x48, 0x9B, 0x4C, 0xA7, 0x82, 0x46, 0x30, 0x91};
    thread_local EncryptedName<sizeof(bytes), 0x3B915119EFD523EFULL> name(bytes);
    return name.Text();
  }());
  const int stormABottom = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x31, 0xF6, 0x9B, 0xBC, 0xEE, 0xB6, 0x12, 0x34, 0x23, 0xEC, 0x9B, 0xB8, 0xDE, 0xA4, 0x12, 0x25,
                                  0x38, 0xF7, 0x9D, 0xBA, 0xEC, 0xC5};
    thread_local EncryptedName<sizeof(bytes), 0x474DC581D5E98357ULL> name(bytes);
    return name.Text();
  }());
  const int stormB = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x0B, 0x2A, 0x8B, 0x5A, 0x8C, 0x08, 0x0A, 0x4C, 0x19, 0x30, 0x8B, 0x5E, 0xBC, 0x19, 0x55};
    thread_local EncryptedName<sizeof(bytes), 0x3F557BE333F95F6DULL> name(bytes);
    return name.Text();
  }());
  const int stormBBottom = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x5F, 0x94, 0xDB, 0x86, 0x8E, 0xD4, 0x0C, 0x74, 0x4D, 0x8E, 0xDB, 0x82, 0xBE, 0xC5, 0x0C, 0x65,
                                  0x56, 0x95, 0xDD, 0x80, 0x8C, 0xA7};
    thread_local EncryptedName<sizeof(bytes), 0x0753A7E1EFA9E139ULL> name(bytes);
    return name.Text();
  }());

  for (int delay = 0; delay < 1000; delay += 250) {
    const int variant = std::max(std::rand() % 2, 0);
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, variant ? stormA : stormB, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot16(delay);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, variant ? stormABottom : stormBBottom, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot16(delay);
    }
  }
}
