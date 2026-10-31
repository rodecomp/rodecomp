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
#include "UI/Window/UIBasicInfoWnd.h"
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

// OFFSET: 0x00E49200
// FUNCTION: fn_24231
void CSession::SetJob(int job) {
  const int previous = this->m_job;
  this->m_job = job;
  if (this->IsTreeGroupThree(previous) != this->IsTreeGroupThree(job)) {
    if (g_windowMgr.unknown_01DC) {
      reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.unknown_01DC)->RefreshLayout();
    }

    if (g_modeMgr.GetGameMode()) {
      g_modeMgr.GetGameMode()->SendMsg(0x144, 0, 0, 0, 0);
    }
  }

  this->InvalidateWindowFrames();
  this->RefreshSkillTree();
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

// OFFSET: 0x00E1A5E0
// FUNCTION: fn_23849
void CSession::InitUnknownMapTables() {
  int key;

  this->unknown_0708.clear();
  key = 0;
  this->unknown_0708[key] = -1;
  key = 1;
  this->unknown_0708[key] = -1;
  key = 2;
  this->unknown_0708[key] = -1;
  key = 3;
  this->unknown_0708[key] = -1;
  key = 4;
  this->unknown_0708[key] = -1;
  key = 5;
  this->unknown_0708[key] = -1;
  key = 6;
  this->unknown_0708[key] = -1;
  key = 7;
  this->unknown_0708[key] = 16;
  key = 8202;
  this->unknown_0700[key] = 16;
  key = 8;
  this->unknown_0708[key] = -1;
  key = 9;
  this->unknown_0708[key] = -1;
  key = 10;
  this->unknown_0708[key] = -1;
  key = 12;
  this->unknown_0708[key] = -1;
  key = 13;
  this->unknown_0708[key] = 15;
  key = 340;
  this->unknown_0708[key] = 451;
  key = 14;
  this->unknown_0708[key] = -1;
  key = 15;
  this->unknown_0708[key] = -1;
  key = 16;
  this->unknown_0708[key] = -1;
  key = 17;
  this->unknown_0708[key] = -1;
  key = 18;
  this->unknown_0708[key] = -1;
  key = 19;
  this->unknown_0708[key] = -1;
  key = 20;
  this->unknown_0708[key] = -1;
  key = 21;
  this->unknown_0708[key] = -1;
  key = 22;
  this->unknown_0708[key] = -1;
  key = 23;
  this->unknown_0708[key] = -1;
  key = 24;
  this->unknown_0708[key] = -1;
  key = 25;
  this->unknown_0708[key] = -1;
  key = 26;
  this->unknown_0708[key] = -1;
  key = 27;
  this->unknown_0708[key] = -1;
  key = 28;
  this->unknown_0708[key] = -1;
  key = 29;
  this->unknown_0708[key] = -1;
  key = 690;
  this->unknown_0708[key] = -1;
  key = 30;
  this->unknown_0708[key] = -1;
  key = 31;
  this->unknown_0708[key] = -1;
  key = 32;
  this->unknown_0708[key] = -1;
  key = 33;
  this->unknown_0708[key] = -1;
  key = 34;
  this->unknown_0708[key] = -1;
  key = 689;
  this->unknown_0708[key] = -1;
  key = 35;
  this->unknown_0708[key] = -1;
  key = 36;
  this->unknown_0708[key] = -1;
  key = 37;
  this->unknown_0708[key] = -1;
  key = 38;
  this->unknown_0708[key] = -1;
  key = 39;
  this->unknown_0708[key] = -1;
  key = 40;
  this->unknown_0708[key] = -1;
  key = 41;
  this->unknown_0708[key] = -1;
  key = 42;
  this->unknown_0708[key] = -1;
  key = 43;
  this->unknown_0708[key] = -1;
  key = 44;
  this->unknown_0708[key] = -1;
  key = 45;
  this->unknown_0708[key] = -1;
  key = 46;
  this->unknown_0708[key] = -1;
  key = 499;
  this->unknown_0708[key] = -1;
  key = 1009;
  this->unknown_0708[key] = -1;
  key = 47;
  this->unknown_0708[key] = -1;
  key = 48;
  this->unknown_0708[key] = -1;
  key = 49;
  this->unknown_0708[key] = -1;
  key = 50;
  this->unknown_0708[key] = 16;
  key = 51;
  this->unknown_0708[key] = 16;
  key = 52;
  this->unknown_0708[key] = 16;
  key = 53;
  this->unknown_0708[key] = 16;
  key = 54;
  this->unknown_0708[key] = -1;
  key = 55;
  this->unknown_0708[key] = -1;
  key = 56;
  this->unknown_0708[key] = -1;
  key = 57;
  this->unknown_0708[key] = -1;
  key = 58;
  this->unknown_0708[key] = -1;
  key = 59;
  this->unknown_0708[key] = -1;
  key = 60;
  this->unknown_0708[key] = -1;
  key = 61;
  this->unknown_0708[key] = -1;
  key = 62;
  this->unknown_0708[key] = -1;
  key = 63;
  this->unknown_0708[key] = -1;
  key = 64;
  this->unknown_0708[key] = -1;
  key = 65;
  this->unknown_0708[key] = -1;
  key = 66;
  this->unknown_0708[key] = -1;
  key = 67;
  this->unknown_0708[key] = -1;
  key = 68;
  this->unknown_0708[key] = -1;
  key = 69;
  this->unknown_0708[key] = -1;
  key = 694;
  this->unknown_0708[key] = -1;
  key = 70;
  this->unknown_0708[key] = -1;
  key = 72;
  this->unknown_0708[key] = -1;
  key = 73;
  this->unknown_0708[key] = -1;
  key = 74;
  this->unknown_0708[key] = -1;
  key = 75;
  this->unknown_0708[key] = -1;
  key = 76;
  this->unknown_0708[key] = -1;
  key = 77;
  this->unknown_0708[key] = -1;
  key = 78;
  this->unknown_0708[key] = -1;
  key = 79;
  this->unknown_0708[key] = -1;
  key = 80;
  this->unknown_0708[key] = 96;
  key = 81;
  this->unknown_0708[key] = 62;
  key = 83;
  this->unknown_0708[key] = -1;
  key = 84;
  this->unknown_0708[key] = 93;
  key = 341;
  this->unknown_0708[key] = 93;
  key = 85;
  this->unknown_0708[key] = -1;
  key = 88;
  this->unknown_0708[key] = -1;
  key = 89;
  this->unknown_0708[key] = 89;
  key = 664;
  this->unknown_0708[key] = 89;
  key = 90;
  this->unknown_0708[key] = -1;
  key = 91;
  this->unknown_0708[key] = -1;
  key = 92;
  this->unknown_0708[key] = -1;
  key = 93;
  this->unknown_0708[key] = -1;
  key = 115;
  this->unknown_0708[key] = -1;
  key = 116;
  this->unknown_0708[key] = -1;
  key = 117;
  this->unknown_0708[key] = -1;
  key = 118;
  this->unknown_0708[key] = -1;
  key = 119;
  this->unknown_0708[key] = -1;
  key = 688;
  this->unknown_0708[key] = -1;
  key = 692;
  this->unknown_0708[key] = -1;
  key = 698;
  this->unknown_0708[key] = -1;
  key = 693;
  this->unknown_0708[key] = -1;
  key = 695;
  this->unknown_0708[key] = -1;
  key = 120;
  this->unknown_0708[key] = -1;
  key = 121;
  this->unknown_0708[key] = -1;
  key = 122;
  this->unknown_0708[key] = -1;
  key = 123;
  this->unknown_0708[key] = -1;
  key = 124;
  this->unknown_0708[key] = -1;
  key = 125;
  this->unknown_0708[key] = -1;
  key = 126;
  this->unknown_0708[key] = -1;
  key = 127;
  this->unknown_0708[key] = -1;
  key = 128;
  this->unknown_0708[key] = -1;
  key = 129;
  this->unknown_0708[key] = -1;
  key = 130;
  this->unknown_0708[key] = -1;
  key = 131;
  this->unknown_0708[key] = -1;
  key = 132;
  this->unknown_0708[key] = -1;
  key = 133;
  this->unknown_0708[key] = -1;
  key = 134;
  this->unknown_0708[key] = -1;
  key = 135;
  this->unknown_0708[key] = -1;
  key = 136;
  this->unknown_0708[key] = -1;
  key = 137;
  this->unknown_0708[key] = -1;
  key = 138;
  this->unknown_0708[key] = -1;
  key = 139;
  this->unknown_0708[key] = -1;
  key = 140;
  this->unknown_0708[key] = -1;
  key = 141;
  this->unknown_0708[key] = -1;
  key = 94;
  this->unknown_0708[key] = -1;
  key = 95;
  this->unknown_0708[key] = -1;
  key = 96;
  this->unknown_0708[key] = -1;
  key = 97;
  this->unknown_0708[key] = -1;
  key = 98;
  this->unknown_0708[key] = -1;
  key = 99;
  this->unknown_0708[key] = -1;
  key = 100;
  this->unknown_0708[key] = -1;
  key = 101;
  this->unknown_0708[key] = -1;
  key = 102;
  this->unknown_0708[key] = -1;
  key = 103;
  this->unknown_0708[key] = -1;
  key = 104;
  this->unknown_0708[key] = -1;
  key = 105;
  this->unknown_0708[key] = -1;
  key = 106;
  this->unknown_0708[key] = -1;
  key = 107;
  this->unknown_0708[key] = -1;
  key = 108;
  this->unknown_0708[key] = 101;
  key = 109;
  this->unknown_0708[key] = -1;
  key = 110;
  this->unknown_0708[key] = -1;
  key = 111;
  this->unknown_0708[key] = -1;
  key = 112;
  this->unknown_0708[key] = 103;
  key = 113;
  this->unknown_0708[key] = -1;
  key = 114;
  this->unknown_0708[key] = 104;
  key = 141;
  this->unknown_0708[key] = -1;
  key = 159;
  this->unknown_0708[key] = -1;
  key = 192;
  this->unknown_0708[key] = -1;
  key = 158;
  this->unknown_0708[key] = -1;
  key = 160;
  this->unknown_0708[key] = -1;
  key = 161;
  this->unknown_0708[key] = -1;
  key = 162;
  this->unknown_0708[key] = 174;
  key = 163;
  this->unknown_0708[key] = 177;
  key = 164;
  this->unknown_0708[key] = 173;
  key = 165;
  this->unknown_0708[key] = 175;
  key = 166;
  this->unknown_0708[key] = 179;
  key = 167;
  this->unknown_0708[key] = 178;
  key = 168;
  this->unknown_0708[key] = 172;
  key = 169;
  this->unknown_0708[key] = 176;
  key = 171;
  this->unknown_0708[key] = -1;
  key = 172;
  this->unknown_0708[key] = -1;
  key = 173;
  this->unknown_0708[key] = 183;
  key = 174;
  this->unknown_0708[key] = -1;
  key = 175;
  this->unknown_0708[key] = -1;
  key = 175;
  this->unknown_0708[key] = 185;
  key = 176;
  this->unknown_0708[key] = -1;
  key = 177;
  this->unknown_0708[key] = -1;
  key = 178;
  this->unknown_0708[key] = -1;
  key = 663;
  this->unknown_0708[key] = 454;
  key = 678;
  this->unknown_0708[key] = 454;
  key = 666;
  this->unknown_0708[key] = 454;
  key = 668;
  this->unknown_0708[key] = 454;
  key = 667;
  this->unknown_0708[key] = 454;
  key = 179;
  this->unknown_0708[key] = -1;
  key = 180;
  this->unknown_0708[key] = -1;
  key = 181;
  this->unknown_0708[key] = -1;
  key = 182;
  this->unknown_0708[key] = -1;
  key = 183;
  this->unknown_0708[key] = -1;
  key = 184;
  this->unknown_0708[key] = -1;
  key = 185;
  this->unknown_0708[key] = -1;
  key = 186;
  this->unknown_0708[key] = -1;
  key = 654;
  this->unknown_0708[key] = -1;
  key = 676;
  this->unknown_0708[key] = -1;
  key = 662;
  this->unknown_0708[key] = -1;
  key = 347;
  this->unknown_0708[key] = -1;
  key = 187;
  this->unknown_0708[key] = -1;
  key = 188;
  this->unknown_0708[key] = -1;
  key = 189;
  this->unknown_0708[key] = -1;
  key = 190;
  this->unknown_0708[key] = -1;
  key = 191;
  this->unknown_0708[key] = -1;
  key = 199;
  this->unknown_0708[key] = -1;
  key = 200;
  this->unknown_0708[key] = -1;
  key = 202;
  this->unknown_0708[key] = -1;
  key = 2246;
  this->unknown_0708[key] = -1;
  key = 2237;
  this->unknown_0708[key] = -1;
  key = 2309;
  this->unknown_0708[key] = -1;
  key = 723;
  this->unknown_0708[key] = 89;
  key = 724;
  this->unknown_0708[key] = 183;
  key = 722;
  this->unknown_0708[key] = -1;
  key = 724;
  this->unknown_0708[key] = 1053;
  key = 723;
  this->unknown_0708[key] = 89;
  key = 757;
  this->unknown_0708[key] = 454;
  key = 758;
  this->unknown_0708[key] = 454;
  key = 759;
  this->unknown_0708[key] = 454;
  key = 760;
  this->unknown_0708[key] = 454;
  key = 762;
  this->unknown_0708[key] = 454;
  key = 763;
  this->unknown_0708[key] = 89;
  key = 768;
  this->unknown_0708[key] = -1;
  key = 5220;
  this->unknown_0708[key] = 15;
  this->unknown_0700.clear();
  key = 0;
  this->unknown_0700[key] = 0;
  key = 1;
  this->unknown_0700[key] = 1;
  key = 2;
  this->unknown_0700[key] = 1;
  key = 3;
  this->unknown_0700[key] = 1;
  key = 4;
  this->unknown_0700[key] = 1;
  key = 5;
  this->unknown_0700[key] = 1;
  key = 6;
  this->unknown_0700[key] = 1;
  key = 7;
  this->unknown_0700[key] = 17;
  key = 8202;
  this->unknown_0700[key] = 17;
  key = 8;
  this->unknown_0700[key] = 1;
  key = 9;
  this->unknown_0700[key] = 1;
  key = 10;
  this->unknown_0700[key] = 1;
  key = 11;
  this->unknown_0700[key] = 1;
  key = 12;
  this->unknown_0700[key] = -1;
  key = 13;
  this->unknown_0700[key] = 1;
  key = 14;
  this->unknown_0700[key] = -1;
  key = 15;
  this->unknown_0700[key] = 28;
  key = 16;
  this->unknown_0700[key] = 23;
  key = 17;
  this->unknown_0700[key] = 1;
  key = 18;
  this->unknown_0700[key] = -1;
  key = 19;
  this->unknown_0700[key] = -1;
  key = 20;
  this->unknown_0700[key] = -1;
  key = 21;
  this->unknown_0700[key] = -1;
  key = 22;
  this->unknown_0700[key] = 1;
  key = 23;
  this->unknown_0700[key] = 1;
  key = 24;
  this->unknown_0700[key] = 1;
  key = 25;
  this->unknown_0700[key] = 1;
  key = 26;
  this->unknown_0700[key] = 1;
  key = 27;
  this->unknown_0700[key] = 1;
  key = 28;
  this->unknown_0700[key] = 1;
  key = 29;
  this->unknown_0700[key] = 1;
  key = 690;
  this->unknown_0700[key] = 1;
  key = 30;
  this->unknown_0700[key] = 1;
  key = 31;
  this->unknown_0700[key] = 1;
  key = 32;
  this->unknown_0700[key] = 1;
  key = 33;
  this->unknown_0700[key] = 1;
  key = 34;
  this->unknown_0700[key] = 1;
  key = 689;
  this->unknown_0700[key] = 1;
  key = 35;
  this->unknown_0700[key] = 1;
  key = 36;
  this->unknown_0700[key] = 1;
  key = 37;
  this->unknown_0700[key] = 1;
  key = 38;
  this->unknown_0700[key] = 1;
  key = 39;
  this->unknown_0700[key] = 1;
  key = 40;
  this->unknown_0700[key] = 1;
  key = 41;
  this->unknown_0700[key] = 1;
  key = 42;
  this->unknown_0700[key] = 10;
  key = 43;
  this->unknown_0700[key] = 1;
  key = 44;
  this->unknown_0700[key] = 1;
  key = 45;
  this->unknown_0700[key] = 1;
  key = 46;
  this->unknown_0700[key] = 1;
  key = 499;
  this->unknown_0700[key] = 1;
  key = 1009;
  this->unknown_0700[key] = 1;
  key = 47;
  this->unknown_0700[key] = 1;
  key = 48;
  this->unknown_0700[key] = 1;
  key = 49;
  this->unknown_0700[key] = 1;
  key = 50;
  this->unknown_0700[key] = 1;
  key = 51;
  this->unknown_0700[key] = 1;
  key = 52;
  this->unknown_0700[key] = 1;
  key = 53;
  this->unknown_0700[key] = 1;
  key = 54;
  this->unknown_0700[key] = 140;
  key = 55;
  this->unknown_0700[key] = -1;
  key = 56;
  this->unknown_0700[key] = 81;
  key = 57;
  this->unknown_0700[key] = -1;
  key = 58;
  this->unknown_0700[key] = 81;
  key = 59;
  this->unknown_0700[key] = 3;
  key = 60;
  this->unknown_0700[key] = -1;
  key = 61;
  this->unknown_0700[key] = -1;
  key = 62;
  this->unknown_0700[key] = 73;
  key = 63;
  this->unknown_0700[key] = -1;
  key = 64;
  this->unknown_0700[key] = -1;
  key = 65;
  this->unknown_0700[key] = -1;
  key = 66;
  this->unknown_0700[key] = 84;
  key = 67;
  this->unknown_0700[key] = 88;
  key = 68;
  this->unknown_0700[key] = 152;
  key = 69;
  this->unknown_0700[key] = -1;
  key = 694;
  this->unknown_0700[key] = -1;
  key = 70;
  this->unknown_0700[key] = -1;
  key = 72;
  this->unknown_0700[key] = -1;
  key = 73;
  this->unknown_0700[key] = -1;
  key = 74;
  this->unknown_0700[key] = 76;
  key = 75;
  this->unknown_0700[key] = 75;
  key = 76;
  this->unknown_0700[key] = 87;
  key = 77;
  this->unknown_0700[key] = 152;
  key = 78;
  this->unknown_0700[key] = 85;
  key = 79;
  this->unknown_0700[key] = 152;
  key = 80;
  this->unknown_0700[key] = 49;
  key = 81;
  this->unknown_0700[key] = 49;
  key = 83;
  this->unknown_0700[key] = 49;
  key = 84;
  this->unknown_0700[key] = 94;
  key = 341;
  this->unknown_0700[key] = 452;
  key = 85;
  this->unknown_0700[key] = 52;
  key = 88;
  this->unknown_0700[key] = 28;
  key = 89;
  this->unknown_0700[key] = 51;
  key = 90;
  this->unknown_0700[key] = 147;
  key = 91;
  this->unknown_0700[key] = 147;
  key = 92;
  this->unknown_0700[key] = 147;
  key = 93;
  this->unknown_0700[key] = 60;
  key = 115;
  this->unknown_0700[key] = 73;
  key = 116;
  this->unknown_0700[key] = -1;
  key = 117;
  this->unknown_0700[key] = -1;
  key = 118;
  this->unknown_0700[key] = -1;
  key = 119;
  this->unknown_0700[key] = -1;
  key = 688;
  this->unknown_0700[key] = -1;
  key = 692;
  this->unknown_0700[key] = -1;
  key = 698;
  this->unknown_0700[key] = -1;
  key = 693;
  this->unknown_0700[key] = -1;
  key = 695;
  this->unknown_0700[key] = -1;
  key = 120;
  this->unknown_0700[key] = -1;
  key = 121;
  this->unknown_0700[key] = -1;
  key = 122;
  this->unknown_0700[key] = -1;
  key = 123;
  this->unknown_0700[key] = -1;
  key = 124;
  this->unknown_0700[key] = -1;
  key = 125;
  this->unknown_0700[key] = -1;
  key = 126;
  this->unknown_0700[key] = -1;
  key = 127;
  this->unknown_0700[key] = -1;
  key = 128;
  this->unknown_0700[key] = -1;
  key = 129;
  this->unknown_0700[key] = 115;
  key = 130;
  this->unknown_0700[key] = -1;
  key = 131;
  this->unknown_0700[key] = -1;
  key = 132;
  this->unknown_0700[key] = -1;
  key = 133;
  this->unknown_0700[key] = -1;
  key = 134;
  this->unknown_0700[key] = -1;
  key = 135;
  this->unknown_0700[key] = 1;
  key = 136;
  this->unknown_0700[key] = 122;
  key = 2005;
  this->unknown_0700[key] = 122;
  key = 137;
  this->unknown_0700[key] = 132;
  key = 138;
  this->unknown_0700[key] = -1;
  key = 139;
  this->unknown_0700[key] = -1;
  key = 140;
  this->unknown_0700[key] = 124;
  key = 141;
  this->unknown_0700[key] = -1;
  key = 94;
  this->unknown_0700[key] = -1;
  key = 95;
  this->unknown_0700[key] = -1;
  key = 96;
  this->unknown_0700[key] = -1;
  key = 97;
  this->unknown_0700[key] = -1;
  key = 98;
  this->unknown_0700[key] = -1;
  key = 99;
  this->unknown_0700[key] = -1;
  key = 100;
  this->unknown_0700[key] = -1;
  key = 101;
  this->unknown_0700[key] = -1;
  key = 102;
  this->unknown_0700[key] = -1;
  key = 103;
  this->unknown_0700[key] = -1;
  key = 104;
  this->unknown_0700[key] = -1;
  key = 105;
  this->unknown_0700[key] = -1;
  key = 106;
  this->unknown_0700[key] = -1;
  key = 107;
  this->unknown_0700[key] = -1;
  key = 108;
  this->unknown_0700[key] = -1;
  key = 109;
  this->unknown_0700[key] = -1;
  key = 110;
  this->unknown_0700[key] = -1;
  key = 111;
  this->unknown_0700[key] = -1;
  key = 112;
  this->unknown_0700[key] = -1;
  key = 113;
  this->unknown_0700[key] = -1;
  key = 114;
  this->unknown_0700[key] = -1;
  key = 141;
  this->unknown_0700[key] = -1;
  key = 143;
  this->unknown_0700[key] = -1;
  key = 153;
  this->unknown_0700[key] = 170;
  key = 159;
  this->unknown_0700[key] = 181;
  key = 192;
  this->unknown_0700[key] = 182;
  key = 158;
  this->unknown_0700[key] = -1;
  key = 160;
  this->unknown_0700[key] = -1;
  key = 161;
  this->unknown_0700[key] = -1;
  key = 162;
  this->unknown_0700[key] = -1;
  key = 163;
  this->unknown_0700[key] = -1;
  key = 164;
  this->unknown_0700[key] = -1;
  key = 165;
  this->unknown_0700[key] = -1;
  key = 166;
  this->unknown_0700[key] = -1;
  key = 167;
  this->unknown_0700[key] = -1;
  key = 168;
  this->unknown_0700[key] = -1;
  key = 169;
  this->unknown_0700[key] = -1;
  key = 171;
  this->unknown_0700[key] = 186;
  key = 172;
  this->unknown_0700[key] = 191;
  key = 173;
  this->unknown_0700[key] = -1;
  key = 174;
  this->unknown_0700[key] = -1;
  key = 175;
  this->unknown_0700[key] = -1;
  key = 176;
  this->unknown_0700[key] = 125;
  key = 177;
  this->unknown_0700[key] = -1;
  key = 178;
  this->unknown_0700[key] = 193;
  key = 663;
  this->unknown_0700[key] = -1;
  key = 179;
  this->unknown_0700[key] = 194;
  key = 180;
  this->unknown_0700[key] = 195;
  key = 181;
  this->unknown_0700[key] = 196;
  key = 182;
  this->unknown_0700[key] = 197;
  key = 183;
  this->unknown_0700[key] = -1;
  key = 184;
  this->unknown_0700[key] = 51;
  key = 185;
  this->unknown_0700[key] = 147;
  key = 186;
  this->unknown_0700[key] = 49;
  key = 654;
  this->unknown_0700[key] = 49;
  key = 662;
  this->unknown_0700[key] = -1;
  key = 187;
  this->unknown_0700[key] = 52;
  key = 188;
  this->unknown_0700[key] = 192;
  key = 189;
  this->unknown_0700[key] = 152;
  key = 190;
  this->unknown_0700[key] = 184;
  key = 347;
  this->unknown_0700[key] = -1;
  key = 191;
  this->unknown_0700[key] = 198;
  key = 199;
  this->unknown_0700[key] = 216;
  key = 200;
  this->unknown_0700[key] = 217;
  key = 202;
  this->unknown_0700[key] = 212;
  key = 2210;
  this->unknown_0700[key] = 216;
  key = 267;
  this->unknown_0700[key] = 1;
  key = 273;
  this->unknown_0700[key] = 1;
  key = 156;
  this->unknown_0700[key] = 152;
  key = 272;
  this->unknown_0700[key] = 122;
  key = 2327;
  this->unknown_0700[key] = -1;
  key = 2329;
  this->unknown_0700[key] = 1;
  key = 2518;
  this->unknown_0700[key] = 1;
  key = 371;
  this->unknown_0700[key] = 1;
  key = 372;
  this->unknown_0700[key] = 1;
  key = 381;
  this->unknown_0700[key] = 115;
  key = 382;
  this->unknown_0700[key] = 1;
  key = 394;
  this->unknown_0700[key] = 1;
  key = 397;
  this->unknown_0700[key] = 81;
  key = 400;
  this->unknown_0700[key] = -1;
  key = 485;
  this->unknown_0700[key] = -1;
  key = 1001;
  this->unknown_0700[key] = 1;
  key = 1016;
  this->unknown_0700[key] = 2;
  key = 534;
  this->unknown_0700[key] = 49;
  key = 537;
  this->unknown_0700[key] = 51;
  key = 2038;
  this->unknown_0700[key] = -1;
  key = 2246;
  this->unknown_0700[key] = -1;
  key = 2237;
  this->unknown_0700[key] = -1;
  key = 2033;
  this->unknown_0700[key] = 1;
  key = 2022;
  this->unknown_0700[key] = 122;
  key = 2031;
  this->unknown_0700[key] = 1;
  key = 2270;
  this->unknown_0700[key] = -1;
  key = 2204;
  this->unknown_0700[key] = 51;
  key = 2203;
  this->unknown_0700[key] = 51;
  key = 2304;
  this->unknown_0700[key] = 183;
  key = 2309;
  this->unknown_0700[key] = -1;
  key = 2448;
  this->unknown_0700[key] = 1;
  key = 2449;
  this->unknown_0700[key] = 182;
  key = 736;
  this->unknown_0700[key] = 182;
  key = 2447;
  this->unknown_0700[key] = 51;
  key = 720;
  this->unknown_0700[key] = 51;
  key = 723;
  this->unknown_0700[key] = 51;
  key = 724;
  this->unknown_0700[key] = -1;
  key = 722;
  this->unknown_0700[key] = 1052;
  key = 721;
  this->unknown_0700[key] = -1;
  key = 723;
  this->unknown_0700[key] = 51;
  key = 742;
  this->unknown_0700[key] = 1;
  key = 757;
  this->unknown_0700[key] = -1;
  key = 768;
  this->unknown_0700[key] = -1;
  key = 5215;
  this->unknown_0700[key] = 1737;
  key = 5214;
  this->unknown_0700[key] = 1740;
  key = 5220;
  this->unknown_0700[key] = 1744;
  key = 5233;
  this->unknown_0700[key] = 1749;
  key = 5234;
  this->unknown_0700[key] = 1737;
  key = 5235;
  this->unknown_0700[key] = 1759;
  key = 5236;
  this->unknown_0700[key] = 1759;
  key = 5237;
  this->unknown_0700[key] = 1784;
  key = 5287;
  this->unknown_0700[key] = 1838;
  key = 5277;
  this->unknown_0700[key] = 1848;
  key = 5298;
  this->unknown_0700[key] = 1842;
  key = 5295;
  this->unknown_0700[key] = 1939;
  key = 5296;
  this->unknown_0700[key] = 1886;
  key = 5241;
  this->unknown_0700[key] = 1990;
  key = 5244;
  this->unknown_0700[key] = 1997;
  key = 5225;
  this->unknown_0700[key] = 1858;
  key = 5226;
  this->unknown_0700[key] = 1858;
  key = 5245;
  this->unknown_0700[key] = 2053;
  key = 5249;
  this->unknown_0700[key] = 2080;
  key = 5250;
  this->unknown_0700[key] = 2082;
  key = 5294;
  this->unknown_0700[key] = 1866;
  key = 5289;
  this->unknown_0700[key] = 1914;
  key = 5314;
  this->unknown_0700[key] = 1967;
  key = 5329;
  this->unknown_0700[key] = 1970;
  key = 5316;
  this->unknown_0700[key] = 2017;
  key = 5382;
  this->unknown_0700[key] = 2016;
  key = 8457;
  this->unknown_0700[key] = 2012;
  key = 8448;
  this->unknown_0700[key] = 2023;
  key = 5369;
  this->unknown_0700[key] = 2030;
  key = 5371;
  this->unknown_0700[key] = 2034;
  key = 8445;
  this->unknown_0700[key] = 2039;
  key = 5370;
  this->unknown_0700[key] = 2045;
  key = 5372;
  this->unknown_0700[key] = 2059;
  key = 5357;
  this->unknown_0700[key] = 2063;
  key = 5319;
  this->unknown_0700[key] = 2093;
  key = 8451;
  this->unknown_0700[key] = 2102;
  key = 8454;
  this->unknown_0700[key] = 2107;
  key = 5383;
  this->unknown_0700[key] = 2111;
  key = 5320;
  this->unknown_0700[key] = 1967;
  key = 5321;
  this->unknown_0700[key] = 2131;
  key = 5373;
  this->unknown_0700[key] = 2137;
  key = 5389;
  this->unknown_0700[key] = 2141;
  key = 5392;
  this->unknown_0700[key] = 2144;
  key = 5393;
  this->unknown_0700[key] = 2147;
  key = 5390;
  this->unknown_0700[key] = 2150;
  key = 5391;
  this->unknown_0700[key] = 2153;
  key = 5315;
  this->unknown_0700[key] = 2156;
  key = 5343;
  this->unknown_0700[key] = 2207;
  key = 5310;
  this->unknown_0700[key] = 2207;
  key = 5340;
  this->unknown_0700[key] = 2215;
  key = 5307;
  this->unknown_0700[key] = 2215;
  key = 5342;
  this->unknown_0700[key] = 2223;
  key = 5309;
  this->unknown_0700[key] = 2223;
  key = 5341;
  this->unknown_0700[key] = 2231;
  key = 5308;
  this->unknown_0700[key] = 2231;
  key = 5384;
  this->unknown_0700[key] = 2253;
  key = 5385;
  this->unknown_0700[key] = 2251;
  key = 783;
  this->unknown_0700[key] = 2304;
  key = 780;
  this->unknown_0700[key] = 2308;
  key = 8054;
  this->unknown_0700[key] = 2325;
  key = 8048;
  this->unknown_0700[key] = 2328;
  key = 8047;
  this->unknown_0700[key] = 2330;
  key = 8056;
  this->unknown_0700[key] = 2336;
  key = 8050;
  this->unknown_0700[key] = 2325;
  key = 8044;
  this->unknown_0700[key] = 2342;
}
