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
#include "Contents/Effects/ArrowEffect.h"
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
#include "Utility/Text/FormatString.h"
#include "Utility/Text/HexString.h"
#include "Contents/World/WorldModelLoading.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Models/ModelActor.h"
#include "Render/Textures/TextureCache.h"
#include "Contents/Movies/BinkMovies.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Actors/Pc.h"

#include <atomic>
#include <cstdlib>
#include <new>
#include <string>
#include <vector>

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

  for (C3dActor* model : this->models) {
    delete model;
  }

  this->models.clear();
  delete this->ground;
  this->ground = nullptr;
  ResourceMgr::Instance().ClearExpired();
  g_texMgr.ClearUnused();
}

// OFFSET: 0x00AC9F10

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

// OFFSET: 0x00CD8000
// FUNCTION: fn_21298
void CGameMode::SpawnArrowEffect(std::uint32_t aid, int target, char flag, int argument) {
  if (target == 0) {
    return;
  }

  CArrowEffect* effect = new CArrowEffect;

  if (effect) {
    this->scene->objects.push_back(effect);
    effect->SendMsg(nullptr, 14, 0, static_cast<int>(aid), static_cast<int>(aid) >> 31, target, 0, reinterpret_cast<int>(&flag),
                    reinterpret_cast<int>(&flag) >> 31, argument, argument >> 31, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00CD80C0
// FUNCTION: fn_21299
void CGameMode::SpawnArrowEffect64(const vector3d* source, const vector3d* target, float speed) {
  CArrowEffect* effect = new CArrowEffect;

  if (effect) {
    this->scene->objects.push_back(effect);
    effect->SendMsg(nullptr, 64, 0, reinterpret_cast<int>(source), reinterpret_cast<int>(source) >> 31, reinterpret_cast<int>(target),
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

// OFFSET: 0x00ACA540
// FUNCTION: fn_17693
bool CWorld::RemoveGroundItem(std::uint32_t aid) {
  for (auto item = this->unknown_0018.begin(); item != this->unknown_0018.end(); ++item) {
    CRenderObject* object = *item;

    if (static_cast<CItem*>(object)->itemAid == aid) {
      for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
        if (*entry == object) {
          delete object;
          this->objects.erase(entry);
          break;
        }
      }

      this->unknown_0018.erase(item);
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00ACA8A0
// FUNCTION: fn_17697
bool CWorld::RemoveSkill(std::uint32_t aid) {
  for (auto entry = this->unknown_0020.begin(); entry != this->unknown_0020.end(); ++entry) {
    CSkill* skill = static_cast<CSkill*>(*entry);

    if (skill->skillAid == aid) {
      if (!skill) {
        return false;
      }

      skill->SendMsg(nullptr, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

      if (skill->HasPendingDamage() == true) {
        for (std::uint32_t pending : this->pendingActorIds) {
          if (pending == aid) {
            return false;
          }
        }

        this->pendingActorIds.push_back(aid);
        return false;
      }

      reinterpret_cast<WorldObjects*>(this)->Remove(skill);
      this->unknown_0020.erase(entry);
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00CDA2F0
// FUNCTION: fn_21308
CMsgEffect* CGameMode::SpawnDamageMsgEffect(const vector3d* position, std::uint32_t color, std::uint32_t accountId) {
  std::string sprite;
  std::string action;
  sprite = FormatOwnedString("%s.spr", g_session.unknown_80EC.c_str());
  action = FormatOwnedString("%s.act", g_session.unknown_80EC.c_str());

  CMsgEffect* effect = new CMsgEffect;
  if (effect) {
    this->scene->objects.push_back(effect);
    effect->SendMsg(nullptr, 22, 0, accountId, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(position), reinterpret_cast<int>(position) >> 31, reinterpret_cast<int>(&sprite),
                    reinterpret_cast<int>(&sprite) >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    effect->render.color = color;
    effect->render.unknown_00B4 = 0;
  }

  return effect;
}

// OFFSET: 0x00CDA580
// FUNCTION: fn_21309
CMsgEffect* CGameMode::SpawnMsgEffect(int effectId, const vector3d* position, std::uint32_t accountId, bool visible) {
  std::string sprite;
  std::string action;
  int type = 0;
  int animation = 0;

  switch (effectId) {
  case 0:
    sprite = "이팩트\\mvp.spr";
    action = "이팩트\\mvp.act";
    type = 2;
    break;
  case 1:
    sprite = "이팩트\\손수레.spr";
    action = "이팩트\\손수레.act";
    type = 3;
    break;
  case 2:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 8;
    type = 4;
    break;
  case 3:
    sprite = "이팩트\\얼음땡.spr";
    action = "이팩트\\얼음땡.act";
    type = 5;
    break;
  case 4:
    sprite = "이팩트\\얼음땡.spr";
    action = "이팩트\\얼음땡.act";
    animation = 1;
    type = 6;
    break;
  case 5:
    sprite = "이팩트\\smoke.spr";
    action = "이팩트\\smoke.act";
    type = 6;
    break;
  case 6:
    sprite = "이팩트\\Status-curse.spr";
    action = "이팩트\\Status-curse.act";
    type = 7;
    break;
  case 7:
    sprite = "이팩트\\Status-sleep.spr";
    action = "이팩트\\Status-sleep.act";
    type = 8;
    break;
  case 8:
    sprite = "이팩트\\Status-stun.spr";
    action = "이팩트\\Status-stun.act";
    type = 9;
    break;
  case 9:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 10;
    type = 10;
    break;
  case 10:
    sprite = FormatOwnedString("%s.spr", g_session.Text80EC());
    action = FormatOwnedString("%s.act", g_session.Text80EC());
    animation = 1;
    type = 11;
    break;
  case 11:
    sprite = FormatOwnedString("%s.spr", g_session.Text80EC());
    action = FormatOwnedString("%s.act", g_session.Text80EC());
    animation = 2;
    type = 12;
    break;
  case 12:
    sprite = FormatOwnedString("%s.spr", g_session.Text80EC());
    action = FormatOwnedString("%s.act", g_session.Text80EC());
    animation = 2;
    type = 15;
    break;
  case 13:
    sprite = FormatOwnedString("%s.spr", g_session.Text80EC());
    action = FormatOwnedString("%s.act", g_session.Text80EC());
    animation = 3;
    type = 17;
    break;
  case 14:
    sprite = "이팩트\\매.spr";
    action = "이팩트\\매.act";
    type = 18;
    break;
  case 16:
    sprite = "이팩트\\손수레1.spr";
    action = "이팩트\\손수레1.act";
    type = 3;
    break;
  case 17:
    sprite = "이팩트\\손수레2.spr";
    action = "이팩트\\손수레2.act";
    type = 3;
    break;
  case 18:
    sprite = "이팩트\\손수레3.spr";
    action = "이팩트\\손수레3.act";
    type = 3;
    break;
  case 19:
    sprite = "이팩트\\손수레4.spr";
    action = "이팩트\\손수레4.act";
    type = 3;
    break;
  case 20:
    sprite = FormatOwnedString("%s.spr", g_session.Text80EC());
    action = FormatOwnedString("%s.act", g_session.Text80EC());
    animation = 1;
    type = 24;
    break;
  case 21:
    sprite = "이팩트\\슈노손수레.spr";
    action = "이팩트\\슈노손수레.act";
    type = 3;
    break;
  case 22:
    sprite = FormatOwnedString("%s.spr", g_session.Text8104());
    if (g_session.m_damageSkinIndex == 0) {
      action = "이팩트\\msg.act";
    } else {
      action = FormatOwnedString("%s.act", g_session.Text8104());
    }
    animation = 2;
    type = 25;
    break;
  case 23:
    sprite = "이팩트\\매2.spr";
    action = "이팩트\\매2.act";
    type = 18;
    break;
  case 24:
    sprite = "이팩트\\매.spr";
    action = "이팩트\\베이비매.act";
    type = 18;
    break;
  case 25:
    sprite = "이팩트\\손수레.spr";
    action = "이팩트\\베이비손수레.act";
    type = 3;
    break;
  case 26:
    sprite = "이팩트\\손수레1.spr";
    action = "이팩트\\베이비손수레1.act";
    type = 3;
    break;
  case 27:
    sprite = "이팩트\\손수레2.spr";
    action = "이팩트\\베이비손수레2.act";
    type = 3;
    break;
  case 28:
    sprite = "이팩트\\손수레3.spr";
    action = "이팩트\\베이비손수레3.act";
    type = 3;
    break;
  case 29:
    sprite = "이팩트\\손수레4.spr";
    action = "이팩트\\베이비손수레4.act";
    type = 3;
    break;
  case 30:
    sprite = "이팩트\\슈노손수레.spr";
    action = "이팩트\\베이비슈노손수레.act";
    type = 3;
    break;
  case 31:
    sprite = "cursors.spr";
    action = "cursors.act";
    animation = 10;
    type = 26;
    break;
  case 32:
    sprite = "몬스터\\demon_pungus.spr";
    action = "몬스터\\demon_pungus.act";
    type = 18;
    break;
  case 33:
    sprite = "몬스터\\explosion.spr";
    action = "몬스터\\explosion.act";
    type = 18;
    break;
  case 34:
    sprite = "몬스터\\lude.spr";
    action = "몬스터\\lude.act";
    type = 18;
    break;
  case 35:
    sprite = "몬스터\\워그.spr";
    action = "몬스터\\워그.act";
    type = 27;
    break;
  case 36:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 80;
    type = 28;
    break;
  case 37:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 81;
    type = 28;
    break;
  case 38:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 82;
    type = 29;
    break;
  case 39:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 83;
    type = 29;
    break;
  case 40:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 84;
    type = 30;
    break;
  case 41:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 85;
    type = 30;
    break;
  case 42:
    sprite = "몬스터\\워그.spr";
    action = "몬스터\\베이비워그.act";
    type = 31;
    break;
  case 43:
    sprite = "이팩트\\shield.spr";
    action = "이팩트\\shield.act";
    type = 32;
    break;
  case 44:
    sprite = "이팩트\\costumebody.spr";
    action = "이팩트\\costumebody.act";
    type = 33;
    break;
  case 45:
    sprite = "이팩트\\손수레5.spr";
    action = "이팩트\\손수레5.act";
    type = 3;
    break;
  case 46:
    sprite = "이팩트\\손수레5베이비.spr";
    action = "이팩트\\손수레5베이비.act";
    type = 3;
    break;
  case 47:
    sprite = "이팩트\\손수레6.spr";
    action = "이팩트\\손수레6.act";
    type = 3;
    break;
  case 48:
    sprite = "이팩트\\손수레6베이비.spr";
    action = "이팩트\\손수레6베이비.act";
    type = 3;
    break;
  case 49:
    sprite = "이팩트\\손수레7.spr";
    action = "이팩트\\손수레7.act";
    type = 3;
    break;
  case 50:
    sprite = "이팩트\\손수레7베이비.spr";
    action = "이팩트\\손수레7베이비.act";
    type = 3;
    break;
  case 51:
    sprite = "이팩트\\손수레8.spr";
    action = "이팩트\\손수레8.act";
    type = 3;
    break;
  case 52:
    sprite = "이팩트\\손수레8베이비.spr";
    action = "이팩트\\손수레8베이비.act";
    type = 3;
    break;
  case 53:
    sprite = "이팩트\\OWL.spr";
    action = "이팩트\\OWL.act";
    type = 18;
    break;
  case 54:
    sprite = "이팩트\\선물상자카트.spr";
    action = "이팩트\\선물상자카트.act";
    type = 3;
    break;
  case 55:
    sprite = "이팩트\\선물상자카트_미니.spr";
    action = "이팩트\\선물상자카트_미니.act";
    type = 3;
    break;
  case 56:
    sprite = "이팩트\\포링싣은카트.spr";
    action = "이팩트\\포링싣은카트.act";
    type = 3;
    break;
  case 57:
    sprite = "이팩트\\포링싣은카트_미니.spr";
    action = "이팩트\\포링싣은카트_미니.act";
    type = 3;
    break;
  case 58:
    sprite = "이팩트\\포링카트.spr";
    action = "이팩트\\포링카트.act";
    type = 3;
    break;
  case 59:
    sprite = "이팩트\\포링카트_미니.spr";
    action = "이팩트\\포링카트_미니.act";
    type = 3;
    break;
  case 60:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 93;
    type = 34;
    break;
  case 61:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 94;
    type = 35;
    break;
  case 62:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 95;
    type = 30;
    break;
  case 63:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 96;
    type = 29;
    break;
  case 64:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 97;
    type = 36;
    break;
  case 65:
    sprite = "이팩트\\마도카트.spr";
    action = "이팩트\\마도카트.act";
    type = 3;
    break;
  case 66:
    sprite = "이팩트\\마도카트_미니.spr";
    action = "이팩트\\마도카트_미니.act";
    type = 3;
    break;
  case 67:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 5;
    type = 37;
    break;
  case 68:
    sprite = "이팩트\\emotion.spr";
    action = "이팩트\\emotion.act";
    animation = 28;
    type = 38;
    break;
  case 69:
    sprite = "이팩트\\WINDHAWK_HAWK.spr";
    action = "이팩트\\WINDHAWK_HAWK.act";
    type = 18;
    break;
  case 70:
    sprite = "이팩트\\WINDHAWK_WOLF.spr";
    action = "이팩트\\WINDHAWK_WOLF.act";
    type = 27;
    break;
  default:
    sprite = "이팩트\\msg.spr";
    action = "이팩트\\msg.act";
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
    effect->SendMsg(nullptr, 22, 0, accountId, 0, type, type >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    effect->m_visible = visible;
    effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(position), reinterpret_cast<int>(position) >> 31, reinterpret_cast<int>(&sprite),
                    reinterpret_cast<int>(&sprite) >> 31, animation, animation >> 31, 0, 0, 0, 0, 0, 0);
  }

  return effect;
}
