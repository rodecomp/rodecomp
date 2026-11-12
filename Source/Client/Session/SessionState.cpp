#include "Client/Mode/ModeMessage.h"
#include "Contents/Actors/ActorEnums.h"
#include "UI/Window/UIEnums.h"
#include "Client/Session/Session.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/World/World.h"
#include "Script/LuaScript.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Status/UIBasicInfoWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
extern "C" {
#include <lua.h>
}

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <map>
#include <string>
#include <vector>

struct SkillDelayField {
  int type;
  const char* name;
};

// GLOBAL: gl_25057
static const SkillDelayField g_skillDelayFields[] = {
  {0, "SkillCastFixedDelay"},
  {1, "SkillCastStatDelay"},
  {2, "SkillGlobalPostDelay"},
  {3, "SkillSinglePostDelay"},
};

// OFFSET: 0x00E30940
// FUNCTION: fn_24168
const char* CSession::GetPlayerMapDisplayName() {
  std::string mapName = this->m_navigationPlayerMap;
  mapName += ".rsw";
  return this->GetMapDisplayName(mapName.c_str());
}

// OFFSET: 0x00E31650
// FUNCTION: fn_24173
void CSession::LoadSkillDelayList(int skillId, std::vector<std::map<int, int>>& delays, int& skillFlag) {
  delays.clear();
  int maxLevel = 0;
  skillFlag = 0;
  this->m_hotkeyScript->Call("GetSkillMaxLv", "d>d", skillId, &maxLevel);
  delays.resize(maxLevel);

  lua_State* L = this->m_hotkeyScript->GetState();
  lua_getfield(L, LUA_GLOBALSINDEX, "SKILL_DELAY_LIST");
  if (lua_type(L, -1) == LUA_TTABLE) {
    lua_pushnumber(L, skillId);
    lua_gettable(L, -2);
    if (lua_type(L, -1) == LUA_TTABLE) {
      lua_pushstring(L, "SkillFlag");
      lua_gettable(L, -2);
      if (lua_type(L, -1) == LUA_TTABLE) {
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
          skillFlag += lua_tonumber(L, -1);
          lua_settop(L, -2);
        }
      }
      lua_settop(L, -2);

      for (const SkillDelayField& field : g_skillDelayFields) {
        lua_pushstring(L, field.name);
        lua_gettable(L, -2);
        if (lua_type(L, -1) == LUA_TTABLE) {
          lua_pushnil(L);
          while (lua_next(L, -2) != 0) {
            const int level = static_cast<int>(lua_tonumber(L, -2) - 1.0);
            if (level >= 0 && static_cast<std::size_t>(level) < delays.size()) {
              const int delay = static_cast<int>(lua_tonumber(L, -1));
              delays[level][field.type] = delay;
            }
            lua_settop(L, -2);
          }
        }
        lua_settop(L, -2);
      }
    }
    lua_settop(L, -2);
  }
  lua_settop(L, -2);

  if (skillId == 2461) {
    if (delays.empty()) {
      delays.resize(1);
    }

    CGameActor* actor = static_cast<CGameActor*>(
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(this->elementalGid)
    );
    int delay;
    if (actor) {
      switch (actor->GetEffectiveJob(-1)) {
      case JT_EL_AGNI_S:
      case JT_EL_AQUA_S:
      case JT_EL_VENTUS_S:
      case JT_EL_TERA_S:
        delay = 3000;
        break;
      case JT_EL_AGNI_M:
      case JT_EL_AQUA_M:
      case JT_EL_VENTUS_M:
      case JT_EL_TERA_M:
        delay = 6000;
        break;
      case JT_EL_AGNI_L:
      case JT_EL_AQUA_L:
      case JT_EL_VENTUS_L:
      case JT_EL_TERA_L:
      case JT_EM_DILUVIO:
      case JT_EM_ARDOR:
      case JT_EM_PROCELLA:
      case JT_EM_TERREMOTUS:
      case JT_EM_SERPENS:
        delay = 9000;
        break;
      default:
        delay = 0;
        break;
      }
    } else {
      delay = 0;
    }
    delays[0][3] = delay;
  }
}

// OFFSET: 0x00E3D3D0
// FUNCTION: fn_24190
bool CSession::IsThirdJobOrHigher(int job) {
  if (g_session.IsTreeGroupThree(job) == true || g_session.IsTreeGroupTwo(job) == 1) {
    return true;
  }

  switch (job) {
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
  case JT_PORING_SNOVICE2:
  case JT_PORING_SNOVICE2_B:
  case JT_KAGEROU:
  case JT_OBORO:
  case JT_FROG_KAGEROU:
  case JT_FROG_OBORO:
  case JT_REBELLION:
  case JT_PECO_REBELLION:
  case JT_DO_SUMMONER:
  case JT_DO_CART_SUMMONER:
  case JT_DO_SUMMONER_B:
  case JT_DO_CART_SUMMONER_B:
  case JT_KAGEROU_B:
  case JT_OBORO_B:
  case JT_REBELLION_B:
  case JT_PECO_REBELLION_B:
  case JT_STAR_EMPEROR:
  case JT_SOUL_REAPER:
  case JT_STAR_EMPEROR_B:
  case JT_SOUL_REAPER_B:
  case JT_STAR2_EMPEROR:
  case JT_STAR2_EMPEROR_B:
  case JT_HAETAE_STAR_EMPEROR:
  case JT_HAETAE_SOUL_REAPER:
  case JT_HAETAE_STAR_EMPEROR_B:
  case JT_HAETAE_SOUL_REAPER_B:
  case JT_KARNOS:
  case JT_KARNOS_B:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x00E2ECD0
// FUNCTION: fn_24150
bool CSession::EquipSwapCostumeItem(const ITEM_INFO& item) {
  const EquipSlots slots = g_session.GetEquipSlots(static_cast<int>(item.identity[3]));
  int order[3];
  order[0] = slots.values[2];
  order[1] = slots.values[1];
  order[2] = slots.values[0];
  if (item.identity[3] == 0x8000) {
    g_session.SetSwapArrowIndex(item.identity[1]);
  }

  for (int i = 0; i < 3; ++i) {
    const int slot = order[i];
    if (slot == -1 || slot >= 10) {
      break;
    }
    this->swapCostumeItems[slot] = item;
    this->swapCostumeItems[slot].quantity = 1;
  }

  if (g_windowMgr.equipWnd) {
    static_cast<UIEquipWnd*>(g_windowMgr.equipWnd)->NotifyEquipmentChanged();
  }
  RefreshItemWindows(&g_windowMgr);
  return true;
}

// OFFSET: 0x00E3D8A0
// FUNCTION: fn_24192
void CSession::UnequipSwapEquippedItem(const ITEM_INFO& item) {
  const int location = static_cast<int>(item.identity[3]);
  if (location == 0x8000) {
    this->SetSwapArrowIndex(0);
  } else {
    const EquipSlots slots = g_session.GetEquipSlots(location);
    if (slots.values[2] < 10) {
      if (slots.values[2] != -1) {
        ITEM_INFO& entry = this->swapEquippedItems[slots.values[2]];
        entry.identity[3] = 0;
        entry.Reset();
      }
      if (slots.values[1] != -1 && slots.values[1] < 10) {
        this->swapEquippedItems[slots.values[1]].Reset();
      }
      if (slots.values[0] != -1 && slots.values[0] < 10) {
        this->swapEquippedItems[slots.values[0]].Reset();
      }
    }
  }

  if (g_windowMgr.equipWnd) {
    static_cast<UIEquipWnd*>(g_windowMgr.equipWnd)->NotifyEquipmentChanged();
  }
  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00E3D960
// FUNCTION: fn_24193
void CSession::UnequipSwapCostumeItem(const ITEM_INFO& item) {
  const int location = static_cast<int>(item.identity[3]);
  if (location == 0x8000) {
    ITEM_INFO equipped = g_session.FindItemByIdentity(item.identity[1], location);
    if (equipped.identity[1] > 0) {
      const char* message = LookupMessage(MSI_ITEM_IS_REMOVED);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s%s", equipped.DisplayName(0).c_str(), message).c_str()), 0xFF, 7, 0, 0);
    }
    this->SetSwapArrowIndex(0);
  } else {
    const EquipSlots slots = g_session.GetEquipSlots(location);
    if (slots.values[2] < 10) {
      if (slots.values[2] != -1) {
        ITEM_INFO& entry = this->swapCostumeItems[slots.values[2]];
        entry.identity[3] = 0;
        entry.Reset();
      }
      if (slots.values[1] != -1 && slots.values[1] < 10) {
        this->swapCostumeItems[slots.values[1]].Reset();
      }
      if (slots.values[0] != -1 && slots.values[0] < 10) {
        this->swapCostumeItems[slots.values[0]].Reset();
      }
    }
  }

  if (g_windowMgr.equipWnd) {
    static_cast<UIEquipWnd*>(g_windowMgr.equipWnd)->NotifyEquipmentChanged();
  }
  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00E49200
// FUNCTION: fn_24231
void CSession::SetJob(int job) {
  const int previous = this->m_job;
  this->m_job = job;
  if (this->IsTreeGroupThree(previous) != this->IsTreeGroupThree(job)) {
    if (g_windowMgr.basicInfoWnd) {
      reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.basicInfoWnd)->RefreshLayout();
    }

    if (g_modeMgr.GetGameMode()) {
      g_modeMgr.GetGameMode()->SendMsg(MM_REFRESH_PLAYERGAGE, 0, 0, 0, 0);
    }
  }

  this->InvalidateWindowFrames();
  this->RefreshSkillTree();
}

// OFFSET: 0x00E497F0
// FUNCTION: fn_24237
void CSession::SetSwapArrowIndex(std::uint32_t value) {
  this->swapArrowIndex = value;
  this->InvalidateWindowFrames();

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!mode) {
    return;
  }

  const auto it = std::find_if(g_activeSkillEffects.begin(), g_activeSkillEffects.end(), [](const ActiveSkillEffect& effect) { return effect.id == 0x2B7; });
  if (it == g_activeSkillEffects.end()) {
    if (value) {
      ActiveSkillEffect effect;
      effect.id = 0x2B7;
      g_activeSkillEffects.push_back(effect);
    }
  } else if (!value) {
    g_activeSkillEffects.erase(it);
  }

  mode->scene->player->skillEffect->SendMsg(nullptr, AM_SETCOUNT, 0, 1, 0, 0, 0, 0);
}

// OFFSET: 0x00E49930
// FUNCTION: fn_24238
void CSession::ResolvePendingActorName(std::uint32_t id, const char* name) {
  if (!name || std::strlen(name) == 0) {
    return;
  }

  UIWindow* chat = g_windowMgr.chatWnd;
  if (!chat) {
    return;
  }

  if (this->pendingActorNames.end() == this->pendingActorNames.find(static_cast<std::uint32_t>(id))) {
    return;
  }

  chat->SendMsg(nullptr, UM_RESOLVE_PLAYER_TAGS, reinterpret_cast<std::intptr_t>(name), 0, 0, 0);
  g_windowMgr.SendNotice(UIM_RESOLVE_PLAYER_TAGS, reinterpret_cast<std::intptr_t>(name), 0, 0, 0, 0);
  this->pendingActorNames.erase(static_cast<std::uint32_t>(id));
}

// OFFSET: 0x00E1A5E0
// FUNCTION: fn_23849
void CSession::InitSkillEffectIds() {
  int key;

  this->skillEffectIds.clear();
  key = 0;
  this->skillEffectIds[key] = -1;
  key = 1;
  this->skillEffectIds[key] = -1;
  key = 2;
  this->skillEffectIds[key] = -1;
  key = 3;
  this->skillEffectIds[key] = -1;
  key = 4;
  this->skillEffectIds[key] = -1;
  key = 5;
  this->skillEffectIds[key] = -1;
  key = 6;
  this->skillEffectIds[key] = -1;
  key = 7;
  this->skillEffectIds[key] = 16;
  key = 8202;
  this->skillFollowEffects[key] = 16;
  key = 8;
  this->skillEffectIds[key] = -1;
  key = 9;
  this->skillEffectIds[key] = -1;
  key = 10;
  this->skillEffectIds[key] = -1;
  key = 12;
  this->skillEffectIds[key] = -1;
  key = 13;
  this->skillEffectIds[key] = 15;
  key = 340;
  this->skillEffectIds[key] = 451;
  key = 14;
  this->skillEffectIds[key] = -1;
  key = 15;
  this->skillEffectIds[key] = -1;
  key = 16;
  this->skillEffectIds[key] = -1;
  key = 17;
  this->skillEffectIds[key] = -1;
  key = 18;
  this->skillEffectIds[key] = -1;
  key = 19;
  this->skillEffectIds[key] = -1;
  key = 20;
  this->skillEffectIds[key] = -1;
  key = 21;
  this->skillEffectIds[key] = -1;
  key = 22;
  this->skillEffectIds[key] = -1;
  key = 23;
  this->skillEffectIds[key] = -1;
  key = 24;
  this->skillEffectIds[key] = -1;
  key = 25;
  this->skillEffectIds[key] = -1;
  key = 26;
  this->skillEffectIds[key] = -1;
  key = 27;
  this->skillEffectIds[key] = -1;
  key = 28;
  this->skillEffectIds[key] = -1;
  key = 29;
  this->skillEffectIds[key] = -1;
  key = 690;
  this->skillEffectIds[key] = -1;
  key = 30;
  this->skillEffectIds[key] = -1;
  key = 31;
  this->skillEffectIds[key] = -1;
  key = 32;
  this->skillEffectIds[key] = -1;
  key = 33;
  this->skillEffectIds[key] = -1;
  key = 34;
  this->skillEffectIds[key] = -1;
  key = 689;
  this->skillEffectIds[key] = -1;
  key = 35;
  this->skillEffectIds[key] = -1;
  key = 36;
  this->skillEffectIds[key] = -1;
  key = 37;
  this->skillEffectIds[key] = -1;
  key = 38;
  this->skillEffectIds[key] = -1;
  key = 39;
  this->skillEffectIds[key] = -1;
  key = 40;
  this->skillEffectIds[key] = -1;
  key = 41;
  this->skillEffectIds[key] = -1;
  key = 42;
  this->skillEffectIds[key] = -1;
  key = 43;
  this->skillEffectIds[key] = -1;
  key = 44;
  this->skillEffectIds[key] = -1;
  key = 45;
  this->skillEffectIds[key] = -1;
  key = 46;
  this->skillEffectIds[key] = -1;
  key = 499;
  this->skillEffectIds[key] = -1;
  key = 1009;
  this->skillEffectIds[key] = -1;
  key = 47;
  this->skillEffectIds[key] = -1;
  key = 48;
  this->skillEffectIds[key] = -1;
  key = 49;
  this->skillEffectIds[key] = -1;
  key = 50;
  this->skillEffectIds[key] = 16;
  key = 51;
  this->skillEffectIds[key] = 16;
  key = 52;
  this->skillEffectIds[key] = 16;
  key = 53;
  this->skillEffectIds[key] = 16;
  key = 54;
  this->skillEffectIds[key] = -1;
  key = 55;
  this->skillEffectIds[key] = -1;
  key = 56;
  this->skillEffectIds[key] = -1;
  key = 57;
  this->skillEffectIds[key] = -1;
  key = 58;
  this->skillEffectIds[key] = -1;
  key = 59;
  this->skillEffectIds[key] = -1;
  key = 60;
  this->skillEffectIds[key] = -1;
  key = 61;
  this->skillEffectIds[key] = -1;
  key = 62;
  this->skillEffectIds[key] = -1;
  key = 63;
  this->skillEffectIds[key] = -1;
  key = 64;
  this->skillEffectIds[key] = -1;
  key = 65;
  this->skillEffectIds[key] = -1;
  key = 66;
  this->skillEffectIds[key] = -1;
  key = 67;
  this->skillEffectIds[key] = -1;
  key = 68;
  this->skillEffectIds[key] = -1;
  key = 69;
  this->skillEffectIds[key] = -1;
  key = 694;
  this->skillEffectIds[key] = -1;
  key = 70;
  this->skillEffectIds[key] = -1;
  key = 72;
  this->skillEffectIds[key] = -1;
  key = 73;
  this->skillEffectIds[key] = -1;
  key = 74;
  this->skillEffectIds[key] = -1;
  key = 75;
  this->skillEffectIds[key] = -1;
  key = 76;
  this->skillEffectIds[key] = -1;
  key = 77;
  this->skillEffectIds[key] = -1;
  key = 78;
  this->skillEffectIds[key] = -1;
  key = 79;
  this->skillEffectIds[key] = -1;
  key = 80;
  this->skillEffectIds[key] = 96;
  key = 81;
  this->skillEffectIds[key] = 62;
  key = 83;
  this->skillEffectIds[key] = -1;
  key = 84;
  this->skillEffectIds[key] = 93;
  key = 341;
  this->skillEffectIds[key] = 93;
  key = 85;
  this->skillEffectIds[key] = -1;
  key = 88;
  this->skillEffectIds[key] = -1;
  key = 89;
  this->skillEffectIds[key] = 89;
  key = 664;
  this->skillEffectIds[key] = 89;
  key = 90;
  this->skillEffectIds[key] = -1;
  key = 91;
  this->skillEffectIds[key] = -1;
  key = 92;
  this->skillEffectIds[key] = -1;
  key = 93;
  this->skillEffectIds[key] = -1;
  key = 115;
  this->skillEffectIds[key] = -1;
  key = 116;
  this->skillEffectIds[key] = -1;
  key = 117;
  this->skillEffectIds[key] = -1;
  key = 118;
  this->skillEffectIds[key] = -1;
  key = 119;
  this->skillEffectIds[key] = -1;
  key = 688;
  this->skillEffectIds[key] = -1;
  key = 692;
  this->skillEffectIds[key] = -1;
  key = 698;
  this->skillEffectIds[key] = -1;
  key = 693;
  this->skillEffectIds[key] = -1;
  key = 695;
  this->skillEffectIds[key] = -1;
  key = 120;
  this->skillEffectIds[key] = -1;
  key = 121;
  this->skillEffectIds[key] = -1;
  key = 122;
  this->skillEffectIds[key] = -1;
  key = 123;
  this->skillEffectIds[key] = -1;
  key = 124;
  this->skillEffectIds[key] = -1;
  key = 125;
  this->skillEffectIds[key] = -1;
  key = 126;
  this->skillEffectIds[key] = -1;
  key = 127;
  this->skillEffectIds[key] = -1;
  key = 128;
  this->skillEffectIds[key] = -1;
  key = 129;
  this->skillEffectIds[key] = -1;
  key = 130;
  this->skillEffectIds[key] = -1;
  key = 131;
  this->skillEffectIds[key] = -1;
  key = 132;
  this->skillEffectIds[key] = -1;
  key = 133;
  this->skillEffectIds[key] = -1;
  key = 134;
  this->skillEffectIds[key] = -1;
  key = 135;
  this->skillEffectIds[key] = -1;
  key = 136;
  this->skillEffectIds[key] = -1;
  key = 137;
  this->skillEffectIds[key] = -1;
  key = 138;
  this->skillEffectIds[key] = -1;
  key = 139;
  this->skillEffectIds[key] = -1;
  key = 140;
  this->skillEffectIds[key] = -1;
  key = 141;
  this->skillEffectIds[key] = -1;
  key = 94;
  this->skillEffectIds[key] = -1;
  key = 95;
  this->skillEffectIds[key] = -1;
  key = 96;
  this->skillEffectIds[key] = -1;
  key = 97;
  this->skillEffectIds[key] = -1;
  key = 98;
  this->skillEffectIds[key] = -1;
  key = 99;
  this->skillEffectIds[key] = -1;
  key = 100;
  this->skillEffectIds[key] = -1;
  key = 101;
  this->skillEffectIds[key] = -1;
  key = 102;
  this->skillEffectIds[key] = -1;
  key = 103;
  this->skillEffectIds[key] = -1;
  key = 104;
  this->skillEffectIds[key] = -1;
  key = 105;
  this->skillEffectIds[key] = -1;
  key = 106;
  this->skillEffectIds[key] = -1;
  key = 107;
  this->skillEffectIds[key] = -1;
  key = 108;
  this->skillEffectIds[key] = 101;
  key = 109;
  this->skillEffectIds[key] = -1;
  key = 110;
  this->skillEffectIds[key] = -1;
  key = 111;
  this->skillEffectIds[key] = -1;
  key = 112;
  this->skillEffectIds[key] = 103;
  key = 113;
  this->skillEffectIds[key] = -1;
  key = 114;
  this->skillEffectIds[key] = 104;
  key = 141;
  this->skillEffectIds[key] = -1;
  key = 159;
  this->skillEffectIds[key] = -1;
  key = 192;
  this->skillEffectIds[key] = -1;
  key = 158;
  this->skillEffectIds[key] = -1;
  key = 160;
  this->skillEffectIds[key] = -1;
  key = 161;
  this->skillEffectIds[key] = -1;
  key = 162;
  this->skillEffectIds[key] = 174;
  key = 163;
  this->skillEffectIds[key] = 177;
  key = 164;
  this->skillEffectIds[key] = 173;
  key = 165;
  this->skillEffectIds[key] = 175;
  key = 166;
  this->skillEffectIds[key] = 179;
  key = 167;
  this->skillEffectIds[key] = 178;
  key = 168;
  this->skillEffectIds[key] = 172;
  key = 169;
  this->skillEffectIds[key] = 176;
  key = 171;
  this->skillEffectIds[key] = -1;
  key = 172;
  this->skillEffectIds[key] = -1;
  key = 173;
  this->skillEffectIds[key] = 183;
  key = 174;
  this->skillEffectIds[key] = -1;
  key = 175;
  this->skillEffectIds[key] = -1;
  key = 175;
  this->skillEffectIds[key] = 185;
  key = 176;
  this->skillEffectIds[key] = -1;
  key = 177;
  this->skillEffectIds[key] = -1;
  key = 178;
  this->skillEffectIds[key] = -1;
  key = 663;
  this->skillEffectIds[key] = 454;
  key = 678;
  this->skillEffectIds[key] = 454;
  key = 666;
  this->skillEffectIds[key] = 454;
  key = 668;
  this->skillEffectIds[key] = 454;
  key = 667;
  this->skillEffectIds[key] = 454;
  key = 179;
  this->skillEffectIds[key] = -1;
  key = 180;
  this->skillEffectIds[key] = -1;
  key = 181;
  this->skillEffectIds[key] = -1;
  key = 182;
  this->skillEffectIds[key] = -1;
  key = 183;
  this->skillEffectIds[key] = -1;
  key = 184;
  this->skillEffectIds[key] = -1;
  key = 185;
  this->skillEffectIds[key] = -1;
  key = 186;
  this->skillEffectIds[key] = -1;
  key = 654;
  this->skillEffectIds[key] = -1;
  key = 676;
  this->skillEffectIds[key] = -1;
  key = 662;
  this->skillEffectIds[key] = -1;
  key = 347;
  this->skillEffectIds[key] = -1;
  key = 187;
  this->skillEffectIds[key] = -1;
  key = 188;
  this->skillEffectIds[key] = -1;
  key = 189;
  this->skillEffectIds[key] = -1;
  key = 190;
  this->skillEffectIds[key] = -1;
  key = 191;
  this->skillEffectIds[key] = -1;
  key = 199;
  this->skillEffectIds[key] = -1;
  key = 200;
  this->skillEffectIds[key] = -1;
  key = 202;
  this->skillEffectIds[key] = -1;
  key = 2246;
  this->skillEffectIds[key] = -1;
  key = 2237;
  this->skillEffectIds[key] = -1;
  key = 2309;
  this->skillEffectIds[key] = -1;
  key = 723;
  this->skillEffectIds[key] = 89;
  key = 724;
  this->skillEffectIds[key] = 183;
  key = 722;
  this->skillEffectIds[key] = -1;
  key = 724;
  this->skillEffectIds[key] = 1053;
  key = 723;
  this->skillEffectIds[key] = 89;
  key = 757;
  this->skillEffectIds[key] = 454;
  key = 758;
  this->skillEffectIds[key] = 454;
  key = 759;
  this->skillEffectIds[key] = 454;
  key = 760;
  this->skillEffectIds[key] = 454;
  key = 762;
  this->skillEffectIds[key] = 454;
  key = 763;
  this->skillEffectIds[key] = 89;
  key = 768;
  this->skillEffectIds[key] = -1;
  key = 5220;
  this->skillEffectIds[key] = 15;
  this->skillFollowEffects.clear();
  key = 0;
  this->skillFollowEffects[key] = 0;
  key = 1;
  this->skillFollowEffects[key] = 1;
  key = 2;
  this->skillFollowEffects[key] = 1;
  key = 3;
  this->skillFollowEffects[key] = 1;
  key = 4;
  this->skillFollowEffects[key] = 1;
  key = 5;
  this->skillFollowEffects[key] = 1;
  key = 6;
  this->skillFollowEffects[key] = 1;
  key = 7;
  this->skillFollowEffects[key] = 17;
  key = 8202;
  this->skillFollowEffects[key] = 17;
  key = 8;
  this->skillFollowEffects[key] = 1;
  key = 9;
  this->skillFollowEffects[key] = 1;
  key = 10;
  this->skillFollowEffects[key] = 1;
  key = 11;
  this->skillFollowEffects[key] = 1;
  key = 12;
  this->skillFollowEffects[key] = -1;
  key = 13;
  this->skillFollowEffects[key] = 1;
  key = 14;
  this->skillFollowEffects[key] = -1;
  key = 15;
  this->skillFollowEffects[key] = 28;
  key = 16;
  this->skillFollowEffects[key] = 23;
  key = 17;
  this->skillFollowEffects[key] = 1;
  key = 18;
  this->skillFollowEffects[key] = -1;
  key = 19;
  this->skillFollowEffects[key] = -1;
  key = 20;
  this->skillFollowEffects[key] = -1;
  key = 21;
  this->skillFollowEffects[key] = -1;
  key = 22;
  this->skillFollowEffects[key] = 1;
  key = 23;
  this->skillFollowEffects[key] = 1;
  key = 24;
  this->skillFollowEffects[key] = 1;
  key = 25;
  this->skillFollowEffects[key] = 1;
  key = 26;
  this->skillFollowEffects[key] = 1;
  key = 27;
  this->skillFollowEffects[key] = 1;
  key = 28;
  this->skillFollowEffects[key] = 1;
  key = 29;
  this->skillFollowEffects[key] = 1;
  key = 690;
  this->skillFollowEffects[key] = 1;
  key = 30;
  this->skillFollowEffects[key] = 1;
  key = 31;
  this->skillFollowEffects[key] = 1;
  key = 32;
  this->skillFollowEffects[key] = 1;
  key = 33;
  this->skillFollowEffects[key] = 1;
  key = 34;
  this->skillFollowEffects[key] = 1;
  key = 689;
  this->skillFollowEffects[key] = 1;
  key = 35;
  this->skillFollowEffects[key] = 1;
  key = 36;
  this->skillFollowEffects[key] = 1;
  key = 37;
  this->skillFollowEffects[key] = 1;
  key = 38;
  this->skillFollowEffects[key] = 1;
  key = 39;
  this->skillFollowEffects[key] = 1;
  key = 40;
  this->skillFollowEffects[key] = 1;
  key = 41;
  this->skillFollowEffects[key] = 1;
  key = 42;
  this->skillFollowEffects[key] = 10;
  key = 43;
  this->skillFollowEffects[key] = 1;
  key = 44;
  this->skillFollowEffects[key] = 1;
  key = 45;
  this->skillFollowEffects[key] = 1;
  key = 46;
  this->skillFollowEffects[key] = 1;
  key = 499;
  this->skillFollowEffects[key] = 1;
  key = 1009;
  this->skillFollowEffects[key] = 1;
  key = 47;
  this->skillFollowEffects[key] = 1;
  key = 48;
  this->skillFollowEffects[key] = 1;
  key = 49;
  this->skillFollowEffects[key] = 1;
  key = 50;
  this->skillFollowEffects[key] = 1;
  key = 51;
  this->skillFollowEffects[key] = 1;
  key = 52;
  this->skillFollowEffects[key] = 1;
  key = 53;
  this->skillFollowEffects[key] = 1;
  key = 54;
  this->skillFollowEffects[key] = 140;
  key = 55;
  this->skillFollowEffects[key] = -1;
  key = 56;
  this->skillFollowEffects[key] = 81;
  key = 57;
  this->skillFollowEffects[key] = -1;
  key = 58;
  this->skillFollowEffects[key] = 81;
  key = 59;
  this->skillFollowEffects[key] = 3;
  key = 60;
  this->skillFollowEffects[key] = -1;
  key = 61;
  this->skillFollowEffects[key] = -1;
  key = 62;
  this->skillFollowEffects[key] = 73;
  key = 63;
  this->skillFollowEffects[key] = -1;
  key = 64;
  this->skillFollowEffects[key] = -1;
  key = 65;
  this->skillFollowEffects[key] = -1;
  key = 66;
  this->skillFollowEffects[key] = 84;
  key = 67;
  this->skillFollowEffects[key] = 88;
  key = 68;
  this->skillFollowEffects[key] = 152;
  key = 69;
  this->skillFollowEffects[key] = -1;
  key = 694;
  this->skillFollowEffects[key] = -1;
  key = 70;
  this->skillFollowEffects[key] = -1;
  key = 72;
  this->skillFollowEffects[key] = -1;
  key = 73;
  this->skillFollowEffects[key] = -1;
  key = 74;
  this->skillFollowEffects[key] = 76;
  key = 75;
  this->skillFollowEffects[key] = 75;
  key = 76;
  this->skillFollowEffects[key] = 87;
  key = 77;
  this->skillFollowEffects[key] = 152;
  key = 78;
  this->skillFollowEffects[key] = 85;
  key = 79;
  this->skillFollowEffects[key] = 152;
  key = 80;
  this->skillFollowEffects[key] = 49;
  key = 81;
  this->skillFollowEffects[key] = 49;
  key = 83;
  this->skillFollowEffects[key] = 49;
  key = 84;
  this->skillFollowEffects[key] = 94;
  key = 341;
  this->skillFollowEffects[key] = 452;
  key = 85;
  this->skillFollowEffects[key] = 52;
  key = 88;
  this->skillFollowEffects[key] = 28;
  key = 89;
  this->skillFollowEffects[key] = 51;
  key = 90;
  this->skillFollowEffects[key] = 147;
  key = 91;
  this->skillFollowEffects[key] = 147;
  key = 92;
  this->skillFollowEffects[key] = 147;
  key = 93;
  this->skillFollowEffects[key] = 60;
  key = 115;
  this->skillFollowEffects[key] = 73;
  key = 116;
  this->skillFollowEffects[key] = -1;
  key = 117;
  this->skillFollowEffects[key] = -1;
  key = 118;
  this->skillFollowEffects[key] = -1;
  key = 119;
  this->skillFollowEffects[key] = -1;
  key = 688;
  this->skillFollowEffects[key] = -1;
  key = 692;
  this->skillFollowEffects[key] = -1;
  key = 698;
  this->skillFollowEffects[key] = -1;
  key = 693;
  this->skillFollowEffects[key] = -1;
  key = 695;
  this->skillFollowEffects[key] = -1;
  key = 120;
  this->skillFollowEffects[key] = -1;
  key = 121;
  this->skillFollowEffects[key] = -1;
  key = 122;
  this->skillFollowEffects[key] = -1;
  key = 123;
  this->skillFollowEffects[key] = -1;
  key = 124;
  this->skillFollowEffects[key] = -1;
  key = 125;
  this->skillFollowEffects[key] = -1;
  key = 126;
  this->skillFollowEffects[key] = -1;
  key = 127;
  this->skillFollowEffects[key] = -1;
  key = 128;
  this->skillFollowEffects[key] = -1;
  key = 129;
  this->skillFollowEffects[key] = 115;
  key = 130;
  this->skillFollowEffects[key] = -1;
  key = 131;
  this->skillFollowEffects[key] = -1;
  key = 132;
  this->skillFollowEffects[key] = -1;
  key = 133;
  this->skillFollowEffects[key] = -1;
  key = 134;
  this->skillFollowEffects[key] = -1;
  key = 135;
  this->skillFollowEffects[key] = 1;
  key = 136;
  this->skillFollowEffects[key] = 122;
  key = 2005;
  this->skillFollowEffects[key] = 122;
  key = 137;
  this->skillFollowEffects[key] = 132;
  key = 138;
  this->skillFollowEffects[key] = -1;
  key = 139;
  this->skillFollowEffects[key] = -1;
  key = 140;
  this->skillFollowEffects[key] = 124;
  key = 141;
  this->skillFollowEffects[key] = -1;
  key = 94;
  this->skillFollowEffects[key] = -1;
  key = 95;
  this->skillFollowEffects[key] = -1;
  key = 96;
  this->skillFollowEffects[key] = -1;
  key = 97;
  this->skillFollowEffects[key] = -1;
  key = 98;
  this->skillFollowEffects[key] = -1;
  key = 99;
  this->skillFollowEffects[key] = -1;
  key = 100;
  this->skillFollowEffects[key] = -1;
  key = 101;
  this->skillFollowEffects[key] = -1;
  key = 102;
  this->skillFollowEffects[key] = -1;
  key = 103;
  this->skillFollowEffects[key] = -1;
  key = 104;
  this->skillFollowEffects[key] = -1;
  key = 105;
  this->skillFollowEffects[key] = -1;
  key = 106;
  this->skillFollowEffects[key] = -1;
  key = 107;
  this->skillFollowEffects[key] = -1;
  key = 108;
  this->skillFollowEffects[key] = -1;
  key = 109;
  this->skillFollowEffects[key] = -1;
  key = 110;
  this->skillFollowEffects[key] = -1;
  key = 111;
  this->skillFollowEffects[key] = -1;
  key = 112;
  this->skillFollowEffects[key] = -1;
  key = 113;
  this->skillFollowEffects[key] = -1;
  key = 114;
  this->skillFollowEffects[key] = -1;
  key = 141;
  this->skillFollowEffects[key] = -1;
  key = 143;
  this->skillFollowEffects[key] = -1;
  key = 153;
  this->skillFollowEffects[key] = 170;
  key = 159;
  this->skillFollowEffects[key] = 181;
  key = 192;
  this->skillFollowEffects[key] = 182;
  key = 158;
  this->skillFollowEffects[key] = -1;
  key = 160;
  this->skillFollowEffects[key] = -1;
  key = 161;
  this->skillFollowEffects[key] = -1;
  key = 162;
  this->skillFollowEffects[key] = -1;
  key = 163;
  this->skillFollowEffects[key] = -1;
  key = 164;
  this->skillFollowEffects[key] = -1;
  key = 165;
  this->skillFollowEffects[key] = -1;
  key = 166;
  this->skillFollowEffects[key] = -1;
  key = 167;
  this->skillFollowEffects[key] = -1;
  key = 168;
  this->skillFollowEffects[key] = -1;
  key = 169;
  this->skillFollowEffects[key] = -1;
  key = 171;
  this->skillFollowEffects[key] = 186;
  key = 172;
  this->skillFollowEffects[key] = 191;
  key = 173;
  this->skillFollowEffects[key] = -1;
  key = 174;
  this->skillFollowEffects[key] = -1;
  key = 175;
  this->skillFollowEffects[key] = -1;
  key = 176;
  this->skillFollowEffects[key] = 125;
  key = 177;
  this->skillFollowEffects[key] = -1;
  key = 178;
  this->skillFollowEffects[key] = 193;
  key = 663;
  this->skillFollowEffects[key] = -1;
  key = 179;
  this->skillFollowEffects[key] = 194;
  key = 180;
  this->skillFollowEffects[key] = 195;
  key = 181;
  this->skillFollowEffects[key] = 196;
  key = 182;
  this->skillFollowEffects[key] = 197;
  key = 183;
  this->skillFollowEffects[key] = -1;
  key = 184;
  this->skillFollowEffects[key] = 51;
  key = 185;
  this->skillFollowEffects[key] = 147;
  key = 186;
  this->skillFollowEffects[key] = 49;
  key = 654;
  this->skillFollowEffects[key] = 49;
  key = 662;
  this->skillFollowEffects[key] = -1;
  key = 187;
  this->skillFollowEffects[key] = 52;
  key = 188;
  this->skillFollowEffects[key] = 192;
  key = 189;
  this->skillFollowEffects[key] = 152;
  key = 190;
  this->skillFollowEffects[key] = 184;
  key = 347;
  this->skillFollowEffects[key] = -1;
  key = 191;
  this->skillFollowEffects[key] = 198;
  key = 199;
  this->skillFollowEffects[key] = 216;
  key = 200;
  this->skillFollowEffects[key] = 217;
  key = 202;
  this->skillFollowEffects[key] = 212;
  key = 2210;
  this->skillFollowEffects[key] = 216;
  key = 267;
  this->skillFollowEffects[key] = 1;
  key = 273;
  this->skillFollowEffects[key] = 1;
  key = 156;
  this->skillFollowEffects[key] = 152;
  key = 272;
  this->skillFollowEffects[key] = 122;
  key = 2327;
  this->skillFollowEffects[key] = -1;
  key = 2329;
  this->skillFollowEffects[key] = 1;
  key = 2518;
  this->skillFollowEffects[key] = 1;
  key = 371;
  this->skillFollowEffects[key] = 1;
  key = 372;
  this->skillFollowEffects[key] = 1;
  key = 381;
  this->skillFollowEffects[key] = 115;
  key = 382;
  this->skillFollowEffects[key] = 1;
  key = 394;
  this->skillFollowEffects[key] = 1;
  key = 397;
  this->skillFollowEffects[key] = 81;
  key = 400;
  this->skillFollowEffects[key] = -1;
  key = 485;
  this->skillFollowEffects[key] = -1;
  key = 1001;
  this->skillFollowEffects[key] = 1;
  key = 1016;
  this->skillFollowEffects[key] = 2;
  key = 534;
  this->skillFollowEffects[key] = 49;
  key = 537;
  this->skillFollowEffects[key] = 51;
  key = 2038;
  this->skillFollowEffects[key] = -1;
  key = 2246;
  this->skillFollowEffects[key] = -1;
  key = 2237;
  this->skillFollowEffects[key] = -1;
  key = 2033;
  this->skillFollowEffects[key] = 1;
  key = 2022;
  this->skillFollowEffects[key] = 122;
  key = 2031;
  this->skillFollowEffects[key] = 1;
  key = 2270;
  this->skillFollowEffects[key] = -1;
  key = 2204;
  this->skillFollowEffects[key] = 51;
  key = 2203;
  this->skillFollowEffects[key] = 51;
  key = 2304;
  this->skillFollowEffects[key] = 183;
  key = 2309;
  this->skillFollowEffects[key] = -1;
  key = 2448;
  this->skillFollowEffects[key] = 1;
  key = 2449;
  this->skillFollowEffects[key] = 182;
  key = 736;
  this->skillFollowEffects[key] = 182;
  key = 2447;
  this->skillFollowEffects[key] = 51;
  key = 720;
  this->skillFollowEffects[key] = 51;
  key = 723;
  this->skillFollowEffects[key] = 51;
  key = 724;
  this->skillFollowEffects[key] = -1;
  key = 722;
  this->skillFollowEffects[key] = 1052;
  key = 721;
  this->skillFollowEffects[key] = -1;
  key = 723;
  this->skillFollowEffects[key] = 51;
  key = 742;
  this->skillFollowEffects[key] = 1;
  key = 757;
  this->skillFollowEffects[key] = -1;
  key = 768;
  this->skillFollowEffects[key] = -1;
  key = 5215;
  this->skillFollowEffects[key] = 1737;
  key = 5214;
  this->skillFollowEffects[key] = 1740;
  key = 5220;
  this->skillFollowEffects[key] = 1744;
  key = 5233;
  this->skillFollowEffects[key] = 1749;
  key = 5234;
  this->skillFollowEffects[key] = 1737;
  key = 5235;
  this->skillFollowEffects[key] = 1759;
  key = 5236;
  this->skillFollowEffects[key] = 1759;
  key = 5237;
  this->skillFollowEffects[key] = 1784;
  key = 5287;
  this->skillFollowEffects[key] = 1838;
  key = 5277;
  this->skillFollowEffects[key] = 1848;
  key = 5298;
  this->skillFollowEffects[key] = 1842;
  key = 5295;
  this->skillFollowEffects[key] = 1939;
  key = 5296;
  this->skillFollowEffects[key] = 1886;
  key = 5241;
  this->skillFollowEffects[key] = 1990;
  key = 5244;
  this->skillFollowEffects[key] = 1997;
  key = 5225;
  this->skillFollowEffects[key] = 1858;
  key = 5226;
  this->skillFollowEffects[key] = 1858;
  key = 5245;
  this->skillFollowEffects[key] = 2053;
  key = 5249;
  this->skillFollowEffects[key] = 2080;
  key = 5250;
  this->skillFollowEffects[key] = 2082;
  key = 5294;
  this->skillFollowEffects[key] = 1866;
  key = 5289;
  this->skillFollowEffects[key] = 1914;
  key = 5314;
  this->skillFollowEffects[key] = 1967;
  key = 5329;
  this->skillFollowEffects[key] = 1970;
  key = 5316;
  this->skillFollowEffects[key] = 2017;
  key = 5382;
  this->skillFollowEffects[key] = 2016;
  key = 8457;
  this->skillFollowEffects[key] = 2012;
  key = 8448;
  this->skillFollowEffects[key] = 2023;
  key = 5369;
  this->skillFollowEffects[key] = 2030;
  key = 5371;
  this->skillFollowEffects[key] = 2034;
  key = 8445;
  this->skillFollowEffects[key] = 2039;
  key = 5370;
  this->skillFollowEffects[key] = 2045;
  key = 5372;
  this->skillFollowEffects[key] = 2059;
  key = 5357;
  this->skillFollowEffects[key] = 2063;
  key = 5319;
  this->skillFollowEffects[key] = 2093;
  key = 8451;
  this->skillFollowEffects[key] = 2102;
  key = 8454;
  this->skillFollowEffects[key] = 2107;
  key = 5383;
  this->skillFollowEffects[key] = 2111;
  key = 5320;
  this->skillFollowEffects[key] = 1967;
  key = 5321;
  this->skillFollowEffects[key] = 2131;
  key = 5373;
  this->skillFollowEffects[key] = 2137;
  key = 5389;
  this->skillFollowEffects[key] = 2141;
  key = 5392;
  this->skillFollowEffects[key] = 2144;
  key = 5393;
  this->skillFollowEffects[key] = 2147;
  key = 5390;
  this->skillFollowEffects[key] = 2150;
  key = 5391;
  this->skillFollowEffects[key] = 2153;
  key = 5315;
  this->skillFollowEffects[key] = 2156;
  key = 5343;
  this->skillFollowEffects[key] = 2207;
  key = 5310;
  this->skillFollowEffects[key] = 2207;
  key = 5340;
  this->skillFollowEffects[key] = 2215;
  key = 5307;
  this->skillFollowEffects[key] = 2215;
  key = 5342;
  this->skillFollowEffects[key] = 2223;
  key = 5309;
  this->skillFollowEffects[key] = 2223;
  key = 5341;
  this->skillFollowEffects[key] = 2231;
  key = 5308;
  this->skillFollowEffects[key] = 2231;
  key = 5384;
  this->skillFollowEffects[key] = 2253;
  key = 5385;
  this->skillFollowEffects[key] = 2251;
  key = 783;
  this->skillFollowEffects[key] = 2304;
  key = 780;
  this->skillFollowEffects[key] = 2308;
  key = 8054;
  this->skillFollowEffects[key] = 2325;
  key = 8048;
  this->skillFollowEffects[key] = 2328;
  key = 8047;
  this->skillFollowEffects[key] = 2330;
  key = 8056;
  this->skillFollowEffects[key] = 2336;
  key = 8050;
  this->skillFollowEffects[key] = 2325;
  key = 8044;
  this->skillFollowEffects[key] = 2342;
}
