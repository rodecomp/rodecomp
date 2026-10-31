#include "Contents/Actors/Skill.h"

#include "Contents/Actors/GraffiSkill.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Models/ModelActor.h"
#include "Contents/World/World.h"
#include "Contents/World/WorldObjects.h"
#include "Render/Renderer.h"
#include "Render/SpriteGeometry.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdlib>

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
    reinterpret_cast<WorldObjects*>(mode->scene)->SendObjectMessage(this, target, 24, 0, 0, 0, 0);
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
      float y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Grid().Height(x, z);
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
