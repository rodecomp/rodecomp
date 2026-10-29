#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Actors/EffectActors.h"
#include "Resource/ResourceMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Actors/Skill.h"
#include "Client/Mode/ActiveGameMode.h"
#include "Contents/World/WorldObjects.h"
#include "Contents/World/WorldScene.h"
#include "Contents/Effects/EffectLists.h"

#include <new>

// OFFSET: 0x00AC8220
// FUNCTION: fn_17667
CRenderObject* WorldScene::GetGameActorByAidIncludeMe(std::uint32_t aid) const {
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

// OFFSET: 0x00ACA320
// FUNCTION: fn_17691
void WorldScene::Clear() {
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
      object->SendObjectMessage(sender, message, message >> 31, a, a >> 31, b, b >> 31, c, c >> 31, d, d >> 31, 0, 0, 0, 0);
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

// OFFSET: 0x00AC8440
// FUNCTION: fn_17671
CRenderObject* WorldScene::GetSkillByAID(std::uint32_t aid) const {
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
  WorldScene* scene = static_cast<CGameMode*>(g_world.ActiveGameMode())->scene;

  if (g_session.m_aid == aid) {
    return reinterpret_cast<CRenderObject*>(scene->player);
  }

  CRenderObject* actor = scene->GetGameActorByAID(aid);

  if (!actor) {
    actor = scene->GetSkillByAID(aid);
  }

  return actor;
}
