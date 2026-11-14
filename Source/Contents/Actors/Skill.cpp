#include "Contents/Actors/Skill.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/Skills/SkillUnitId.h"
#include "Contents/Jobs/JobId.h"

#include "Contents/Actors/ActorPickNode.h"
#include "Contents/Skills/GraffiSkill.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/Skills/CaneOfEvilEyeEffect.h"
#include "Contents/Effects/Particles/EZ2STREffect.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/Models/ModelActor.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Resource/Models/ModelResource.h"
#include "Resource/ResourceMgr.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
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
  /* 0008 */ int action;
  /* 000C */ std::uint32_t targetAid;
  /* 0010 */ int startTime;
  /* 0014 */ int monsterValue;
  /* 0018 */ int hitCount;
  /* 001C */ int cellX;
  /* 0020 */ int cellY;
  /* 0024 */ int attackMotionTime;
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
  this->render.visibilityOptions[1] = 0;
  this->render.enabled = 0;
  this->actorWindows[0] = 0;
  this->m_model = nullptr;
  this->m_effect = nullptr;
  this->sourceActorId = 0;
  this->range = 0;
  this->m_hasEffect = false;
  this->m_targets.clear();
}

// OFFSET: 0x00E4AA30
// FUNCTION: fn_24259
CSkill::~CSkill() {
  if (this->actorWindows[0] != 0) {
    g_windowMgr.Schedule(this->actorWindows[0]);
    this->actorWindows[0] = 0;
  }

  this->ReleaseModel();
  for (std::vector<std::uint64_t>::iterator it = this->m_targets.begin(); it != this->m_targets.end(); ++it) {
    std::uint64_t target = *it;
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->SendObjectMessage(this, target, 24, 0, 0, 0, 0);
  }

  this->m_targets.clear();
}

// OFFSET: 0x00E4B400
// FUNCTION: fn_24268
const char* CSkill::GetTrapModelName() {
  switch (this->GetEffectiveJob(-1)) {
    case UNT_BLASTMINE:
    case UNT_REVERBERATION:
    case UNT_NETHERWORLD:
    case UNT_WALLOFTHORN:
      return "외부소품\\트랩03_3.rsm";
    case UNT_SKIDTRAP:
      return "외부소품\\트랩02.rsm";
    case UNT_THORNS_TRAP:
      return "event\\3차트랩_가시01.rsm";
    case UNT_LANDMINE:
      return "외부소품\\트랩03.rsm";
    case UNT_SHOCKWAVE:
      return "외부소품\\트랩03_6.rsm";
    case UNT_SANDMAN:
      return "외부소품\\트랩03_4.rsm";
    case UNT_FLASHER:
      return "외부소품\\트랩03_5.rsm";
    case UNT_FREEZINGTRAP:
      return "외부소품\\트랩03_2.rsm";
    case UNT_CLAYMORETRAP:
      return "외부소품\\트랩04.rsm";
    case UNT_TALKIEBOX:
      return "외부소품\\트랩05.rsm";
    case UNT_ANKLESNARE:
    case UNT_EARTHSTRAIN:
      return "외부소품\\트랩01.rsm";
    case UNT_MAGENTATRAP:
      return "event\\3차트랩_변화01.rsm";
    case UNT_COBALTTRAP:
      return "event\\3차트랩_변수01.rsm";
    case UNT_MAIZETRAP:
      return "event\\3차트랩_변지01.rsm";
    case UNT_VERDURETRAP:
      return "event\\3차트랩_변풍01.rsm";
    case UNT_FIRINGTRAP:
      return "event\\3차트랩_화01.rsm";
    case UNT_ICEBOUNDTRAP:
      return "event\\3차트랩_수01.rsm";
    case UNT_ELECTRICSHOCKER:
      return "event\\3차트랩_풍01.rsm";
    case UNT_CLUSTERBOMB:
      return "event\\3차트랩_지01.rsm";
    case UNT_DEEPBLINDTRAP:
      return "effect\\deepblindtrap\\deep_trap.rsm2";
    case UNT_SOLIDTRAP:
      return "effect\\solidtrap\\sol_trap.rsm2";
    case UNT_SWIFTTRAP:
      return "effect\\swifttrap\\swi_trap.rsm2";
    case UNT_FLAMETRAP:
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
    this->m_effect->SendMsg(nullptr, AM_MASTER_DEAD, 0, 0, 0, 0, 0, 0);
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
  if (this->render.enabled) {
    float x;
    float y;
    float reciprocalDepth;
    g_renderer->ProjectViewport(&this->render.position.x, reinterpret_cast<const float*>(argument), &x, &y, &reciprocalDepth);
    CRenderer* renderer = g_renderer;
    float scale = renderer->ScaleDistance(reciprocalDepth);
    scale /= g_heightScale;
    this->render.distanceScale = scale;

    float bounds[5];
    bounds[0] = x - (renderer->viewport.uiScaleX + renderer->viewport.uiScaleX) * scale;
    bounds[1] = y - (renderer->viewport.uiScaleY + renderer->viewport.uiScaleY) * scale;
    bounds[2] = (renderer->viewport.uiScaleX + renderer->viewport.uiScaleX) * scale + x;
    bounds[3] = y;
    bounds[4] = reciprocalDepth;
    this->SetRenderInfo(bounds, x, y);
  } else {
    this->UpdateTlvertex(reinterpret_cast<const float*>(argument));
  }
}

// OFFSET: 0x00E4B690
// FUNCTION: fn_24271
__declspec(safebuffers) void CSkill::LaunchGroundEffects(int effectId, int, int duration) {
  if (g_effectMgr->HasAllocator(effectId)) {
    g_effectMgr->Spawn(this->render.accountId, effectId, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 1, 0);
  }

  unsigned interval = EffectLifetime(effectId) - 20;
  if (duration > 0) {
    interval = duration - 20;
  }
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

// OFFSET: 0x00E55100
// FUNCTION: fn_24303
void CSkill::SetRenderInfo(const float* box, float x, float y) {
  if (this->render.visibilityOptions[1] != 0) {
    // GLOBAL: gl_25171
    static int minimum = static_cast<int>(g_renderer->UiScaleX() * 34.0f);
    ActorPickInfo* info = new ActorPickInfo;
    info->left = box[0] < box[2] ? box[0] : box[2];
    info->top = box[1];
    info->depth = box[4] + 350.0f;
    info->right = box[0] > box[2] ? box[0] : box[2];
    info->bottom = box[3];
    info->depthOffset = box[4] + 350.0f;
    info->accountId = this->skillAid;
    info->job = this->GetEffectiveJob(-1);
    info->healthState = 0;
    info->kind = 2;

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

    g_actorPickNode.AddPickInfo(info);
  }

  this->SetTlvert(x, y);
}

// OFFSET: 0x00E55320
// FUNCTION: fn_24304
__declspec(safebuffers) void CSkill::ShowModel(const char* modelName) {
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
  const vector3d position = this->render.position;
  this->m_model->SetPosition(position);
  this->m_model->Assign(*resource);
  this->m_model->Animate(3.0);
  this->m_model->occlusionEnabled = 0;
  this->m_model->playback = 0;
  this->m_model->Prepare(diffuse, white, ambient);
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->spatial.InsertModel(*this->m_model, 0);
}

// OFFSET: 0x00E51BD0
// FUNCTION: fn_24302
void CSkill::SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

  switch (message) {
  case AM_CHAT: {
    const char* text = reinterpret_cast<const char*>(static_cast<std::intptr_t>(b));
    if (std::strlen(text) == 0) {
      return;
    }

    this->chatTick = timeGetTime();
    if (this->actorWindows[0] == 0) {
      UITransBalloonText* balloon =
        new UITransBalloonText();
      this->actorWindows[0] = balloon;
      balloon->SetSize(8, 8);
      g_windowMgr.AddRenderedFront(this->actorWindows[0]);
    }

    UITransBalloonText* balloon = static_cast<UITransBalloonText*>(this->actorWindows[0]);
    balloon->SetText(text, 35);
    balloon->SetColors(0xFF8080, 0);
    this->chatWidth = balloon->geometry.width;
    this->chatHeight = balloon->geometry.height;
    balloon->Move(this->render.screenX - this->chatWidth / 2, this->render.screenY - g_renderer->viewport.height * 110 / 480);
    return;
  }
  case AM_SKILL: {
    const SkillHitMessage* hit = reinterpret_cast<const SkillHitMessage*>(static_cast<std::intptr_t>(b));
    const int skillId = hit->skillId;
    const int monsterValue = hit->monsterValue;
    const int cellX = hit->cellX;
    const int cellY = hit->cellY;
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
    case MG_FIREWALL:
      effectId = 0x31;
      break;
    case PR_MAGNUS:
      effectId = 0x98;
      break;
    case WZ_VERMILION:
    case SJ_BOOKOFCREATINGSTAR:
      effectId = 0x34;
      break;
    case WZ_STORMGUST:
    case NPC_STORMGUST2:
      effectId = 0x33;
      break;
    case SG_SUN_WARM:
    case SG_MOON_WARM:
    case SG_STAR_WARM:
      effectId = 0x247;
      break;
    case AG_MYSTERY_ILLUSION:
      effectId = 0x6EF;
      break;
    case AG_VIOLENT_QUAKE_ATK:
    case AG_STRANTUM_TREMOR:
      effectId = 0x764;
      break;
    case AG_ALL_BLOOM_ATK:
    case AG_ALL_BLOOM_ATK2:
      effectId = 0x707;
      break;
    case AG_TORNADO_STORM:
      effectId = 0x6F4;
      break;
    case AG_FLORAL_FLARE_ROAD:
      effectId = 0x70F;
      break;
    case ABC_ABYSS_SQUARE:
    case EM_LIGHTNING_LAND:
    case EM_VENOM_SWAMP:
    case EM_CONFLAGRATION:
    case MH_BLAST_FORGE:
      effectId = g_session.FindSkillFollowEffect(skillId);
      break;
    case WH_DEEPBLINDTRAP:
      effectId = 0x753;
      break;
    case WH_SOLIDTRAP:
      effectId = 0x75B;
      break;
    case WH_SWIFTTRAP:
      effectId = 0x757;
      break;
    case WH_FLAMETRAP:
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
      damageMessage.cellX = cellX;
      damageMessage.cellY = cellY;
      damageMessage.hitEffect = effectId;
      damageMessage.effectId = -1;
      damageMessage.monsterValue = monsterValue;
      damageMessage.motionSpeed = static_cast<float>(motionSpeed) / 288.0f;
      damageMessage.blueDamage = 0;
      damageMessage.skillId = skillId;
      damageMessage.skillTarget = 0;

      const int address = reinterpret_cast<int>(&damageMessage);
      int total = damage;
      target->SendMsg(this, AM_WILL_BE_ATTACKED, address, total, 0, 0, 0, 0);
      for (int index = 0; index < hitCount - 1;) {
        ++index;
        damageMessage.time += g_multiHitInterval;
        total += damage;
        damageMessage.skillTarget = index;
        target->SendMsg(this, AM_WILL_BE_ATTACKED, address, total, 0, 0, 0, 0);
      }
    }

    if (this->m_model != nullptr) {
      this->m_model->playback = 1;
    }
    return;
  }
  case AM_SKILLSTANDENTRY: {
    const SkillUnitMessage* unit = reinterpret_cast<const SkillUnitMessage*>(static_cast<std::intptr_t>(b));
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
    case UNT_ICEWALL:
      this->render.visibilityOptions[1] = 1;
      return;
    case UNT_FIREWALL:
    case UNT_FIRE_MANTLE: {
      const vector3d& player = mode->scene->player->render.position;
      g_soundMgr->PlayWave("effect\\EF_FireWall.wav", position.x - player.x, 0.0f, position.z - player.z, 250, 40, 1.0f, 0);
      this->render.visibilityOptions[1] = 0;
      return;
    }
    case UNT_TATAMIGAESHI:
    case UNT_FIREWALK:
    case UNT_ELECTRICWALK:
    case UNT_VACUUM_EXTREME:
      this->render.visibilityOptions[1] = 0;
      return;
    case UNT_GROUNDDRIFT_WIND:
      this->m_effectId = 0x6B;
      this->render.visibilityOptions[1] = 0;
      return;
    case UNT_WALLOFTHORN:
      this->m_effectId = 0x6A;
      this->render.visibilityOptions[1] = 1;
      return;
    case UNT_WARMER:
      this->m_effectId = 0x6A;
      this->render.visibilityOptions[1] = 0;
      return;
    case UNT_GROUNDDRIFT_DARK:
    case UNT_GROUNDDRIFT_FIRE:
    case UNT_GROUNDDRIFT_NEUTRAL:
      this->m_effectId = 0x63;
      this->render.visibilityOptions[1] = 0;
      return;
    case UNT_GROUNDDRIFT_POISON:
      this->m_effectId = 0x8B;
      this->render.visibilityOptions[1] = 0;
      return;
    case UNT_MAGENTATRAP:
    case UNT_COBALTTRAP:
    case UNT_MAIZETRAP:
    case UNT_VERDURETRAP:
      this->m_effectId = 0x8B;
      this->render.visibilityOptions[1] = 0;
      this->ShowModel(this->GetTrapModelName());
      return;
    case UNT_ELECTRICSHOCKER:
      this->m_effectId = 0x6B;
      this->render.visibilityOptions[1] = 0;
      this->ShowModel(this->GetTrapModelName());
      if (!this->HasId(EF_SHOCKWAVE)) {
        this->LaunchEffect(EF_SHOCKWAVE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      return;
    case UNT_FIRINGTRAP:
    case UNT_CLUSTERBOMB:
      this->m_effectId = 0x6B;
      this->render.visibilityOptions[1] = 0;
      this->ShowModel(this->GetTrapModelName());
      return;
    case UNT_ICEBOUNDTRAP:
      this->m_effectId = 0x6C;
      this->render.visibilityOptions[1] = 0;
      this->ShowModel(this->GetTrapModelName());
      return;
    case UNT_GROUNDDRIFT_WATER:
      this->m_effectId = 0x6C;
      this->render.visibilityOptions[1] = 0;
      return;
    case UNT_KAEN:
      if (rand() % 8 == 0) {
        const vector3d& player = mode->scene->player->render.position;
        g_soundMgr->PlayWave("effect\\화염진.wav", position.x - player.x, 0.0f, position.z - player.z, 250, 40, 1.0f, 0);
      }
      this->render.visibilityOptions[1] = 0;
      return;
    case UNT_TRAP:
      this->render.visibilityOptions[1] = 1;
      this->ShowModel(this->GetTrapModelName());
      return;
    case UNT_BLASTMINE:
      this->render.visibilityOptions[1] = 1;
      this->m_effectId = 0x6A;
      this->ShowModel(this->GetTrapModelName());
      return;
    case UNT_REVERBERATION:
    case UNT_NETHERWORLD:
      this->render.visibilityOptions[1] = 1;
      this->m_effectId = 0x1D0;
      return;
    case UNT_SKIDTRAP:
    case UNT_ANKLESNARE:
    case UNT_LANDMINE:
    case UNT_TALKIEBOX:
    case UNT_THORNS_TRAP:
      this->render.visibilityOptions[1] = 1;
      this->ShowModel(this->GetTrapModelName());
      return;
    case UNT_SANDMAN:
      this->render.visibilityOptions[1] = 1;
      this->m_effectId = 0x8B;
      this->ShowModel(this->GetTrapModelName());
      return;
    case UNT_FLASHER:
      this->render.visibilityOptions[1] = 1;
      this->m_effectId = 0x63;
      this->ShowModel(this->GetTrapModelName());
      return;
    case UNT_FREEZINGTRAP:
      this->render.visibilityOptions[1] = 1;
      this->m_effectId = 0x6C;
      this->ShowModel(this->GetTrapModelName());
      return;
    case UNT_CLAYMORETRAP:
      this->render.visibilityOptions[1] = 1;
      this->m_effectId = 0x6B;
      this->ShowModel(this->GetTrapModelName());
      return;
    case UNT_SHOCKWAVE:
      this->render.visibilityOptions[1] = 1;
      this->ShowModel(this->GetTrapModelName());
      if (this->render.enabled && !this->HasId(EF_SHOCKWAVE)) {
        this->LaunchEffect(EF_SHOCKWAVE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      return;
    case UNT_FEINTBOMB: {
      CPc* source;
      if (unit->owner == g_session.m_aid) {
        source = static_cast<CPc*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
      } else {
        source = static_cast<CPc*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(this->sourceActorId));
      }
      if (source != nullptr) {
        CreateHallucinationPc(
          source->GetEffectiveJob(-1), source->sex, source->head, &this->render.position, 180, source->render.bodyPalette,
          source->headPalette, source->GetBodyCostume()
        );
      }
      return;
    }
    case UNT_KINGS_GRACE: {
      const vector3d& player = mode->scene->player->render.position;
      g_soundMgr->PlayWave("effect\\lg_kings_grace.wav", position.x - player.x, 0.0f, position.z - player.z, 250, 40, 1.0f, 0);
      this->render.visibilityOptions[1] = 0;
      return;
    }
    case UNT_MAGMA_ERUPTION: {
      const vector3d& player = mode->scene->player->render.position;
      g_soundMgr->PlayWave("effect\\NC_MAGMA_ERUPTION.wav", position.x - player.x, 0.0f, position.z - player.z, 250, 40, 1.0f, 0);
      this->render.visibilityOptions[1] = 0;
      return;
    }
    case UNT_PNEUMA:
      g_effectMgr->Spawn(this->render.accountId, EF_PNEUMA, position, 0, -1, 0, 0);
      return;
    case UNT_B_TRAP:
      switch (unit->level) {
      case 1:
        g_effectMgr->Spawn(this->render.accountId, EF_BIND_TRAP1, position, 0, -1, 0, 0);
        return;
      case 2:
      case 3:
        g_effectMgr->Spawn(this->render.accountId, EF_BIND_TRAP2, position, 0, -1, 0, 0);
        return;
      case 4:
      case 5:
        g_effectMgr->Spawn(this->render.accountId, EF_BIND_TRAP3, position, 0, -1, 0, 0);
        return;
      }
      return;
    case UNT_CATNIPPOWDER:
      g_effectMgr->Spawn(this->render.accountId, EF_SU_CN_POWDERING, position, 0, unit->level, 0, 0);
      return;
    case UNT_NYANGGRASS: {
      const vector3d& player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->render.position;
      g_soundMgr->PlayWave("effect\\su_nyanggrass.wav", position.x - player.x, 0.0f, position.z - player.z, 250, 40, 1.0f, 0);
      g_effectMgr->Spawn(this->render.accountId, EF_SU_LOPE, position, 0, unit->level, 0, 0);
      return;
    }
    case UNT_CREATINGSTAR: {
      g_effectMgr->Spawn(this->render.accountId, EF_THUNDER_PLAZMA_BLAST, position, 0, -1, 0, 0);
      const vector3d& player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->render.position;
      g_soundMgr->PlayWave("effect\\g카트토네이도.wav", position.x - player.x, 0.0f, position.z - player.z, 250, 40, 1.0f, 0);
      return;
    }
    case UNT_SEEDTRAP:
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
    case UNT_PNEUMATICUS_PROCELLA:
      g_effectMgr->Spawn(this->render.accountId, EF_PNEUMATICUS_PROCELLA, position, 0, unit->level, 0, 0);
      return;
    case UNT_RAIN_OF_CRYSTAL:
      if (!g_effectMgr->Has(this->render.accountId, 0x6D8)) {
        g_effectMgr->Retire(this->render.accountId, 0x6D8);
      }
      g_effectMgr->Spawn(this->render.accountId, 0x6D8, position, 0, unit->level, 0, 0);
      return;
    case UNT_VIOLENT_QUAKE:
      g_effectMgr->Retire(this->render.accountId, 0x701);
      g_effectMgr->Spawn(this->render.accountId, EF_VIOLENT_QUAKE_BOTTOM, position, 0, unit->level, 0, 0);
      return;
    case UNT_ASTRAL_STRIKE:
      if (!g_effectMgr->Has(this->render.accountId, 0x6E8)) {
        g_effectMgr->Retire(this->render.accountId, 0x6E8);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_ASTRAL_STRIKE, position, 0, unit->level, 0, 0);
      return;
    case UNT_MYSTERY_ILLUSION:
      if (!g_effectMgr->Has(this->render.accountId, 0x6EC)) {
        g_effectMgr->Retire(this->render.accountId, 0x6EC);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_MYSTERY_ILLUSION_BOTTOM, position, 0, unit->level, 0, 0);
      return;
    case UNT_TORNADO_STORM:
      if (!g_effectMgr->Has(this->render.accountId, 0x6F0)) {
        g_effectMgr->Retire(this->render.accountId, 0x6F0);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_TORNADO_STORM, position, 0, unit->level, 0, 0);
      if (!g_effectMgr->Has(this->render.accountId, 0x6F1)) {
        g_effectMgr->Retire(this->render.accountId, 0x6F1);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_TORNADO_STORM_BOTTOM, position, 0, unit->level, 0, 0);
      return;
    case UNT_ALL_BLOOM:
      g_effectMgr->Retire(this->render.accountId, 0x702);
      g_effectMgr->Spawn(this->render.accountId, EF_ALL_BLOOM, position, 0, unit->level, 0, 0);
      g_effectMgr->Retire(this->render.accountId, 0x703);
      g_effectMgr->Spawn(this->render.accountId, EF_ALL_BLOOM_BOTTOM, position, 0, unit->level, 0, 0);
      return;
    case UNT_FLORAL_FLARE_ROAD:
      g_effectMgr->Retire(this->render.accountId, 0x70C);
      g_effectMgr->Spawn(this->render.accountId, EF_FLORAL_FLARE_ROAD, position, 0, unit->level, 0, 0);
      return;
    case UNT_STRANTUM_TREMOR:
      for (std::int64_t effectId = 0x765; effectId <= 0x766; ++effectId) {
        if (!g_effectMgr->Has(this->render.accountId, static_cast<int>(effectId))) {
          g_effectMgr->Retire(this->render.accountId, static_cast<int>(effectId));
        }
        CEffect* effect = g_effectMgr->Spawn(this->render.accountId, static_cast<int>(effectId), position, 0, unit->level, 0, 0);
        if (effect != nullptr) {
          effect->SetAngle(90.0f);
          static_cast<CEZ2STRScreenEffect*>(effect)->followCamera = 1;
        }
      }
      return;
    case UNT_VENOM_SWAMP:
      if (!g_effectMgr->Has(this->render.accountId, 0x7EF)) {
        g_effectMgr->Retire(this->render.accountId, 0x7EF);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_VENOM_SWAMP_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      return;
    case UNT_LIGHTNING_LAND:
      if (!g_effectMgr->Has(this->render.accountId, 0x7FA)) {
        g_effectMgr->Retire(this->render.accountId, 0x7FA);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_LIGHTNING_LAND_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (!g_effectMgr->Has(this->render.accountId, 0x7F9)) {
        g_effectMgr->Retire(this->render.accountId, 0x7F9);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_LIGHTNING_LAND, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      return;
    case UNT_CONFLAGRATION:
      if (!g_effectMgr->Has(this->render.accountId, 0x808)) {
        g_effectMgr->Retire(this->render.accountId, 0x808);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_CONFLAGRATION_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (!g_effectMgr->Has(this->render.accountId, 0x807)) {
        g_effectMgr->Retire(this->render.accountId, 0x807);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_CONFLAGRATION, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      return;
    case UNT_ABYSS_SQUARE:
      g_effectMgr->Spawn(this->render.accountId, EF_ABYSS_SQUARE_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, EF_ABYSS_SQUARE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      return;
    case UNT_ACIDIFIED_ZONE_FIRE: {
      g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_FIRE_START, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_FIRE_START_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_FIRE_LOOP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->SetDelay(10);
      }
      effect = g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_FIRE_LOOP_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->SetDelay(10);
      }
      return;
    }
    case UNT_ACIDIFIED_ZONE_WATER: {
      g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_WATER_START, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_WATER_START_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_WATER_LOOP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->SetDelay(10);
      }
      effect = g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_WATER_LOOP_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->SetDelay(10);
      }
      return;
    }
    case UNT_ACIDIFIED_ZONE_WIND: {
      g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_WIND_START, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_WIND_START_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_WIND_LOOP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->SetDelay(10);
      }
      effect = g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_WIND_LOOP_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->SetDelay(10);
      }
      return;
    }
    case UNT_ACIDIFIED_ZONE_GROUND: {
      g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_GROUND_START, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_GROUND_START_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_GROUND_LOOP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->SetDelay(10);
      }
      effect = g_effectMgr->Spawn(this->render.accountId, EF_ACIDIFIED_ZONE_GROUND_LOOP_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect != nullptr) {
        effect->SetDelay(10);
      }
      return;
    }
    case UNT_CANE_OF_EVIL_EYE:
      g_effectMgr->Spawn(this->render.accountId, 0x903, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      return;
    case UNT_DEEPBLINDTRAP:
      this->render.visibilityOptions[1] = 1;
      this->ShowModel(this->GetTrapModelName());
      if (this->render.enabled) {
        g_effectMgr->Spawn(this->render.accountId, EF_DEEP_BLIND_TRAP, position, 0, -1, 0, 0);
      }
      return;
    case UNT_SWIFTTRAP:
      this->render.visibilityOptions[1] = 1;
      this->ShowModel(this->GetTrapModelName());
      if (this->render.enabled) {
        g_effectMgr->Spawn(this->render.accountId, EF_SWIFT_TRAP, position, 0, -1, 0, 0);
      }
      return;
    case UNT_SOLIDTRAP:
      this->render.visibilityOptions[1] = 1;
      this->ShowModel(this->GetTrapModelName());
      if (this->render.enabled) {
        g_effectMgr->Spawn(this->render.accountId, EF_SOLID_TRAP, position, 0, -1, 0, 0);
      }
      return;
    case UNT_FLAMETRAP:
      this->render.visibilityOptions[1] = 1;
      this->ShowModel(this->GetTrapModelName());
      if (this->render.enabled) {
        g_effectMgr->Spawn(this->render.accountId, EF_FLAME_TRAP, position, 0, -1, 0, 0);
      }
      return;
    default: {
      CGameActor* source = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAidIncludeMe(this->sourceActorId);
      CSkillActionMgr* actions = CSkillActionMgr::GetInstance();
      actions->OnSkillObjectCreate(this->GetEffectiveJob(-1), unit->level, this, source);
      return;
    }
    case JT_EP18_ARMED_VILLAGER01:
      g_effectMgr->Spawn(this->render.accountId, EF_BLAST_FORGE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      return;
    }
  }
  case AM_SETJOB:
    this->SetActorJob(static_cast<int>(b));
    switch (this->GetEffectiveJob(-1)) {
    case UNT_WARP_WAITING: {
      const vector3d& player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->render.position;
      g_soundMgr->PlayWave(
        "effect\\EF_Portal.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0
      );
      break;
    }
    case UNT_USED_TRAPS:
      if (this->m_effectId != -1 &&
          !g_effectMgr->Spawn(0, this->m_effectId, this->render.position, 0, -1, 0, 0)) {
        CRagEffect* effect = this->LaunchEffect(this->m_effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect != nullptr) {
          effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&this->render.position), 0, 0, 0, 0);
        }
      }
      break;
    }

    this->m_effectTimer = 36000;
    this->ShowModel(this->GetTrapModelName());
    if (this->m_model != nullptr) {
      this->m_model->playback = 1;
    }
    return;
  case AM_DISAPPEAR:
    if (this->GetEffectiveJob(-1) == UNT_CANE_OF_EVIL_EYE) {
      CEffect* effect = g_effectMgr->Find(this->render.accountId, 0x903);
      if (effect != nullptr) {
        static_cast<CCaneOfEvilEyeEffect*>(effect)->Finish();
      }
    } else {
      CSkillActionMgr* actions = CSkillActionMgr::GetInstance();
      actions->OnSkillObjectRemove(this->GetEffectiveJob(-1), this);
    }
    return;
  case AM_SKILL_UPDATE:
    if (this->render.enabled) {
      return;
    }

    this->render.enabled = 1;
    this->ShowModel(this->GetTrapModelName());
    if (this->GetEffectiveJob(-1) == UNT_SHOCKWAVE && !this->HasId(EF_SHOCKWAVE)) {
      this->LaunchEffect(EF_SHOCKWAVE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }

    switch (this->GetEffectiveJob(-1)) {
    case UNT_DEEPBLINDTRAP:
      g_effectMgr->Spawn(this->render.accountId, EF_DEEP_BLIND_TRAP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      return;
    case UNT_SOLIDTRAP:
      g_effectMgr->Spawn(this->render.accountId, EF_SOLID_TRAP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      return;
    case UNT_SWIFTTRAP:
      g_effectMgr->Spawn(this->render.accountId, EF_SWIFT_TRAP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      return;
    case UNT_FLAMETRAP:
      g_effectMgr->Spawn(this->render.accountId, EF_FLAME_TRAP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      return;
    }
    return;
  default:
    this->CGameActor::SendMsg(sender, message, b, c, d, e, 0, 0);
    return;
  }
}
