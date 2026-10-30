#include "Client/Mode/CGameMode.h"
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
  void sub_5A4930(int level, CGameActor* source);
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
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_PULSE_OF_MADNESS : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_AT_FRENZY_FANG : public CSkillAction_DamageWereWolfForm {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_PINION_SHOT : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_QUILL_SPEAR : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_QUILL_SPEAR_S : public CSkillAction_DamageWereRaptorForm {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_NATURE_HARMONY : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
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
  void Slot08(int level, CGameActor* source) override;
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

class CSkillAction_AT_ROARING_PIERCER : public CSkillAction_ThunderingBasic {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_ROARING_PIERCEER_S : public CSkillAction_ThunderingExpand {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_FURIOS_STORM : public CSkillAction_Thundering {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot17(CGameActor* source) override;
};

class CEfstAction_FURIOS_STORM : public CEfstAction {
public:
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_AT_ALPHA_PHASE : public CSkillAction {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_ALPHA_PHASE : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_AT_APEX_PHASE : public CSkillAction {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_AT_APEX_PHASE : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_AT_FLIP_FLAP : public CSkillAction {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_FLIP_FLAP : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_AT_AERO_SYNC : public CSkillAction {
public:
  bool Slot00() override;
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_FLIP_FLAP_TARGET : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_AT_SAVAGE_LUNGE : public CSkillAction_DamageWereWolfForm {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_TEMPEST_FLAP : public CSkillAction_DamageWereRaptorForm {
public:
  int Slot01() override;
  int Slot02() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_TERRA_HARVEST : public CSkillAction_Earthing {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AT_TERRA_WAVE : public CSkillAction_Earthing {
public:
  void Slot12(vector3d position, int argument2, int argument3) override;
};

class CSkillAction_AT_SOLID_STOMP : public CSkillAction_Earthing {
public:
  int Slot02() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_SOLID_STOMP : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_AT_ZEPHYR_LINK : public CSkillAction {
public:
  int Slot02() override;
};

class CEfstAction_ZEPHYR_LINK : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_INSANE : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_INSANE2 : public CEfstAction_INSANE {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
};

class CEfstAction_INSANE3 : public CEfstAction_INSANE {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
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

// OFFSET: 0x005A69D0
// FUNCTION: fn_03191
void CSkillAction_AT_PULSE_OF_MADNESS::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x005A6A00
// FUNCTION: fn_03192
void CSkillAction_AT_PULSE_OF_MADNESS::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source && source->GetUnknown0404() != 0) {
    this->Slot17(source);
  }

  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x57, 0x74, 0x6B, 0x1E, 0x66, 0x9A, 0x9C, 0x49, 0x78, 0x6C, 0x66, 0x09, 0x6D, 0xA0, 0x80, 0x5C, 0x27};
    thread_local EncryptedName<sizeof(bytes), 0x2FF3C5036D070127ULL> name(bytes);
    return name.Text();
  }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
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

// OFFSET: 0x005A6C70
// FUNCTION: fn_03196
void CSkillAction_AT_FRENZY_FANG::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source || !source->GetUnknown0404() || !this->Slot17(source)) {
    return;
  }

  if (!target) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0xD5, 0x15, 0xA6, 0x3D, 0x83, 0xE2, 0x6A, 0x71, 0xD2, 0x09, 0xA4, 0x53};
        thread_local EncryptedName<sizeof(bytes), 0x17359BF953C367B3ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(
      &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
    );
    effect->Slot26(angle < 90.0f || angle > 270.0f);
  }
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

// OFFSET: 0x005A6F00
// FUNCTION: fn_03199
void CSkillAction_AT_PINION_SHOT::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x005A6F30
// FUNCTION: fn_03200
void CSkillAction_AT_PINION_SHOT::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  if (!source || !source->GetUnknown0404() || !this->Slot17(source)) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0x67, 0x90, 0x21, 0xD0, 0x94, 0xDB, 0x70, 0xB6, 0x7F, 0x96, 0x3B, 0xB9};
        thread_local EncryptedName<sizeof(bytes), 0xC52FB5FBB94FF917ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(
      &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
    );
    effect->Slot17(angle);
  }
}

// OFFSET: 0x005A7180
// FUNCTION: fn_03201
void CSkillAction_AT_PINION_SHOT::Slot15(int level, CGameActor* source, CGameActor* target) {
  this->sub_5A4930(level, source);
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

// OFFSET: 0x005A71C0
// FUNCTION: fn_03204
void CSkillAction_AT_QUILL_SPEAR::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(56, level / 16);
  }
}

// OFFSET: 0x005A71F0
// FUNCTION: fn_03205
void CSkillAction_AT_QUILL_SPEAR::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  if (!source || !source->GetUnknown0404() || !this->Slot17(source)) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0xB4, 0xB8, 0xEE, 0x99, 0xCD, 0xFC, 0x94, 0xE3, 0xA0, 0xAC, 0xF5, 0xAA, 0x92, 0xDB, 0xD0, 0x93};
        thread_local EncryptedName<sizeof(bytes), 0x93E7A3A1F587CDC5ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(
      &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
    );
    effect->Slot17(angle);
    source->LaunchEffect(2442, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x005A74A0
// FUNCTION: fn_03206
void CSkillAction_AT_QUILL_SPEAR_S::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  if (!source || !source->GetUnknown0404() || !this->Slot17(source)) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
        const std::uint8_t bytes[] = {0xA6, 0x3A, 0x6A, 0xF9, 0xB5, 0xDA, 0xDC, 0x4F, 0xB2, 0x2E, 0x71, 0xCA, 0xEC, 0xFD, 0x98, 0x3F};
        thread_local EncryptedName<sizeof(bytes), 0x3FAF85D995034FD7ULL> name(bytes);
        return name.Text();
      }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    const float angle = ScreenAngle(
      &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
    );
    effect->Slot17(angle);
    source->LaunchEffect(2442, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
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

// OFFSET: 0x005A7770
// FUNCTION: fn_03209
void CSkillAction_AT_NATURE_HARMONY::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(12, level / 16);
  }
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

// OFFSET: 0x005A7A30
// FUNCTION: fn_03215
void CSkillAction_AT_GRAVITY_HOLE::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(12, level / 16);
  }
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

// OFFSET: 0x005A9F50
// FUNCTION: fn_03234
void CSkillAction_AT_ROARING_PIERCER::Slot11(int level, CGameActor* source, CGameActor* target) {
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
    effect->Slot17(angle);
  }
}

// OFFSET: 0x005AA180
// FUNCTION: fn_03235
void CSkillAction_AT_ROARING_PIERCEER_S::Slot11(int level, CGameActor* source, CGameActor* target) {
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
    effect->Slot17(angle);
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

// OFFSET: 0x005AABF0
// FUNCTION: fn_03238
void CSkillAction_AT_FURIOS_STORM::Slot17(CGameActor* source) {
}

// OFFSET: 0x005AAC00
// FUNCTION: fn_03239
void CEfstAction_FURIOS_STORM::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
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
int CSkillAction_AT_ALPHA_PHASE::Slot01() {
  return 66;
}

// OFFSET: 0x005AB600
// FUNCTION: fn_03241
int CSkillAction_AT_ALPHA_PHASE::Slot02() {
  return 68;
}

// OFFSET: 0x005AB610
// FUNCTION: fn_03242
void CSkillAction_AT_ALPHA_PHASE::Slot08(int level, CGameActor* source) {
  if (source) {
    source->LaunchEffect(54, level / 16);
  }
}

// OFFSET: 0x005AB640
// FUNCTION: fn_03243
void CSkillAction_AT_ALPHA_PHASE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x72, 0xAB, 0x73, 0x7D, 0x9A, 0xCE, 0x67, 0x27, 0x72, 0xB4, 0x66, 0x15};
      thread_local EncryptedName<sizeof(bytes), 0x4F1791FB1503C713ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005AB840
// FUNCTION: fn_03244
EfstActionMessage CEfstAction_ALPHA_PHASE::Slot03() {
  return {LookupMessage(4670), 0xFFFF00};
}

// OFFSET: 0x005AB870
// FUNCTION: fn_03245
EfstActionMessage CEfstAction_ALPHA_PHASE::Slot04() {
  return {LookupMessage(4671), 0xFF};
}

// OFFSET: 0x005AB8A0
// FUNCTION: fn_03246
int CSkillAction_AT_APEX_PHASE::Slot02() {
  return 80;
}

// OFFSET: 0x005AB8B0
// FUNCTION: fn_03247
void CSkillAction_AT_APEX_PHASE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xB2, 0x9D, 0xF2, 0x6D, 0x12, 0x2B, 0xCD, 0xC0, 0xA0, 0x88, 0x97};
      thread_local EncryptedName<sizeof(bytes), 0xA1A55B4D1597EDD3ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005ABAB0
// FUNCTION: fn_03248
EfstActionMessage CEfstAction_AT_APEX_PHASE::Slot03() {
  return {LookupMessage(4682), 0xFFFF00};
}

// OFFSET: 0x005ABAE0
// FUNCTION: fn_03249
EfstActionMessage CEfstAction_AT_APEX_PHASE::Slot04() {
  return {LookupMessage(4683), 0xFF};
}

// OFFSET: 0x005ABB10
// FUNCTION: fn_03250
int CSkillAction_AT_FLIP_FLAP::Slot02() {
  return 80;
}

// OFFSET: 0x005ABB20
// FUNCTION: fn_03251
void CSkillAction_AT_FLIP_FLAP::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x9B, 0x91, 0x14, 0x83, 0xBE, 0x73, 0x95, 0x7A, 0x8D, 0xFD};
      thread_local EncryptedName<sizeof(bytes), 0x1BF915E1F37DFDFDULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005ABD20
// FUNCTION: fn_03252
EfstActionMessage CEfstAction_FLIP_FLAP::Slot03() {
  return {LookupMessage(4678), 0xFFFF00};
}

// OFFSET: 0x005ABD50
// FUNCTION: fn_03253
EfstActionMessage CEfstAction_FLIP_FLAP::Slot04() {
  return {LookupMessage(4679), 0xFF};
}

// OFFSET: 0x005ABD80
// FUNCTION: fn_03254
bool CSkillAction_AT_AERO_SYNC::Slot00() {
  return true;
}

// OFFSET: 0x005ABD90
// FUNCTION: fn_03255
int CSkillAction_AT_AERO_SYNC::Slot02() {
  return 70;
}

// OFFSET: 0x005ABDA0
// FUNCTION: fn_03256
void CSkillAction_AT_AERO_SYNC::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xBE, 0xBC, 0x2F, 0x06, 0x4C, 0xE0, 0x86, 0x6F, 0xBC, 0x86, 0x2E, 0x10, 0x7D, 0xF0, 0xFF};
      thread_local EncryptedName<sizeof(bytes), 0x01FF9313695DD9DFULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xFA, 0xC8, 0x07, 0xA0, 0x8E, 0x4E, 0x94, 0x03, 0xF8, 0xF2, 0x18, 0xAE, 0xB8, 0x53, 0xED};
      thread_local EncryptedName<sizeof(bytes), 0x6DED3DD1CF75AD9BULL> name(bytes);
      return name.Text();
    }()), source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xA0, 0xB6, 0xA9, 0x18, 0xC4, 0x06, 0xBA, 0x27, 0xA2, 0x8C, 0xB6, 0x16, 0xF2, 0x1B, 0x9C, 0x2B,
                                    0xAE, 0xA7, 0xAF, 0x18, 0xF6, 0x75};
      thread_local EncryptedName<sizeof(bytes), 0x49C3759B77DBD3C1ULL> name(bytes);
      return name.Text();
    }()), source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005AC300
// FUNCTION: fn_03257
EfstActionMessage CEfstAction_FLIP_FLAP_TARGET::Slot03() {
  return {LookupMessage(4680), 0xFFFF00};
}

// OFFSET: 0x005AC330
// FUNCTION: fn_03258
EfstActionMessage CEfstAction_FLIP_FLAP_TARGET::Slot04() {
  return {LookupMessage(4681), 0xFF};
}

// OFFSET: 0x005AC360
// FUNCTION: fn_03259
void CEfstAction_FLIP_FLAP_TARGET::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xCE, 0x2A, 0x59, 0x8C, 0x80, 0x66, 0x2A, 0x51, 0xCC, 0x10, 0x58, 0x9A, 0xB1, 0x76, 0x53};
      thread_local EncryptedName<sizeof(bytes), 0x3F5315DFE32B4FAFULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005AC570
// FUNCTION: fn_03260
int CSkillAction_AT_SAVAGE_LUNGE::Slot02() {
  return 73;
}

// OFFSET: 0x005AC970
// FUNCTION: fn_03262
int CSkillAction_AT_TEMPEST_FLAP::Slot01() {
  return 66;
}

// OFFSET: 0x005AC980
// FUNCTION: fn_03263
int CSkillAction_AT_TEMPEST_FLAP::Slot02() {
  return 67;
}

// OFFSET: 0x005ACBA0
// FUNCTION: fn_03265
void CSkillAction_AT_TEMPEST_FLAP::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x15, 0xCA, 0xE2, 0x49, 0xEE, 0xBA, 0x8B, 0x48, 0x07, 0xC3, 0xEE, 0x49, 0x8B};
      thread_local EncryptedName<sizeof(bytes), 0x17FFC98B398FAF61ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x47, 0xF6, 0x20, 0xE5, 0xFE, 0xE6, 0x2F, 0xC4, 0x55, 0xFF, 0x2C, 0xE5, 0xC4, 0xF7, 0x34, 0xEF,
                                    0x47, 0xFC, 0x20, 0x95};
      thread_local EncryptedName<sizeof(bytes), 0x9B5B959B954D9333ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005ACF40
// FUNCTION: fn_03266
int CSkillAction_AT_TERRA_HARVEST::Slot02() {
  return 62;
}

// OFFSET: 0x005ACF50
// FUNCTION: fn_03267
void CSkillAction_AT_TERRA_HARVEST::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const int variant = std::max(std::rand() % 2, 0);
  const int effectId = CEZ2STREffectMgr::GetInstance()->EffectId(variant ? std::string([] {
    const std::uint8_t bytes[] = {0xF1, 0x04, 0xF9, 0x65, 0x1E, 0x3E, 0xCD, 0xCE, 0xF7, 0x17, 0xEE, 0x64, 0x0B, 0x3E, 0xD7, 0xCA,
                                  0xE1, 0x61};
    thread_local EncryptedName<sizeof(bytes), 0xAFA5617F178B6185ULL> name(bytes);
    return name.Text();
  }()) : std::string([] {
    const std::uint8_t bytes[] = {0x39, 0x40, 0xEB, 0x3B, 0x32, 0x1C, 0xE9, 0x66, 0x3F, 0x53, 0xFC, 0x3A, 0x27, 0x1C, 0xF8, 0x62,
                                  0x21, 0x49, 0xF6, 0x3E, 0x53};
    thread_local EncryptedName<sizeof(bytes), 0x078143534999254DULL> name(bytes);
    return name.Text();
  }()));
  const int bottomId = CEZ2STREffectMgr::GetInstance()->EffectId(variant ? std::string([] {
    const std::uint8_t bytes[] = {0x3B, 0x2E, 0xC1, 0xFD, 0x4C, 0x4A, 0x29, 0xE6, 0x3D, 0x3D, 0xD6, 0xFC, 0x59, 0x4A, 0x33, 0xE2,
                                  0x2B, 0x14, 0xD1, 0xE0, 0x59, 0x61, 0x2E, 0xEA, 0x4F};
    thread_local EncryptedName<sizeof(bytes), 0x8741152D8FB34B4FULL> name(bytes);
    return name.Text();
  }()) : std::string([] {
    const std::uint8_t bytes[] = {0x73, 0xDE, 0x9B, 0x3F, 0x98, 0x12, 0x35, 0xF4, 0x75, 0xCD, 0x8C, 0x3E, 0x8D, 0x12, 0x24, 0xF0,
                                  0x6B, 0xD7, 0x86, 0x3A, 0xA6, 0x2F, 0x32, 0xE1, 0x73, 0xD4, 0x84, 0x4D};
    thread_local EncryptedName<sizeof(bytes), 0x955D4DF94DE9BB07ULL> name(bytes);
    return name.Text();
  }()));
  g_effectMgr->Spawn(target->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(target->render.accountId, bottomId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x005AD630
// FUNCTION: fn_03268
void CSkillAction_AT_TERRA_WAVE::Slot12(vector3d position, int argument2, int argument3) {
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x7F, 0xD0, 0xFB, 0x67, 0x68, 0xE8, 0xEE, 0xA2, 0x7D, 0xD0, 0x89};
    thread_local EncryptedName<sizeof(bytes), 0xC399B7091589B50BULL> name(bytes);
    return name.Text();
  }()), position.x, position.y, position.z, 0, -1, 0, 0);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x1B, 0xC8, 0x53, 0x9B, 0x5E, 0x58, 0x4E, 0xD6, 0x19, 0xC8, 0x7E, 0x8B, 0x50, 0x73, 0x4D, 0xD8,
                                  0x02, 0xAD};
    thread_local EncryptedName<sizeof(bytes), 0xB739073FE921AD6FULL> name(bytes);
    return name.Text();
  }()), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005AD970
// FUNCTION: fn_03269
int CSkillAction_AT_SOLID_STOMP::Slot02() {
  return 63;
}

// OFFSET: 0x005AD980
// FUNCTION: fn_03270
void CSkillAction_AT_SOLID_STOMP::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xAA, 0x56, 0x7F, 0x10, 0xAF, 0xDC, 0x0E, 0x8B, 0xB6, 0x54, 0x63, 0x79};
      thread_local EncryptedName<sizeof(bytes), 0xFF7D83CB791339D9ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x66, 0xFC, 0xDF, 0xA8, 0x83, 0x1C, 0x22, 0x99, 0x7A, 0xFE, 0xC3, 0x9E, 0x85, 0x2C, 0x25, 0x99,
                                    0x7A, 0xFE, 0xB3};
      thread_local EncryptedName<sizeof(bytes), 0xED5143E7C1B39315ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005ADD10
// FUNCTION: fn_03271
EfstActionMessage CEfstAction_SOLID_STOMP::Slot03() {
  return {LookupMessage(4686), 0xFFFF00};
}

// OFFSET: 0x005ADD40
// FUNCTION: fn_03272
EfstActionMessage CEfstAction_SOLID_STOMP::Slot04() {
  return {LookupMessage(4687), 0xFF};
}

// OFFSET: 0x005ADD70
// FUNCTION: fn_03273
int CSkillAction_AT_ZEPHYR_LINK::Slot02() {
  return 0;
}

// OFFSET: 0x005ADD80
// FUNCTION: fn_03274
void CEfstAction_ZEPHYR_LINK::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    this->Slot01(efstId, value, argument3, argument4, actor);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xEB, 0xA6, 0xCD, 0x09, 0x1C, 0xE3, 0x62, 0x55, 0xF8, 0xAD, 0xD6, 0x61};
      thread_local EncryptedName<sizeof(bytes), 0x393D916561BDC391ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xEF, 0x98, 0x61, 0xDB, 0x28, 0x17, 0xCA, 0x5D, 0xFC, 0x93, 0x7A, 0xEC, 0x33, 0x0A, 0xE1, 0x45,
                                    0xFA, 0x90, 0x11};
      thread_local EncryptedName<sizeof(bytes), 0x31956551B311FD95ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x005AE130
// FUNCTION: fn_03275
void CEfstAction_ZEPHYR_LINK::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xF3, 0x58, 0x1D, 0x8D, 0xDE, 0x73, 0x78, 0x43, 0xE0, 0x53, 0x06, 0xE5};
      thread_local EncryptedName<sizeof(bytes), 0x2F2701A7E56D3D89ULL> name(bytes);
      return name.Text();
    }()));
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x5F, 0xF4, 0x77, 0xCB, 0x98, 0x73, 0x6C, 0x07, 0x4C, 0xFF, 0x6C, 0xFC, 0x83, 0x6E, 0x47, 0x1F,
                                    0x4A, 0xFC, 0x07};
      thread_local EncryptedName<sizeof(bytes), 0x6B3301E1A3079125ULL> name(bytes);
      return name.Text();
    }()));
  }
}

// OFFSET: 0x005AE460
// FUNCTION: fn_03276
void CEfstAction_ZEPHYR_LINK::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    this->Slot00(efstId, value, 0, 0, actor, false);
  }
}

// OFFSET: 0x005AE480
// FUNCTION: fn_03277
void CEfstAction_INSANE::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    this->Slot01(efstId, value, argument3, argument4, actor);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xCA, 0xC7, 0xB6, 0x43, 0x48, 0x00, 0xD9, 0x02, 0xCE, 0xCF, 0xA0, 0x68, 0x27, 0x58, 0xB7};
      thread_local EncryptedName<sizeof(bytes), 0x71B7691737C5A1AFULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x06, 0x5F, 0x0E, 0xD5, 0x06, 0xDC, 0xF1, 0xB0, 0x02, 0x57, 0x18, 0xFE, 0x3B, 0xDA, 0xEB, 0xB7,
                                    0x0C, 0x54, 0x22, 0x91, 0x68, 0xB5};
      thread_local EncryptedName<sizeof(bytes), 0xC39FB559A17D3963ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    static_cast<CPc*>(actor)->unknown_04F4 = static_cast<int>(0xFFFFF200);
  }
}

// OFFSET: 0x005AE840
// FUNCTION: fn_03278
void CEfstAction_INSANE::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    static_cast<CPc*>(actor)->unknown_04F4 = 0;
  }
}

// OFFSET: 0x005AE860
// FUNCTION: fn_03279
void CEfstAction_INSANE::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    this->Slot00(efstId, value, 0, 0, actor, false);
  }
}

// OFFSET: 0x005AE880
// FUNCTION: fn_03280
void CEfstAction_INSANE2::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    this->Slot01(efstId, value, argument3, argument4, actor);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x32, 0x97, 0xC6, 0xBF, 0x5C, 0x98, 0x8D, 0x36, 0x36, 0x9F, 0xD0, 0x94, 0x33, 0xC3, 0xE3};
      thread_local EncryptedName<sizeof(bytes), 0x45E3F103CBB5F157ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xCE, 0x29, 0xEC, 0xD7, 0xDA, 0x46, 0x7F, 0x74, 0xCA, 0x21, 0xFA, 0xFC, 0xE7, 0x40, 0x65, 0x73,
                                    0xC4, 0x22, 0xC0, 0x93, 0xB7, 0x2F};
      thread_local EncryptedName<sizeof(bytes), 0x07112F85A39F4FABULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    static_cast<CPc*>(actor)->unknown_04F4 = static_cast<int>(0xFFFF7F27);
  }
}

// OFFSET: 0x005AEC40
// FUNCTION: fn_03281
void CEfstAction_INSANE3::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    this->Slot01(efstId, value, argument3, argument4, actor);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xBA, 0x21, 0xDE, 0x13, 0x40, 0x10, 0x5D, 0x5E, 0xBE, 0x29, 0xC8, 0x38, 0x2F, 0x4A, 0x33};
      thread_local EncryptedName<sizeof(bytes), 0x2D33791F67AD47DFULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0x90, 0xE1, 0x8E, 0x89, 0x70, 0x54, 0x01, 0xE0, 0x94, 0xE9, 0x98, 0xA2, 0x4D, 0x52, 0x1B, 0xE7,
                                    0x9A, 0xEA, 0xA2, 0xCD, 0x1C, 0x3D};
      thread_local EncryptedName<sizeof(bytes), 0x936F3D2FFDFD87F5ULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    static_cast<CPc*>(actor)->unknown_04F4 = static_cast<int>(0xFFED1C24);
  }
}

// OFFSET: 0x005AF030
// FUNCTION: fn_03285
bool __stdcall sub_5AF030(int) {
  return false;
}

// OFFSET: 0x005A5E50
// FUNCTION: fn_03170
bool CSkillAction_AnimalForm::CanUse(CGameActor* actor) {
  return actor && actor->HasShapeShift() && this->Slot17(actor);
}

// OFFSET: 0x005AC580
// FUNCTION: fn_03261
void CSkillAction_AT_SAVAGE_LUNGE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target || !this->CanUse(source)) {
    return;
  }

  if (CRagEffect* effect = source->LaunchEffect(2439, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
  }
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x9A, 0xB2, 0x4B, 0x66, 0x52, 0x4C, 0x54, 0x89, 0x9C, 0xBD, 0x5A, 0x62, 0x35};
    thread_local EncryptedName<sizeof(bytes), 0xE50B2935073DD3E9ULL> name(bytes);
    return name.Text();
  }()), target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0x82, 0xA2, 0x9F, 0xDC, 0xCA, 0x68, 0x2A, 0x11, 0x84, 0xAD, 0x8E, 0xD8, 0xF2, 0x6F, 0x1A, 0x09,
                                  0x85, 0xAC, 0x84, 0xBD};
    thread_local EncryptedName<sizeof(bytes), 0x7D750DADBDE9C3F1ULL> name(bytes);
    return name.Text();
  }()), target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
}

// OFFSET: 0x005AC990
// FUNCTION: fn_03264
void CSkillAction_AT_TEMPEST_FLAP::Slot08(int level, CGameActor* source) {
  if (this->CanUse(source)) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId([] {
      const std::uint8_t bytes[] = {0xCB, 0xD8, 0xB2, 0x4F, 0x28, 0x5E, 0xF7, 0x6C, 0xD9, 0xD1, 0xBE, 0x4F, 0x12, 0x4E, 0xE2, 0x40,
                                    0xCB, 0xBD};
      thread_local EncryptedName<sizeof(bytes), 0x33832D4D3FDFBDBFULL> name(bytes);
      return name.Text();
    }()), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}
