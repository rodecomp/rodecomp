#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/CharacterParticleBundle.h"
#include "Contents/Effects/GravityControlEffect.h"
#include "Contents/Effects/SkillEffects.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/AMachineEffect.h"
#include "Contents/Effects/AcidifiedThrow.h"
#include "Contents/Effects/AstralStrikeEffect.h"
#include "Contents/Effects/AxeBoomerang.h"
#include "Contents/Effects/BindTrapEffect.h"
#include "Contents/Effects/BlastForge.h"
#include "Contents/Effects/CaneOfEvilEyeEffect.h"
#include "Contents/Effects/CartCannon.h"
#include "Contents/Effects/CellRangeEffect.h"
#include "Contents/Effects/ChangeSizeEffect.h"
#include "Contents/Effects/ChargingPierceEffect.h"
#include "Contents/Effects/CherryBlossomEffect.h"
#include "Contents/Effects/ClimaxEffect.h"
#include "Contents/Effects/CloudEffect.h"
#include "Contents/Effects/CloudKillEffect.h"
#include "Contents/Effects/CommonFloorEffect.h"
#include "Contents/Effects/Competentia.h"
#include "Contents/Effects/CresciveBolt.h"
#include "Contents/Effects/EZ2STREffect.h"
#include "Contents/Effects/EZ2STREffectEx.h"
#include "Contents/Effects/EmitterEffect.h"
#include "Contents/Effects/EnergyOrbEffect.h"
#include "Contents/Effects/EvilLandEffect.h"
#include "Contents/Effects/EvilsPawEffect.h"
#include "Contents/Effects/FriggSongEffect.h"
#include "Contents/Effects/FuumaShouakuEffect.h"
#include "Contents/Effects/HealEffect.h"
#include "Contents/Effects/JupitelThunderStormEffect.h"
#include "Contents/Effects/Level150Effect.h"
#include "Contents/Effects/Level99Effect.h"
#include "Contents/Effects/Level99Orb1Effect.h"
#include "Contents/Effects/Level99Orb2Effect.h"
#include "Contents/Effects/LightSphereEffect.h"
#include "Contents/Effects/MagicFloorEffect.h"
#include "Contents/Effects/MapChainEffect.h"
#include "Contents/Effects/MonsterSkillEffects.h"
#include "Contents/Effects/NyanggrassEffect.h"
#include "Contents/Effects/PowderingEffect.h"
#include "Contents/Effects/RectUpEffect.h"
#include "Contents/Effects/RotateItemEffect.h"
#include "Contents/Effects/RotateLineEffect.h"
#include "Contents/Effects/ScatterEffect.h"
#include "Contents/Effects/ShadowExceedEffect.h"
#include "Contents/Effects/ShowEffect.h"
#include "Contents/Effects/SmokeEffect.h"
#include "Contents/Effects/SoulCollectEffect.h"
#include "Contents/Effects/SoulUnityEffect.h"
#include "Contents/Effects/SpriteEffect.h"
#include "Contents/Effects/SquareRangeEffect.h"
#include "Contents/Effects/StarEmperorEffect.h"
#include "Contents/Effects/StormCannonEffect.h"
#include "Contents/Effects/StormKickEffect.h"
#include "Contents/Effects/SunStanceEffect.h"
#include "Contents/Effects/TalismanMarkEffect.h"
#include "Contents/Effects/TetraBallEffect.h"
#include "Contents/Effects/ThrowEffects.h"
#include "Contents/Effects/TriangleShotArrow.h"
#include "Contents/Effects/ViolentQuakeEffect.h"
#include "Contents/Effects/WindEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Render/Renderer.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>
#include <cstdint>
#include <new>
#include <string>

// GLOBAL: gl_24787
int g_levelEffectId1;

// GLOBAL: gl_24788
int g_levelEffectId2;

void CRenderObject::ClearLevelEffects() {
  reinterpret_cast<unsigned char*>(this)[0x2B8] = 0;
  this->SendMsg(this, 0x6D, 0, 0xC8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(this, 0x6D, 0, 0x3D0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(this, 0x6D, 0, 0x3D1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(this, 0x6D, 0, 0x3D2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(this, 0x6D, 0, 0x3D3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(this, 0x6D, 0, 0x3FE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(this, 0x6D, 0, 0x3FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(this, 0x6D, 0, 0x4CA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(this, 0x6D, 0, 0x4CB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, 0x6D, 0, 0x2F5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, 0x6D, 0, 0x370, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, 0x6D, 0, 0xC8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, 0x6D, 0, 0x18D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, 0x6D, 0, 0x18E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, 0x6D, 0, 0x16A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, 0x6D, 0, 0x16A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, 0x6D, 0, 0xC9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, 0x6D, 0, 0xCA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(nullptr, 0x6D, 0, 0xCA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(this, 0x6D, 0, g_levelEffectId1, g_levelEffectId1 >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->SendMsg(this, 0x6D, 0, g_levelEffectId2, g_levelEffectId2 >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00B25750
// FUNCTION: fn_18783
CEffect* EffectLists::Find(int actor, int id) const {
  const auto found = this->active.find(actor);

  if (found != this->active.end()) {
    for (CEffect* effect : found->second) {
      if (effect->id == id) {
        return effect;
      }
    }
  }

  return nullptr;
}

// OFFSET: 0x00B3A530
// FUNCTION: fn_18788
bool EffectLists::Has(int actor, int id) const {
  return this->Find(actor, id) != nullptr;
}

// OFFSET: 0x00B3ABE0
// FUNCTION: fn_18791
bool EffectLists::HasAllocator(int id) const {
  return this->allocators.find(id) != this->allocators.end();
}

// OFFSET: 0x00B3B040
// FUNCTION: fn_18794
void EffectLists::Render() {
  if (!g_session.IsEffectRenderEnabled()) {
    return;
  }

  for (auto& entry : this->active) {
    for (CEffect* effect : entry.second) {
      effect->UpdateVisibility();
    }
  }
}

// OFFSET: 0x00B3BE50
// FUNCTION: fn_18803
float DirectionAngle(float x, float z) {
  const float length = SquareRoot(x * x + z * z);
  const float angle = static_cast<float>(std::acos(static_cast<double>(1.0f / length * x))) / 3.141592f * 180.0f;
  if (z < 0.0f) {
    return 360.0f - angle;
  }

  return angle;
}

// OFFSET: 0x00B3BED0
// FUNCTION: fn_18804
double ScreenAngle(vector3d from, vector3d to) {
  float fromReciprocalDepth = 0.0f;
  float toReciprocalDepth = 0.0f;
  float fromX;
  float fromY;
  float toX;
  float toY;
  g_renderer->ProjectViewport(&from.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &fromX, &fromY, &fromReciprocalDepth);
  g_renderer->ProjectViewport(&to.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &toX, &toY, &toReciprocalDepth);
  const float dx = toX - fromX;
  const float dy = toY - fromY;
  double angle = static_cast<float>(std::acos(static_cast<double>(dx / SquareRoot(dy * dy + dx * dx)))) / 3.141592f * 180.0f;
  if (fromY > toY) {
    angle = 360.0 - angle;
  }

  return angle;
}

// OFFSET: 0x00B3BFD0
// FUNCTION: fn_18805
std::string CurrentMapScriptName() {
  std::string name = reinterpret_cast<const char*>(g_modeMgr.GetGameMode()->SendMsg(9, 0, 0, 0, 0));
  const std::size_t at = name.rfind('@');
  if (at != std::string::npos && at > 3) {
    name.erase(at - 4, 3);
  }

  return name.substr(0, name.find("."));
}

// OFFSET: 0x00B3C1C0
// FUNCTION: fn_18806
std::string EffectFunctionName(std::string name) {
  std::string result = "_" + name + "_Effect";
  const std::size_t at = result.find("@");
  if (at != std::string::npos) {
    result.erase(at, 1);
  }

  return result;
}

// OFFSET: 0x00B257C0
// FUNCTION: fn_18784
CEffect* EffectLists::Find(int actor, int instance, int id) const {
  const auto found = this->active.find(actor);

  if (found != this->active.end()) {
    for (CEffect* effect : found->second) {
      if (effect->id == id && effect->unknown_00C4 == instance) {
        return effect;
      }
    }
  }

  return nullptr;
}

// OFFSET: 0x00B253A0
// FUNCTION: fn_18779
void EffectLists::Retire(CEffect* effect, unsigned char removeActive) {
  if (effect) {
    this->retired.push_back(effect);

    if (removeActive == 1 && this->active.find(static_cast<int>(effect->actor)) != this->active.end()) {
      this->active[static_cast<int>(effect->actor)].remove(effect);
    }
  }
}

// OFFSET: 0x00B25120
// FUNCTION: fn_18777
void EffectLists::Retire(int actor, int id) {
  if (CEffect* effect = this->Find(actor, id)) {
    this->retired.push_back(effect);

    if (this->active.find(static_cast<int>(effect->actor)) != this->active.end()) {
      this->active[static_cast<int>(effect->actor)].remove(effect);
    }
  }
}

// OFFSET: 0x00B25240
// FUNCTION: fn_18778
void EffectLists::Retire(int actor, int instance, int id) {
  if (actor > 0 && instance > 0) {
    if (CEffect* effect = this->Find(actor, instance, id)) {
      this->retired.push_back(effect);

      if (this->active.find(static_cast<int>(effect->actor)) != this->active.end()) {
        this->active[static_cast<int>(effect->actor)].remove(effect);
      }
    }
  }
}

// OFFSET: 0x00B24320
// FUNCTION: fn_18772
void EffectLists::RetireActor(int actor) {
  const auto found = this->active.find(actor);

  if (found == this->active.end()) {
    return;
  }

  for (auto effect = found->second.begin(); effect != found->second.end(); ++effect) {
    this->retired.push_back(*effect);
  }

  found->second.clear();
}

// OFFSET: 0x00B25830
// FUNCTION: fn_18785
CEffect* EffectLists::At(int id, unsigned index) const {
  const auto found = this->indexed.find(id);

  if (found == this->indexed.end() || index >= found->second.size()) {
    return nullptr;
  }

  return found->second[index];
}

// OFFSET: 0x00B24190
// FUNCTION: fn_18771
void EffectLists::Add(int actor, CEffect* effect) {
  this->active[actor].push_back(effect);

  switch (effect->id) {
  case EF_EMITTER:
  case EF_MAGIC_FLOOR:
  case EF_EVILS_PAW:
  case EF_ANIMATED_EMITTER:
  case EF_NEW_RANGEDATTACK:
  case EF_UNK_2343:
    this->indexed[effect->id].push_back(effect);
    break;
  }
}

// OFFSET: 0x00B24A50
// FUNCTION: fn_18774
CEffect* EffectLists::Spawn(int actor, int id, float x, float y, float z, std::uint32_t sourceActor, int frame, unsigned char reuse, int instance) {
  CEffect* effect = nullptr;

  if (id < 0 || id > 60000) {
    switch (id) {
    case EF_READYPORTAL:
    case EF_PORTAL:
    case EF_LOCKON:
    case EF_BOTTOM:
    case EF_BOTTOM2:
    case EF_FIREPILLARON:
    case EF_REFINEOK:
    case EF_REFINEFAIL:
    case EF_TOPRANK:
    case EF_PARTY:
    case EF_STATUS_STATE:
    case EF_TALK_FROSTJOKE:
    case EF_TALK_SCREAM:
    case EF_BLIND:
    case EF_ANGEL2:
    case EF_MINI_TETRIS:
    case EF_ANGEL:
    case EF_REFLECTBODY:
    case EF_ASURABODY:
    case EF_QUAKEBODY:
    case EF_JUMPBODY:
    case EF_LANDBODY:
    case EF_TAE_READY:
    case EF_DEVIL1:
    case EF_DEVIL2:
    case EF_DEVIL3:
    case EF_DEVIL4:
    case EF_DEVIL5:
    case EF_DEVIL6:
    case EF_DEVIL7:
    case EF_DEVIL8:
    case EF_DEVIL9:
    case EF_DEVIL10:
    case EF_BLUEBODY:
    case EF_REDLIGHTBODY:
    case EF_RED_HIT:
    case EF_BLUE_HIT:
    case EF_ANGEL3:
    case EF_UNDEADBODY:
    case EF_UNDEADBODY_DEL:
    case EF_FIREPILLARON2:
    case EF_JUMPBODY1:
    case EF_UNK_1398:
      break;
    default:
      return nullptr;
    }
  } else if (id == EF_LEVEL150 || id == EF_LEVEL150_SUB) {
    if (this->Find(actor, EF_LEVEL99)) {
      return nullptr;
    }
  }

  if (actor) {
    effect = this->Find(actor, instance, id);
    if (effect) {
      if (instance > 0) {
        return nullptr;
      }
      if (effect->retained || reuse) {
        return effect;
      }
    }
  }

  const auto allocator = this->allocators.find(id);
  if (allocator == this->allocators.end()) {
    return nullptr;
  }

  effect = allocator->second->Create();
  if (effect) {
    effect->SetActor(actor);
    effect->SetSourceActor(sourceActor);
    effect->position[0] = x;
    effect->position[1] = y;
    effect->position[2] = z;
    effect->Slot29(id);
    if (frame != -1) {
      effect->unknown_00B0 = frame;
    }
    if (instance > 0) {
      effect->unknown_00C4 = instance;
      effect->retained = 1;
    }
    effect->Slot01();
    effect->Slot12();
    this->Add(actor, effect);
  }

  return effect;
}

// OFFSET: 0x00B254C0
// FUNCTION: fn_18780
void EffectLists::Clear() {
  for (auto entry = this->active.begin(); entry != this->active.end(); ++entry) {
    this->RetireActor(entry->first);
  }

  this->active.clear();
  this->RetireActor(0);
  this->indexed.clear();

  for (auto effect = this->retired.begin(); effect != this->retired.end(); ++effect) {
    (*effect)->Close();
    delete *effect;
  }

  this->retired.clear();
}

// OFFSET: 0x00B255B0
// FUNCTION: fn_18781
void EffectLists::Release() {
  for (auto entry = this->allocators.begin(); entry != this->allocators.end(); ++entry) {
    delete entry->second;
  }

  this->allocators.clear();
  this->Clear();
}

// OFFSET: 0x00B1D520
// FUNCTION: fn_18557
void __fastcall sub_B1D520(void* self) {
  *reinterpret_cast<std::uintptr_t*>(self) = 0x01182CA8;
}

// OFFSET: 0x00B1D530
// FUNCTION: fn_45453
EffectLists::~EffectLists() {
  this->Release();
}

// OFFSET: 0x00B25730
// FUNCTION: fn_18782
void DestroyEffectMgr() {
  if (g_effectMgr) {
    delete g_effectMgr;
    g_effectMgr = nullptr;
  }
}

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

  /* 0000 */ std::uint8_t m_text[Length];
  /* 0044 */ bool m_encrypted;
};

class CBezierArcThrowEffect : public CEffect {
public:
  // INLINED
  explicit CBezierArcThrowEffect(const std::string& name)
      : CEffect(-1), name(name), unknown_00F0(0), unknown_00F4(0), unknown_00F8(0) {}

  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ std::string name;
  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ int unknown_00F4;
  /* 00F8 */ int unknown_00F8;
};

class CThunderingOrb : public CBezierArcThrowEffect {
public:
  // INLINED
  explicit CThunderingOrb(const std::string& name) : CBezierArcThrowEffect(name) {}
};

class CWindBomb : public CBezierArcThrowEffect {
public:
  // INLINED
  explicit CWindBomb(const std::string& name) : CBezierArcThrowEffect(name) {}
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CBezierArcThrowEffect) == 0xFC);
static_assert(sizeof(CThunderingOrb) == 0xFC);
static_assert(sizeof(CWindBomb) == 0xFC);
#endif

template <class T>
struct CEffectAllocator : CAllocator {
  CEffect* Create() override;
};

}

// OFFSET: 0x00B1D9E0
// FUNCTION: fn_18561
__declspec(noinline) EncryptedName<0x44, 0x2F8B0F37118D157FULL>& ThunderingOrbName() {
  const std::uint8_t source[] = {
      0x3B, 0x47, 0xD8, 0x58, 0x73, 0x53, 0xC0, 0x7D, 0x20, 0x41, 0xC5, 0x44, 0x79, 0x4B, 0xCE, 0x7D,
      0x36, 0x5B, 0xCA, 0x4E, 0x78, 0x5D, 0xC9, 0x73, 0x0B, 0x7D, 0xF8, 0x7F, 0x53, 0x6A, 0xF9, 0x46,
      0x11, 0x72, 0xD2, 0x7E, 0x45, 0x6D, 0xD4, 0x40, 0x0D, 0x77, 0xD1, 0x65, 0x5F, 0x7A, 0xE5, 0x4B,
      0x1A, 0x67, 0xE4, 0x7F, 0x50, 0x50, 0xE4, 0x5D, 0x1D, 0x4A, 0xFE, 0x7C, 0x56, 0x63, 0xE7, 0x01,
      0x0C, 0x61, 0xFF, 0x11};
  thread_local EncryptedName<sizeof(source), 0x2F8B0F37118D157FULL> name(source);
  return name;
}

// OFFSET: 0x00B24FA0
// FUNCTION: fn_18776
bool CreateEffectMgr() {
  if (g_effectMgr) {
    return false;
  }

  EffectLists* mgr = new (std::nothrow) EffectLists();
  if (mgr) {
    mgr->initialized = false;
    if (mgr->RegisterAllocators()) {
      mgr->initialized = true;
    }
  }

  g_effectMgr = mgr;
  if (!g_effectMgr) {
    return false;
  }

  if (!g_effectMgr->initialized) {
    delete g_effectMgr;
    g_effectMgr = nullptr;
    return false;
  }

  return true;
}

__declspec(noinline) bool EffectLists::RegisterAllocators() {
  return true;
}

// OFFSET: 0x00B24DE0
// FUNCTION: fn_18775
void EffectLists::SpawnLevel99(CRenderObject* actor, unsigned char flag, int extra) {
  if (!actor || extra != 0) {
    return;
  }

  actor->ClearLevelEffects();

  if (!flag) {
    return;
  }

  const int aid = static_cast<int>(actor->render.accountId);
  const float x = actor->render.position.x;
  const float y = actor->render.position.y;
  const float z = actor->render.position.z;
  CEffect* effect = nullptr;

  if (aid) {
    effect = this->Find(aid, 0, EF_LEVEL99);
    if (effect && effect->retained) {
      goto applyLevel99;
    }
  }

  {
    const auto allocator = this->allocators.find(EF_LEVEL99);
    if (allocator == this->allocators.end()) {
      return;
    }

    effect = allocator->second->Create();
    if (!effect) {
      return;
    }

    effect->SetActor(static_cast<std::uint32_t>(aid));
    effect->SetSourceActor(0);
    effect->position[0] = x;
    effect->position[1] = y;
    effect->position[2] = z;
    effect->Slot29(EF_LEVEL99);
    effect->Slot01();
    effect->Slot12();
    this->Add(aid, effect);
  }

applyLevel99:
  {
    const char path[] = {'e', 'f', 'f', 'e', 'c', 't', '\\', '\xb9', '\xf8', '\xb0', '\xb3', '3', '.', 'b', 'm', 'p'};
    effect->Slot11(std::string(path, sizeof(path)));
    effect->Slot08(0x50, 0x50, 0xFF);
  }
}

namespace {

// OFFSET: 0x00B20B20
// FUNCTION: fn_18655
template <>
CEffect* CEffectAllocator<CThunderingOrb>::Create() {
  void* memory = ::operator new(0xFC, std::nothrow);
  if (!memory) {
    return nullptr;
  }

  EncryptedName<0x44, 0x2F8B0F37118D157FULL>& name = ThunderingOrbName();
  if (name.m_encrypted) {
    utility::XorBytes(name.m_text, 0x44, 0x2F8B0F37118D157FULL);
    name.m_encrypted = false;
  }

  return new (memory) CThunderingOrb(reinterpret_cast<const char*>(name.m_text));
}

// OFFSET: 0x00B20E70
// FUNCTION: fn_18660
template <>
CEffect* CEffectAllocator<CWindBomb>::Create() {
  void* memory = ::operator new(sizeof(CWindBomb), std::nothrow);
  if (!memory) {
    return nullptr;
  }

  const std::uint8_t source[] = {
      0xCB, 0xCB, 0xBE, 0x44, 0xF1, 0x23, 0xBB, 0x6B, 0xD0, 0xCE, 0xA2, 0x43, 0xF1, 0x20, 0xBD, 0x76,
      0xC2, 0xDB, 0xB7, 0x7A, 0xDC, 0x11, 0x9B, 0x66, 0xED, 0xF6, 0x86, 0x6F, 0xEA, 0x0B, 0x90, 0x4B,
      0xE1, 0xF8, 0x8F, 0x62, 0xE9, 0x08, 0x96, 0x57, 0xEB, 0xC6, 0x89, 0x62, 0xD8, 0x1D, 0xA0, 0x4D,
      0xE0, 0xEB, 0x85, 0x6C, 0xD1, 0x10, 0xD1, 0x4A, 0xFB, 0xEB, 0xEB};
  thread_local EncryptedName<sizeof(source), 0x39FF7FB50DEB998FULL> name(source);
  const std::string text = name.Text();
  return new (memory) CWindBomb(text);
}

// OFFSET: 0x00B1F010
// FUNCTION: fn_18604
template <>
CEffect* CEffectAllocator<CGravityControlEffect>::Create() {
  return new (std::nothrow) CGravityControlEffect();
}

// OFFSET: 0x00B1F740
// FUNCTION: fn_18618
template <>
CEffect* CEffectAllocator<CMeteorEffect>::Create() {
  return new (std::nothrow) CMeteorEffect();
}

// OFFSET: 0x00B20110
// FUNCTION: fn_18637
template <>
CEffect* CEffectAllocator<CShadowExceedNew>::Create() {
  return new (std::nothrow) CShadowExceedNew();
}

// OFFSET: 0x00B20400
// FUNCTION: fn_18642
template <>
CEffect* CEffectAllocator<CSprArrowEffect>::Create() {
  return new (std::nothrow) CSprArrowEffect();
}

// OFFSET: 0x00B1E1D0
// FUNCTION: fn_18576
template <>
CEffect* CEffectAllocator<CChopChopClawMarks>::Create() {
  return new (std::nothrow) CChopChopClawMarks();
}

// OFFSET: 0x00B1DAD0

// OFFSET: 0x00B1DB50

// OFFSET: 0x00B1DC50

// OFFSET: 0x00B1DCD0

// OFFSET: 0x00B1DD50

// OFFSET: 0x00B1DDD0

// OFFSET: 0x00B1DE50

// OFFSET: 0x00B1DED0

// OFFSET: 0x00B1DF50

// OFFSET: 0x00B1DFD0

// OFFSET: 0x00B1E050

// OFFSET: 0x00B1E0D0

// OFFSET: 0x00B1E150

// OFFSET: 0x00B1E290

// OFFSET: 0x00B1E310

// OFFSET: 0x00B1E390

// OFFSET: 0x00B1E410

// OFFSET: 0x00B1E490

// OFFSET: 0x00B1E510

// OFFSET: 0x00B1E590

// OFFSET: 0x00B1E610

// OFFSET: 0x00B1E690

// OFFSET: 0x00B1E710

// OFFSET: 0x00B1E790

// OFFSET: 0x00B1E810

// OFFSET: 0x00B1E890

// OFFSET: 0x00B1E910

// OFFSET: 0x00B1E990

// OFFSET: 0x00B1EA10

// OFFSET: 0x00B1EA90

// OFFSET: 0x00B1EB10

// OFFSET: 0x00B1EB90

// OFFSET: 0x00B1EC10

// OFFSET: 0x00B1EC90

// OFFSET: 0x00B1ED10

// OFFSET: 0x00B1ED90

// OFFSET: 0x00B1EE10

// OFFSET: 0x00B1EE90
// FUNCTION: fn_18601
template <>
CEffect* CEffectAllocator<CFullScreenEffect>::Create() {
  return new (std::nothrow) CFullScreenEffect();
}

// OFFSET: 0x00B1EF10
// FUNCTION: fn_18602
template <>
CEffect* CEffectAllocator<CFuumaShouakuEffect>::Create() {
  return new (std::nothrow) CFuumaShouakuEffect();
}

// OFFSET: 0x00B1EF90
// FUNCTION: fn_18603
template <>
CEffect* CEffectAllocator<CGaleStorm>::Create() {
  return new (std::nothrow) CGaleStorm();
}

// OFFSET: 0x00B1F0C0
// FUNCTION: fn_18605
template <>
CEffect* CEffectAllocator<CHawkVumerangEffect>::Create() {
  return new (std::nothrow) CHawkVumerangEffect();
}

// OFFSET: 0x00B1F140
// FUNCTION: fn_18606
template <>
CEffect* CEffectAllocator<CHealEffect>::Create() {
  return new (std::nothrow) CHealEffect();
}

// OFFSET: 0x00B1F1C0
// FUNCTION: fn_18607
template <>
CEffect* CEffectAllocator<CDropItemEffect>::Create() {
  return new (std::nothrow) CDropItemEffect();
}

// OFFSET: 0x00B1F240
// FUNCTION: fn_18608
template <>
CEffect* CEffectAllocator<CJupitelThunderHit>::Create() {
  return new (std::nothrow) CJupitelThunderHit();
}

// OFFSET: 0x00B1F2C0
// FUNCTION: fn_18609
template <>
CEffect* CEffectAllocator<CJupitelThunderStormEffect>::Create() {
  return new (std::nothrow) CJupitelThunderStormEffect();
}

// OFFSET: 0x00B1F340
// FUNCTION: fn_18610
template <>
CEffect* CEffectAllocator<CLevel150Effect>::Create() {
  return new (std::nothrow) CLevel150Effect();
}

// OFFSET: 0x00B1F3C0
// FUNCTION: fn_18611
template <>
CEffect* CEffectAllocator<CLevel150SubEffect>::Create() {
  return new (std::nothrow) CLevel150SubEffect();
}

// OFFSET: 0x00B1F440
// FUNCTION: fn_18612
template <>
CEffect* CEffectAllocator<CLevel99Effect>::Create() {
  return new (std::nothrow) CLevel99Effect();
}

// OFFSET: 0x00B1F4C0
// FUNCTION: fn_18613
template <>
CEffect* CEffectAllocator<CLevel99Orb1Effect>::Create() {
  return new (std::nothrow) CLevel99Orb1Effect();
}

// OFFSET: 0x00B1F540
// FUNCTION: fn_18614
template <>
CEffect* CEffectAllocator<CLevel99Orb2Effect>::Create() {
  return new (std::nothrow) CLevel99Orb2Effect();
}

// OFFSET: 0x00B1F5C0
// FUNCTION: fn_18615
template <>
CEffect* CEffectAllocator<CLightSphereEffect>::Create() {
  return new (std::nothrow) CLightSphereEffect();
}

// OFFSET: 0x00B1F640
// FUNCTION: fn_18616
template <>
CEffect* CEffectAllocator<CMagicFloorEffect>::Create() {
  return new (std::nothrow) CMagicFloorEffect();
}

// OFFSET: 0x00B1F6C0
// FUNCTION: fn_18617
template <>
CEffect* CEffectAllocator<CMapChainEffect>::Create() {
  return new (std::nothrow) CMapChainEffect();
}

// OFFSET: 0x00B1F810
// FUNCTION: fn_18619
template <>
CEffect* CEffectAllocator<CNewCloudKillEffect>::Create() {
  return new (std::nothrow) CNewCloudKillEffect();
}

// OFFSET: 0x00B1F890
// FUNCTION: fn_18620
template <>
CEffect* CEffectAllocator<CNyanggrassEffect>::Create() {
  return new (std::nothrow) CNyanggrassEffect();
}

// OFFSET: 0x00B1F910
// FUNCTION: fn_18621
template <>
CEffect* CEffectAllocator<CPetitioEffect>::Create() {
  return new (std::nothrow) CPetitioEffect();
}

// OFFSET: 0x00B1F990
// FUNCTION: fn_18622
template <>
CEffect* CEffectAllocator<CPneumaticusProcellaEffect>::Create() {
  return new (std::nothrow) CPneumaticusProcellaEffect();
}

// OFFSET: 0x00B1FA10
// FUNCTION: fn_18623
template <>
CEffect* CEffectAllocator<CPowderingEffect>::Create() {
  return new (std::nothrow) CPowderingEffect();
}

// OFFSET: 0x00B1FA90
// FUNCTION: fn_18624
template <>
CEffect* CEffectAllocator<CRandomHitEffect>::Create() {
  return new (std::nothrow) CRandomHitEffect();
}

// OFFSET: 0x00B1FB10
// FUNCTION: fn_18625
template <>
CEffect* CEffectAllocator<CRandomOptEffect>::Create() {
  return new (std::nothrow) CRandomOptEffect();
}

// OFFSET: 0x00B1FB90
// FUNCTION: fn_18626
template <>
CEffect* CEffectAllocator<CRectUpEffect>::Create() {
  return new (std::nothrow) CRectUpEffect();
}

// OFFSET: 0x00B1FC10
// FUNCTION: fn_18627
template <>
CEffect* CEffectAllocator<CRichsCoinEffect>::Create() {
  return new (std::nothrow) CRichsCoinEffect();
}

// OFFSET: 0x00B1FC90
// FUNCTION: fn_18628
template <>
CEffect* CEffectAllocator<CRootTwistEffect>::Create() {
  return new (std::nothrow) CRootTwistEffect();
}

// OFFSET: 0x00B1FD10
// FUNCTION: fn_18629
template <>
CEffect* CEffectAllocator<CRotateItemEffect>::Create() {
  return new (std::nothrow) CRotateItemEffect();
}

// OFFSET: 0x00B1FD90
// FUNCTION: fn_18630
template <>
CEffect* CEffectAllocator<CRotateLineEffect>::Create() {
  return new (std::nothrow) CRotateLineEffect();
}

// OFFSET: 0x00B1FE10
// FUNCTION: fn_18631
template <>
CEffect* CEffectAllocator<CSavageImpactEffect>::Create() {
  return new (std::nothrow) CSavageImpactEffect();
}

// OFFSET: 0x00B1FE90
// FUNCTION: fn_18632
template <>
CEffect* CEffectAllocator<CScatterEffect>::Create() {
  return new (std::nothrow) CScatterEffect();
}

// OFFSET: 0x00B1FF10
// FUNCTION: fn_18633
template <>
CEffect* CEffectAllocator<CServantWeaponEffect>::Create() {
  return new (std::nothrow) CServantWeaponEffect();
}

// OFFSET: 0x00B1FF90
// FUNCTION: fn_18634
template <>
CEffect* CEffectAllocator<CServantWeaponNewEffect>::Create() {
  return new (std::nothrow) CServantWeaponNewEffect();
}

// OFFSET: 0x00B20010
// FUNCTION: fn_18635
template <>
CEffect* CEffectAllocator<CSevereRainStorm>::Create() {
  return new (std::nothrow) CSevereRainStorm();
}

// OFFSET: 0x00B20090
// FUNCTION: fn_18636
template <>
CEffect* CEffectAllocator<CShadowExceed>::Create() {
  return new (std::nothrow) CShadowExceed();
}

// OFFSET: 0x00B20200
// FUNCTION: fn_18638
template <>
CEffect* CEffectAllocator<CShowEffect>::Create() {
  return new (std::nothrow) CShowEffect();
}

// OFFSET: 0x00B20280
// FUNCTION: fn_18639
template <>
CEffect* CEffectAllocator<CSmokeEffect>::Create() {
  return new (std::nothrow) CSmokeEffect();
}

// OFFSET: 0x00B20300
// FUNCTION: fn_18640
template <>
CEffect* CEffectAllocator<CSoulCollectEffect>::Create() {
  return new (std::nothrow) CSoulCollectEffect();
}

// OFFSET: 0x00B20380
// FUNCTION: fn_18641
template <>
CEffect* CEffectAllocator<CSoulUnityEffect>::Create() {
  return new (std::nothrow) CSoulUnityEffect();
}

// OFFSET: 0x00B20520
// FUNCTION: fn_18643
template <>
CEffect* CEffectAllocator<CSpriteEffect>::Create() {
  return new (std::nothrow) CSpriteEffect();
}

// OFFSET: 0x00B205A0
// FUNCTION: fn_18644
template <>
CEffect* CEffectAllocator<CSpriteMableEffect>::Create() {
  return new (std::nothrow) CSpriteMableEffect();
}

// OFFSET: 0x00B20620
// FUNCTION: fn_18645
template <>
CEffect* CEffectAllocator<CSquareRangeEffect>::Create() {
  return new (std::nothrow) CSquareRangeEffect();
}

// OFFSET: 0x00B206A0
// FUNCTION: fn_18646
template <>
CEffect* CEffectAllocator<CStarEmperorEffect>::Create() {
  return new (std::nothrow) CStarEmperorEffect();
}

// OFFSET: 0x00B20720
// FUNCTION: fn_18647
template <>
CEffect* CEffectAllocator<CStemSpearEffect>::Create() {
  return new (std::nothrow) CStemSpearEffect();
}

// OFFSET: 0x00B207A0
// FUNCTION: fn_18648
template <>
CEffect* CEffectAllocator<CStormCannonEffect>::Create() {
  return new (std::nothrow) CStormCannonEffect();
}

// OFFSET: 0x00B20820
// FUNCTION: fn_18649
template <>
CEffect* CEffectAllocator<CStormKickEffect>::Create() {
  return new (std::nothrow) CStormKickEffect();
}

// OFFSET: 0x00B208A0
// FUNCTION: fn_18650
template <>
CEffect* CEffectAllocator<CSunStanceEffect>::Create() {
  return new (std::nothrow) CSunStanceEffect();
}

// OFFSET: 0x00B20920
// FUNCTION: fn_18651
template <>
CEffect* CEffectAllocator<CSvgSpiritEffect>::Create() {
  return new (std::nothrow) CSvgSpiritEffect();
}

// OFFSET: 0x00B209A0
// FUNCTION: fn_18652
template <>
CEffect* CEffectAllocator<CTalismanMarkEffect>::Create() {
  return new (std::nothrow) CTalismanMarkEffect();
}

// OFFSET: 0x00B20A20
// FUNCTION: fn_18653
template <>
CEffect* CEffectAllocator<CTetraBallEffect>::Create() {
  return new (std::nothrow) CTetraBallEffect();
}

// OFFSET: 0x00B20AA0
// FUNCTION: fn_18654
template <>
CEffect* CEffectAllocator<CThrowItemEffect>::Create() {
  return new (std::nothrow) CThrowItemEffect();
}

// OFFSET: 0x00B20C70
// FUNCTION: fn_18656
template <>
CEffect* CEffectAllocator<CTriangleShotArrow>::Create() {
  return new (std::nothrow) CTriangleShotArrow();
}

// OFFSET: 0x00B20CF0
// FUNCTION: fn_18657
template <>
CEffect* CEffectAllocator<CTunaPartyEffect>::Create() {
  return new (std::nothrow) CTunaPartyEffect();
}

// OFFSET: 0x00B20D70
// FUNCTION: fn_18658
template <>
CEffect* CEffectAllocator<CViolentQuakeEffect>::Create() {
  return new (std::nothrow) CViolentQuakeEffect();
}

// OFFSET: 0x00B20DF0
// FUNCTION: fn_18659
template <>
CEffect* CEffectAllocator<CWarningPlaneEffect>::Create() {
  return new (std::nothrow) CWarningPlaneEffect();
}

// OFFSET: 0x00B21070
// FUNCTION: fn_18661
template <>
CEffect* CEffectAllocator<CWindEffect>::Create() {
  return new (std::nothrow) CWindEffect();
}

// OFFSET: 0x00B210F0
// FUNCTION: fn_18662
template <>
CEffect* CEffectAllocator<EffectResourceSwitcher>::Create() {
  return new (std::nothrow) EffectResourceSwitcher();
}

template struct CEffectAllocator<CAMachineEffect>;
template struct CEffectAllocator<CAcidifiedThrow>;
template struct CEffectAllocator<CAstralStrikeEffect>;
template struct CEffectAllocator<CAxeBoomerang>;
template struct CEffectAllocator<CBindTrapEffect>;
template struct CEffectAllocator<CBlastForge>;
template struct CEffectAllocator<CCaneOfEvilEyeEffect>;
template struct CEffectAllocator<CCartCannon>;
template struct CEffectAllocator<CCellRangeEffect>;
template struct CEffectAllocator<CChangeSizeEffect>;
template struct CEffectAllocator<CChargingPierceEffect>;
template struct CEffectAllocator<CChargingPierceNewEffect>;
template struct CEffectAllocator<CCherryBlossomEffect>;
template struct CEffectAllocator<CClimaxEffect>;
template struct CEffectAllocator<CCloud>;
template struct CEffectAllocator<CCloudKillEffect>;
template struct CEffectAllocator<CCommonFloorEffect>;
template struct CEffectAllocator<CCompetentia>;
template struct CEffectAllocator<CCresciveBolt>;
template struct CEffectAllocator<CCrimsonArrowEffect>;
template struct CEffectAllocator<CCurseOfCubeEffect>;
template struct CEffectAllocator<CCylinderEffect>;
template struct CEffectAllocator<CDancingKnifeEffect>;
template struct CEffectAllocator<CEZ2STRScreenEffect>;
template struct CEffectAllocator<CEZ2STREffectEx>;
template struct CEffectAllocator<CEmitterEffect>;
template struct CEffectAllocator<CEnergyOrbEffect>;
template struct CEffectAllocator<CEternalSlashAttackEffect>;
template struct CEffectAllocator<CEternalSlashCountEffect>;
template struct CEffectAllocator<CEvilLandEffect>;
template struct CEffectAllocator<CEvilsPawEffect>;
template struct CEffectAllocator<CFallenAngelEffect>;
template struct CEffectAllocator<CFloralFlareRoad>;
template struct CEffectAllocator<CFootprintEffect>;
template struct CEffectAllocator<CFootprintStrEffect>;
template struct CEffectAllocator<CFriggSongEffect>;
template struct CEffectAllocator<CFromTheAbyssEffect>;
template struct CEffectAllocator<CFullScreenEffect>;
template struct CEffectAllocator<CFuumaShouakuEffect>;
template struct CEffectAllocator<CGaleStorm>;
template struct CEffectAllocator<CHawkVumerangEffect>;
template struct CEffectAllocator<CHealEffect>;
template struct CEffectAllocator<CDropItemEffect>;
template struct CEffectAllocator<CJupitelThunderHit>;
template struct CEffectAllocator<CJupitelThunderStormEffect>;
template struct CEffectAllocator<CLevel150Effect>;
template struct CEffectAllocator<CLevel150SubEffect>;
template struct CEffectAllocator<CLevel99Effect>;
template struct CEffectAllocator<CLevel99Orb1Effect>;
template struct CEffectAllocator<CLevel99Orb2Effect>;
template struct CEffectAllocator<CLightSphereEffect>;
template struct CEffectAllocator<CMagicFloorEffect>;
template struct CEffectAllocator<CMapChainEffect>;
template struct CEffectAllocator<CNewCloudKillEffect>;
template struct CEffectAllocator<CNyanggrassEffect>;
template struct CEffectAllocator<CPetitioEffect>;
template struct CEffectAllocator<CPneumaticusProcellaEffect>;
template struct CEffectAllocator<CPowderingEffect>;
template struct CEffectAllocator<CRandomHitEffect>;
template struct CEffectAllocator<CRandomOptEffect>;
template struct CEffectAllocator<CRectUpEffect>;
template struct CEffectAllocator<CRichsCoinEffect>;
template struct CEffectAllocator<CRootTwistEffect>;
template struct CEffectAllocator<CRotateItemEffect>;
template struct CEffectAllocator<CRotateLineEffect>;
template struct CEffectAllocator<CSavageImpactEffect>;
template struct CEffectAllocator<CScatterEffect>;
template struct CEffectAllocator<CServantWeaponEffect>;
template struct CEffectAllocator<CServantWeaponNewEffect>;
template struct CEffectAllocator<CSevereRainStorm>;
template struct CEffectAllocator<CShadowExceed>;
template struct CEffectAllocator<CShowEffect>;
template struct CEffectAllocator<CSmokeEffect>;
template struct CEffectAllocator<CSoulCollectEffect>;
template struct CEffectAllocator<CSoulUnityEffect>;
template struct CEffectAllocator<CSpriteEffect>;
template struct CEffectAllocator<CSpriteMableEffect>;
template struct CEffectAllocator<CSquareRangeEffect>;
template struct CEffectAllocator<CStarEmperorEffect>;
template struct CEffectAllocator<CStemSpearEffect>;
template struct CEffectAllocator<CStormCannonEffect>;
template struct CEffectAllocator<CStormKickEffect>;
template struct CEffectAllocator<CSunStanceEffect>;
template struct CEffectAllocator<CSvgSpiritEffect>;
template struct CEffectAllocator<CTalismanMarkEffect>;
template struct CEffectAllocator<CTetraBallEffect>;
template struct CEffectAllocator<CThrowItemEffect>;
template struct CEffectAllocator<CTriangleShotArrow>;
template struct CEffectAllocator<CTunaPartyEffect>;
template struct CEffectAllocator<CViolentQuakeEffect>;
template struct CEffectAllocator<CWarningPlaneEffect>;
template struct CEffectAllocator<CWindEffect>;
template struct CEffectAllocator<EffectResourceSwitcher>;
template struct CEffectAllocator<CChopChopClawMarks>;
template struct CEffectAllocator<CGravityControlEffect>;
template struct CEffectAllocator<CMeteorEffect>;
template struct CEffectAllocator<CShadowExceedNew>;
template struct CEffectAllocator<CSprArrowEffect>;
template struct CEffectAllocator<CThunderingOrb>;
template struct CEffectAllocator<CWindBomb>;

}

CGravityControlEffect::~CGravityControlEffect() {
}

CMeteorEffect::~CMeteorEffect() {
}

// OFFSET: 0x00B1D6E0

CSprArrowEffect::~CSprArrowEffect() {
}

CChopChopClawMarks::~CChopChopClawMarks() {
}

// OFFSET: 0x00B3A430
// FUNCTION: fn_18787
bool EffectLists::RegisterAllocator(int id, CAllocator* allocator) {
  return this->allocators.try_emplace(id, allocator).second;
}

// OFFSET: 0x00B3AF30
// FUNCTION: fn_18793
void EffectLists::Update() {
  for (auto& entry : this->active) {
    std::list<CEffect*>& effects = entry.second;
    effects.remove_if([](CEffect* effect) { return effect->RetireIfInactive(); });
    for (CEffect* effect : effects) {
      effect->Update(entry.first);
    }
  }
}
