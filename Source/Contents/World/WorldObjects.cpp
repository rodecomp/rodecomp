#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/ActorEnums.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Mode/GameModeEnums.h"
#include "Client/Session.h"
#include "Contents/Actors/EffectActors.h"
#include "Resource/ResourceMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Actors/Skill.h"
#include "Contents/Actors/Item.h"
#include "Contents/Actors/GameActor.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/World/World.h"
#include "Contents/Effects/Skills/ArrowEffect.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Effects/Skills/MandraAttackEffect.h"
#include "Contents/Effects/Core/MsgEffect.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "UI/Window/UIEmblemWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/HexString.h"
#include "Contents/World/WorldModelLoading.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Models/ModelActor.h"
#include "Render/Textures/TextureCache.h"
#include "Contents/Movies/BinkMovies.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Actors/Pc.h"
#include "ActorPickNode.h"
#include "Client/Input/Mouse.h"
#include "Client/Replay.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Guild/GuildMgr.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "File/ShortcutFilename.h"
#include "Contents/Clan/ClanInfo.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Text/UINameBalloonText.h"

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <string>
#include <vector>
#include "Contents/Guild/GuildInfo.h"

#include "Resource/MSGSTRINGID.h"
// GLOBAL: gl_24371
HANDLE hObject;
// GLOBAL: gl_24757
std::vector<DeferredWorldModel> g_deferredWorldModels;
// OFFSET: 0x00AC8220
// FUNCTION: fn_17667
CRenderObject* CWorld::GetGameActorByAidIncludeMe(std::uint32_t aid) const {
  if (aid == g_session.m_aid) {
    return reinterpret_cast<CRenderObject*>(this->player);
  }

  for (auto it = this->actors.begin(); it != this->actors.end(); ++it) {
    CRenderObject* actor = *it;

    if (actor->render.accountId == aid) {
      return actor;
    }
  }

  return nullptr;
}

// OFFSET: 0x00ACA4E0
// FUNCTION: fn_17692
__declspec(noinline) void CWorld::RemoveObject(CRenderObject* object) {
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
// GLOBAL: gl_24693
std::uint32_t g_fogVertexMode = 0xFFFFFFFF;
// GLOBAL: gl_24692
std::uint32_t g_worldOverlayColor;
// GLOBAL: gl_24694
std::uint32_t g_fogRange = 0xFFFFFFFF;
// GLOBAL: gl_24695
float g_fogDensity;

// OFFSET: 0x00AC9CA0
// FUNCTION: fn_17685
void CWorld::OnExitFrame() {
  if (hObject) {
    WaitForSingleObject(hObject, INFINITE);
    CloseHandle(hObject);
    hObject = nullptr;
  }

  g_deferredWorldModels.clear();
  this->pending = 0;
  this->spatial.ClearChildren();
  this->Clear();

  for (auto model = this->models.begin(); model != this->models.end(); ++model) {
    delete *model;
  }

  this->models.erase(this->models.begin(), this->models.end());
  delete this->ground;
  this->ground = nullptr;
  ResourceMgr::Instance().ClearExpired();
  g_texMgr.ClearUnused();
}

// OFFSET: 0x00AC9F10
// FUNCTION: fn_17688
void CWorld::OnProcess() {
  if (this->movies != nullptr) {
    this->movies->Update();
  }

  std::atomic_ref<int> pending(this->pending);
  const int ready = pending.load(std::memory_order_acquire);
  if (ready != 0) {
    const int end = this->published + ready;
    while (this->published < end) {
      C3dActor& model = *this->models[this->published];
      for (int index = 0; index < 4; ++index) {
        WorldSpatialTree* child = this->spatial.children[index];
        if (model.oriented.Intersects(child->minimum)) {
          child->InsertModel(model, 1);
        }
      }

      pending.fetch_sub(1, std::memory_order_seq_cst);
      ++this->published;
    }
  }

  auto objectEntry = this->objects.begin();
  while (objectEntry != this->objects.end()) {
    CRenderObject* object = *objectEntry;
    if (object->OnProcess()) {
      ++objectEntry;
      continue;
    }

    for (auto actorEntry = this->actors.begin(); actorEntry != this->actors.end(); ++actorEntry) {
      if (*actorEntry == object) {
        this->RemovePcPos((*actorEntry)->render.accountId, true);
        this->RemovePcPosForMove((*actorEntry)->render.accountId, true);

        this->actors.erase(actorEntry);
        break;
      }
    }

    delete *objectEntry;
    objectEntry = this->objects.erase(objectEntry);
  }

  for (auto idEntry = this->pendingActorIds.begin(); idEntry != this->pendingActorIds.end(); ++idEntry) {
    std::uint32_t aid = *idEntry;
    auto skillEntry = this->skills.begin();
    for (; skillEntry != this->skills.end(); ++skillEntry) {
      if (static_cast<CSkill*>(*skillEntry)->skillAid == aid) {
        break;
      }
    }

    if (skillEntry == this->skills.end()) {
      continue;
    }

    CSkill* skill = static_cast<CSkill*>(*skillEntry);
    if (skill == nullptr) {
      continue;
    }

    skill->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (skill->HasPendingDamage() == true) {
      if (std::find(this->pendingActorIds.begin(), this->pendingActorIds.end(), aid) == this->pendingActorIds.end()) {
        this->pendingActorIds.push_back(aid);
      }
    } else {
      for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
        if (*entry == skill) {
          delete skill;
          this->objects.erase(entry);
          break;
        }
      }
      this->skills.erase(skillEntry);
    }
  }
}

// OFFSET: 0x00ACA200
// FUNCTION: fn_17690
bool CWorld::RemoveItem(std::uint32_t aid) {
  auto actorEntry = this->actors.begin();

  for (; actorEntry != this->actors.end(); ++actorEntry) {
    if ((*actorEntry)->render.accountId == aid) {
      break;
    }
  }

  if (actorEntry == this->actors.end()) {
    return false;
  }

  this->RemovePcPos(aid, true);
  this->RemovePcPosForMove((*actorEntry)->render.accountId, true);

  CRenderObject* actor = *actorEntry;

  for (auto objectEntry = this->objects.begin(); objectEntry != this->objects.end(); ++objectEntry) {
    if (*objectEntry == actor) {
      delete actor;
      this->objects.erase(objectEntry);
      break;
    }
  }

  this->actors.erase(actorEntry);
  return true;
}

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
  this->items.clear();
  this->skills.clear();
  g_effectMgr->Clear();
  this->pendingActorIds.clear();
  this->actorCells.clear();
  this->unindexedActorIds.clear();
}

// OFFSET: 0x00ACA980
// FUNCTION: fn_17698
void CWorld::SendObjectMessage(CGameObject* sender, std::uint64_t identity, int message, int a, int b, int c, int d) {
  if (this == nullptr) {
    return;
  }

  if (identity == 0) {
    return;
  }

  auto it = std::find_if(this->objects.begin(), this->objects.end(), [identity](CRenderObject* object) { return identity == object->identity; });

  if (it != this->objects.end()) {
    (*it)->SendMsg(sender, message, message >> 31, a, a >> 31, b, b >> 31, c, c >> 31, d, d >> 31, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00CD8000
// FUNCTION: fn_21298
void CGameMode::SpawnArrowEffect(std::uint32_t aid, int target, char flag, int argument) {
  if (target == 0) {
    return;
  }

  CArrowEffect* effect = new CArrowEffect;

  if (effect) {
    this->scene->objects.push_back(effect);
    effect->SendMsg(nullptr, AM_SETPOS, 0, static_cast<int>(aid), static_cast<int>(aid) >> 31, target, 0, reinterpret_cast<int>(&flag),
                    reinterpret_cast<int>(&flag) >> 31, argument, argument >> 31, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00CD80C0
// FUNCTION: fn_21299
void CGameMode::SpawnArrowEffect64(const vector3d* source, const vector3d* target, float speed) {
  CArrowEffect* effect = new CArrowEffect;

  if (effect) {
    this->scene->objects.push_back(effect);
    effect->SendMsg(nullptr, AM_SETPOS2, 0, reinterpret_cast<int>(source), reinterpret_cast<int>(source) >> 31, reinterpret_cast<int>(target),
                    reinterpret_cast<int>(target) >> 31, reinterpret_cast<int>(&speed), reinterpret_cast<int>(&speed) >> 31, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00CD8730
// FUNCTION: fn_21300
void CGameMode::UnusedArrowStub(int, int, int, int, int, int, int, int, int, int) {
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
    corpse->SetActionName(action);
    corpse->SetSpriteName(sprite);
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
  for (auto it = this->items.begin(); it != this->items.end(); ++it) {
    CItem* item = static_cast<CItem*>(*it);

    if (item->itemAid == aid) {
      return item;
    }
  }

  return nullptr;
}

// OFFSET: 0x00AC8380
// FUNCTION: fn_17670
CRenderObject* CWorld::GetActorByGID(int gid) const {
  auto it = std::find_if(this->actors.begin(), this->actors.end(), [gid](CRenderObject* object) {
    CGameActor* actor = static_cast<CGameActor*>(object);
    return !actor->objectType && gid == actor->GetPkState();
  });

  if (it != this->actors.end()) {
    return *it;
  }

  return nullptr;
}

// OFFSET: 0x00AC8440
// FUNCTION: fn_17671
CRenderObject* CWorld::GetSkillByAID(std::uint32_t aid) const {
  for (auto it = this->skills.begin(); it != this->skills.end(); ++it) {
    CSkill* skill = static_cast<CSkill*>(*it);

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
      effect->effectArgument = argument;
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
  effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
  if (argument != -1) {
    effect->SendMsg(nullptr, AM_SETCOUNT, 0, argument, 0, 0, 0, 0);
  }
  return true;
}

// OFFSET: 0x00CD7800
// FUNCTION: fn_21297
void CGameMode::UpdateNameBalloon(CGameActor* actor) {
  if (!actor) {
    return;
  }

  this->EnsureNameBalloon(this->nameBalloon);
  CActorNameCard card = this->nameCard.Lookup(actor->render.accountId);
  if (!card.received) {
    return;
  }

  if (actor->hasHp == 1 && (this->scene->isPKZone == 1 || this->scene->isSiegeMode == 1)) {
    card.name = "??????";
  } else {
    const std::string name = this->nameCard.Name(actor->render.accountId);
    card.name = name;
  }

  if (g_session.IsMonsterActor(actor) == 1) {
    UINameBalloonText* balloon = this->nameBalloon;
    unsigned char monsterType;
    if (actor->GetEffectiveJob(-1) == 20269) {
      monsterType = 0;
    } else {
      monsterType = actor->hpGaugeFlags;
    }
    balloon->SetName(card, actor->IsPlayerClass(actor->GetEffectiveJob(-1)) == 0, monsterType);
  } else if (actor->objectType == 7 || actor->objectType == 8) {
    CActorNameCard masked = card;
    char name[24] = {};
    strncpy_s(name, sizeof(name), masked.name.c_str(), masked.name.size());
    if (g_forbiddenWords.Contains(name) == true) {
      std::string unused;
      g_forbiddenWords.Mask(name, &unused);
      masked.name = name;
    }
    this->nameBalloon->SetName(masked, actor->IsPlayerClass(actor->GetEffectiveJob(-1)) == 0, 0);
  } else {
    bool stripHash = true;
    if (actor->IsPlayerClass(actor->GetEffectiveJob(-1)) == 1) {
      stripHash &= g_session.IsAllyActor(actor->render.accountId, actor->GetEffectiveJob(-1)) != 0;
    }
    this->nameBalloon->SetName(card, stripHash, 0);
  }

  const unsigned char showEmblem = g_serviceOptions.Get(0x40);
  if ((g_session.IsNpcJob(actor->GetEffectiveJob(-1)) || g_session.IsMonsterActor(actor)) && card.groupIconId) {
    this->nameBalloon->SetIcon(FormatOwnedString("유저인터페이스\\group\\group_%d.bmp", card.groupIconId).c_str(), 0);
  } else if (g_session.IsAllyActor(actor->render.accountId, actor->GetEffectiveJob(-1)) && card.groupIconId) {
    this->nameBalloon->SetIcon(FormatOwnedString("유저인터페이스\\group\\group_%d.bmp", card.groupIconId).c_str(), 0);
  } else if (actor->GetGdid() && showEmblem) {
    const unsigned char framedEmblem = g_serviceOptions.Get(0xF3);
    std::string emblem = g_guildMgr->GetEmblemFilename(actor->GetGdid());
    this->nameBalloon->SetIcon(emblem.c_str(), static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->isSiegeMode == 1 && framedEmblem ? 1 : 0);
  } else if (reinterpret_cast<CGameActor*>(this->scene->player) == actor) {
    char path[256];
    std::memset(path, 0, sizeof(path));
    if (g_clanInfo->m_hasClan == true && g_clanInfo->m_clanId) {
      StringFormat(path, sizeof(path), static_cast<std::size_t>(-1), "유저인터페이스\\clan_system\\clan_emblem%02d.bmp", g_clanInfo->m_clanId);
    }
    this->nameBalloon->SetIcon(path, 0);
  } else if (actor->hasSp) {
    char path[256];
    std::memset(path, 0, sizeof(path));
    std::sprintf(path, "유저인터페이스\\clan_system\\clan_emblem%02d.bmp", actor->spGaugeValue);
    this->nameBalloon->SetIcon(path, 0);
  } else {
    this->nameBalloon->SetIcon("", 0);
  }

  UINameBalloonText* balloon = this->nameBalloon;
  *reinterpret_cast<int*>(this->nameBalloonSize) = balloon->geometry.width;
  *reinterpret_cast<int*>(&this->nameBalloonSize[4]) = balloon->geometry.height;
  if (balloon) {
    const int aid = actor->render.accountId;
    if (g_serviceType == 12) {
      switch (aid) {
      case 0x189111:
      case 0x189112:
      case 0x189113:
      case 0x189114:
      case 0x189116:
      case 0x189117:
      case 0x189119:
      case 0x18911A:
        balloon->m_textColor = 0xFF;
        balloon->m_outlineColor = 0x646464;
        break;
      default:
        balloon->m_textColor = this->ActorNameColor(actor->GetEffectiveJob(-1), aid);
        balloon->m_outlineColor = 0;
        break;
      }
    } else {
      balloon->m_textColor = this->ActorNameColor(actor->GetEffectiveJob(-1), aid);
      balloon->m_outlineColor = 0;
    }

    if (!actor->IsPlayerClass(actor->GetEffectiveJob(-1)) && static_cast<short>(actor->characterLevel) > 0 && g_session.IsMonsterActor(actor)) {
      const short difference = static_cast<short>(g_session.m_level - actor->characterLevel);
      if (difference < -10) {
        balloon->m_textColor = 0xFF;
        balloon->m_outlineColor = 0;
      } else if (difference >= 16) {
        balloon->m_textColor = 0x9B9B9B;
        balloon->m_outlineColor = 0;
      }
    }

    if (g_session.m_aid == actor->emblemVersion) {
      balloon->m_textColor = 0xC8C8C8;
      balloon->m_outlineColor = 0;
    }
  }

  this->PlaceWindowAtActor(this->nameBalloon, actor);
}

// OFFSET: 0x00CD8820
// FUNCTION: fn_21302
void CGameMode::ShowGuildEmblem(CGameActor* actor, int guildId, int unused) {
  if (!actor || !guildId || actor->sceneNameWindow != 0) {
    return;
  }

  if (IsAdminAccount(actor->render.accountId)) {
    return;
  }

  if (!IsAdminAccount(g_session.m_aid)) {
    if (StateFlag02(actor->GetStateFlags()) || StateFlag01(actor->GetStateFlags()) || StateFlag06(actor->GetStateFlags())) {
      return;
    }
  }

  UIEmblemWnd* window = new UIEmblemWnd;
  window->SendMsg(nullptr, UM_SETINFO, actor->render.accountId, guildId, 0, 0);
  g_windowMgr.AddRenderedFront(window);
  actor->sceneNameWindow = reinterpret_cast<int>(window);
  this->AddNotificationWindow(window);
}

// OFFSET: 0x00CD8950
// FUNCTION: fn_21303
CMsgEffect* CGameMode::SpawnCashEmotion(int aid, int packId, int emotionId, std::uint32_t duration) {
  CashEmotionData* data = g_cashEmotionMgr->GetEmotionData(packId, emotionId);
  if (!data) {
    return nullptr;
  }

  struct {
    std::string sprite;
    std::string action;
  } names;
  names.sprite = g_cashEmotionMgr->GetSprFileName(data->actId);
  names.action = g_cashEmotionMgr->GetPackName(data->sprId);

  CMsgEffect* effect = new (std::nothrow) CMsgEffect;
  if (effect) {
    if (this->scene) {
      this->scene->objects.emplace_back(effect);
    }

    effect->SendMsg(nullptr, AM_SETMASTERGID, 0, duration, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    effect->SendMsg(nullptr, AM_SETPOS, 0, aid, aid >> 31, reinterpret_cast<int>(&names), reinterpret_cast<int>(&names) >> 31, data->actionIndex,
                    data->actionIndex >> 31, 0, 0, 0, 0, 0, 0);
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
  if (enabled) {
    g_depthStart = static_cast<float>(std::atof((*parameters)[0].c_str()));
    g_depthEnd = static_cast<float>(std::atof((*parameters)[1].c_str()));
    g_worldOverlayColor = ParseHexString((*parameters)[2].c_str());
    g_fogDensity = static_cast<float>(std::atof((*parameters)[3].c_str()));
    g_renderer->SetFog(1);
  } else {
    g_renderer->SetFog(0);
  }
}

// OFFSET: 0x00CD8F30
// FUNCTION: fn_21306
void CGameMode::SpawnMandraAttack(std::uint32_t aid, int target, char flag) {
  CMandraAttackEffect* effect = new CMandraAttackEffect;

  if (effect) {
    this->scene->objects.push_back(effect);
    effect->SendMsg(nullptr, AM_SETPOS, 0, aid, 0, reinterpret_cast<int>(&flag), reinterpret_cast<int>(&flag) >> 31, target, target >> 31, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00CD8FE0
// FUNCTION: fn_21307
void CGameMode::OpenActorMenu(const ActorPickInfo* pick, int handled) {
  if (g_replayAnimationScaling != 0 || g_mouse.buttons[0] == 2 || HIBYTE(GetAsyncKeyState(VK_SHIFT)) != 0) {
    return;
  }

  const bool isAdmin = IsAdminAccount(g_session.m_aid);
  if (pick == nullptr || handled != 0 || g_mouse.buttons[1] != 3 || g_windowMgr.itemStoreWnd != nullptr) {
    return;
  }

  UIWindow* menu;
  char text[256];
  const int job = pick->job;
  CGameActor* actor = nullptr;
  if ((job >= 45 && job < 1000) || g_session.IsMonsterClass(job) || g_session.IsCompanionJob(job) ||
      pick->kind == 4) {
    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(pick->accountId));
  }

  if (actor != nullptr && g_session.IsHomunAid(actor->render.accountId)) {
    if (g_windowMgr.menuWnd != nullptr) {
      g_windowMgr.CloseWindow(WID_MENUWND);
    }
    this->actorMenuIds.erase(this->actorMenuIds.begin(), this->actorMenuIds.end());
    menu = g_windowMgr.EnsureWindow(WID_MENUWND);
    this->menuTargetAid = pick->accountId;

    this->actorMenuIds.push_back(0x25);
    sprintf(text, LookupMessage(MSI_HOMUN_SHOWINFO));
    menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
    this->actorMenuIds.push_back(0x26);
    sprintf(text, LookupMessage(MSI_HOMUN_FEEDING));
    menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
    this->actorMenuIds.push_back(0x27);
    sprintf(text, LookupMessage(MSI_HOMUN_WAITING));
    menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
  } else if (actor != nullptr && g_session.IsMercenaryAid(pick->accountId)) {
    if (g_windowMgr.menuWnd != nullptr) {
      g_windowMgr.CloseWindow(WID_MENUWND);
    }
    this->actorMenuIds.erase(this->actorMenuIds.begin(), this->actorMenuIds.end());
    menu = g_windowMgr.EnsureWindow(WID_MENUWND);
    this->menuTargetAid = pick->accountId;

    this->actorMenuIds.push_back(0x28);
    sprintf(text, LookupMessage(MSI_HOMUN_SHOWINFO));
    menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
    this->actorMenuIds.push_back(0x29);
    sprintf(text, LookupMessage(MSI_HOMUN_WAITING));
    menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
  } else if (actor != nullptr && pick->accountId == g_session.petGid && actor->objectType == 7) {
    if (g_windowMgr.menuWnd != nullptr) {
      g_windowMgr.CloseWindow(WID_MENUWND);
    }
    this->actorMenuIds.erase(this->actorMenuIds.begin(), this->actorMenuIds.end());
    menu = g_windowMgr.EnsureWindow(WID_MENUWND);
    this->menuTargetAid = pick->accountId;

    this->actorMenuIds.push_back(0x21);
    sprintf(text, LookupMessage(MSI_PET_SHOWINFO));
    menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
    this->actorMenuIds.push_back(0x1D);
    sprintf(text, LookupMessage(MSI_PET_FEEDING));
    menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
    this->actorMenuIds.push_back(0x1E);
    sprintf(text, LookupMessage(MSI_PET_PERFORMANCE));
    menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
    this->actorMenuIds.push_back(0x20);
    sprintf(text, LookupMessage(MSI_PET_ACC_OFF));
    menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
    this->actorMenuIds.push_back(0x1F);
    sprintf(text, LookupMessage(MSI_PET_RETURN_EGG));
    menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
  } else {
    if (pick->accountId == g_session.m_aid) {
      return;
    }
    if (!IsPlayerJob(pick->job) && !isAdmin) {
      return;
    }
    if (g_session.IsAllyActor(pick->accountId, pick->job) == 1) {
      return;
    }
    if (!this->nameCard.Contains(pick->accountId) && !isAdmin) {
      return;
    }
    if (this->ActorName(pick->accountId).empty() && !isAdmin) {
      return;
    }
    if (this->rBtnClickX != g_mouse.x || this->rBtnClickY != g_mouse.y || !IsPlayerJob(pick->job)) {
      return;
    }

    CGameActor* target = static_cast<CGameActor*>(this->scene->GetGameActorByAID(pick->accountId));
    if (target == nullptr) {
      return;
    }

    if (target->storedActorEntry == 1) {
      if (isAdmin != true) {
        return;
      }

      if (g_windowMgr.menuWnd != nullptr) {
        g_windowMgr.CloseWindow(WID_MENUWND);
      }
      this->actorMenuIds.erase(this->actorMenuIds.begin(), this->actorMenuIds.end());
      menu = g_windowMgr.EnsureWindow(WID_MENUWND);
      this->menuTargetAid = pick->accountId;

      this->actorMenuIds.push_back(0x41);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INVALID_MAPPROPERTY_REQUEST_USER)), 0, 0, 0);
    } else {
      std::string name;
      if (target->hasHp == 1 && (this->scene->isPKZone == 1 || this->scene->isSiegeMode == 1)) {
        name = "??????";
      } else {
        name = this->ActorName(pick->accountId);
      }

      if (g_windowMgr.menuWnd != nullptr) {
        g_windowMgr.CloseWindow(WID_MENUWND);
      }
      this->actorMenuIds.erase(this->actorMenuIds.begin(), this->actorMenuIds.end());
      menu = g_windowMgr.EnsureWindow(WID_MENUWND);
      this->menuTargetAid = pick->accountId;

      char line[128];
      std::memset(line, 0, sizeof(line));
      if (isAdmin) {
        this->actorMenuIds.push_back(0x1C);
        sprintf(line, LookupMessage(MSI_BAN_USER), name.c_str());
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(line), 0, 0, 0);
      }

      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      if (!StateFlag01(player->GetStateFlags()) && !StateFlag02(player->GetStateFlags())) {
        this->actorMenuIds.push_back(0x2A);
        sprintf(line, LookupMessage(MSI_REQ_VIEW_OTHERUSER), name.c_str());
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(line), 0, 0, 0);
      }
      if (!StateFlag01(player->GetStateFlags()) && !StateFlag02(player->GetStateFlags())) {
        this->actorMenuIds.push_back(4);
        sprintf(line, LookupMessage(MSI_REQ_DEAL_WITH2), name.c_str());
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(line), 0, 0, 0);
      }

      const bool canInviteParty =
        g_session.partyLeader != 0 && g_session.PartyByName(name.c_str()).valid == 0;
      if (canInviteParty) {
        this->actorMenuIds.push_back(5);
        sprintf(line, LookupMessage(MSI_REQ_JOIN_PARTY2), name.c_str());
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(line), 0, 0, 0);
      }

      if (g_session.IsPartyRecommendTarget(pick->accountId) == true) {
        this->actorMenuIds.push_back(0x24);
        sprintf(line, LookupMessage(MSI_REQ_JOIN_BABY), name.c_str());
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(line), 0, 0, 0);
      }

      if (g_guildInfo.m_guildId != 0) {
        CGameActor* member = static_cast<CGameActor*>(this->scene->GetGameActorByAID(pick->accountId));
        const std::uint32_t guildId = member != nullptr ? member->GetGdid() : 0;
        if (g_guildInfo.canInvite != 0 && guildId == 0) {
          this->actorMenuIds.push_back(0x16);
          sprintf(line, LookupMessage(MSI_REQ_JOIN_GUILD), name.c_str());
          menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(line), 0, 0, 0);
        }
        if (g_guildInfo.isMaster != 0 && guildId != g_guildInfo.m_guildId) {
          menu->SendMsg(nullptr, UM_ADDBOUNDARY, 0, 0, 0, 0);
          this->actorMenuIds.push_back(0x19);
          menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REQ_ALLY_GUILD)), 0, 0, 0);
          this->actorMenuIds.push_back(0x1A);
          menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REQ_HOSTILE_GUILD)), 0, 0, 0);
        }
      }

      menu->SendMsg(nullptr, UM_ADDBOUNDARY, 0, 0, 0, 0);
      this->actorMenuIds.push_back(0x14);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_OPEN_1ON1_WINDOW)), 0, 0, 0);
      if (!IsAdminAccount(pick->accountId) && !g_windowMgr.IsFriend(name.c_str()) && g_serviceMode != 3) {
        this->actorMenuIds.push_back(10);
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ADD_TO_FRIEND_CHAR_LIST)), 0, 0, 0);
      }
      this->actorMenuIds.push_back(0x3A);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_USA_PERSONALINFO_DEATH)), 0, 0, 0);
      menu->SendMsg(nullptr, UM_ADDBOUNDARY, 0, 0, 0, 0);

      if (g_whisperBlockMgr->Contains(name)) {
        this->actorMenuIds.push_back(0xD);
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GRADE_ENCHANT_ITEM_UNBREAKABLE)), 0, 0, 0);
      } else {
        this->actorMenuIds.push_back(0xC);
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GRADE_ENCHANT_ADDITIONAL_SOURCE_NOT_SELECT)), 0, 0, 0);
      }

      if (IsAdminAccount(g_session.m_aid)) {
        menu->SendMsg(nullptr, UM_ADDBOUNDARY, 0, 0, 0, 0);
        this->actorMenuIds.push_back(0x2C);
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REMOVE_EQUIPEDITEM)), 0, 0, 0);
        if (g_serviceMode != 3) {
          this->actorMenuIds.push_back(0x17);
          menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GIVE_PLUS_MANNER_POINT)), 0, 0, 0);
          this->actorMenuIds.push_back(0x18);
          menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GIVE_MINUS_MANNER_POINT)), 0, 0, 0);
          sprintf(line, LookupMessage(MSI_PROHIBIT_LOG), g_session.CountProhibitionLog(this->menuTargetAid));
          this->actorMenuIds.push_back(0x23);
          menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(line), 0, 0, 0);
        }
        sprintf(line, "%s : %d (GID) : %s", g_session.m_serverName.c_str(), this->menuTargetAid, name.c_str());
        this->actorMenuIds.push_back(0x22);
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(line), 0, 0, 0);
      }

      this->menuTargetName = name;
      menu->SendMsg(nullptr, UM_ADDBOUNDARY, 0, 0, 0, 0);
      this->actorMenuIds.push_back(0x3F);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_USER_REPORT_COOLTIME)), 0xFF, 0, 0);
      menu->SendMsg(nullptr, UM_ADDBOUNDARY, 0, 0, 0, 0);

      if (CPc* pc = dynamic_cast<CPc*>(static_cast<CGameActor*>(this->scene->GetGameActorByAID(pick->accountId)))) {
        const std::string code = std::string("[C-Code] : ") + ShortcutFilename(pc->GetPkState());
        this->actorMenuIds.push_back(0x39);
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(code.c_str()), 0, 0, 0);
      }

      menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
      menu->Move(g_mouse.x + 1, g_mouse.y + 1);
      return;
    }
  }

  menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
  menu->Move(g_mouse.x + 1, g_mouse.y + 1);
}

// OFFSET: 0x00ACA540
// FUNCTION: fn_17693
bool CWorld::RemoveGroundItem(std::uint32_t aid) {
  for (auto item = this->items.begin(); item != this->items.end(); ++item) {
    if (static_cast<CItem*>(*item)->itemAid == aid) {
      CRenderObject* object = *item;

      for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
        if (*entry == object) {
          delete object;
          this->objects.erase(entry);
          break;
        }
      }

      this->items.erase(item);
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00ACA8A0
// FUNCTION: fn_17697
bool CWorld::RemoveSkill(std::uint32_t aid) {
  for (auto entry = this->skills.begin(); entry != this->skills.end(); ++entry) {
    if (static_cast<CSkill*>(*entry)->skillAid == aid) {
      CSkill* skill = static_cast<CSkill*>(*entry);

      if (!skill) {
        return false;
      }

      skill->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

      if (skill->HasPendingDamage() == true) {
        for (std::uint32_t pending : this->pendingActorIds) {
          if (pending == aid) {
            return false;
          }
        }

        this->pendingActorIds.push_back(aid);
        return false;
      }

      this->RemoveObject(skill);
      this->skills.erase(entry);
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00CDA2F0
// FUNCTION: fn_21308
CMsgEffect* CGameMode::SpawnDamageMsgEffect(const vector3d* position, std::uint32_t color, std::uint32_t accountId) {
  struct {
    std::string sprite;
    std::string action;
  } names;
  names.sprite = static_cast<const std::string&>(FormatOwnedString("%s.spr", g_session.damageSpriteName.c_str()));
  names.action = static_cast<const std::string&>(FormatOwnedString("%s.act", g_session.damageSpriteName.c_str()));

  CMsgEffect* effect = new CMsgEffect;
  if (effect) {
    this->scene->objects.push_back(effect);
    effect->SendMsg(nullptr, AM_SETMASTERGID, 0, accountId, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(position), reinterpret_cast<int>(position) >> 31, reinterpret_cast<int>(&names),
                    reinterpret_cast<int>(&names) >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    effect->render.color = color;
    effect->render.useGroundLighting = 0;
  }

  return effect;
}

// OFFSET: 0x00CDA580
// FUNCTION: fn_21309
CMsgEffect* CGameMode::SpawnMsgEffect(int effectId, const vector3d* position, std::uint32_t accountId, bool visible) {
  std::string names[2];
  int type = 0;
  int animation = 0;

  switch (effectId) {
  case MAKETYPE_MVP_EFFECT:
    names[0] = "이팩트\\mvp.spr";
    names[1] = "이팩트\\mvp.act";
    type = 2;
    break;
  case MAKETYPE_CART_EFFECT:
    names[0] = "이팩트\\손수레.spr";
    names[1] = "이팩트\\손수레.act";
    type = 3;
    break;
  case MAKETYPE_PK_EFFECT:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 8;
    type = 4;
    break;
  case MAKETYPE_ICE_EFFECT:
    names[0] = "이팩트\\얼음땡.spr";
    names[1] = "이팩트\\얼음땡.act";
    type = 5;
    break;
  case MAKETYPE_ICEBREAK_EFFECT:
    names[0] = "이팩트\\얼음땡.spr";
    names[1] = "이팩트\\얼음땡.act";
    animation = 1;
    type = 6;
    break;
  case MAKETYPE_SMOKE_EFFECT:
    names[0] = "이팩트\\smoke.spr";
    names[1] = "이팩트\\smoke.act";
    type = 6;
    break;
  case MAKETYPE_CURSE_EFFECT:
    names[0] = "이팩트\\Status-curse.spr";
    names[1] = "이팩트\\Status-curse.act";
    type = 7;
    break;
  case MAKETYPE_SLEEP_EFFECT:
    names[0] = "이팩트\\Status-sleep.spr";
    names[1] = "이팩트\\Status-sleep.act";
    type = 8;
    break;
  case MAKETYPE_STUN_EFFECT:
    names[0] = "이팩트\\Status-stun.spr";
    names[1] = "이팩트\\Status-stun.act";
    type = 9;
    break;
  case MAKETYPE_SILENCE_EFFECT:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 10;
    type = 10;
    break;
  case MAKETYPE_ANGELUS_EFFECT:
    names[0] = FormatOwnedString("%s.spr", g_session.GetDamageSpriteName());
    names[1] = FormatOwnedString("%s.act", g_session.GetDamageSpriteName());
    animation = 1;
    type = 11;
    break;
  case MAKETYPE_CRUSIS_EFFECT:
    names[0] = FormatOwnedString("%s.spr", g_session.GetDamageSpriteName());
    names[1] = FormatOwnedString("%s.act", g_session.GetDamageSpriteName());
    animation = 2;
    type = 12;
    break;
  case MAKETYPE_CRITICAL_EFFECT:
    names[0] = FormatOwnedString("%s.spr", g_session.GetDamageSpriteName());
    names[1] = FormatOwnedString("%s.act", g_session.GetDamageSpriteName());
    animation = 2;
    type = 15;
    break;
  case MAKETYPE_LUCKY_EFFECT:
    names[0] = FormatOwnedString("%s.spr", g_session.GetDamageSpriteName());
    names[1] = FormatOwnedString("%s.act", g_session.GetDamageSpriteName());
    animation = 3;
    type = 17;
    break;
  case MAKETYPE_BIRD_EFFECT:
    names[0] = "이팩트\\매.spr";
    names[1] = "이팩트\\매.act";
    type = 18;
    break;
  case MAKETYPE_CART_EFFECT2:
    names[0] = "이팩트\\손수레1.spr";
    names[1] = "이팩트\\손수레1.act";
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT3:
    names[0] = "이팩트\\손수레2.spr";
    names[1] = "이팩트\\손수레2.act";
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT4:
    names[0] = "이팩트\\손수레3.spr";
    names[1] = "이팩트\\손수레3.act";
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT5:
    names[0] = "이팩트\\손수레4.spr";
    names[1] = "이팩트\\손수레4.act";
    type = 3;
    break;
  case MAKETYPE_GUARD_EFFECT:
    names[0] = FormatOwnedString("%s.spr", g_session.GetDamageSpriteName());
    names[1] = FormatOwnedString("%s.act", g_session.GetDamageSpriteName());
    animation = 1;
    type = 24;
    break;
  case MAKETYPE_CART_EFFECT6:
    names[0] = "이팩트\\슈노손수레.spr";
    names[1] = "이팩트\\슈노손수레.act";
    type = 3;
    break;
  case MAKETYPE_BLUE_EFFECT:
    names[0] = FormatOwnedString("%s.spr", g_session.GetDamageSkinAuxiliaryName());
    if (g_session.m_damageSkinIndex == 0) {
      names[1] = "이팩트\\msg.act";
    } else {
      names[1] = FormatOwnedString("%s.act", g_session.GetDamageSkinAuxiliaryName());
    }
    animation = 2;
    type = 25;
    break;
  case MAKETYPE_BIRD_EFFECT2:
    names[0] = "이팩트\\매2.spr";
    names[1] = "이팩트\\매2.act";
    type = 18;
    break;
  case MAKETYPE_BIRD_EFFECT3:
    names[0] = "이팩트\\매.spr";
    names[1] = "이팩트\\베이비매.act";
    type = 18;
    break;
  case MAKETYPE_CART_EFFECT11:
    names[0] = "이팩트\\손수레.spr";
    names[1] = "이팩트\\베이비손수레.act";
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT12:
    names[0] = "이팩트\\손수레1.spr";
    names[1] = "이팩트\\베이비손수레1.act";
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT13:
    names[0] = "이팩트\\손수레2.spr";
    names[1] = "이팩트\\베이비손수레2.act";
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT14:
    names[0] = "이팩트\\손수레3.spr";
    names[1] = "이팩트\\베이비손수레3.act";
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT15:
    names[0] = "이팩트\\손수레4.spr";
    names[1] = "이팩트\\베이비손수레4.act";
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT16:
    names[0] = "이팩트\\슈노손수레.spr";
    names[1] = "이팩트\\베이비슈노손수레.act";
    type = 3;
    break;
  case MAKETYPE_CURSOR_EFFECT:
    names[0] = "cursors.spr";
    names[1] = "cursors.act";
    animation = 10;
    type = 26;
    break;
  case MAKETYPE_BIRD_EFFECT4:
    names[0] = "몬스터\\demon_pungus.spr";
    names[1] = "몬스터\\demon_pungus.act";
    type = 18;
    break;
  case MAKETYPE_BIRD_EFFECT5:
    names[0] = "몬스터\\explosion.spr";
    names[1] = "몬스터\\explosion.act";
    type = 18;
    break;
  case MAKETYPE_BIRD_EFFECT6:
    names[0] = "몬스터\\lude.spr";
    names[1] = "몬스터\\lude.act";
    type = 18;
    break;
  case 35:
    names[0] = "몬스터\\워그.spr";
    names[1] = "몬스터\\워그.act";
    type = 27;
    break;
  case 36:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 80;
    type = 28;
    break;
  case 37:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 81;
    type = 28;
    break;
  case 38:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 82;
    type = 29;
    break;
  case 39:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 83;
    type = 29;
    break;
  case 40:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 84;
    type = 30;
    break;
  case 41:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 85;
    type = 30;
    break;
  case 42:
    names[0] = "몬스터\\워그.spr";
    names[1] = "몬스터\\베이비워그.act";
    type = 31;
    break;
  case 43:
    names[0] = "이팩트\\shield.spr";
    names[1] = "이팩트\\shield.act";
    type = 32;
    break;
  case 44:
    names[0] = "이팩트\\costumebody.spr";
    names[1] = "이팩트\\costumebody.act";
    type = 33;
    break;
  case 45:
    names[0] = "이팩트\\손수레5.spr";
    names[1] = "이팩트\\손수레5.act";
    type = 3;
    break;
  case 46:
    names[0] = "이팩트\\손수레5베이비.spr";
    names[1] = "이팩트\\손수레5베이비.act";
    type = 3;
    break;
  case 47:
    names[0] = "이팩트\\손수레6.spr";
    names[1] = "이팩트\\손수레6.act";
    type = 3;
    break;
  case 48:
    names[0] = "이팩트\\손수레6베이비.spr";
    names[1] = "이팩트\\손수레6베이비.act";
    type = 3;
    break;
  case 49:
    names[0] = "이팩트\\손수레7.spr";
    names[1] = "이팩트\\손수레7.act";
    type = 3;
    break;
  case 50:
    names[0] = "이팩트\\손수레7베이비.spr";
    names[1] = "이팩트\\손수레7베이비.act";
    type = 3;
    break;
  case 51:
    names[0] = "이팩트\\손수레8.spr";
    names[1] = "이팩트\\손수레8.act";
    type = 3;
    break;
  case 52:
    names[0] = "이팩트\\손수레8베이비.spr";
    names[1] = "이팩트\\손수레8베이비.act";
    type = 3;
    break;
  case 53:
    names[0] = "이팩트\\OWL.spr";
    names[1] = "이팩트\\OWL.act";
    type = 18;
    break;
  case 54:
    names[0] = "이팩트\\선물상자카트.spr";
    names[1] = "이팩트\\선물상자카트.act";
    type = 3;
    break;
  case 55:
    names[0] = "이팩트\\선물상자카트_미니.spr";
    names[1] = "이팩트\\선물상자카트_미니.act";
    type = 3;
    break;
  case 56:
    names[0] = "이팩트\\포링싣은카트.spr";
    names[1] = "이팩트\\포링싣은카트.act";
    type = 3;
    break;
  case 57:
    names[0] = "이팩트\\포링싣은카트_미니.spr";
    names[1] = "이팩트\\포링싣은카트_미니.act";
    type = 3;
    break;
  case 58:
    names[0] = "이팩트\\포링카트.spr";
    names[1] = "이팩트\\포링카트.act";
    type = 3;
    break;
  case 59:
    names[0] = "이팩트\\포링카트_미니.spr";
    names[1] = "이팩트\\포링카트_미니.act";
    type = 3;
    break;
  case 60:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 93;
    type = 34;
    break;
  case 61:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 94;
    type = 35;
    break;
  case 62:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 95;
    type = 30;
    break;
  case 63:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 96;
    type = 29;
    break;
  case 64:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 97;
    type = 36;
    break;
  case 65:
    names[0] = "이팩트\\마도카트.spr";
    names[1] = "이팩트\\마도카트.act";
    type = 3;
    break;
  case 66:
    names[0] = "이팩트\\마도카트_미니.spr";
    names[1] = "이팩트\\마도카트_미니.act";
    type = 3;
    break;
  case 67:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 5;
    type = 37;
    break;
  case 68:
    names[0] = "이팩트\\emotion.spr";
    names[1] = "이팩트\\emotion.act";
    animation = 28;
    type = 38;
    break;
  case 69:
    names[0] = "이팩트\\WINDHAWK_HAWK.spr";
    names[1] = "이팩트\\WINDHAWK_HAWK.act";
    type = 18;
    break;
  case 70:
    names[0] = "이팩트\\WINDHAWK_WOLF.spr";
    names[1] = "이팩트\\WINDHAWK_WOLF.act";
    type = 27;
    break;
  default:
    names[0] = "이팩트\\msg.spr";
    names[1] = "이팩트\\msg.act";
    break;
  }

  CMsgEffect* effect = nullptr;
  if (CMsgEffect::UsesMsgEffect(type)) {
    effect = new CSummonActorEffect;
  } else {
    effect = new CMsgEffect;
  }

  if (effect) {
    this->scene->objects.push_back(effect);
    effect->SendMsg(nullptr, AM_SETMASTERGID, 0, accountId, 0, type, type >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    effect->m_visible = visible;
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(position), reinterpret_cast<int>(position) >> 31, reinterpret_cast<int>(names),
                    reinterpret_cast<int>(names) >> 31, animation, animation >> 31, 0, 0, 0, 0, 0, 0);
  }

  return effect;
}
