#include "Client/Mode/CGameMode.h"
#include "Contents/Jobs/JobId.h"
#include "Network/Packets/NoticeEnums.h"
#include "UI/GameSettings/TalkType.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Actors/ActorEnums.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Mode/GameModeEnums.h"
#include "Client/Session/Session.h"
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
#include "Render/Primitives/TextureQuad.h"
#include "UI/Guild/UIEmblemWnd.h"
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
#include "Contents/Actors/ActorPickNode.h"
#include "Client/Input/Mouse.h"
#include "Client/Replay/Replay.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
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

// INLINED
static CGameActor* FindActorByAid(const std::list<CGameActor*>& actors, std::uint32_t aid) {
  for (auto it = actors.begin(); it != actors.end(); ++it) {
    CGameActor* actor = *it;

    if (actor->render.accountId == aid) {
      return actor;
    }
  }

  return nullptr;
}

// OFFSET: 0x00AC8220
// FUNCTION: fn_17667
CGameActor* CWorld::GetGameActorByAidIncludeMe(std::uint32_t aid) const {
  if (aid == g_session.m_aid) {
    return this->player;
  }

  return FindActorByAid(this->actors, aid);
}

// OFFSET: 0x00ACA4E0
// FUNCTION: fn_17692
__declspec(noinline) void CWorld::RemoveObject(CGameObject* object) {
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
  this->ClearChildren();
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

// OFFSET: 0x00ACA2B0
// FUNCTION: fn_46302
__forceinline bool CWorld::RemoveActor(CGameObject* object) {
  auto it = std::find(this->actors.begin(), this->actors.end(), object);
  if (it == this->actors.end()) {
    return false;
  }
  this->RemovePcPos((*it)->render.accountId, true);
  this->RemovePcPosForMove((*it)->render.accountId, true);
  this->actors.erase(it);
  return true;
}

// OFFSET: 0x00AC9F10
// FUNCTION: fn_17688
void CWorld::OnProcess() {
  if (this->movies != nullptr) {
    this->movies->Update();
  }

  const int ready = this->pending;
  if (ready != 0) {
    const int end = this->published + ready;
    WorldSpatialTree& root = this->spatial;
    while (this->published < end) {
      C3dActor& model = *this->models[this->published];
      for (int index = 0; index < 4; ++index) {
        WorldSpatialTree* child = root.children[index];
        if (IntersectsBounds(model.oriented, child->minimum)) {
          child->InsertModel(&model, 1);
        }
      }

      _InterlockedDecrement(&this->pending);
      ++this->published;
    }
  }

  auto objectEntry = this->objects.begin();
  while (objectEntry != this->objects.end()) {
    if (!(*objectEntry)->OnProcess()) {
      this->RemoveActor(*objectEntry);
      delete *objectEntry;
      objectEntry = this->objects.erase(objectEntry);
    } else {
      ++objectEntry;
    }
  }

  for (auto idEntry = this->pendingActorIds.begin(); idEntry != this->pendingActorIds.end(); ++idEntry) {
    std::uint32_t aid = *idEntry;
    auto skillEntry = std::find_if(this->skills.begin(), this->skills.end(), [aid](CSkill* entry) {
      return entry->skillAid == aid;
    });

    if (skillEntry == this->skills.end()) {
      continue;
    }

    CSkill* skill = *skillEntry;
    if (skill == nullptr) {
      continue;
    }

    skill->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
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
  auto actorEntry = std::find_if(this->actors.begin(), this->actors.end(), [aid](CGameActor* actor) {
    return actor->render.accountId == aid;
  });

  if (actorEntry == this->actors.end()) {
    return false;
  }

  this->RemovePcPos(aid, true);
  this->RemovePcPosForMove((*actorEntry)->render.accountId, true);

  CGameActor* actor = *actorEntry;

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

  const std::uint32_t low = static_cast<std::uint32_t>(identity);
  const std::uint32_t high = static_cast<std::uint32_t>(identity >> 32);
  if (low == 0 && high == 0) {
    return;
  }

  auto it = std::find_if(this->objects.begin(), this->objects.end(), [identity](CGameObject* object) { return identity == object->identity; });

  if (it != this->objects.end()) {
    (*it)->SendMsg(sender, message, a, b, c, d, 0, 0);
  }
}

// OFFSET: 0x00CD8000
// FUNCTION: fn_21298
void CGameMode::SpawnArrowEffect(const vector3d* source, std::uint32_t target, float speed, int job) {
  if (target == 0) {
    return;
  }

  CArrowEffect* effect = new CArrowEffect;

  if (effect) {
    this->scene->objects.push_back(effect);
    effect->SendMsg(nullptr, AM_SETPOS, reinterpret_cast<std::intptr_t>(source), target, reinterpret_cast<std::intptr_t>(&speed), job, 0, 0);
  }
}

// OFFSET: 0x00CD80C0
// FUNCTION: fn_21299
void CGameMode::SpawnArrowEffectToPosition(const vector3d* source, const vector3d* target, float speed) {
  CArrowEffect* effect = new CArrowEffect;

  if (effect) {
    this->scene->objects.push_back(effect);
    effect->SendMsg(nullptr, AM_SETPOS2, reinterpret_cast<int>(source), reinterpret_cast<int>(target), reinterpret_cast<int>(&speed), 0, 0, 0);
  }
}

// OFFSET: 0x00CD8580
// FUNCTION: fn_46329
void CGameMode::SpawnBlowEffect(float x, float y, float z) {
  CBlowEffect* effect = new CBlowEffect;
  if (effect) {
    this->scene->objects.push_back(effect);
    vector3d position{x, y, z};
    effect->SendMsg(nullptr, AM_SETPOS, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0, 0);
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
    CItem* item = *it;

    if (item->itemAid == aid) {
      return item;
    }
  }

  return nullptr;
}

// OFFSET: 0x00AC8380
// FUNCTION: fn_17670
CGameActor* CWorld::GetActorByGID(int gid) const {
  auto it = std::find_if(this->actors.begin(), this->actors.end(), [gid](CGameActor* actor) {
    return !actor->objectType && gid == actor->GetPkState();
  });

  if (it != this->actors.end()) {
    return *it;
  }

  return nullptr;
}

// OFFSET: 0x00AC8440
// FUNCTION: fn_17671
CSkill* CWorld::GetSkillByAID(std::uint32_t aid) const {
  for (auto it = this->skills.begin(); it != this->skills.end(); ++it) {
    CSkill* skill = *it;

    if (skill->skillAid == aid) {
      return skill;
    }
  }

  return nullptr;
}

// OFFSET: 0x00B3C350
// FUNCTION: fn_18807
CRenderObject* FindRenderObject(std::uint32_t aid) {
  CWorld* scene = g_modeMgr.GetGameMode()->scene;

  if (g_session.m_aid == aid) {
    return static_cast<CRenderObject*>(scene->player);
  }

  CRenderObject* actor = scene->GetGameActorByAID(aid);

  if (!actor) {
    actor = scene->GetSkillByAID(aid);
  }

  return actor;
}

// OFFSET: 0x00CD76A0
// FUNCTION: fn_21296
__declspec(safebuffers) bool CGameMode::LaunchTargetEffect(int effectId, CGameActor* source, CGameActor* target, int argument) {
  if (!source || !target) {
    return false;
  }

  if (g_effectMgr->HasAllocator(effectId)) {
    CEffect* effect = g_effectMgr->Spawn(source->render.accountId, effectId, source->render.position, target->render.accountId, -1, 0, 0);
    if (!effect) {
      return false;
    }

    if (argument != -1) {
      effect->effectArgument = argument;
    }

    if (effectId == EF_THROWITEM4) {
      g_effectMgr->Spawn(source->render.accountId, EF_THROWITEM4_1, source->render.position,
                         target->render.accountId, -1, 0, 0);
    }
    return true;
  }

  CRagEffect* effect = source->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  if (effect) {
    vector3d position = target->render.position;
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    if (argument != -1) {
      effect->SendMsg(nullptr, AM_SETCOUNT, 0, argument, 0, 0, 0, 0);
    }
    return true;
  }
  return false;
}

// INLINED
static std::string NameFromCard(CActorNameCardHolder& cards, std::uint32_t id) {
  return cards.Name(id);
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
    card.name = std::string_view("??????");
  } else {
    card.name = std::string_view(NameFromCard(this->nameCard, actor->render.accountId));
  }

  if (g_session.IsMonsterActor(actor) == 1) {
    this->nameBalloon->SetName(
      card, actor->IsPlayerClass(actor->GetEffectiveJob(-1)) == 0, actor->GetEffectiveJob(-1) == 20269 ? 0 : actor->hpGaugeFlags
    );
  } else if (actor->objectType == NPC_PET_TYPE || actor->objectType == NPC_HO_TYPE) {
    CActorNameCard masked = card;
    char name[24] = {};
    strncpy_s(name, sizeof(name), card.name.c_str(), card.name.size());
    if (g_forbiddenWords.Contains(name) == true) {
      {
        std::string unused = "";
        g_forbiddenWords.Mask(name, &unused);
      }
      masked.name = name;
    }
    this->nameBalloon->SetName(masked, actor->IsPlayerClass(actor->GetEffectiveJob(-1)) == 0, 0);
  } else {
    bool stripHash = true;
    if (actor->IsPlayerClass(actor->GetEffectiveJob(-1)) == 1) {
      if (!g_session.IsAllyActor(actor->render.accountId, actor->GetEffectiveJob(-1))) {
        stripHash = false;
      }
    }
    this->nameBalloon->SetName(card, stripHash, 0);
  }

  const unsigned char showEmblem = g_serviceOptions.Get(TT_SHOW_EMBLEM_ON_OFF);
  if ((g_session.IsNpcJob(actor->GetEffectiveJob(-1)) || g_session.IsMonsterActor(actor)) && card.groupIconId) {
    this->nameBalloon->SetIcon(FormatOwnedString("유저인터페이스\\group\\group_%d.bmp", card.groupIconId).c_str(), 0);
  } else if (g_session.IsAllyActor(actor->render.accountId, actor->GetEffectiveJob(-1)) && card.groupIconId) {
    this->nameBalloon->SetIcon(FormatOwnedString("유저인터페이스\\group\\group_%d.bmp", card.groupIconId).c_str(), 0);
  } else if (actor->GetGdid() && showEmblem) {
    const unsigned char framedEmblem = g_serviceOptions.Get(TT_EMBLEM_FRAME_ON_OFF);
    CGuildMgr* guildMgr = g_guildMgr;
    std::string emblem = guildMgr->GetEmblemFilename(actor->GetGdid());
    if (g_modeMgr.GetGameMode()->scene->isSiegeMode == 1 && framedEmblem) {
      this->nameBalloon->SetIcon(emblem.c_str(), 1);
    } else {
      this->nameBalloon->SetIcon(emblem.c_str(), 0);
    }
  } else {
    char path[256];
    if (this->scene->player == actor) {
      std::memset(path, 0, sizeof(path));
      if (g_clanInfo->m_hasClan == true && g_clanInfo->m_clanId) {
        StringFormat(path, sizeof(path), static_cast<std::size_t>(-1), "유저인터페이스\\clan_system\\clan_emblem%02d.bmp", g_clanInfo->m_clanId);
      }
      this->nameBalloon->SetIcon(path, 0);
    } else if (actor->hasSp) {
      std::memset(path, 0, sizeof(path));
      std::sprintf(path, "유저인터페이스\\clan_system\\clan_emblem%02d.bmp", actor->spGaugeValue);
      this->nameBalloon->SetIcon(path, 0);
    } else {
      this->nameBalloon->SetIcon("", 0);
    }
  }

  this->nameBalloonWidth = this->nameBalloon->geometry.width;
  this->nameBalloonHeight = this->nameBalloon->geometry.height;
  UINameBalloonText* balloon = this->nameBalloon;
  if (balloon) {
    const int aid = actor->render.accountId;
    if (g_serviceType == ServiceBrazil) {
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
      default: {
        const int job = actor->GetEffectiveJob(-1);
        balloon->m_textColor = this->ActorNameColor(job, aid);
        balloon->m_outlineColor = 0;
        break;
      }
      }
    } else {
      const int job = actor->GetEffectiveJob(-1);
      balloon->m_textColor = this->ActorNameColor(job, aid);
      balloon->m_outlineColor = 0;
    }

    if (!actor->IsPlayerClass(actor->GetEffectiveJob(-1)) && static_cast<short>(actor->characterLevel) > 0 && g_session.IsMonsterActor(actor)) {
      const short difference = static_cast<short>(g_session.m_level - actor->characterLevel);
      if (difference < -10) {
        balloon->m_textColor = 0xFF;
        balloon->m_outlineColor = 0;
      } else if (difference > 15) {
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
  if (!actor || !guildId || actor->sceneNameWindow != nullptr) {
    return;
  }

  if (IsAdminAccount(actor->render.accountId)) {
    return;
  }

  if (!IsAdminAccount(g_session.m_aid)) {
    if (IsCloaking(actor->GetStateFlags()) || IsHiding(actor->GetStateFlags()) || IsInvisible(actor->GetStateFlags())) {
      return;
    }
  }

  UIEmblemWnd* window = new UIEmblemWnd;
  window->SendMsg(nullptr, UM_SETINFO, actor->render.accountId, guildId, 0, 0);
  g_windowMgr.AddRenderedFront(window);
  actor->sceneNameWindow = window;
  this->AddNotificationWindow(window);
}

// OFFSET: 0x00CD8950
// FUNCTION: fn_21303
CMsgEffect* CGameMode::SpawnCashEmotion(const vector3d* position, int packId, int emotionId, std::uint32_t duration) {
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

    effect->SendMsg(nullptr, AM_SETMASTERGID, duration, 1, 0, 0, 0, 0);
    effect->SendMsg(nullptr, AM_SETPOS, reinterpret_cast<int>(position), reinterpret_cast<int>(&names), data->actionIndex, 0, 0, 0);
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
void CGameMode::SpawnMandraAttack(std::uint32_t target, int job, float startFrame) {
  CMandraAttackEffect* effect = new CMandraAttackEffect;

  if (effect) {
    this->scene->objects.push_back(effect);
    effect->SendMsg(nullptr, AM_SETPOS, target, reinterpret_cast<std::intptr_t>(&startFrame), job, 0, 0, 0);
  }
}

template __declspec(noinline) std::string std::operator+(std::string&&, std::string&&);

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
  if ((job >= JT_WARPNPC && job < JT_MON_BEGIN) || g_session.IsMonsterClass(job) || g_session.IsCompanionJob(job) ||
      pick->kind == 4) {
    CGameActor* actor = this->scene->GetGameActorByAID(pick->accountId);
    if (actor != nullptr) {
      if (g_session.IsHomunAid(actor->render.accountId)) {
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
        menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
        menu->Move(g_mouse.x + 1, g_mouse.y + 1);
        return;
      }
      if (g_session.IsMercenaryAid(pick->accountId)) {
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
        menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
        menu->Move(g_mouse.x + 1, g_mouse.y + 1);
        return;
      }
      if (pick->accountId == g_session.petGid && actor->objectType == NPC_PET_TYPE) {
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
        menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
        menu->Move(g_mouse.x + 1, g_mouse.y + 1);
        return;
      }
    }
  }

  if (pick->accountId == g_session.m_aid) {
    return;
  }
  if (!g_session.IsPlayerJob(pick->job) && !isAdmin) {
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
  if (this->rBtnClickX != g_mouse.x || this->rBtnClickY != g_mouse.y || !g_session.IsPlayerJob(pick->job)) {
    return;
  }

  CGameActor* target = this->scene->GetGameActorByAID(pick->accountId);
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
    menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_STORE_ASSISTANT_TRADE_FORCED_REMOVAL)), 0, 0, 0);
    menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
    menu->Move(g_mouse.x + 1, g_mouse.y + 1);
  } else {
    std::string name;
    if (target->hasHp == 1 && (this->scene->isPKZone == 1 || this->scene->isSiegeMode == 1)) {
      name = std::string_view("??????");
    } else {
      name = static_cast<const std::string&>(this->ActorName(pick->accountId));
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

    CPlayer* player = g_modeMgr.GetGameMode()->scene->player;
    if (!IsHiding(player->GetStateFlags()) && !IsCloaking(player->GetStateFlags())) {
      this->actorMenuIds.push_back(0x2A);
      sprintf(line, LookupMessage(MSI_REQ_VIEW_OTHERUSER), name.c_str());
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(line), 0, 0, 0);
    }
    if (!IsHiding(player->GetStateFlags()) && !IsCloaking(player->GetStateFlags())) {
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
      CGameActor* member = this->scene->GetGameActorByAID(pick->accountId);
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
    if (!IsAdminAccount(pick->accountId) && !g_windowMgr.IsFriend(name.c_str()) && g_serviceMode != ServerInstantEvent) {
      this->actorMenuIds.push_back(10);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ADD_TO_FRIEND_CHAR_LIST)), 0, 0, 0);
    }
    this->actorMenuIds.push_back(0x3A);
    menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SEND_RODEX)), 0, 0, 0);
    menu->SendMsg(nullptr, UM_ADDBOUNDARY, 0, 0, 0, 0);

    CWhisperBlockMgr* whisperBlockMgr = g_whisperBlockMgr;
    if (whisperBlockMgr->Contains(name)) {
      this->actorMenuIds.push_back(0xD);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_UNBLOCK_CHAT)), 0, 0, 0);
    } else {
      this->actorMenuIds.push_back(0xC);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BLOCK_CHAT)), 0, 0, 0);
    }

    if (IsAdminAccount(g_session.m_aid)) {
      menu->SendMsg(nullptr, UM_ADDBOUNDARY, 0, 0, 0, 0);
      this->actorMenuIds.push_back(0x2C);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REMOVE_EQUIPEDITEM)), 0, 0, 0);
      if (g_serviceMode != ServerInstantEvent) {
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
    menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_USER_REPORT)), 0xFF, 0, 0);
    menu->SendMsg(nullptr, UM_ADDBOUNDARY, 0, 0, 0, 0);

    if (CPc* pc = dynamic_cast<CPc*>(this->scene->GetGameActorByAID(pick->accountId))) {
      const std::string code = std::string("[C-Code] : ") + g_session.ShortcutFilename(pc->GetPkState());
      this->actorMenuIds.push_back(0x39);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(code.c_str()), 0, 0, 0);
    }

    menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
    menu->Move(g_mouse.x + 1, g_mouse.y + 1);
    return;
  }
}

// OFFSET: 0x00ACA540
// FUNCTION: fn_17693
bool CWorld::RemoveGroundItem(std::uint32_t aid) {
  for (auto item = this->items.begin(); item != this->items.end(); ++item) {
    if ((*item)->itemAid == aid) {
      CItem* object = *item;

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
  auto entry = std::find_if(this->skills.begin(), this->skills.end(), [aid](CSkill* skill) {
    return skill->skillAid == aid;
  });
  if (entry == this->skills.end()) {
    return false;
  }

  CSkill* skill = *entry;
  if (!skill) {
    return false;
  }

  skill->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);

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
    effect->SendMsg(nullptr, AM_SETMASTERGID, accountId, 13, 0, 0, 0, 0);
    effect->SendMsg(nullptr, AM_SETPOS, reinterpret_cast<int>(position), reinterpret_cast<int>(&names), 0, 0, 0, 0);
    effect->render.color = color;
    effect->render.useGroundLighting = 0;
  }

  return effect;
}

// OFFSET: 0x00CDA580
// FUNCTION: fn_21309
CMsgEffect* CGameMode::SpawnMsgEffect(int effectId, const vector3d* position, std::uint32_t accountId, bool visible) {
  struct {
    std::string sprite;
    std::string action;
  } names;
  int animation = 0;
  int type = 0;

  switch (effectId) {
  case MAKETYPE_MVP_EFFECT:
    names.sprite = "이팩트\\mvp.spr";
    names.action = "이팩트\\mvp.act";
    animation = 0;
    type = 2;
    break;
  case MAKETYPE_CART_EFFECT:
    names.sprite = "이팩트\\손수레.spr";
    names.action = "이팩트\\손수레.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT2:
    names.sprite = "이팩트\\손수레1.spr";
    names.action = "이팩트\\손수레1.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT3:
    names.sprite = "이팩트\\손수레2.spr";
    names.action = "이팩트\\손수레2.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT4:
    names.sprite = "이팩트\\손수레3.spr";
    names.action = "이팩트\\손수레3.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT5:
    names.sprite = "이팩트\\손수레4.spr";
    names.action = "이팩트\\손수레4.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT6:
    names.sprite = "이팩트\\슈노손수레.spr";
    names.action = "이팩트\\슈노손수레.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT11:
    names.sprite = "이팩트\\손수레.spr";
    names.action = "이팩트\\베이비손수레.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT12:
    names.sprite = "이팩트\\손수레1.spr";
    names.action = "이팩트\\베이비손수레1.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT13:
    names.sprite = "이팩트\\손수레2.spr";
    names.action = "이팩트\\베이비손수레2.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT14:
    names.sprite = "이팩트\\손수레3.spr";
    names.action = "이팩트\\베이비손수레3.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT15:
    names.sprite = "이팩트\\손수레4.spr";
    names.action = "이팩트\\베이비손수레4.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_CART_EFFECT16:
    names.sprite = "이팩트\\슈노손수레.spr";
    names.action = "이팩트\\베이비슈노손수레.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_WARG_EFFECT:
    names.sprite = "몬스터\\워그.spr";
    names.action = "몬스터\\워그.act";
    animation = 0;
    type = MET_WARG;
    break;
  case MAKETYPE_BABY_WARG_EFFECT:
    names.sprite = "몬스터\\워그.spr";
    names.action = "몬스터\\베이비워그.act";
    animation = 0;
    type = MET_BABY_WARG;
    break;
  case MAKETYPE_BIRD_EFFECT:
    names.sprite = "이팩트\\매.spr";
    names.action = "이팩트\\매.act";
    animation = 0;
    type = 18;
    break;
  case MAKETYPE_BIRD_EFFECT2:
    names.sprite = "이팩트\\매2.spr";
    names.action = "이팩트\\매2.act";
    animation = 0;
    type = 18;
    break;
  case MAKETYPE_BIRD_EFFECT3:
    names.sprite = "이팩트\\매.spr";
    names.action = "이팩트\\베이비매.act";
    animation = 0;
    type = 18;
    break;
  case MAKETYPE_BIRD_EFFECT4:
    names.sprite = "몬스터\\demon_pungus.spr";
    names.action = "몬스터\\demon_pungus.act";
    animation = 0;
    type = 18;
    break;
  case MAKETYPE_BIRD_EFFECT5:
    names.sprite = "몬스터\\explosion.spr";
    names.action = "몬스터\\explosion.act";
    animation = 0;
    type = 18;
    break;
  case MAKETYPE_BIRD_EFFECT6:
    names.sprite = "몬스터\\lude.spr";
    names.action = "몬스터\\lude.act";
    animation = 0;
    type = 18;
    break;
  case MAKETYPE_PK_EFFECT:
    names.sprite = "이팩트\\emotion.spr";
    names.action = "이팩트\\emotion.act";
    animation = 8;
    type = 4;
    break;
  case MAKETYPE_ICE_EFFECT:
    names.sprite = "이팩트\\얼음땡.spr";
    names.action = "이팩트\\얼음땡.act";
    animation = 0;
    type = 5;
    break;
  case MAKETYPE_ICEBREAK_EFFECT:
    names.sprite = "이팩트\\얼음땡.spr";
    names.action = "이팩트\\얼음땡.act";
    animation = 1;
    type = 6;
    break;
  case MAKETYPE_SMOKE_EFFECT:
    names.sprite = "이팩트\\smoke.spr";
    names.action = "이팩트\\smoke.act";
    animation = 0;
    type = 6;
    break;
  case MAKETYPE_CURSE_EFFECT:
    names.sprite = "이팩트\\Status-curse.spr";
    names.action = "이팩트\\Status-curse.act";
    animation = 0;
    type = 7;
    break;
  case MAKETYPE_SLEEP_EFFECT:
    names.sprite = "이팩트\\Status-sleep.spr";
    names.action = "이팩트\\Status-sleep.act";
    animation = 0;
    type = 8;
    break;
  case MAKETYPE_STUN_EFFECT:
    names.sprite = "이팩트\\Status-stun.spr";
    names.action = "이팩트\\Status-stun.act";
    animation = 0;
    type = 9;
    break;
  case MAKETYPE_SILENCE_EFFECT:
    names.sprite = "이팩트\\emotion.spr";
    names.action = "이팩트\\emotion.act";
    animation = 10;
    type = 10;
    break;
  case MAKETYPE_ANGELUS_EFFECT: {
    std::string formatted = FormatOwnedString("%s.spr", g_session.GetDamageSpriteName());

    names.sprite = std::move(formatted);
    names.action = static_cast<const std::string&>(FormatOwnedString("%s.act", g_session.GetDamageSpriteName()));
    animation = 1;
    type = 11;
    break;
  }
  case MAKETYPE_CRUSIS_EFFECT: {
    std::string formatted = FormatOwnedString("%s.spr", g_session.GetDamageSpriteName());

    names.sprite = std::move(formatted);
    names.action = static_cast<const std::string&>(FormatOwnedString("%s.act", g_session.GetDamageSpriteName()));
    animation = 2;
    type = 12;
    break;
  }
  case MAKETYPE_CRITICAL_EFFECT: {
    std::string formatted = FormatOwnedString("%s.spr", g_session.GetDamageSpriteName());

    names.sprite = std::move(formatted);
    names.action = static_cast<const std::string&>(FormatOwnedString("%s.act", g_session.GetDamageSpriteName()));
    animation = 2;
    type = 15;
    break;
  }
  case MAKETYPE_BLUE_EFFECT: {
    std::string formatted = FormatOwnedString("%s.spr", g_session.GetDamageSkinAuxiliaryName());

    names.sprite = std::move(formatted);
    if (g_session.m_damageSkinIndex == 0) {
      names.action = "이팩트\\msg.act";
    } else {
      names.action = static_cast<const std::string&>(FormatOwnedString("%s.act", g_session.GetDamageSkinAuxiliaryName()));
    }
    animation = 2;
    type = 25;
    break;
  }
  case MAKETYPE_LUCKY_EFFECT: {
    std::string formatted = FormatOwnedString("%s.spr", g_session.GetDamageSpriteName());

    names.sprite = std::move(formatted);
    names.action = static_cast<const std::string&>(FormatOwnedString("%s.act", g_session.GetDamageSpriteName()));
    animation = 3;
    type = 17;
    break;
  }
  case MAKETYPE_GUARD_EFFECT: {
    std::string formatted = FormatOwnedString("%s.spr", g_session.GetDamageSpriteName());

    names.sprite = std::move(formatted);
    names.action = static_cast<const std::string&>(FormatOwnedString("%s.act", g_session.GetDamageSpriteName()));
    animation = 1;
    type = 24;
    break;
  }
  case MAKETYPE_CURSOR_EFFECT:
    names.sprite = "cursors.spr";
    names.action = "cursors.act";
    animation = 10;
    type = 26;
    break;
  case 36:
  case 37:
  case 38:
  case 39:
  case 40:
  case 41:
    names.sprite = "이팩트\\emotion.spr";
    names.action = "이팩트\\emotion.act";
    if (effectId == 36) {
      animation = 80;
      type = effectId - 8;
    } else if (effectId == 37) {
      animation = 81;
      type = effectId - 9;
    } else if (effectId == 38) {
      animation = 82;
      type = effectId - 9;
    } else if (effectId == 39) {
      animation = 83;
      type = effectId - 10;
    } else if (effectId == 40) {
      animation = 84;
      type = effectId - 10;
    } else if (effectId == 41) {
      animation = 85;
      type = effectId - 11;
    }
    break;
  case 60:
    names.sprite = "이팩트\\emotion.spr";
    names.action = "이팩트\\emotion.act";
    animation = 93;
    type = 34;
    break;
  case 61:
    names.sprite = "이팩트\\emotion.spr";
    names.action = "이팩트\\emotion.act";
    animation = 94;
    type = 35;
    break;
  case 62:
    names.sprite = "이팩트\\emotion.spr";
    names.action = "이팩트\\emotion.act";
    animation = 95;
    type = 30;
    break;
  case 63:
    names.sprite = "이팩트\\emotion.spr";
    names.action = "이팩트\\emotion.act";
    animation = 96;
    type = 29;
    break;
  case 64:
    names.sprite = "이팩트\\emotion.spr";
    names.action = "이팩트\\emotion.act";
    animation = 97;
    type = 36;
    break;
  case MAKETYPE_SHIELD_EFFECT:
    names.sprite = "이팩트\\shield.spr";
    names.action = "이팩트\\shield.act";
    animation = 0;
    type = MET_SHIELD_EFFECT;
    break;
  case MAKETYPE_COSTUME_BODY_EFFECT:
    names.sprite = "이팩트\\costumebody.spr";
    names.action = "이팩트\\costumebody.act";
    animation = 0;
    type = MET_COSTUME_BODY_EFFECT;
    break;
  case 45:
    names.sprite = "이팩트\\손수레5.spr";
    names.action = "이팩트\\손수레5.act";
    animation = 0;
    type = 3;
    break;
  case 47:
    names.sprite = "이팩트\\손수레6.spr";
    names.action = "이팩트\\손수레6.act";
    animation = 0;
    type = 3;
    break;
  case 49:
    names.sprite = "이팩트\\손수레7.spr";
    names.action = "이팩트\\손수레7.act";
    animation = 0;
    type = 3;
    break;
  case 51:
    names.sprite = "이팩트\\손수레8.spr";
    names.action = "이팩트\\손수레8.act";
    animation = 0;
    type = 3;
    break;
  case 46:
    names.sprite = "이팩트\\손수레5베이비.spr";
    names.action = "이팩트\\손수레5베이비.act";
    animation = 0;
    type = 3;
    break;
  case 48:
    names.sprite = "이팩트\\손수레6베이비.spr";
    names.action = "이팩트\\손수레6베이비.act";
    animation = 0;
    type = 3;
    break;
  case 50:
    names.sprite = "이팩트\\손수레7베이비.spr";
    names.action = "이팩트\\손수레7베이비.act";
    animation = 0;
    type = 3;
    break;
  case 52:
    names.sprite = "이팩트\\손수레8베이비.spr";
    names.action = "이팩트\\손수레8베이비.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_OWL_EFFECT:
    names.sprite = "이팩트\\OWL.spr";
    names.action = "이팩트\\OWL.act";
    animation = 0;
    type = 18;
    break;
  case MAKETYPE_GIFT_BOX_CART_EFFECT:
    names.sprite = "이팩트\\선물상자카트.spr";
    names.action = "이팩트\\선물상자카트.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_GIFT_BOX_CART_MINI_EFFECT:
    names.sprite = "이팩트\\선물상자카트_미니.spr";
    names.action = "이팩트\\선물상자카트_미니.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_PORING_LOADED_CART_EFFECT:
    names.sprite = "이팩트\\포링싣은카트.spr";
    names.action = "이팩트\\포링싣은카트.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_PORING_LOADED_CART_MINI_EFFECT:
    names.sprite = "이팩트\\포링싣은카트_미니.spr";
    names.action = "이팩트\\포링싣은카트_미니.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_PORING_CART_EFFECT:
    names.sprite = "이팩트\\포링카트.spr";
    names.action = "이팩트\\포링카트.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_PORING_CART_MINI_EFFECT:
    names.sprite = "이팩트\\포링카트_미니.spr";
    names.action = "이팩트\\포링카트_미니.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_MADO_CART_EFFECT:
    names.sprite = "이팩트\\마도카트.spr";
    names.action = "이팩트\\마도카트.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_MADO_CART_MINI_EFFECT:
    names.sprite = "이팩트\\마도카트_미니.spr";
    names.action = "이팩트\\마도카트_미니.act";
    animation = 0;
    type = 3;
    break;
  case MAKETYPE_LASSITUDE_EFFECT:
    names.sprite = "이팩트\\emotion.spr";
    names.action = "이팩트\\emotion.act";
    animation = 5;
    type = MET_LASSITUDE_EFFECT;
    break;
  case MAKETYPE_MISFORTUNE_EFFECT:
    names.sprite = "이팩트\\emotion.spr";
    names.action = "이팩트\\emotion.act";
    animation = 28;
    type = MET_MISFORTUNE_EFFECT;
    break;
  case MAKETYPE_WINDHAWK_HAWK_EFFECT:
    names.sprite = "이팩트\\WINDHAWK_HAWK.spr";
    names.action = "이팩트\\WINDHAWK_HAWK.act";
    animation = 0;
    type = 18;
    break;
  case MAKETYPE_WINDHAWK_WOLF_EFFECT:
    names.sprite = "이팩트\\WINDHAWK_WOLF.spr";
    names.action = "이팩트\\WINDHAWK_WOLF.act";
    animation = 0;
    type = MET_WARG;
    break;
  default:
    names.sprite = "이팩트\\msg.spr";
    names.action = "이팩트\\msg.act";
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
    effect->SendMsg(nullptr, AM_SETMASTERGID, accountId, type, 0, 0, 0, 0);
    effect->m_visible = visible;
    effect->SendMsg(nullptr, AM_SETPOS, reinterpret_cast<int>(position), reinterpret_cast<int>(&names), animation, 0, 0, 0);
  }

  return effect;
}
