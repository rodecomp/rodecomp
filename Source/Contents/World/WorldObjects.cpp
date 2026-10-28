#include "Contents/Actors/RenderObject.h"
#include "Contents/World/WorldActors.h"
#include "Contents/World/WorldObjects.h"
#include "Contents/World/WorldScene.h"
#include "Contents/Effects/EffectLists.h"

// OFFSET: 0x00AC8220
// FUNCTION: fn_17667
CRenderObject* WorldActors::Lookup(std::uint32_t accountId) const {
  if (accountId == this->localAccountId) {
    return this->localPlayer;
  }

  for (CRenderObject* actor : this->actors) {
    if (actor->render.accountId == accountId) {
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
