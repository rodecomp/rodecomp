#include "Client/Session.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/World/World.h"
#include "Script/LuaScript.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

extern "C" {
void lua_settop(lua_State* L, int index);
int lua_type(lua_State* L, int index);
double lua_tonumber(lua_State* L, int index);
void lua_pushnil(lua_State* L);
void lua_pushnumber(lua_State* L, double value);
void lua_pushstring(lua_State* L, const char* text);
void lua_gettable(lua_State* L, int index);
void lua_getfield(lua_State* L, int index, const char* key);
int lua_next(lua_State* L, int index);
}

constexpr int LUA_GLOBALSINDEX = -10002;
constexpr int LUA_TTABLE = 5;

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
          skillFlag = static_cast<int>(skillFlag + lua_tonumber(L, -1));
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
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(this->unknown_5BC0)
    );
    int delay = 0;
    if (actor) {
      switch (actor->GetEffectiveJob(-1)) {
      case 2114:
      case 2117:
      case 2120:
      case 2123:
        delay = 3000;
        break;
      case 2115:
      case 2118:
      case 2121:
      case 2124:
        delay = 6000;
        break;
      case 2116:
      case 2119:
      case 2122:
      case 2125:
      case 20816:
      case 20817:
      case 20818:
      case 20819:
      case 20820:
        delay = 9000;
        break;
      }
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
bool CSession::EquipItem61B8(const ITEM_INFO& item) {
  const EquipSlots slots = g_session.GetEquipSlots(static_cast<int>(item.unknown_0000[3]));
  int order[3];
  order[0] = slots.values[2];
  order[1] = slots.values[1];
  order[2] = slots.values[0];
  if (item.unknown_0000[3] == 0x8000) {
    g_session.SetUnknown7EC8(item.unknown_0000[1]);
  }

  for (int i = 0; i < 3; ++i) {
    const int slot = order[i];
    if (slot == -1 || slot >= 10) {
      break;
    }
    this->unknown_61B8[slot] = item;
    this->unknown_61B8[slot].quantity = 1;
  }

  if (g_windowMgr.unknown_01E0) {
    static_cast<UIEquipWnd*>(g_windowMgr.unknown_01E0)->NotifyEquipmentChanged();
  }
  RefreshItemWindows(&g_windowMgr);
  return true;
}

// OFFSET: 0x00E3D8A0
// FUNCTION: fn_24192
void CSession::UnequipItem6B68(const ITEM_INFO& item) {
  const int location = static_cast<int>(item.unknown_0000[3]);
  if (location == 0x8000) {
    this->SetUnknown7EC8(0);
  } else {
    const EquipSlots slots = g_session.GetEquipSlots(location);
    if (slots.values[2] < 10) {
      if (slots.values[2] != -1) {
        this->unknown_6B68[slots.values[2]].unknown_0000[3] = 0;
        this->unknown_6B68[slots.values[2]].Reset();
      }
      if (slots.values[1] != -1 && slots.values[1] < 10) {
        this->unknown_6B68[slots.values[1]].Reset();
      }
      if (slots.values[0] != -1 && slots.values[0] < 10) {
        this->unknown_6B68[slots.values[0]].Reset();
      }
    }
  }

  if (g_windowMgr.unknown_01E0) {
    static_cast<UIEquipWnd*>(g_windowMgr.unknown_01E0)->NotifyEquipmentChanged();
  }
  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00E3D960
// FUNCTION: fn_24193
void CSession::UnequipItem61B8(const ITEM_INFO& item) {
  const int location = static_cast<int>(item.unknown_0000[3]);
  if (location == 0x8000) {
    ITEM_INFO equipped = g_session.FindItemByIdentity(item.unknown_0000[1], location);
    if (equipped.unknown_0000[1] != 0) {
      const char* message = LookupMessage(0xAB);
      ItemDescriptions descriptions{g_itemDescriptions};
      bool languagePrefix = false;
      WORD language = g_serverLanguage->language;
      ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
      const std::string name = naming.DisplayName(equipped, 0);
      const std::string text = FormatOwnedString("%s%s", name.c_str(), message);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFF, 7, 0, 0);
    }
    this->SetUnknown7EC8(0);
  } else {
    const EquipSlots slots = g_session.GetEquipSlots(location);
    if (slots.values[2] < 10) {
      if (slots.values[2] != -1) {
        this->unknown_61B8[slots.values[2]].unknown_0000[3] = 0;
        this->unknown_61B8[slots.values[2]].Reset();
      }
      if (slots.values[1] != -1 && slots.values[1] < 10) {
        this->unknown_61B8[slots.values[1]].Reset();
      }
      if (slots.values[0] != -1 && slots.values[0] < 10) {
        this->unknown_61B8[slots.values[0]].Reset();
      }
    }
  }

  if (g_windowMgr.unknown_01E0) {
    static_cast<UIEquipWnd*>(g_windowMgr.unknown_01E0)->NotifyEquipmentChanged();
  }
  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00E497F0
// FUNCTION: fn_24237
void CSession::SetUnknown7EC8(std::uint32_t value) {
  this->unknown_7EC8 = value;
  this->InvalidateWindowFrames();

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!mode) {
    return;
  }

  auto it = g_activeSkillEffects.begin();
  for (; it != g_activeSkillEffects.end(); ++it) {
    if (it->id == 0x2B7) {
      break;
    }
  }

  if (it == g_activeSkillEffects.end()) {
    if (value) {
      ActiveSkillEffect effect;
      effect.id = 0x2B7;
      g_activeSkillEffects.push_back(effect);
    }
  } else if (!value) {
    g_activeSkillEffects.erase(it);
  }

  mode->scene->player->unknown_0544->SendMsg(nullptr, 44, 0, 1, 0, 0, 0, 0);
}

// OFFSET: 0x00E49930
// FUNCTION: fn_24238
void CSession::NotifyUnknown7F94Name(std::uint32_t id, const char* name) {
  if (!name || std::strlen(name) == 0) {
    return;
  }

  UIWindow* chat = g_windowMgr.window_01C8;
  if (!chat) {
    return;
  }

  if (this->unknown_7F94.find(id) == this->unknown_7F94.end()) {
    return;
  }

  chat->SendMsg(nullptr, 0x9A, reinterpret_cast<std::intptr_t>(name), 0, 0, 0);
  g_windowMgr.SendNotice(0x9A, reinterpret_cast<std::intptr_t>(name), 0, 0, 0, 0);
  this->unknown_7F94.erase(id);
}
