#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Actors/EffectActors.h"
#include "Resource/ResourceMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Actors/Skill.h"
#include "Contents/Actors/Item.h"
#include "Contents/Actors/GameActor.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/World/WorldObjects.h"
#include "Contents/World/World.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/CashEmotionMgr.h"
#include "Contents/Effects/MandraAttackEffect.h"
#include "Contents/Effects/MsgEffect.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "UI/Window/UIEmblemWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/HexString.h"

#include <cstdlib>
#include <new>
#include <string>
#include <vector>

// OFFSET: 0x00AC8220
// FUNCTION: fn_17667
CRenderObject* CWorld::GetGameActorByAidIncludeMe(std::uint32_t aid) const {
  if (aid == g_session.m_aid) {
    return reinterpret_cast<CRenderObject*>(this->player);
  }

  for (CRenderObject* actor : this->actors) {
    if (actor->render.accountId == aid) {
      return actor;
    }
  }

  return nullptr;
}

// OFFSET: 0x00ACA4E0
// FUNCTION: fn_17692
void WorldObjects::Remove(CRenderObject* object) {
  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    if (*entry == object) {
      delete object;
      this->objects.erase(entry);
      return;
    }
  }
}

// GLOBAL: gl_24483
EffectLists* g_effectMgr;
// GLOBAL: gl_24692
std::uint32_t g_worldOverlayColor;
// GLOBAL: gl_24695
float g_fogDensity;

// OFFSET: 0x00ACA320
// FUNCTION: fn_17691
void CWorld::Clear() {
  auto entry = this->objects.begin();

  while (entry != this->objects.end()) {
    delete *entry;
    entry = this->objects.erase(entry);
  }

  this->player = nullptr;
  this->actors.clear();
  this->unknown_0018.clear();
  this->unknown_0020.clear();
  g_effectMgr->Clear();
  this->pendingActorIds.clear();
  this->actorCells.clear();
  this->unindexedActorIds.clear();
}

// OFFSET: 0x00ACA980
// FUNCTION: fn_17698
void WorldObjects::SendObjectMessage(CGameObject* sender, std::uint64_t identity, int message, int a, int b, int c, int d) {
  if (this == nullptr) {
    return;
  }

  if (identity == 0) {
    return;
  }

  for (CRenderObject* object : this->objects) {
    if (object->identity == identity) {
      object->SendMsg(sender, message, message >> 31, a, a >> 31, b, b >> 31, c, c >> 31, d, d >> 31, 0, 0, 0, 0);
      return;
    }
  }
}

// OFFSET: 0x00CD8740
// FUNCTION: fn_21301
CCorpse* CGameMode::SpawnCorpse(float x, float y, float z, const char* action, const char* sprite, int animation, int motion, float angle) {
  auto* corpse = new (std::nothrow) CCorpse;

  if (corpse) {
    this->scene->objects.push_back(corpse);
    corpse->render.position.x = x;
    corpse->render.position.y = y;
    corpse->render.position.z = z;
    corpse->SetActionName(ResourceMgr::Instance(), action);
    corpse->SetSpriteName(ResourceMgr::Instance(), sprite);
    corpse->render.previousAnimation = animation;
    corpse->render.animation = animation;
    corpse->render.curMotion = motion;
    corpse->render.angle = angle;
  }

  return corpse;
}

// OFFSET: 0x00AC8350
// FUNCTION: fn_17669
CItem* CWorld::GetItemByAID(std::uint32_t aid) const {
  for (CRenderObject* object : this->unknown_0018) {
    CItem* item = static_cast<CItem*>(object);

    if (item->itemAid == aid) {
      return item;
    }
  }

  return nullptr;
}

// OFFSET: 0x00AC8380
// FUNCTION: fn_17670
CRenderObject* CWorld::GetActorByGID(int gid) const {
  auto it = this->actors.begin();

  for (; it != this->actors.end(); ++it) {
    CGameActor* actor = static_cast<CGameActor*>(*it);

    if (!actor->unknown_0314 && gid == actor->Slot42()) {
      break;
    }
  }

  if (it == this->actors.end()) {
    return nullptr;
  }

  return *it;
}

// OFFSET: 0x00AC8440
// FUNCTION: fn_17671
CRenderObject* CWorld::GetSkillByAID(std::uint32_t aid) const {
  for (CRenderObject* object : this->unknown_0020) {
    CSkill* skill = static_cast<CSkill*>(object);

    if (skill->skillAid == aid) {
      return skill;
    }
  }

  return nullptr;
}

// OFFSET: 0x00B3C350
// FUNCTION: fn_18807
CRenderObject* FindRenderObject(std::uint32_t aid) {
  CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;

  if (g_session.m_aid == aid) {
    return reinterpret_cast<CRenderObject*>(scene->player);
  }

  CRenderObject* actor = scene->GetGameActorByAID(aid);

  if (!actor) {
    actor = scene->GetSkillByAID(aid);
  }

  return actor;
}

// OFFSET: 0x00CD76A0
// FUNCTION: fn_21296
bool CGameMode::LaunchTargetEffect(int effectId, CGameActor* source, CGameActor* target, int argument) {
  if (!source || !target) {
    return false;
  }

  if (g_effectMgr->HasAllocator(effectId)) {
    CEffect* effect = g_effectMgr->Spawn(source->render.accountId, effectId, source->render.position.x, source->render.position.y,
                                         source->render.position.z, target->render.accountId, -1, 0, 0);
    if (!effect) {
      return false;
    }

    if (argument != -1) {
      effect->unknown_0030 = argument;
    }

    if (effectId == 539) {
      g_effectMgr->Spawn(source->render.accountId, 980, source->render.position.x, source->render.position.y, source->render.position.z,
                         target->render.accountId, -1, 0, 0);
    }
    return true;
  }

  CRagEffect* effect = source->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  if (!effect) {
    return false;
  }

  vector3d position = target->render.position;
  effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
  if (argument != -1) {
    effect->SendMsg(nullptr, 44, 0, argument, 0, 0, 0, 0);
  }
  return true;
}

// OFFSET: 0x00CD8820
// FUNCTION: fn_21302
void CGameMode::ShowGuildEmblem(CGameActor* actor, int guildId, int unused) {
  if (!actor || !guildId || actor->unknown_0188 != 0) {
    return;
  }

  if (IsAdminAccount(actor->render.accountId)) {
    return;
  }

  if (!IsAdminAccount(g_session.m_aid)) {
    if (StateFlag02(actor->Slot13()) || StateFlag01(actor->Slot13()) || StateFlag06(actor->Slot13())) {
      return;
    }
  }

  UIEmblemWnd* window = new UIEmblemWnd;
  window->SendMsg(nullptr, 34, actor->render.accountId, guildId, 0, 0);
  g_windowMgr.AddRenderedFront(window);
  actor->unknown_0188 = reinterpret_cast<int>(window);
  this->unknown_0604.push_back(window);
}

// OFFSET: 0x00CD8950
// FUNCTION: fn_21303
CMsgEffect* CGameMode::SpawnCashEmotion(int aid, int packId, int emotionId, std::uint32_t duration) {
  CashEmotionData* data = g_cashEmotionMgr->GetEmotionData(packId, emotionId);
  CMsgEffect* effect = nullptr;

  if (data) {
    std::string sprite;
    std::string pack;
    sprite = g_cashEmotionMgr->GetSprFileName(data->actId);
    pack = g_cashEmotionMgr->GetPackName(data->sprId);

    effect = new (std::nothrow) CMsgEffect;
    if (effect) {
      if (this->scene) {
        this->scene->objects.push_back(effect);
      }

      effect->SendMsg(nullptr, 22, 0, duration, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      effect->SendMsg(nullptr, 14, 0, aid, aid >> 31, reinterpret_cast<int>(&sprite), reinterpret_cast<int>(&sprite) >> 31, data->unknown_0020,
                      data->unknown_0020 >> 31, 0, 0, 0, 0, 0, 0);
    }
  }

  return effect;
}

// OFFSET: 0x00CD8C10
// FUNCTION: fn_21304
void CGameMode::ApplyMapFog(int enabled) {
  if (!enabled) {
    g_renderer->SetFog(0);
    return;
  }

  const std::vector<std::string>* parameters = g_session.GetFogParameters(this->mapName, &enabled);
  if (!enabled) {
    g_renderer->SetFog(0);
    return;
  }

  g_depthStart = static_cast<float>(std::atof((*parameters)[0].c_str()));
  g_depthEnd = static_cast<float>(std::atof((*parameters)[1].c_str()));
  g_worldOverlayColor = ParseHexString((*parameters)[2].c_str());
  g_fogDensity = static_cast<float>(std::atof((*parameters)[3].c_str()));
  g_renderer->SetFog(1);
}

// OFFSET: 0x00CD8F30
// FUNCTION: fn_21306
void CGameMode::SpawnMandraAttack(std::uint32_t aid, int target, char flag) {
  CMandraAttackEffect* effect = new CMandraAttackEffect;

  if (effect) {
    this->scene->objects.push_back(effect);
    effect->SendMsg(nullptr, 14, 0, aid, 0, reinterpret_cast<int>(&flag), reinterpret_cast<int>(&flag) >> 31, target, target >> 31, 0, 0, 0, 0, 0, 0);
  }
}
