#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/CharacterParticleBundle.h"
#include "Contents/Effects/GravityControlEffect.h"
#include "Contents/Effects/SkillEffects.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/AMachineEffect.h"
#include "Contents/Effects/AcidifiedThrow.h"
#include "Contents/Effects/AnimatedEmitterEffect.h"
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
#include "Contents/Effects/Particle.h"
#include "Script/LuaScript.h"

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

// OFFSET: 0x00B1DBD0
// FUNCTION: fn_18564
template <>
CEffect* CEffectAllocator<CAnimatedEmitterEffect>::Create() {
  return new (std::nothrow) CAnimatedEmitterEffect();
}

// OFFSET: 0x00B1DAD0
// FUNCTION: fn_18562
template <>
CEffect* CEffectAllocator<CAMachineEffect>::Create() {
  return new (std::nothrow) CAMachineEffect();
}

// OFFSET: 0x00B1DB50
// FUNCTION: fn_18563
template <>
CEffect* CEffectAllocator<CAcidifiedThrow>::Create() {
  return new (std::nothrow) CAcidifiedThrow();
}

// OFFSET: 0x00B1DC50
// FUNCTION: fn_18565
template <>
CEffect* CEffectAllocator<CAstralStrikeEffect>::Create() {
  return new (std::nothrow) CAstralStrikeEffect();
}

// OFFSET: 0x00B1DCD0
// FUNCTION: fn_18566
template <>
CEffect* CEffectAllocator<CAxeBoomerang>::Create() {
  return new (std::nothrow) CAxeBoomerang();
}

// OFFSET: 0x00B1DD50
// FUNCTION: fn_18567
template <>
CEffect* CEffectAllocator<CBindTrapEffect>::Create() {
  return new (std::nothrow) CBindTrapEffect();
}

// OFFSET: 0x00B1DDD0
// FUNCTION: fn_18568
template <>
CEffect* CEffectAllocator<CBlastForge>::Create() {
  return new (std::nothrow) CBlastForge();
}

// OFFSET: 0x00B1DE50
// FUNCTION: fn_18569
template <>
CEffect* CEffectAllocator<CCaneOfEvilEyeEffect>::Create() {
  return new (std::nothrow) CCaneOfEvilEyeEffect();
}

// OFFSET: 0x00B1DED0
// FUNCTION: fn_18570
template <>
CEffect* CEffectAllocator<CCartCannon>::Create() {
  return new (std::nothrow) CCartCannon();
}

// OFFSET: 0x00B1DF50
// FUNCTION: fn_18571
template <>
CEffect* CEffectAllocator<CCellRangeEffect>::Create() {
  return new (std::nothrow) CCellRangeEffect();
}

// OFFSET: 0x00B1DFD0
// FUNCTION: fn_18572
template <>
CEffect* CEffectAllocator<CChangeSizeEffect>::Create() {
  return new (std::nothrow) CChangeSizeEffect();
}

// OFFSET: 0x00B1E050
// FUNCTION: fn_18573
template <>
CEffect* CEffectAllocator<CChargingPierceEffect>::Create() {
  return new (std::nothrow) CChargingPierceEffect();
}

// OFFSET: 0x00B1E0D0
// FUNCTION: fn_18574
template <>
CEffect* CEffectAllocator<CChargingPierceNewEffect>::Create() {
  return new (std::nothrow) CChargingPierceNewEffect();
}

// OFFSET: 0x00B1E150
// FUNCTION: fn_18575
template <>
CEffect* CEffectAllocator<CCherryBlossomEffect>::Create() {
  return new (std::nothrow) CCherryBlossomEffect();
}

// OFFSET: 0x00B1E290
// FUNCTION: fn_18577
template <>
CEffect* CEffectAllocator<CClimaxEffect>::Create() {
  return new (std::nothrow) CClimaxEffect();
}

// OFFSET: 0x00B1E310
// FUNCTION: fn_18578
template <>
CEffect* CEffectAllocator<CCloud>::Create() {
  return new (std::nothrow) CCloud();
}

// OFFSET: 0x00B1E390
// FUNCTION: fn_18579
template <>
CEffect* CEffectAllocator<CCloudKillEffect>::Create() {
  return new (std::nothrow) CCloudKillEffect();
}

// OFFSET: 0x00B1E410
// FUNCTION: fn_18580
template <>
CEffect* CEffectAllocator<CCommonFloorEffect>::Create() {
  return new (std::nothrow) CCommonFloorEffect();
}

// OFFSET: 0x00B1E490
// FUNCTION: fn_18581
template <>
CEffect* CEffectAllocator<CCompetentia>::Create() {
  return new (std::nothrow) CCompetentia();
}

// OFFSET: 0x00B1E510
// FUNCTION: fn_18582
template <>
CEffect* CEffectAllocator<CCresciveBolt>::Create() {
  return new (std::nothrow) CCresciveBolt();
}

// OFFSET: 0x00B1E590
// FUNCTION: fn_18583
template <>
CEffect* CEffectAllocator<CCrimsonArrowEffect>::Create() {
  return new (std::nothrow) CCrimsonArrowEffect();
}

// OFFSET: 0x00B1E610
// FUNCTION: fn_18584
template <>
CEffect* CEffectAllocator<CCurseOfCubeEffect>::Create() {
  return new (std::nothrow) CCurseOfCubeEffect();
}

// OFFSET: 0x00B1E690
// FUNCTION: fn_18585
template <>
CEffect* CEffectAllocator<CCylinderEffect>::Create() {
  return new (std::nothrow) CCylinderEffect();
}

// OFFSET: 0x00B1E710
// FUNCTION: fn_18586
template <>
CEffect* CEffectAllocator<CDancingKnifeEffect>::Create() {
  return new (std::nothrow) CDancingKnifeEffect();
}

// OFFSET: 0x00B1E790
// FUNCTION: fn_18587
template <>
CEffect* CEffectAllocator<CEZ2STRScreenEffect>::Create() {
  return new (std::nothrow) CEZ2STRScreenEffect();
}

// OFFSET: 0x00B1E810
// FUNCTION: fn_18588
template <>
CEffect* CEffectAllocator<CEZ2STREffectEx>::Create() {
  return new (std::nothrow) CEZ2STREffectEx();
}

// OFFSET: 0x00B1E890
// FUNCTION: fn_18589
template <>
CEffect* CEffectAllocator<CEmitterEffect>::Create() {
  return new (std::nothrow) CEmitterEffect();
}

// OFFSET: 0x00B1E910
// FUNCTION: fn_18590
template <>
CEffect* CEffectAllocator<CEnergyOrbEffect>::Create() {
  return new (std::nothrow) CEnergyOrbEffect();
}

// OFFSET: 0x00B1E990
// FUNCTION: fn_18591
template <>
CEffect* CEffectAllocator<CEternalSlashAttackEffect>::Create() {
  return new (std::nothrow) CEternalSlashAttackEffect();
}

// OFFSET: 0x00B1EA10
// FUNCTION: fn_18592
template <>
CEffect* CEffectAllocator<CEternalSlashCountEffect>::Create() {
  return new (std::nothrow) CEternalSlashCountEffect();
}

// OFFSET: 0x00B1EA90
// FUNCTION: fn_18593
template <>
CEffect* CEffectAllocator<CEvilLandEffect>::Create() {
  return new (std::nothrow) CEvilLandEffect();
}

// OFFSET: 0x00B1EB10
// FUNCTION: fn_18594
template <>
CEffect* CEffectAllocator<CEvilsPawEffect>::Create() {
  return new (std::nothrow) CEvilsPawEffect();
}

// OFFSET: 0x00B1EB90
// FUNCTION: fn_18595
template <>
CEffect* CEffectAllocator<CFallenAngelEffect>::Create() {
  return new (std::nothrow) CFallenAngelEffect();
}

// OFFSET: 0x00B1EC10
// FUNCTION: fn_18596
template <>
CEffect* CEffectAllocator<CFloralFlareRoad>::Create() {
  return new (std::nothrow) CFloralFlareRoad();
}

// OFFSET: 0x00B1EC90
// FUNCTION: fn_18597
template <>
CEffect* CEffectAllocator<CFootprintEffect>::Create() {
  return new (std::nothrow) CFootprintEffect();
}

// OFFSET: 0x00B1ED10
// FUNCTION: fn_18598
template <>
CEffect* CEffectAllocator<CFootprintStrEffect>::Create() {
  return new (std::nothrow) CFootprintStrEffect();
}

// OFFSET: 0x00B1ED90
// FUNCTION: fn_18599
template <>
CEffect* CEffectAllocator<CFriggSongEffect>::Create() {
  return new (std::nothrow) CFriggSongEffect();
}

// OFFSET: 0x00B1EE10
// FUNCTION: fn_18600
template <>
CEffect* CEffectAllocator<CFromTheAbyssEffect>::Create() {
  return new (std::nothrow) CFromTheAbyssEffect();
}

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
template struct CEffectAllocator<CAnimatedEmitterEffect>;
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
// FUNCTION: fn_18558
CShadowExceedNew::~CShadowExceedNew() {
}

CSprArrowEffect::~CSprArrowEffect() {
}

CChopChopClawMarks::~CChopChopClawMarks() {
}

// OFFSET: 0x00B25890
// FUNCTION: fn_18786
bool EffectLists::RegisterAllocators() {
  if (!this->RegisterAllocator(EF_LEVEL99, new (std::nothrow) CEffectAllocator<CLevel99Effect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LEVEL99_ORB1, new (std::nothrow) CEffectAllocator<CLevel99Orb1Effect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LEVEL99_ORB2, new (std::nothrow) CEffectAllocator<CLevel99Orb2Effect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SMOKE, new (std::nothrow) CEffectAllocator<CSmokeEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_EMITTER, new (std::nothrow) CEffectAllocator<CEmitterEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LEVEL150, new (std::nothrow) CEffectAllocator<CLevel150Effect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LEVEL150_SUB, new (std::nothrow) CEffectAllocator<CLevel150SubEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MAPCHAIN, new (std::nothrow) CEffectAllocator<CMapChainEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MAGIC_FLOOR, new (std::nothrow) CEffectAllocator<CMagicFloorEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LEVEL160, new (std::nothrow) CEffectAllocator<CLevel150Effect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LEVEL160_SUB, new (std::nothrow) CEffectAllocator<CLevel150SubEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_POTION1, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_POTION2, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_POTION3, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_POTION4, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_POTION5, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_POTION6, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_POTION7, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_POTION8, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROWITEM, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROWITEM2, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROWITEM3, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROWITEM4, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROWITEM4_1, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROWITEM5, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROWITEM6, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROWITEM7, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROWITEM8, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROWITEM9, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROWITEM10, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROWITEM11, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROW_BAKURETSU, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROW_HAPPOKUNAI, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THROW_MULTIPLE_COIN, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ROTATE_HUUMARANKA, new (std::nothrow) CEffectAllocator<CRotateItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ROTATE_LINE_GRAY, new (std::nothrow) CEffectAllocator<CRotateLineEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ROTATE_LINE_BLUE, new (std::nothrow) CEffectAllocator<CRotateLineEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CHOOKGI, new (std::nothrow) CEffectAllocator<CEnergyOrbEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CHOOKGI_WATER, new (std::nothrow) CEffectAllocator<CEnergyOrbEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CHOOKGI_GROUND, new (std::nothrow) CEffectAllocator<CEnergyOrbEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CHOOKGI_FIRE, new (std::nothrow) CEffectAllocator<CEnergyOrbEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CHOOKGI_WIND, new (std::nothrow) CEffectAllocator<CEnergyOrbEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_KAIHOU, new (std::nothrow) CEffectAllocator<CThrowItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_GROUND_EXPLOSION, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_KG_KAGEHUMI, new (std::nothrow) CEffectAllocator<CSquareRangeEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_KO_JYUMONJIKIRI, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_KO_SETSUDAN, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RED_CROSS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_KO_IZAYOI, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_OB_GENSOU, new (std::nothrow) CEffectAllocator<CEnergyOrbEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_KO_HUUMARANKA, new (std::nothrow) CEffectAllocator<CScatterEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_KO_MAKIBISHI, new (std::nothrow) CEffectAllocator<CCellRangeEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_KAIHOU1, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_AKAITSUKI, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ZANGETSU, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_KG_KYOMU, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_GENSOU, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_KO_ZENKAI_WATER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_KO_ZENKAI_WIND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_KO_ZENKAI_FIRE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_KO_ZENKAI_LAND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CHERRYBLOSSOM, new (std::nothrow) CEffectAllocator<CCherryBlossomEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STORMKICK, new (std::nothrow) CEffectAllocator<CStormKickEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HAT_EFFECT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MOONSTAR, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MUSIC_HAT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HAPPINESSSTAR, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STRANGELIGHTS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SUPER_STAR, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_EVILS_PAW, new (std::nothrow) CEffectAllocator<CEvilsPawEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RA_UNLIMIT, new (std::nothrow) CEffectAllocator<CCommonFloorEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WM_FRIGG_SONG, new (std::nothrow) CEffectAllocator<CFriggSongEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HAMMER_OF_GOD, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FIRE_RAIN, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MASS_SPIRAL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BANISHING_BUSTER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SLUGSHOT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_D_TAIL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BIND_TRAP1, new (std::nothrow) CEffectAllocator<CBindTrapEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BIND_TRAP2, new (std::nothrow) CEffectAllocator<CBindTrapEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BIND_TRAP3, new (std::nothrow) CEffectAllocator<CBindTrapEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RL_EXPLOSION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ANIMATED_EMITTER, new (std::nothrow) CEffectAllocator<CAnimatedEmitterEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_C_MAKER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_C_MAKER_1, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_QD_SHOT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_P_ALTER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_S_STORM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CLOUD_KILL, new (std::nothrow) CEffectAllocator<CCloudKillEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FLOWERSMOKE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FSTONE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_QSCARABA, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LJOSALFAR, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MAPLE_FALLS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MARKING_USE_CHANGEMONSTER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_POWER_OF_GAIA, new (std::nothrow) CEffectAllocator<CRectUpEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MAGICAL_FEATHER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MERMAID_LONGING, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACH_COMPLETE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TIME_ACCESSORY, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SPRITEMABLE, new (std::nothrow) CEffectAllocator<CSpriteMableEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TUNAPARTY, new (std::nothrow) CEffectAllocator<CTunaPartyEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FRESHSHRIMP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1099, new (std::nothrow) CEffectAllocator<CMeteorEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1111, new (std::nothrow) CEffectAllocator<CMeteorEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1106, new (std::nothrow) CEffectAllocator<CMeteorEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1112, new (std::nothrow) CEffectAllocator<CMeteorEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1100, new (std::nothrow) CEffectAllocator<CStemSpearEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_SV_STEMSPEAR, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_TUNABELLY, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_SCRATCH, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_BITE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_SCAROFTAROU, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_ARCLOUSEDASH, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_PICKYPECK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1104, new (std::nothrow) CEffectAllocator<CPowderingEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1110, new (std::nothrow) CEffectAllocator<CRootTwistEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_BUNCHOFSHRIMP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LORD_OF_VERMILLION_VISUAL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_METEOR_STORM_VISUAL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_QUAGMIRE_VISUAL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }

  std::list<int> hatEffects = g_luaScript->TableKeys("hatEffectTable", 14);
  for (int hatEffect : hatEffects) {
    if (!this->RegisterAllocator(hatEffect + 2485, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
      return false;
    }
  }

  if (!this->RegisterAllocator(EF_HEAL, new (std::nothrow) CEffectAllocator<CHealEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HEAL2, new (std::nothrow) CEffectAllocator<CHealEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HEAL3, new (std::nothrow) CEffectAllocator<CHealEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HEAL4, new (std::nothrow) CEffectAllocator<CHealEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABSORBSPIRITS, new (std::nothrow) CEffectAllocator<CHealEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1113, new (std::nothrow) CEffectAllocator<CHealEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1143, new (std::nothrow) CEffectAllocator<CHealEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1144, new (std::nothrow) CEffectAllocator<CHealEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1114, new (std::nothrow) CEffectAllocator<CWarningPlaneEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BITE_SCAR, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WIDE_CRITICAL_WOUND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_GROOMING, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_CHATTERING, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_LOPE, new (std::nothrow) CEffectAllocator<CNyanggrassEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_HISS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_POWEROFFLOCK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1128, new (std::nothrow) CEffectAllocator<CSvgSpiritEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SU_SVG_SPIRIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_PNEUMA, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1130, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FIREDANCE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RICHS_COIN_A, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RL_ANTI_MATERIAL_BLAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_E_CHAIN, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HEAT_BARREL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_H_MINE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FALLEN_ANGEL, new (std::nothrow) CEffectAllocator<CFallenAngelEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RL_B_TRAP, new (std::nothrow) CEffectAllocator<CRichsCoinEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WE_ONEFOREVER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WE_CHEERUP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1146, new (std::nothrow) CEffectAllocator<CCloud>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1147, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1151, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1152, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1153, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1154, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1155, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1156, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1157, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1158, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1159, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1160, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1187, new (std::nothrow) CEffectAllocator<CRandomOptEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1188, new (std::nothrow) CEffectAllocator<CRandomOptEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1191, new (std::nothrow) CEffectAllocator<CRandomOptEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1192, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1196, new (std::nothrow) CEffectAllocator<CNewCloudKillEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LIGHTSPHERE_SUN, new (std::nothrow) CEffectAllocator<CLightSphereEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LIGHTSPHERE_MOON, new (std::nothrow) CEffectAllocator<CLightSphereEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LIGHTSPHERE_STAR, new (std::nothrow) CEffectAllocator<CLightSphereEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1200, new (std::nothrow) CEffectAllocator<CJupitelThunderHit>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1201, new (std::nothrow) CEffectAllocator<CGravityControlEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NOVAEXPLOSING, new (std::nothrow) CEffectAllocator<CStarEmperorEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STAR_EMPEROR, new (std::nothrow) CEffectAllocator<CStarEmperorEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1211, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1212, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1213, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1214, new (std::nothrow) CEffectAllocator<CSunStanceEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1215, new (std::nothrow) CEffectAllocator<CSunStanceEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1216, new (std::nothrow) CEffectAllocator<CSunStanceEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1217, new (std::nothrow) CEffectAllocator<CSunStanceEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1222, new (std::nothrow) CEffectAllocator<CSoulUnityEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOLARBURST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SJ_DOCUMENT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FALLING_STAR, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FLASH_KICK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STORMKICK8, new (std::nothrow) CEffectAllocator<CStormKickEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1225, new (std::nothrow) CEffectAllocator<CSoulCollectEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1226, new (std::nothrow) CEffectAllocator<CLevel150Effect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1227, new (std::nothrow) CEffectAllocator<CLevel150SubEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOUL_GOLEM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEWMOON_KICK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FULLMOON_KICK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BOOK_OF_DIMENSION, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1232, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CURSE_EXPLOSION, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOUL_REAPER, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1235, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1236, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1237, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOUL_CURSE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_PROMINENCE_KICK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOUL_EXPLOSION, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOUL_REVOLVE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }

  for (int id = EF_TEST_EFFECT_00; id <= EF_TEST_EFFECT_09; ++id) {
    if (!this->RegisterAllocator(id, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
      return false;
    }
  }

  if (!this->RegisterAllocator(EF_NEW_METEORSTORM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_METEORSTORM_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ADORAMUS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ADORAMUS_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_CORE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_BALL_1, new (std::nothrow) CEffectAllocator<CTetraBallEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_BALL_2, new (std::nothrow) CEffectAllocator<CTetraBallEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_BALL_3, new (std::nothrow) CEffectAllocator<CTetraBallEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_BALL_4, new (std::nothrow) CEffectAllocator<CTetraBallEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_FIRE_EXP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_WATER_EXP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_WIND_EXP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_EARTH_EXP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_FIRE_GROUND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_WATER_GROUND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_WIND_GROUND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_EARTH_GROUND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_CORE_GROUND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_FIRE_SUB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_WATER_SUB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_WIND_SUB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_EARTH_SUB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CRIMSONROCK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CRIMSONROCK_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CRIMSONROCK_SUB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1276, new (std::nothrow) CEffectAllocator<CSevereRainStorm>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SEVERERAINSTORM_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DIAMONDDUST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DIAMONDDUST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DIAMONDDUST_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SECRAMENT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SECRAMENT_AURA, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_PSYCHICWAVE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_PSYCHICWAVE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_PSYCHICWAVE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARROWSTORM_00, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARROWSTORM_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_TRIANGLESHOT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_TRIANGLESHOT_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1324, new (std::nothrow) CEffectAllocator<CTriangleShotArrow>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_RAMPAGEBLASTER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_RAMPAGEBLASTER_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_FEINTBOMB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_FEINTBOMB_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CROSSRIPPERSLASHER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CROSSRIPPERSLASHER_00, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CROSSRIPPERSLASHER_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CROSSRIPPERSLASHER_03, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CROSSRIPPERSLASHER_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CROSSRIPPERSLASHER_06, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CROSSRIPPERSLASHER_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CROSSRIPPERSLASHER_09, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CROSSRIPPERSLASHER_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ROLLINGCUTTER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ROLLINGCUTTER_COUNT_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ROLLINGCUTTER_COUNT_02, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ROLLINGCUTTER_COUNT_03, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ROLLINGCUTTER_COUNT_04, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ROLLINGCUTTER_COUNT_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ROLLINGCUTTER_COUNT_06, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ROLLINGCUTTER_COUNT_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ROLLINGCUTTER_COUNT_08, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ROLLINGCUTTER_COUNT_09, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ROLLINGCUTTER_COUNT_10, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_TIGERCANNON, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARMSCANNON_01_DOWN, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARMSCANNON_01_UP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARMSCANNON_05_DOWN, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARMSCANNON_05_UP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARMSCANNON_07_DOWN, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARMSCANNON_07_UP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARMSCANNON_11_DOWN, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARMSCANNON_11_UP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARMSCANNON_EXPLOSION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARMSCANNON_EXPLOSION_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARMSCANNON_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_ARMSCANNON_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1357, new (std::nothrow) CEffectAllocator<CCartCannon>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CART_CANNON_EXPLOSION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CART_CANNON_EXPLOSION_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CART_CANNON_FIRE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CART_CANNON_FIRE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CART_CANNON_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CART_CANNON_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_QSCARABA_VISUAL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXETORNADO, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXETORNADO_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CART_TORNADO, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CART_TORNADO_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HELP_ANGEL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HELP_ANGEL_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BANISHINGPOINT_TARGET, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BANISHINGPOINT_TARGET_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BANISHINGPOINT_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BANISHINGPOINT_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BANISHINGPOINT_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BANISHINGPOINT_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BANISHINGPOINT_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BANISHINGPOINT_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1377, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DRAGONBREATH_BOMB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DRAGONBREATH_BOMB_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DRAGONBREATH_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DRAGONBREATH_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DRAGONBREATH_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DRAGONBREATH_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DRAGONBREATH_WATER_BOMB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DRAGONBREATH_WATER_BOMB_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DRAGONBREATH_WATER_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DRAGONBREATH_WATER_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DRAGONBREATH_WATER_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DRAGONBREATH_WATER_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_COUNTER_SLASH, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_COUNTER_SLASH_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_FATAL_MENACE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_FATAL_MENACE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_FATAL_MENACE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_FATAL_MENACE_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CANNON_SPEAR_12, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CANNON_SPEAR_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CANNON_SPEAR_03, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CANNON_SPEAR_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CANNON_SPEAR_06, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CANNON_SPEAR_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CANNON_SPEAR_09, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CANNON_SPEAR_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CANNON_SPEAR_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BASICATTACK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CRITICALATTACK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_RANGEDATTACK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_COMET, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_COMET_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_COMET_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_COMET_SUB_EXP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_COMET_SUB_EXP_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_COMET_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1419, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_RAYOFGENESIS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_RAYOFGENESIS_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_RAYOFGENESIS_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_RAYOFGENESIS_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ADORAMUS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ADORAMUS_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TETRA_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TETRA_CORE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TETRA_FIRE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TETRA_WATER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TETRA_WIND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TETRA_EARTH, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TETRA_FIRE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TETRA_WATER_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TETRA_WIND_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TETRA_EARTH_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TETRA_CORE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CRIMSONROCK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CRIMSONROCK_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JACKFROST_00, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JACKFROST_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JACKFROST_02, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JACKFROST_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JACKFROST_RUNE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_DIAMONDDUST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_DIAMONDDUST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_DIAMONDDUST_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_SECRAMENT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_PSYCHICWAVE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_PSYCHICWAVE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_PSYCHICWAVE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ARROWSTORM_00, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ARROWSTORM_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TRIANGLESHOT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TRIANGLESHOT_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_RAMPAGEBLASTER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_RAMPAGEBLASTER_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_FEINTBOMB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_FEINTBOMB_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_EARTHGRAVE_VISUAL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_EARTHGRAVE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_EARTHGRAVE_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ROLLINGCUTTER_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ROLLINGCUTTER_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ROLLINGCUTTER_02, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ROLLINGCUTTER_03, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ROLLINGCUTTER_04, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ROLLINGCUTTER_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ROLLINGCUTTER_06, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ROLLINGCUTTER_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ROLLINGCUTTER_08, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ROLLINGCUTTER_09, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ROLLINGCUTTER_10, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CROSSRIPPERSLASHER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CROSSRIPPERSLASHER_12, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CROSSRIPPERSLASHER_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CROSSRIPPERSLASHER_03, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CROSSRIPPERSLASHER_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CROSSRIPPERSLASHER_06, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CROSSRIPPERSLASHER_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CROSSRIPPERSLASHER_09, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CROSSRIPPERSLASHER_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ARMSCANNON_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ARMSCANNON_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ARMSCANNON_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ARMSCANNON_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ARMSCANNON_EXPLOSION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ARMSCANNON_EXPLOSION_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ARMSCANNON_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_ARMSCANNON_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_AXETORNADO, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_AXETORNADO_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CART_CANNON_EXPLOSION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CART_CANNON_EXPLOSION_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CART_CANNON_FIRE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CART_CANNON_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CART_CANNON_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CART_TORNADO, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CART_TORNADO_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_BANISHINGPOINT_TARGET, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_BANISHINGPOINT_TARGET_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_BANISHINGPOINT_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_BANISHINGPOINT_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_BANISHINGPOINT_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_BANISHINGPOINT_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_BANISHINGPOINT_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_BANISHINGPOINT_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CANNON_SPEAR_12, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CANNON_SPEAR_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CANNON_SPEAR_03, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CANNON_SPEAR_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CANNON_SPEAR_06, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CANNON_SPEAR_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CANNON_SPEAR_09, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CANNON_SPEAR_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_CANNON_SPEAR_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_DRAGONBREATH_BOMB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_DRAGONBREATH_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_DRAGONBREATH_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_DRAGONBREATH_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_DRAGONBREATH_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_DRAGONBREATH_WATER_BOMB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_DRAGONBREATH_WATER_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_DRAGONBREATH_WATER_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_DRAGONBREATH_WATER_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_DRAGONBREATH_WATER_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_TIGERCANNON, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_SEVERERAINSTORM_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_COMET, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_COMET_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_COMET_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_COMET_SUB_EXP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_COMET_SUB_EXP_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_COMET_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_JACKFROST_00, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_JACKFROST_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_JACKFROST_02, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_JACKFROST_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MINEFFECT_JACKFROST_RUNE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_POISONSMOKE_1, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_POISONSMOKE_2, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_POISONSMOKE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_FEARBREEZE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_HELLINFERNO, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_HELLINFERNO_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_HELLINFERNO_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_HELLINFERNO_SUB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_HELLINFERNO_SUB_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CLEARANCE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CLEARANCE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CLEARANCE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_VARETYRSPEAR, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_VARETYRSPEAR_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_VARETYRSPEAR_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_VARETYRSPEAR_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1555, new (std::nothrow) CEffectAllocator<CEvilLandEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CROSSIMPACT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CROSSIMPACT_TARGET, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_REVERBERATION_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_FALLENEMPIRE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_FALLENEMPIRE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_FALLENEMPIRE_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_GATEOFHELL_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_GATEOFHELL_TARGET, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_HOWLINGOFLION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_HOWLINGOFLION_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_HOWLINGOFLION_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_HOWLINGOFLION_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_HOWLINGOFLION_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_METALLICSOUND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_METALLICSOUND_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_METALLICSOUND_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_REPRODUCE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_REPRODUCE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SOULEXPANSION_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SOULEXPANSION_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SOULEXPANSION_LOOP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SOULEXPANSION_LOOP_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SOULEXPANSION_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SOULEXPANSION_HIT_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AROMAOIL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AROMAOIL_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BATHFOAM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BATHFOAM_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_CORE_REUSE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_GRADE_ENCHANT_INTRO, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_GRADE_ENCHANT_SUCCESS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_GRADE_ENCHANT_FAILED, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BLUE_WAVE_1, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BLUE_WAVE_2, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BLUE_WAVE_3, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RED_WAVE_1, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RED_WAVE_2, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RED_WAVE_3, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1617, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1618, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1619, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_WALLOFTHORN_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_WALLOFTHORN_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_IGNITIONBREAK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_IGNITIONBREAK_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_IGNITIONBREAK_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JUDEX_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JUDEX_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JUDEX_CAST_LOOP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JUDEX_CAST_LOOP_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JUDEX_TARGET, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JUDEX_TARGET_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JUDEX_LIGHT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JUDEX_LIGHT_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CHAINLIGHTNING_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CHAINLIGHTNING_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CHAINLIGHTNING_LOCKON, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CHAINLIGHTNING_TARGET, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_MANDRAGORA, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_MANDRAGORA_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_MANDRAGORA_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SOUNDOFDESTRUCTION_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SOUNDOFDESTRUCTION_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SOUNDOFDESTRUCTION_TARGET, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AIMEDBOLT_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AIMEDBOLT_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AIMEDBOLT_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AIMEDBOLT_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AIMEDBOLT_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AIMEDBOLT_END, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AIMEDBOLT_TARGET_LOOP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_UNLIMIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_UNLIMIT_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_EPICLESIS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1657, new (std::nothrow) CEffectAllocator<CAxeBoomerang>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXEBOOMERANG_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXEBOOMERANG_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXEBOOMERANG_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1661, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1662, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1663, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SPELLFIST_FIRE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SPELLFIST_FIRE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SPELLFIST_WATER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SPELLFIST_WATER_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_SPELLFIST_WIND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1670, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1671, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1672, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1673, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1674, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CARTBOOST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_CARTBOOST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_OVERBRAND_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_OVERBRAND_03, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_OVERBRAND_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_OVERBRAND_06, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_OVERBRAND_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_OVERBRAND_09, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_OVERBRAND_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_OVERBRAND_12, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_OVERBRAND_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_OVERBRAND_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_OVERBRAND_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_WINDCUTTER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_WINDCUTTER_BOTTOM_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_WINDCUTTER_BOTTOM_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_WINDCUTTER_TARGET, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2460, new (std::nothrow) CEffectAllocator<CServantWeaponNewEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1694, new (std::nothrow) CEffectAllocator<CServantWeaponEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1695, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1696, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1697, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_POWERSWING_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_POWERSWING_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_POWERSWING_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_POWERSWING_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_POWERSWING_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_VULCANARM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_VULCANARM_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_VULCANARM_SHOTHOLE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_EARTHDRIVE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_EARTHDRIVE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_EARTHDRIVE_TARGET, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1710, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2280, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1711, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1712, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1713, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1714, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1715, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1716, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1717, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1718, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1719, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1720, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1721, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DRAGONIC_AURA, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DRAGONIC_AURA_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DRAGONIC_AURA_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DRAGONIC_AURA_HIT_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DRAGONIC_AURA_BUFF, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DRAGONIC_AURA_BUFF_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1728, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1729, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1750, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1751, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AUTOSHADOWSPELL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AUTOSHADOWSPELL_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DESTRUCTIVE_HURRICANE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DESTRUCTIVE_HURRICANE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DESTRUCTIVE_HURRICANE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DESTRUCTIVE_HURRICANE_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DESTRUCTIVE_HURRICANE_CLIMAX, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DESTRUCTIVE_HURRICANE_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DEADLY_PROJECTION_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DEADLY_PROJECTION_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DEADLY_PROJECTION_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DEADLY_PROJECTION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOUL_VC_STRIKE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOUL_VC_STRIKE_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOUL_VC_STRIKE_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ROCKDOWN, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ROCKDOWN_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ROCKDOWN_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ROCKDOWN_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ROCKDOWN_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1752, new (std::nothrow) CEffectAllocator<CSevereRainStorm>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RAIN_OF_CRYSTAL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RAIN_OF_CRYSTAL_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1755, new (std::nothrow) CEffectAllocator<CStormCannonEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1756, new (std::nothrow) CEffectAllocator<CCrimsonArrowEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CRIMSON_ARROW_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CRIMSON_ARROW_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CRIMSON_ARROW_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1760, new (std::nothrow) CEffectAllocator<CViolentQuakeEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_VIOLENT_QUAKE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_VIOLENT_QUAKE_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_VIOLENT_QUAKE_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_VIOLENT_QUAKE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2461, new (std::nothrow) CEffectAllocator<CChargingPierceNewEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1764, new (std::nothrow) CEffectAllocator<CChargingPierceEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1765, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1766, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1767, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1768, new (std::nothrow) CEffectAllocator<CAstralStrikeEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ASTRAL_STRIKE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ASTRAL_STRIKE_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MYSTERY_ILLUSION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MYSTERY_ILLUSION_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MYSTERY_ILLUSION_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MYSTERY_ILLUSION_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MYSTERY_ILLUSION_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TORNADO_STORM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TORNADO_STORM_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TORNADO_STORM_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TORNADO_STORM_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TORNADO_STORM_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FROZEN_SLASH, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FROZEN_SLASH_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FROZEN_SLASH_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FROZEN_SLASH_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FROZEN_SLASH_ICE_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FROZEN_SLASH_ICE_02, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FROZEN_SLASH_ICE_03, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FROZEN_SLASH_ICE_04, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FROZEN_SLASH_ICE_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FROZEN_SLASH_ICE_06, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FROZEN_SLASH_ICE_07, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FROZEN_SLASH_ICE_08, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ALL_BLOOM_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ALL_BLOOM_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ALL_BLOOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ALL_BLOOM_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1798, new (std::nothrow) CEffectAllocator<CViolentQuakeEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ALL_BLOOM_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1800, new (std::nothrow) CEffectAllocator<CClimaxEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CLIMAX_LIGHT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CLIMAX_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CLIMAX_CAST_LOOP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1804, new (std::nothrow) CEffectAllocator<CFloralFlareRoad>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FLORAL_FLARE_ROAD_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FLORAL_FLARE_ROAD_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FLORAL_FLARE_ROAD_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MEDIALE_VOTUM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MEDIALE_VOTUM_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MEDIALE_VOTUM_LOOP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARGUTUS_VITA, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARGUTUS_VITA_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARGUTUS_TELUM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARGUTUS_TELUM_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_PRESENS_ACIES, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_PRESENS_ACIES_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABR_MOTHER_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABR_MOTHER_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABR_MOTHER_02, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_D_MACHINE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_D_MACHINE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_D_MACHINE_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_D_MACHINE_START, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_D_MACHINE_END, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1839, new (std::nothrow) CEffectAllocator<CAMachineEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_A_MACHINE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_A_MACHINE_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXE_STOMP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXE_STOMP_12, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXE_STOMP_01, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXE_STOMP_03, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXE_STOMP_05, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXE_STOMP_06, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXE_STOMP_08, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXE_STOMP_09, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXE_STOMP_11, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXE_STOMP_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_AXE_STOMP_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_RUSH_QUAKE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_RUSH_QUAKE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_RUSH_QUAKE_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARBITRIUM_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARBITRIUM_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARBITRIUM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARBITRIUM_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARBITRIUM_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1829, new (std::nothrow) CEffectAllocator<CDancingKnifeEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1827, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1828, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1830, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_REPARATIO_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_REPARATIO_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_REPARATIO, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_REPARATIO_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_REPARATIO_TARGET, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_REPARATIO_TARGET_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1837, new (std::nothrow) CEffectAllocator<CSavageImpactEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ETERNAL_SLASH_HIT_1, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_EFFLIGO, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_EFFLIGO_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_EFFLIGO_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_COMPETENTIA, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_COMPETENTIA_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1851, new (std::nothrow) CEffectAllocator<CCompetentia>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_COMPETENTIA_LOOP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_COMPETENTIA_LOOP_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CRYSTAL_IMPACT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CRYSTAL_IMPACT_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CRYSTAL_IMPACT_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CRYSTAL_IMPACT_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CRYSTAL_IMPACT_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CRYSTAL_IMPACT_BUFF, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1862, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1863, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RELIGIO, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RELIGIO_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FATAL_SHADOW_CROW, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FATAL_SHADOW_CROW_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FATAL_SHADOW_CROW_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1869, new (std::nothrow) CEffectAllocator<CDropItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1870, new (std::nothrow) CEffectAllocator<CDropItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1871, new (std::nothrow) CEffectAllocator<CDropItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1189, new (std::nothrow) CEffectAllocator<CDropItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1190, new (std::nothrow) CEffectAllocator<CDropItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1186, new (std::nothrow) CEffectAllocator<CDropItemEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FLAME_TRAP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FLAME_TRAP_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FLAME_TRAP_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2467, new (std::nothrow) CEffectAllocator<CShadowExceedNew>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1868, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1867, new (std::nothrow) CEffectAllocator<CShadowExceed>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DEEP_BLIND_TRAP_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DEEP_BLIND_TRAP_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DEEP_BLIND_TRAP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SWIFT_TRAP_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SWIFT_TRAP_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SWIFT_TRAP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOLID_TRAP_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOLID_TRAP_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOLID_TRAP_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOLID_TRAP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BENEDICTUM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BENEDICTUM_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BENEDICTUM_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STRATUM_TREMOR_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STRATUM_TREMOR_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STRATUM_TREMOR_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STRATUM_TREMOR, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STRATUM_TREMOR_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1895, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1896, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1898, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WIND_SIGN, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WIND_SIGN_LOOP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABR_INFINITY, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABR_INFINITY_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABR_INFINITY_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABR_INFINITY_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BATTLE_W, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BATTLE_W_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BATTLE_W_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BATTLE_W_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_D_CANNON, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_D_CANNON_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_D_CANNON_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HAWK_RUSH, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1911, new (std::nothrow) CEffectAllocator<CPetitioEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_PNEUMATICUS_PROCELLA_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1957, new (std::nothrow) CEffectAllocator<CPneumaticusProcellaEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_PRAMEN_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_PRAMEN_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_PRAMEN, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DILECTIO_HEAL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DILECTIO_HEAL_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DILECTIO_HEAL_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DILECTIO_HEAL_TARGET, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_DILECTIO_HEAL_TARGET_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1912, new (std::nothrow) CEffectAllocator<CEternalSlashAttackEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1913, new (std::nothrow) CEffectAllocator<CEternalSlashCountEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ETERNAL_SLASH_HIT_2, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1960, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1961, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1962, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_POTENT_VENOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1950, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1951, new (std::nothrow) CEffectAllocator<CCresciveBolt>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1952, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1953, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1954, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1955, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SINCERE_FAITH, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SINCERE_FAITH_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_DAGGER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_DAGGER_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1968, new (std::nothrow) CEffectAllocator<CHawkVumerangEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HAWK_BOOMERANG_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HAWK_BOOMERANG_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HAWK_BOOMERANG_START, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_GALE_STORM_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_1973, new (std::nothrow) CEffectAllocator<CGaleStorm>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_GALE_STORM_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FIRST_FAITH_POWER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FIRST_FAITH_POWER_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FIRST_FAITH_POWER_START, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_JUDGE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_JUDGE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_JUDGE_START, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_JUDGE_AURA, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THIRD_EXOR_FLAME, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THIRD_EXOR_FLAME_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THIRD_EXOR_FLAME_START, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THIRD_EXOR_FLAME_AURA, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_OLEUM_SANCTUM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_OLEUM_SANCTUM_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_OLEUM_SANCTUM_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_OLEUM_SANCTUM_LOOP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_POWERFUL_FAITH, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_INCREASING_ACTIVITY, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_INCREASING_ACTIVITY_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_INCREASING_ACTIVITY_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_EXPLOSION_BLASTER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_EXPLOSION_BLASTER_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_EXPLOSION_BLASTER_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FIRST_BRAND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FIRST_BRAND_AURA, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FIRST_BRAND_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FIRST_BRAND_START, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SECOND_FAITH_START, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SECOND_FAITH, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SECOND_FAITH_AURA, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2080, new (std::nothrow) CEffectAllocator<CRandomHitEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_SLAYER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_SLAYER_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_SLAYER_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MASSIVE_FLAME_BLASTER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MASSIVE_FLAME_BLASTER_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MASSIVE_FLAME_BLASTER_HIT_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THIRD_FLAME_BOMB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THIRD_FLAME_BOMB_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SECOND_FLAME, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SECOND_FLAME_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2082, new (std::nothrow) CEffectAllocator<CRandomHitEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THIRD_CONSECRATION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THIRD_CONSECRATION_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THIRD_PUNISH, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THIRD_PUNISH_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FIRM_FAITH, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2006, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2007, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2008, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CHAIN_REACTION_SHOT_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CHAIN_REACTION_SHOT_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ETERNAL_SLASH_HIT_3, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CHAIN_REACTION_SHOT_HITSUB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CHAIN_REACTION_SHOT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2000, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2001, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2002, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACTIVITY_BURN, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACTIVITY_BURN_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACTIVITY_BURN_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SERPENS_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SERPENS_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SERPENS_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SERPENS_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SERPENS_SHIELD, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SERPENS_DEEP_POISONING, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SERPENS_DEEP_POISONING_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DILUVIO_ICE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DILUVIO_ICE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DILUVIO_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DILUVIO_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DILUVIO_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DILUVIO_ARMOR, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DILUVIO_COLD_FORCE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DILUVIO_COLD_FORCE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SPELL_ENCHANTING, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2026, new (std::nothrow) CEffectAllocator<CSevereRainStorm>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DIAMOND_STORM_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DIAMOND_STORM_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DIAMOND_STORM_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DIAMOND_STORM_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_VENOM_SWAMP_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_VENOM_SWAMP_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_VENOM_SWAMP_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_VENOM_SWAMP_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2046, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2047, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2048, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2049, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2050, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARDOR, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARDOR_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARDOR_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARDOR_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARDOR_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARDOR_FLAMEARMOR, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARDOR_FLAMETECHNIC, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ARDOR_FLAMETECHNIC_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LIGHTNING_LAND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LIGHTNING_LAND_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LIGHTNING_LAND_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LIGHTNING_LAND_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_LIGHTNING_LAND_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CONFLAGRATION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CONFLAGRATION_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CONFLAGRATION_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CONFLAGRATION_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CONFLAGRATION_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOUND_BLEND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOUND_BLEND_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOUND_BLEND_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SOUND_BLEND_RED, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_METALLIC_FURY, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2065, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2066, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2067, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2068, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2069, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2070, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2071, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2072, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2073, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2074, new (std::nothrow) CEffectAllocator<CCrimsonArrowEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NIFLHEIM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NIFLHEIM_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2077, new (std::nothrow) CEffectAllocator<CCylinderEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_PROCELLA, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_PROCELLA_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_PROCELLA_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_PROCELLA_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_PROCELLA_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_PROCELLA_STORM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_PROCELLA_GRACE_BREEZE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_PROCELLA_GRACE_BREEZE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TERREMOTUS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TERREMOTUS_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TERREMOTUS_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TERREMOTUS_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TERREMOTUS_PROTECT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TERREMOTUS_EARTH_CARE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TERREMOTUS_EARTH_CARE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_STRIKE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_STRIKE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_STRIKE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_STRIKE_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_STRIKE_GROUND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_STRIKE_GROUND_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_STRIKE_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2109, new (std::nothrow) CEffectAllocator<CFromTheAbyssEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FROM_THE_ABYSS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_STRIKE_HIT_2, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DEFT_STAB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_SQUARE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_SQUARE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_DAGGER_HIT_2, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2114, new (std::nothrow) CEffectAllocator<CCrimsonArrowEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_FRENZY_SHOT_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2116, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2117, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2118, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2119, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2120, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2121, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MUSICAL_INTERLUDE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_MUSICAL_INTERLUDE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2124, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2125, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TERRA_DRIVE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TERRA_DRIVE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TERRA_DRIVE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TERRA_DRIVE_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TERRA_DRIVE_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_FIRE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_FIRE_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_FIRE_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_LAND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_LAND_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_LAND_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_POISON, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_POISON_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_POISON_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_WATER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_WATER_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_WATER_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_WIND, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_WIND_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_WIND_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2154, new (std::nothrow) CEffectAllocator<CCrimsonArrowEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNLUCKY_RUSH_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABYSS_DAGGER_HIT_3, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2157, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2158, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2159, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2160, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ADVANCED_PROTECTION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ADVANCED_PROTECTION_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ADVANCED_PROTECTION_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ADVANCED_PROTECTION_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2462, new (std::nothrow) CEffectAllocator<CSevereRainStorm>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2165, new (std::nothrow) CEffectAllocator<CSevereRainStorm>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2166, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2167, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2168, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2175, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2176, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2177, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2178, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2179, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2169, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2170, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2171, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2172, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2173, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2174, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2181, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2180, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2182, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2183, new (std::nothrow) CEffectAllocator<CAxeBoomerang>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SHIELD_SHOOTING_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_SHIELD_SHOOTING_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THE_W_PROTECTION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THE_W_PROTECTION_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2188, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2189, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2190, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2191, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2192, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2193, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2194, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2195, new (std::nothrow) CEffectAllocator<CCrimsonArrowEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2196, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2197, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2198, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2199, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2200, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2201, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2202, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_FIRE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_FIRE_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2206, new (std::nothrow) CEffectAllocator<CAcidifiedThrow>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_FIRE_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_FIRE_START, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_FIRE_START_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_FIRE_LOOP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_FIRE_LOOP_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WATER_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WATER_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2214, new (std::nothrow) CEffectAllocator<CAcidifiedThrow>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WATER_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WATER_START, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WATER_START_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WATER_LOOP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WATER_LOOP_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WIND_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WIND_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2222, new (std::nothrow) CEffectAllocator<CAcidifiedThrow>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WIND_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WIND_START, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WIND_START_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WIND_LOOP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_WIND_LOOP_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_GROUND_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_GROUND_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2230, new (std::nothrow) CEffectAllocator<CAcidifiedThrow>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_GROUND_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_GROUND_START, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_GROUND_START_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_GROUND_LOOP, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ACIDIFIED_ZONE_GROUND_LOOP_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HELLTREE_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HELLTREE_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HELLTREE_DUSTY, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HELLTREE_HOWLING, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RESEARCH_REPORT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RESEARCH_REPORT_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RESEARCH_REPORT_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WOODEN_WARRIOR_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WOODEN_WARRIOR_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WOODEN_WARRIOR_ATTACK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WOODEN_WARRIOR_ATTACK_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WOODEN_WARRIOR_THROWROCK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WOODEN_WARRIOR_THROWROCK_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WOODEN_FAIRY_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WOODEN_FAIRY_CAST_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_WOODEN_FAIRY_DUSTY, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STATE_LASSITUDE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STATE_SWOONING, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STATE_HOLYFLAME, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STATE_LIGHTNINGSTRIKE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STATE_CONFLAGRATION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STATE_FROSTBITE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STATE_FROSTBITE_FINISH, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2272, new (std::nothrow) CEffectAllocator<CFullScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STATE_DEADLYPOISON, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_STATE_CRYSTALLIZATION, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2275, new (std::nothrow) CEffectAllocator<CLevel150Effect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2276, new (std::nothrow) CEffectAllocator<CLevel150SubEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_KILLING_AURA, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_KILLING_AURA_END, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_KILLING_AURA_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2305, new (std::nothrow) CEffectAllocator<CCurseOfCubeEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2306, new (std::nothrow) CEffectAllocator<CCurseOfCubeEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2307, new (std::nothrow) CEffectAllocator<CCaneOfEvilEyeEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_CANE_OF_EVIL_EYE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UI_ENCHANT_INTRO_YELLOW, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UI_ENCHANT_SUCCESS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UI_ENCHANT_FAIL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UI_ENCHANT_INTRO_BLUE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UI_LEVELUP_SUCCESS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UI_LEVELUP_FAIL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UI_ENCHANT_INTRO_GREEN, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UI_RESET_SUCCESS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UI_RESET_FAIL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RUNE_ACTIVE_FINISH, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RUNESET_ACTIVE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RUNESET_UPGRADE_SUCCESS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RUNESET_UPGRADE_FAIL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RUNE_BROKEN, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RUNE_SLOT_SMALL, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_RUNE_SLOT_BIG, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TOXIN_OF_MANDARA_BOMB, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TOXIN_OF_MANDARA_BOMB_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TOXIN_OF_MANDARA_SHOCK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEEDLE_STINGER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEEDLE_STINGER_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEEDLE_STINGER_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABSOLUTE_ZEPHYR, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABSOLUTE_ZEPHYR_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ABSOLUTE_ZEPHYR_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TWISTER_CUTTER, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TWISTER_CUTTER_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HEILIGE_PFERD, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HEILIGE_PFERD_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_HEILIGE_PFERD_CAST, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_GOLDENE_TONE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_GLANZEN_SPIES, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_GLANZEN_SPIES_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BLAZING_AND_FURIOUS, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THE_ONE_FIGHTER_RISES_BOTTOM, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TEMPERING, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2341, new (std::nothrow) CEffectAllocator<CBlastForge>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_BLAST_FORGE_HIT, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2343, new (std::nothrow) CEffectAllocator<CWindEffect>)) {
    return false;
  }

  std::vector<int> ez2strEffects = CEZ2STREffectMgr::GetInstance()->EffectIds();
  for (int id : ez2strEffects) {
    this->RegisterAllocator(id, new (std::nothrow) CEffectAllocator<CEZ2STREffectEx>);
  }

  if (!this->RegisterAllocator(EF_UNK_2348, new (std::nothrow) CEffectAllocator<CSevereRainStorm>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2349, new (std::nothrow) CEffectAllocator<CSevereRainStorm>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2350, new (std::nothrow) CEffectAllocator<CSevereRainStorm>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2351, new (std::nothrow) CEffectAllocator<CSevereRainStorm>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2354, new (std::nothrow) CEffectAllocator<CSevereRainStorm>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2352, new (std::nothrow) CEffectAllocator<CFuumaShouakuEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2355, new (std::nothrow) CEffectAllocator<CTalismanMarkEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2366, new (std::nothrow) CEffectAllocator<CJupitelThunderStormEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_WATER_SUB_2, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_TETRA_VORTEX_EARTH_SUB_2, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CHANGE_SIZE, new (std::nothrow) CEffectAllocator<CChangeSizeEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2375, new (std::nothrow) CEffectAllocator<CShowEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_20TH_CRYSTAL_S, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_20TH_CRYSTAL_M, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_20TH_CRYSTAL_L, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_GATE_GREEN_1, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_GATE_PURPLE_1, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_GATE_RED_1, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_GATE_GREEN_2, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_GATE_PURPLE_2, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_GATE_RED_2, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_GATE_GREEN_3, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_GATE_PURPLE_3, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_GATE_RED_3, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_20TH_FIREWORK, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NPC_GATE_SKYBLUE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2394, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2400, new (std::nothrow) CEffectAllocator<CGravityControlEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_BATTLE_W_HIT_2, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_PRAMEN_REUSE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_THE_W_PROTECTION_REUSE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_ELEMENTAL_BUSTER_FIRE_REUSE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_DESTRUCTIVE_HURRICANE_BOTTOM_REUSE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_CRYSTAL_IMPACT_BOTTOM_REUSE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_NEW_JUDEX_TARGET_REUSE, new (std::nothrow) CEffectAllocator<CEZ2STRScreenEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2404, new (std::nothrow) CEffectAllocator<CSprArrowEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2411, new (std::nothrow) CEffectAllocator<CFootprintEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2412, new (std::nothrow) CEffectAllocator<CFootprintStrEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2420, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2424, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2428, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2429, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2430, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2431, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2441, new (std::nothrow) CEffectAllocator<CThunderingOrb>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2466, new (std::nothrow) CEffectAllocator<CChopChopClawMarks>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2468, new (std::nothrow) CEffectAllocator<CWindBomb>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2472, new (std::nothrow) CEffectAllocator<EffectResourceSwitcher>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2473, new (std::nothrow) CEffectAllocator<EffectResourceSwitcher>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2458, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }
  if (!this->RegisterAllocator(EF_UNK_2443, new (std::nothrow) CEffectAllocator<CSpriteEffect>)) {
    return false;
  }

  return true;
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

// OFFSET: 0x00B3A9C0
// FUNCTION: fn_18790
bool HasEffectScript(std::string path, std::string name) {
  bool exists = false;
  std::string versionKey = EmitterKey(name.substr(), "effect_version");
  g_luaScript->Call("IsExistValue", "s>b", versionKey.c_str(), &exists);
  if (exists) {
    return true;
  }

  std::string emitterKey = EmitterKey(name.substr(), "emitterInfo");
  g_luaScript->Call("IsExistValue", "s>b", emitterKey.c_str(), &exists);
  return exists;
}

// OFFSET: 0x00B3B0A0
// FUNCTION: fn_18795
void ReadEffectScriptV1(std::string name) {
  g_luaScript->Call("ReadEmitterInfoz_ver_1_0", "s", EmitterInfoKey(name.substr()).c_str());
  g_luaScript->Call("ReadEffectInfoz", "s", EffectFunctionName(name.substr()).c_str());
}

// OFFSET: 0x00B3B240
// FUNCTION: fn_18796
void ReadEffectScriptV2(std::string name) {
  g_luaScript->Call("ReadEmitterInfoz", "s", EmitterKey(name.substr(), "emitterInfo").c_str());
  g_luaScript->Call("ReadMagicFloorInfoz", "s", EmitterKey(name.substr(), "magicfloorInfo").c_str());
  g_luaScript->Call("ReadEvilsPawInfoz", "s", EmitterKey(name.substr(), "evilsPawInfo").c_str());
  g_luaScript->Call("ReadAnimatedEmitterInfoz", "s", EmitterKey(name.substr(), "animatedEmitterInfo").c_str());
  g_luaScript->Call("ReadEZ2STRInfoz", "s", EmitterKey(name.substr(), "ez2strInfo").c_str());
  g_luaScript->Call("ReadWindEffectInfoz", "s", EmitterKey(name.substr(), "windEffectInfo").c_str());
}

// OFFSET: 0x00B3AC30
// FUNCTION: fn_18792
void EffectLists::LoadMapEffectScript(bool reload) {
  if (this->indexed.empty()) {
    return;
  }

  std::string directory = "Lua Files\\EffectTool\\";
  std::string name = CurrentMapScriptName();
  std::string path = directory + name;
  if (HasEffectScript(path.c_str(), name.substr()) || g_luaScript->Load(path.c_str(), true, false)) {
    int version = 0;
    std::string versionKey = EmitterKey(name.substr(), "effect_version");
    g_luaScript->Call("GetValue", "s>d", versionKey.c_str(), &version);
    switch (version) {
    case 0:
    case 1:
      ReadEffectScriptV1(name.substr());
      break;
    case 2:
      ReadEffectScriptV2(name.substr());
      break;
    }
  }
}
