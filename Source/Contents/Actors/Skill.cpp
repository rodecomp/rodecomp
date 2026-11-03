#include "Contents/Actors/Skill.h"

#include "ActorPickNode.h"
#include "Contents/Actors/GraffiSkill.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/CaneOfEvilEyeEffect.h"
#include "Contents/Effects/EZ2STREffect.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/Models/ModelActor.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Resource/Models/ModelResource.h"
#include "Resource/ResourceMgr.h"
#include "Render/Renderer.h"
#include "Render/SpriteGeometry.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstring>
#include <cstdlib>
#include <new>
#include <windows.h>
#include <timeapi.h>

struct SkillHitMessage {
  /* 0000 */ int skillId;
  /* 0004 */ int damage;
  /* 0008 */ int unknown_0008;
  /* 000C */ std::uint32_t targetAid;
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ int hitCount;
  /* 001C */ int unknown_001C;
  /* 0020 */ int unknown_0020;
  /* 0024 */ int unknown_0024;
  /* 0028 */ int motionSpeed;
};

struct SkillUnitMessage {
  /* 0000 */ std::uint32_t aid;
  /* 0004 */ std::uint32_t owner;
  /* 0008 */ int type;
  /* 000C */ int x;
  /* 0010 */ int y;
  /* 0014 */ unsigned char visible;
  /* 0018 */ int range;
  /* 001C */ int level;
  /* 0020 */ std::uint32_t characterId;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SkillHitMessage) == 0x2C);
static_assert(sizeof(SkillUnitMessage) == 0x24);
#endif

// OFFSET: 0x00E4A350
// FUNCTION: fn_24252
CGraffiSkill::CGraffiSkill() : CSkill(0) {
}

// OFFSET: 0x00E4A6D0
// FUNCTION: fn_24256
CSkill::CSkill(std::uint32_t aid) {
  this->skillAid = aid;
  this->SetActorJob(0x7E);
  this->m_effectTimer = 0x8CA0;
  this->render.unknown_00A4Bytes[1] = 0;
  this->render.enabled = 0;
  this->unknown_0264[0] = 0;
  this->m_model = nullptr;
  this->m_effect = nullptr;
  this->unknown_0410 = 0;
  this->range = 0;
  this->m_hasEffect = false;
  this->m_targets.clear();
}

// OFFSET: 0x00E4AA30
// FUNCTION: fn_24259
CSkill::~CSkill() {
  if (this->unknown_0264[0] != 0) {
    g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0264[0]));
    this->unknown_0264[0] = 0;
  }

  this->ReleaseModel();
  for (std::uint64_t target : this->m_targets) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    mode->scene->SendObjectMessage(this, target, 24, 0, 0, 0, 0);
  }

  this->m_targets.clear();
}

// OFFSET: 0x00E4B400
// FUNCTION: fn_24268
const char* CSkill::GetTrapModelName() {
  switch (this->GetEffectiveJob(-1)) {
    case 0x8F:
    case 0xDA:
    case 0xDE:
    case 0xE6:
      return "외부소품\\트랩03_3.rsm";
    case 0x90:
      return "외부소품\\트랩02.rsm";
    case 0x91:
    case 0xCB:
      return "외부소품\\트랩01.rsm";
    case 0x93:
      return "외부소품\\트랩03.rsm";
    case 0x94:
      return "외부소품\\트랩03_6.rsm";
    case 0x95:
      return "외부소품\\트랩03_4.rsm";
    case 0x96:
      return "외부소품\\트랩03_5.rsm";
    case 0x97:
      return "외부소품\\트랩03_2.rsm";
    case 0x98:
      return "외부소품\\트랩04.rsm";
    case 0x99:
      return "외부소품\\트랩05.rsm";
    case 0xD2:
      return "event\\3차트랩_변화01.rsm";
    case 0xD3:
      return "event\\3차트랩_변수01.rsm";
    case 0xD4:
      return "event\\3차트랩_변지01.rsm";
    case 0xD5:
      return "event\\3차트랩_변풍01.rsm";
    case 0xD6:
      return "event\\3차트랩_화01.rsm";
    case 0xD7:
      return "event\\3차트랩_수01.rsm";
    case 0xD8:
      return "event\\3차트랩_풍01.rsm";
    case 0xD9:
      return "event\\3차트랩_지01.rsm";
    case 0xE5:
      return "event\\3차트랩_가시01.rsm";
    case 0x5174:
      return "effect\\deepblindtrap\\deep_trap.rsm2";
    case 0x5175:
      return "effect\\solidtrap\\sol_trap.rsm2";
    case 0x5176:
      return "effect\\swifttrap\\swi_trap.rsm2";
    case 0x5177:
      return "effect\\flametrap\\fla_trap.rsm2";
    default:
      return nullptr;
  }
}

// OFFSET: 0x00E4B580
// FUNCTION: fn_24269
void CSkill::ReleaseModel() {
  if (this->m_model == nullptr) {
    return;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  WorldSpatialTree* spatial = &mode->scene->spatial;
  if (spatial != nullptr) {
    spatial->RemoveModel(this->m_model, 0);
  }

  delete this->m_model;
  this->m_model = nullptr;
  if (this->m_effect != nullptr) {
    this->m_effect->SendMsg(nullptr, 24, 0, 0, 0, 0, 0, 0);
  }

  this->m_effect = nullptr;
  if (this->m_hasEffect) {
    g_effectMgr->Find(this->skillAid, this->m_effectId)->RetireIfInactive();
    this->m_hasEffect = false;
  }
}

// OFFSET: 0x00E503F0
// FUNCTION: fn_24294
void CSkill::Render(int argument) {
  if (!this->render.enabled) {
    this->UpdateTlvertex(reinterpret_cast<const float*>(argument));
    return;
  }

  float x;
  float y;
  float reciprocalDepth;
  g_renderer->ProjectViewport(&this->render.position.x, reinterpret_cast<const float*>(argument), &x, &y, &reciprocalDepth);
  const float scale = g_renderer->ScaleDistance(reciprocalDepth) / g_heightScale;
  this->render.unknown_005C = scale;

  float bounds[5];
  bounds[0] = x - (g_renderer->viewport.uiScaleX + g_renderer->viewport.uiScaleX) * scale;
  bounds[1] = y - (g_renderer->viewport.uiScaleY + g_renderer->viewport.uiScaleY) * scale;
  bounds[2] = (g_renderer->viewport.uiScaleX + g_renderer->viewport.uiScaleX) * scale + x;
  bounds[3] = y;
  bounds[4] = reciprocalDepth;
  this->SetRenderInfo(bounds, x, y);
}

// OFFSET: 0x00E4B690
// FUNCTION: fn_24271
void CSkill::LaunchGroundEffects(int effectId, int, int duration) {
  if (g_effectMgr->HasAllocator(effectId)) {
    g_effectMgr->Spawn(this->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 1, 0);
  }

  if (duration <= 0) {
    duration = EffectLifetime(effectId);
  }
  const unsigned interval = duration - 20;
  if (this->m_effectTimer < interval) {
    return;
  }

  this->m_effectTimer = 0;
  const int range = this->range < 0 ? 0 : this->range;
  for (int column = -range; column <= range; ++column) {
    for (int row = -range; row <= range; ++row) {
      const float x = static_cast<float>(column * 6) + this->render.position.x + static_cast<float>(rand() % 11) * 0.2f - 1.0f;
      const float z = static_cast<float>(row * 6) + this->render.position.z + static_cast<float>(rand() % 11) * 0.2f - 1.0f;
      float y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Height(x, z);
      if (effectId == 0x3BF || effectId == 0x3CF) {
        y += 20.0f;
      }
      y -= static_cast<float>(rand() % 11) * 0.4f;

      vector3d position;
      position.x = x;
      position.y = y;
      position.z = z;
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LaunchDummyEffect(&position, effectId, interval + 20, 0);
    }
  }
}

struct SkillPickInfo {
  /* 0000 */ float left;
  /* 0004 */ float top;
  /* 0008 */ float depth;
  /* 000C */ float right;
  /* 0010 */ float bottom;
  /* 0014 */ float depthOffset;
  /* 0018 */ std::uint32_t aid;
  /* 001C */ int job;
  /* 0020 */ int kind;
  /* 0024 */ float unused;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SkillPickInfo) == 0x28);
#endif

// OFFSET: 0x00E55100
// FUNCTION: fn_24303
void CSkill::sub_E55100(const float* box, float x, float y) {
  if (this->render.unknown_00A4Bytes[1] != 0) {
    // GLOBAL: gl_25171
    static int minimum = static_cast<int>(g_renderer->UiScaleX() * 34.0f);
    auto* info = static_cast<SkillPickInfo*>(operator new(sizeof(SkillPickInfo)));
    if (info != nullptr) {
      info->left = 0.0f;
      info->top = 0.0f;
      info->depth = 0.0f;
      info->right = 0.0f;
      info->bottom = 0.0f;
      info->depthOffset = 0.0f;
    }
    info->left = box[0] < box[2] ? box[0] : box[2];
    info->top = box[1];
    info->depth = box[4] + 350.0f;
    info->right = box[0] > box[2] ? box[0] : box[2];
    info->bottom = box[3];
    info->depthOffset = box[4] + 350.0f;
    info->aid = this->skillAid;
    info->job = this->GetEffectiveJob(-1);
    info->kind = 2;
    info->unused = 0.0f;
    const int width = static_cast<int>(info->right - info->left);
    if (width < minimum) {
      const float pad = static_cast<float>((minimum - width) / 2);
      info->right += pad;
      info->left -= pad;
    }
    const int height = static_cast<int>(info->bottom - info->top);
    if (height < minimum) {
      const float pad = static_cast<float>((minimum - height) / 2);
      info->bottom += pad;
      info->top -= pad;
    }
    g_actorPickNode.AddPickInfo(reinterpret_cast<ActorPickInfo*>(info));
  }
  this->SetTlvert(x, y);
}

// OFFSET: 0x00E55320
// FUNCTION: fn_24304
void CSkill::sub_E55320(const char* modelName) {
  if (modelName == nullptr || this->render.enabled == 0) {
    return;
  }

  this->ReleaseModel();
  const float diffuse[3] = {0.5f, 0.7f, 0.5f};
  const float white[3] = {1.0f, 1.0f, 1.0f};
  const float ambient[3] = {0.5f, 0.5f, 0.5f};
  auto* resource = static_cast<C3dModelRes*>(ResourceMgr::Instance().Load(modelName));
  if (resource->major == 1) {
    this->m_model = new C3dActorV1;
  } else if (resource->major == 2) {
    this->m_model = new C3dActorV2;
  }

  this->m_model->CopyVolumes(resource->volumes);
  this->m_model->SetSpeed(1.0f);
  std::memcpy(this->m_model->position, &this->render.position.x, sizeof(this->m_model->position));
  this->m_model->dirty = 1;
  this->m_model->Assign(*resource);
  this->m_model->Animate(3.0);
  this->m_model->occlusionEnabled = 0;
  this->m_model->playback = 0;
  using PrepareFn = void (__thiscall*)(C3dActor*, const float*, const float*, const float*);
  const PrepareFn prepare = reinterpret_cast<PrepareFn>((*reinterpret_cast<void***>(this->m_model))[4]);
  prepare(this->m_model, diffuse, white, ambient);
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->spatial.InsertModel(*this->m_model, 0);
}

// OFFSET: 0x00E51BD0
// FUNCTION: fn_24302
void CSkill::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

  switch (static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message)) {
  case 7: {
    const char* text = reinterpret_cast<const char*>(b);
    if (std::strlen(text) == 0) {
      return;
    }

    this->unknown_0248 = timeGetTime();
    if (this->unknown_0264[0] == 0) {
      UITransBalloonText* balloon =
        new UITransBalloonText();
      this->unknown_0264[0] = reinterpret_cast<int>(balloon);
      balloon->SetSize(8, 8);
      g_windowMgr.AddRenderedFront(reinterpret_cast<UIWindow*>(this->unknown_0264[0]));
    }

    UITransBalloonText* balloon = reinterpret_cast<UITransBalloonText*>(this->unknown_0264[0]);
    balloon->SetText(text, 35);
    balloon->SetColors(0xFF8080, 0);
    this->unknown_0288 = balloon->geometry.width;
    this->unknown_028C = balloon->geometry.height;
    balloon->Move(this->render.screenX - this->unknown_0288 / 2, this->render.screenY - g_renderer->viewport.height * 110 / 480);
    return;
  }
  case 0x25: {
    const SkillHitMessage* hit = reinterpret_cast<const SkillHitMessage*>(b);
    const int skillId = hit->skillId;
    const int unknown_0014 = hit->unknown_0014;
    const int unknown_001C = hit->unknown_001C;
    const int unknown_0020 = hit->unknown_0020;
    const int motionSpeed = hit->motionSpeed;
    int hitCount = hit->hitCount;
    if (hitCount == 0) {
      hitCount = 1;
    }
    const int damage = hit->damage / hitCount;

    CRenderObject* target = nullptr;
    if (g_session.m_aid == hit->targetAid) {
      target = mode->scene->player;
      g_session.GetDisplayJob();
    } else {
      CRenderObject* actor = mode->scene->GetGameActorByAID(hit->targetAid);
      CRenderObject* skill = mode->scene->GetSkillByAID(hit->targetAid);
      if (actor != nullptr) {
        target = actor;
      } else if (skill != nullptr) {
        target = skill;
      }
      if (target != nullptr) {
        static_cast<CGameActor*>(target)->GetEffectiveJob(-1);
      }
    }

    int effectId = skillId < 493 ? 0 : -1;
    switch (skillId) {
    case 18:
      effectId = 0x31;
      break;
    case 79:
      effectId = 0x98;
      break;
    case 85:
    case 2587:
      effectId = 0x34;
      break;
    case 89:
    case 723:
      effectId = 0x33;
      break;
    case 428:
    case 429:
    case 430:
      effectId = 0x247;
      break;
    case 5217:
      effectId = 0x6EF;
      break;
    case 5219:
    case 5221:
      effectId = 0x764;
      break;
    case 5223:
    case 5224:
      effectId = 0x707;
      break;
    case 5227:
      effectId = 0x6F4;
      break;
    case 5229:
      effectId = 0x70F;
      break;
    case 5321:
    case 5370:
    case 5371:
    case 5372:
    case 8044:
      effectId = g_session.FindUnknown0700(skillId);
      break;
    case 5331:
      effectId = 0x753;
      break;
    case 5332:
      effectId = 0x75B;
      break;
    case 5333:
      effectId = 0x757;
      break;
    case 5335:
      effectId = 0x782;
      break;
    }

    if (target != nullptr) {
      ActorDamageMessage damageMessage;
      damageMessage.targetAid = this->hitTargetAid;
      damageMessage.action = 26;
      damageMessage.damage = damage;
      damageMessage.time = timeGetTime();
      damageMessage.attackerAid = this->skillAid;
      damageMessage.unknown_0014 = unknown_001C;
      damageMessage.unknown_0018 = unknown_0020;
      damageMessage.unknown_001C = effectId;
      damageMessage.effectId = -1;
      damageMessage.unknown_0024 = unknown_0014;
      damageMessage.motionSpeed = static_cast<float>(motionSpeed) / 288.0f;
      damageMessage.unknown_002C = 0;
      damageMessage.unknown_0030 = skillId;
      damageMessage.unknown_0034 = 0;

      const int address = reinterpret_cast<int>(&damageMessage);
      int total = damage;
      target->SendMsg(this, 0x1B, 0, address, address >> 31, total, total >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      for (int index = 0; index < hitCount - 1;) {
        ++index;
        damageMessage.time += g_multiHitInterval;
        total += damage;
        damageMessage.unknown_0034 = index;
        target->SendMsg(this, 0x1B, 0, address, address >> 31, total, total >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      }
    }

    if (this->m_model != nullptr) {
      this->m_model->playback = 1;
    }
    return;
  }
  case 0x2D: {
    const SkillUnitMessage* unit = reinterpret_cast<const SkillUnitMessage*>(b);
    this->range = unit->range;
    this->skillAid = unit->aid;
    this->render.accountId = unit->aid;
    this->SetActorJob(unit->type);
    this->render.enabled = unit->visible;
    if (unit->visible) {
      this->render.enabled = 1;
    } else {
      const bool visible =
        g_session.m_aid == unit->owner ||
        g_session.PartyByCharacter(unit->characterId).valid != 0;
      this->render.enabled = visible;
    }

    mode->CellCenter(unit->x, unit->y, this->render.position.x, this->render.position.z);
    this->render.position.y = mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    this->m_effectId = -1;

    const vector3d& position = this->render.position;
    switch (this->GetEffectiveJob(-1)) {
    case 0x7F:
    case 0xED: {
      const vector3d& player = mode->scene->player->render.position;
      g_soundMgr->PlayWave("effect\\EF_FireWall.wav", position.x - player.x, 0.0f, position.z - player.z, 250, 40, 1.0f, 0);
      this->render.unknown_00A4Bytes[1] = 0;
      return;
    }
    case 0x85:
      g_effectMgr->Spawn(this->render.accountId, 0x8D, position.x, position.y, position.z, 0, -1, 0, 0);
      return;
    case 0x8A:
    case 0x90:
    case 0x91:
    case 0x93:
    case 0x99:
    case 0xE5:
      this->render.unknown_00A4Bytes[1] = 1;
      this->sub_E55320(this->GetTrapModelName());
      return;
    case 0x8D:
      this->render.unknown_00A4Bytes[1] = 1;
      return;
    case 0x8F:
      this->render.unknown_00A4Bytes[1] = 1;
      this->m_effectId = 0x6A;
      this->sub_E55320(this->GetTrapModelName());
      return;
    case 0x94:
      this->render.unknown_00A4Bytes[1] = 1;
      this->sub_E55320(this->GetTrapModelName());
      if (this->render.enabled && !this->HasId(0x91)) {
        this->LaunchEffect(0x91, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      return;
    case 0x95:
      this->render.unknown_00A4Bytes[1] = 1;
      this->m_effectId = 0x8B;
      this->sub_E55320(this->GetTrapModelName());
      return;
    case 0x96:
      this->render.unknown_00A4Bytes[1] = 1;
      this->m_effectId = 0x63;
      this->sub_E55320(this->GetTrapModelName());
      return;
    case 0x97:
      this->render.unknown_00A4Bytes[1] = 1;
      this->m_effectId = 0x6C;
      this->sub_E55320(this->GetTrapModelName());
      return;
    case 0x98:
      this->render.unknown_00A4Bytes[1] = 1;
      this->m_effectId = 0x6B;
      this->sub_E55320(this->GetTrapModelName());
      return;
    case 0xBC:
    case 0xDC:
    case 0xDD:
    case 0xEB:
      this->render.unknown_00A4Bytes[1] = 0;
      return;
    case 0xBD:
      if (rand() % 8 == 0) {
        const vector3d& player = mode->scene->player->render.position;
        g_soundMgr->PlayWave("effect\\화염진.wav", position.x - player.x, 0.0f, position.z - player.z, 250, 40, 1.0f, 0);
      }
      this->render.unknown_00A4Bytes[1] = 0;
      return;
    case 0xBE:
      this->m_effectId = 0x6B;
      this->render.unknown_00A4Bytes[1] = 0;
      return;
    case 0xBF:
    case 0xC2:
    case 0x108:
      this->m_effectId = 0x63;
      this->render.unknown_00A4Bytes[1] = 0;
      return;
    case 0xC0:
      this->m_effectId = 0x8B;
      this->render.unknown_00A4Bytes[1] = 0;
      return;
    case 0xC1:
      this->m_effectId = 0x6C;
      this->render.unknown_00A4Bytes[1] = 0;
      return;
    case 0xD1: {
      CPc* source;
      if (unit->owner == g_session.m_aid) {
        source = reinterpret_cast<CPc*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
      } else {
        source = static_cast<CPc*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(this->unknown_0410));
      }
      if (source != nullptr) {
        CreateHallucinationPc(
          source->GetEffectiveJob(-1), source->unknown_0260, source->unknown_0440, &this->render.position, 180, source->render.unknown_0048,
          source->unknown_0444, source->Slot46()
        );
      }
      return;
    }
    case 0xD2:
    case 0xD3:
    case 0xD4:
    case 0xD5:
      this->m_effectId = 0x8B;
      this->render.unknown_00A4Bytes[1] = 0;
      this->sub_E55320(this->GetTrapModelName());
      return;
    case 0xD6:
    case 0xD9:
      this->m_effectId = 0x6B;
      this->render.unknown_00A4Bytes[1] = 0;
      this->sub_E55320(this->GetTrapModelName());
      return;
    case 0xD7:
      this->m_effectId = 0x6C;
      this->render.unknown_00A4Bytes[1] = 0;
      this->sub_E55320(this->GetTrapModelName());
      return;
    case 0xD8:
      this->m_effectId = 0x6B;
      this->render.unknown_00A4Bytes[1] = 0;
      this->sub_E55320(this->GetTrapModelName());
      if (!this->HasId(0x91)) {
        this->LaunchEffect(0x91, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      return;
    case 0xDA:
    case 0xDE:
      this->render.unknown_00A4Bytes[1] = 1;
      this->m_effectId = 0x1D0;
      return;
    case 0xE4:
      this->m_effectId = 0x6A;
      this->render.unknown_00A4Bytes[1] = 0;
      return;
    case 0xE6:
      this->m_effectId = 0x6A;
      this->render.unknown_00A4Bytes[1] = 1;
      return;
    case 0x101: {
      const vector3d& player = mode->scene->player->render.position;
      g_soundMgr->PlayWave("effect\\NC_MAGMA_ERUPTION.wav", position.x - player.x, 0.0f, position.z - player.z, 250, 40, 1.0f, 0);
      this->render.unknown_00A4Bytes[1] = 0;
      return;
    }
    case 0x102: {
      const vector3d& player = mode->scene->player->render.position;
      g_soundMgr->PlayWave("effect\\lg_kings_grace.wav", position.x - player.x, 0.0f, position.z - player.z, 250, 40, 1.0f, 0);
      this->render.unknown_00A4Bytes[1] = 0;
      return;
    }
    case 0x104:
      switch (unit->level) {
      case 1:
        g_effectMgr->Spawn(this->render.accountId, 0x42D, position.x, position.y, position.z, 0, -1, 0, 0);
        return;
      case 2:
      case 3:
        g_effectMgr->Spawn(this->render.accountId, 0x42E, position.x, position.y, position.z, 0, -1, 0, 0);
        return;
      case 4:
      case 5:
        g_effectMgr->Spawn(this->render.accountId, 0x42F, position.x, position.y, position.z, 0, -1, 0, 0);
        return;
      }
      return;
    case 0x106:
      g_effectMgr->Spawn(this->render.accountId, 0x450, position.x, position.y, position.z, 0, unit->level, 0, 0);
      return;
    case 0x107: {
      const vector3d& player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->render.position;
      g_soundMgr->PlayWave("effect\\su_nyanggrass.wav", position.x - player.x, 0.0f, position.z - player.z, 250, 40, 1.0f, 0);
      g_effectMgr->Spawn(this->render.accountId, 0x465, position.x, position.y, position.z, 0, unit->level, 0, 0);
      return;
    }
    case 0x109: {
      g_effectMgr->Spawn(this->render.accountId, 0x4B0, position.x, position.y, position.z, 0, -1, 0, 0);
      const vector3d& player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->render.position;
      g_soundMgr->PlayWave("effect\\g카트토네이도.wav", position.x - player.x, 0.0f, position.z - player.z, 250, 40, 1.0f, 0);
      return;
    }
    case 0x10A:
      if (!g_effectMgr->Has(this->render.accountId, 0x6D8)) {
        g_effectMgr->Retire(this->render.accountId, 0x6D8);
      }
      g_effectMgr->Spawn(this->render.accountId, 0x6D8, position.x, position.y, position.z, 0, unit->level, 0, 0);
      return;
    case 0x10B:
      if (!g_effectMgr->Has(this->render.accountId, 0x6EC)) {
        g_effectMgr->Retire(this->render.accountId, 0x6EC);
      }
      g_effectMgr->Spawn(this->render.accountId, 0x6EC, position.x, position.y, position.z, 0, unit->level, 0, 0);
      return;
    case 0x10C:
      switch (unit->level) {
      case 1:
        this->m_effectId = 0x642;
        break;
      case 2:
        this->m_effectId = 0x643;
        break;
      case 3:
        this->m_effectId = 0x644;
        break;
      case 4:
        this->m_effectId = 0x645;
        break;
      case 5:
        this->m_effectId = 0x646;
        break;
      }
      return;
    case 0x10D:
      for (std::int64_t effectId = 0x765; effectId <= 0x766; ++effectId) {
        if (!g_effectMgr->Has(this->render.accountId, static_cast<int>(effectId))) {
          g_effectMgr->Retire(this->render.accountId, static_cast<int>(effectId));
        }
        CEffect* effect = g_effectMgr->Spawn(this->render.accountId, static_cast<int>(effectId), position.x, position.y, position.z, 0, unit->level, 0, 0);
        if (effect != nullptr) {
          effect->Slot17(90.0f);
          static_cast<CEZ2STRScreenEffect*>(effect)->followCamera = 1;
        }
      }
      return;
    case 0x10E:
      g_effectMgr->Retire(this->render.accountId, 0x701);
      g_effectMgr->Spawn(this->render.accountId, 0x701, position.x, position.y, position.z, 0, unit->level, 0, 0);
      return;
    case 0x10F:
      g_effectMgr->Retire(this->render.accountId, 0x702);
      g_effectMgr->Spawn(this->render.accountId, 0x702, position.x, position.y, position.z, 0, unit->level, 0, 0);
      g_effectMgr->Retire(this->render.accountId, 0x703);
      g_effectMgr->Spawn(this->render.accountId, 0x703, position.x, position.y, position.z, 0, unit->level, 0, 0);
      return;
    case 0x110:
      if (!g_effectMgr->Has(this->render.accountId, 0x6F0)) {
        g_effectMgr->Retire(this->render.accountId, 0x6F0);
      }
      g_effectMgr->Spawn(this->render.accountId, 0x6F0, position.x, position.y, position.z, 0, unit->level, 0, 0);
      if (!g_effectMgr->Has(this->render.accountId, 0x6F1)) {
        g_effectMgr->Retire(this->render.accountId, 0x6F1);
      }
      g_effectMgr->Spawn(this->render.accountId, 0x6F1, position.x, position.y, position.z, 0, unit->level, 0, 0);
      return;
    case 0x111:
      g_effectMgr->Retire(this->render.accountId, 0x70C);
      g_effectMgr->Spawn(this->render.accountId, 0x70C, position.x, position.y, position.z, 0, unit->level, 0, 0);
      return;
    case 0x112:
      if (!g_effectMgr->Has(this->render.accountId, 0x6E8)) {
        g_effectMgr->Retire(this->render.accountId, 0x6E8);
      }
      g_effectMgr->Spawn(this->render.accountId, 0x6E8, position.x, position.y, position.z, 0, unit->level, 0, 0);
      return;
    case 0x114:
      g_effectMgr->Spawn(this->render.accountId, 0x7A5, position.x, position.y, position.z, 0, unit->level, 0, 0);
      return;
    case 0x115:
      g_effectMgr->Spawn(this->render.accountId, 0x852, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, 0x851, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      return;
    case 0x116: {
      g_effectMgr->Spawn(this->render.accountId, 0x8A8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, 0x8A9, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(this->render.accountId, 0x8AA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->Slot16(10);
      }
      effect = g_effectMgr->Spawn(this->render.accountId, 0x8AB, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->Slot16(10);
      }
      return;
    }
    case 0x117: {
      g_effectMgr->Spawn(this->render.accountId, 0x8B8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, 0x8B9, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(this->render.accountId, 0x8BA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->Slot16(10);
      }
      effect = g_effectMgr->Spawn(this->render.accountId, 0x8BB, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->Slot16(10);
      }
      return;
    }
    case 0x118: {
      g_effectMgr->Spawn(this->render.accountId, 0x8B0, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, 0x8B1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(this->render.accountId, 0x8B2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->Slot16(10);
      }
      effect = g_effectMgr->Spawn(this->render.accountId, 0x8B3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->Slot16(10);
      }
      return;
    }
    case 0x119: {
      g_effectMgr->Spawn(this->render.accountId, 0x8A0, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, 0x8A1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(this->render.accountId, 0x8A2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->Slot16(10);
      }
      effect = g_effectMgr->Spawn(this->render.accountId, 0x8A3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->Slot16(10);
      }
      return;
    }
    case 0x11A:
      if (!g_effectMgr->Has(this->render.accountId, 0x7FA)) {
        g_effectMgr->Retire(this->render.accountId, 0x7FA);
      }
      g_effectMgr->Spawn(this->render.accountId, 0x7FA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (!g_effectMgr->Has(this->render.accountId, 0x7F9)) {
        g_effectMgr->Retire(this->render.accountId, 0x7F9);
      }
      g_effectMgr->Spawn(this->render.accountId, 0x7F9, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      return;
    case 0x11B:
      if (!g_effectMgr->Has(this->render.accountId, 0x7EF)) {
        g_effectMgr->Retire(this->render.accountId, 0x7EF);
      }
      g_effectMgr->Spawn(this->render.accountId, 0x7EF, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      return;
    case 0x11C:
      if (!g_effectMgr->Has(this->render.accountId, 0x808)) {
        g_effectMgr->Retire(this->render.accountId, 0x808);
      }
      g_effectMgr->Spawn(this->render.accountId, 0x808, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (!g_effectMgr->Has(this->render.accountId, 0x807)) {
        g_effectMgr->Retire(this->render.accountId, 0x807);
      }
      g_effectMgr->Spawn(this->render.accountId, 0x807, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      return;
    case 0x11D:
      g_effectMgr->Spawn(this->render.accountId, 0x903, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      return;
    case 0x5174:
      this->render.unknown_00A4Bytes[1] = 1;
      this->sub_E55320(this->GetTrapModelName());
      if (this->render.enabled) {
        g_effectMgr->Spawn(this->render.accountId, 0x752, position.x, position.y, position.z, 0, -1, 0, 0);
      }
      return;
    case 0x5175:
      this->render.unknown_00A4Bytes[1] = 1;
      this->sub_E55320(this->GetTrapModelName());
      if (this->render.enabled) {
        g_effectMgr->Spawn(this->render.accountId, 0x75A, position.x, position.y, position.z, 0, -1, 0, 0);
      }
      return;
    case 0x5176:
      this->render.unknown_00A4Bytes[1] = 1;
      this->sub_E55320(this->GetTrapModelName());
      if (this->render.enabled) {
        g_effectMgr->Spawn(this->render.accountId, 0x756, position.x, position.y, position.z, 0, -1, 0, 0);
      }
      return;
    case 0x5177:
      this->render.unknown_00A4Bytes[1] = 1;
      this->sub_E55320(this->GetTrapModelName());
      if (this->render.enabled) {
        g_effectMgr->Spawn(this->render.accountId, 0x77F, position.x, position.y, position.z, 0, -1, 0, 0);
      }
      return;
    case 0x532C:
      g_effectMgr->Spawn(this->render.accountId, 0x925, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      return;
    default: {
      CRenderObject* source = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAidIncludeMe(this->unknown_0410);
      CSkillActionMgr* actions = CSkillActionMgr::GetInstance();
      actions->ObjectSlot00(this->GetEffectiveJob(-1), unit->level, this, static_cast<CGameActor*>(source));
      return;
    }
    }
  }
  case 0x2F:
    this->SetActorJob(b);
    switch (this->GetEffectiveJob(-1)) {
    case 0x80: {
      const vector3d& player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->render.position;
      g_soundMgr->PlayWave(
        "effect\\EF_Portal.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
      );
      break;
    }
    case 0x8C:
      if (this->m_effectId != -1 &&
          !g_effectMgr->Spawn(0, this->m_effectId, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0)) {
        CRagEffect* effect = this->LaunchEffect(this->m_effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect != nullptr) {
          effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&this->render.position), 0, 0, 0, 0);
        }
      }
      break;
    }

    this->m_effectTimer = 36000;
    this->sub_E55320(this->GetTrapModelName());
    if (this->m_model != nullptr) {
      this->m_model->playback = 1;
    }
    return;
  case 0x35:
    if (this->GetEffectiveJob(-1) == 0x11D) {
      CEffect* effect = g_effectMgr->Find(this->render.accountId, 0x903);
      if (effect != nullptr) {
        static_cast<CCaneOfEvilEyeEffect*>(effect)->Finish();
      }
    } else {
      CSkillActionMgr* actions = CSkillActionMgr::GetInstance();
      actions->ObjectSlot01(this->GetEffectiveJob(-1), this);
    }
    return;
  case 0x77:
    if (this->render.enabled) {
      return;
    }

    this->render.enabled = 1;
    this->sub_E55320(this->GetTrapModelName());
    if (this->GetEffectiveJob(-1) == 0x94 && !this->HasId(0x91)) {
      this->LaunchEffect(0x91, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }

    switch (this->GetEffectiveJob(-1)) {
    case 0x5174:
      g_effectMgr->Spawn(this->render.accountId, 0x752, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      return;
    case 0x5175:
      g_effectMgr->Spawn(this->render.accountId, 0x75A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      return;
    case 0x5176:
      g_effectMgr->Spawn(this->render.accountId, 0x756, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      return;
    case 0x5177:
      g_effectMgr->Spawn(this->render.accountId, 0x77F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      return;
    }
    return;
  default:
    this->CGameActor::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }
}
