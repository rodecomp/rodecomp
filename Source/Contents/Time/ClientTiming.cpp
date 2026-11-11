#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Session/Session.h"
#include "Contents/Achievement/AchievementMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Map/MapInfoMgr.h"
#include "Contents/Navigation/NavigationManager.h"
#include "File/CommonObject.h"
#include "File/File.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/LocalEncryptedName.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <timeapi.h>
#include <windows.h>
#include "Utility/StdStringCompare.h"

#include "Utility/StdTraitsEqual.h"

// GLOBAL: gl_24705
int g_serviceMode;

namespace {

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptName(const char (&source)[Length]) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

}

struct NaviUnknownBlock {
  /* 0000 */ int padding_0000;
  /* 0004 */ int padding_0004;
  /* 0008 */ int padding_0008;
  /* 000C */ int padding_000C;
  /* 0010 */ int padding_0010;
  /* 0014 */ int padding_0014;
  /* 0018 */ int padding_0018;
  /* 001C */ int padding_001C;
  /* 0020 */ int padding_0020;

  NaviUnknownBlock();
};

// OFFSET: 0x00B8C870
// FUNCTION: fn_19426
CNavi_Object::CNavi_Object()
    : cellX(0), cellY(0), job(0), sex(0), bodyPalette(0), head(0), headPalette(0),
      accessory0(0), accessory1(0), accessory2(0), robe(0) {
  this->id = -1;
  this->type = 101;
  this->index = 0;
  this->cellX = 0;
  this->cellY = 0;
  this->info = nullptr;
}

// OFFSET: 0x00B8C990
// FUNCTION: fn_19427
NaviUnknownBlock::NaviUnknownBlock()
    : padding_0000(0), padding_0004(0), padding_0008(0), padding_000C(0), padding_0010(0), padding_0014(0), padding_0018(0), padding_001C(0),
      padding_0020(0) {
}

// OFFSET: 0x00B8C710
// FUNCTION: fn_19425
CNavi_Object::CNavi_Object(int type, int index, const char* name, const char* title, int cellX, int cellY, CNaviInfo* info)
    : cellX(0), cellY(0), job(0), sex(0), bodyPalette(0), head(0), headPalette(0), accessory0(0),
      accessory1(0), accessory2(0), robe(0) {
  this->type = type;
  this->id = -1;
  this->index = index;
  if (name) {
    this->name = name;
  }
  if (title) {
    this->title = title;
  }
  this->cellX = cellX;
  this->cellY = cellY;
  this->info = info;
}

// OFFSET: 0x00B8C520
// FUNCTION: fn_19423
CNavi_Link::CNavi_Link() : linkedCellX(0), linkedCellY(0) {
  this->type = 200;
  this->linkedInfo = nullptr;
  this->linkedCellX = 0;
  this->linkedCellY = 0;
  this->depth = -1;
}

// OFFSET: 0x00B8C2B0
// FUNCTION: fn_19422
CNavi_Link::CNavi_Link(
  int type, int index, const char* name, const char* title, int cellX, int cellY, CNaviInfo* info, CNaviInfo* linkedInfo, int linkedCellX, int linkedCellY
)
    : linkedCellX(0), linkedCellY(0) {
  this->type = type;
  this->index = index;
  if (name) {
    this->name = name;
  }
  if (title) {
    this->title = title;
  }
  this->cellX = cellX;
  this->cellY = cellY;
  this->info = info;
  this->linkedInfo = linkedInfo;
  this->linkedCellX = linkedCellX;
  this->linkedCellY = linkedCellY;
  this->depth = 0;
}

// OFFSET: 0x00B8C5E0
// FUNCTION: fn_19424
CNaviInfo::CNaviInfo(int index, const char* name, const char* title, int cellX, int cellY, int type) {
  this->index = index;
  this->type = type;
  this->name = name;
  this->title = title;
  this->cellX = cellX;
  this->cellY = cellY;
  this->visited = false;
}

// OFFSET: 0x00B886A0
// FUNCTION: fn_19372
bool CNaviData::AddDestination(CNaviInfo* info) {
  CNaviInfo* entry = info;
  if (!entry) {
    return false;
  }
  this->destinations.push_back(entry);
  return true;
}

// OFFSET: 0x00B886E0
// FUNCTION: fn_19373
bool CNaviData::AddObject(CNavi_Link* link) {
  std::vector<CNavi_Link*>& objects = this->objects;
  CNavi_Link* entry = link;
  if (!entry) {
    return false;
  }
  objects.push_back(entry);
  return true;
}

// OFFSET: 0x00B9E580
// FUNCTION: fn_19629
bool CNavigationManager::LoadScript(LuaScript* script) {
  char message[260];
  std::memset(message, 0, sizeof(message));
  this->data.ClearAll();

  int type;
  int cellX = 0;
  int cellY = 0;
  int linkedX = 0;
  int linkedY = 0;
  int targetId = -1;
  int cost = 0;
  for (int index = 1; index < 9999; ++index) {
    const char* name = nullptr;
    const char* title = nullptr;
    if (!script->Call("queryNavi_MapInfo", "d>ssddd", index, &name, &title, &type, &cellX, &cellY)) {
      std::sprintf(message, "Map 루아 읽기 실패 %d", index);
      MessageBoxA(hWnd, message, "Error", 0);
      break;
    }
    if (!name) {
      break;
    }
    CNaviInfo* info = new CNaviInfo(static_cast<int>(this->data.destinations.size()), name, title, cellX, cellY, type);
    if (info) {
      this->data.AddDestination(info);
    }
  }

  for (int index = 1; index < 99999; ++index) {
    int objectType = 100;
    const char* title = nullptr;
    const char* name = nullptr;
    const char* mapName = nullptr;
    int objectX = 0;
    int objectY = 0;
    int objectIndex = 0;
    int id;
    if (!script->Call("queryNavi_NpcInfo", "d>sdddssdd", index, &mapName, &id, &objectType, &objectIndex, &name, &title, &objectX, &objectY)) {
      std::sprintf(message, "Npc 루아 읽기 실패 %d", index);
      MessageBoxA(hWnd, message, "Error", 0);
      break;
    }
    if (!mapName) {
      break;
    }
    CNaviInfo* info = this->FindObject(mapName);
    if (info) {
      CNavi_Object* npc = new CNavi_Object(objectType, objectIndex, name, title, objectX, objectY, info);
      npc->SetId(id);
      info->AddNpc(npc);
    }
  }

  for (int index = 1; index < 99999; ++index) {
    int objectType = 100;
    const char* title = nullptr;
    const char* name = nullptr;
    const char* mapName = nullptr;
    int objectX = 0;
    int objectY = 0;
    int objectIndex = 0;
    int id;
    if (!script->Call("queryNavi_MobInfo", "d>sdddssdd", index, &mapName, &id, &objectType, &objectIndex, &name, &title, &objectX, &objectY)) {
      std::sprintf(message, "몬스터 루아 읽기 실패 %d", index);
      MessageBoxA(hWnd, message, "Error", 0);
      break;
    }
    if (!mapName) {
      break;
    }
    CNaviInfo* info = this->FindObject(mapName);
    if (info) {
      CNavi_Object* mob = new CNavi_Object(objectType, objectIndex, name, title, objectX, objectY, info);
      mob->SetId(id);
      info->AddMob(mob);
    }
  }

  for (int index = 1; index < 99999; ++index) {
    int objectType = 100;
    const char* linkedMap = nullptr;
    const char* title = nullptr;
    const char* name = nullptr;
    const char* mapName = nullptr;
    linkedX = 0;
    linkedY = 0;
    int objectX = 0;
    int objectY = 0;
    int objectIndex = 0;
    int id;
    if (!script->Call(
          "queryNavi_LinkInfo", "d>sdddssddsdd", index, &mapName, &id, &objectType, &objectIndex, &name, &title, &objectX, &objectY, &linkedMap, &linkedX,
          &linkedY
        )) {
      std::sprintf(message, "Link 루아 읽기 실패 %d", index);
      MessageBoxA(hWnd, message, "Error", 0);
      break;
    }
    if (!mapName) {
      break;
    }
    CNaviInfo* info = this->FindObject(mapName);
    CNaviInfo* linked = this->FindObject(linkedMap);
    if (info && linked) {
      CNavi_Link* link = new CNavi_Link(objectType, objectIndex, name, title, objectX, objectY, info, linked, linkedX, linkedY);
      link->SetId(id);
      info->AddLink(link);
    }
  }

  int job = 0;
  int sex = 0;
  int bodyPalette = 0;
  int head = 0;
  int headPalette = 0;
  int accessory0 = 0;
  int accessory1 = 0;
  int accessory2 = 0;
  int robe = 0;
  for (int index = 1; index < 9999; ++index) {
    const char* mapName = nullptr;
    int pickX = 0;
    int pickY = 0;
    if (!script->Call(
          "queryNavi_PickNpc", "d>sddddddddddd", index, &mapName, &pickX, &pickY, &job, &sex, &bodyPalette, &head, &headPalette, &accessory0, &accessory1,
          &accessory2, &robe
        )) {
      StringFormat(message, sizeof(message), static_cast<std::size_t>(-1), "Failed to read Link lua %d", index);
      MessageBoxA(hWnd, message, "Error queryNavi_PickNpc ", 0);
      break;
    }
    if (!mapName) {
      break;
    }
    CNaviInfo* info = this->FindObject(mapName);
    if (info) {
      CNavi_Object* npc = info->FindNpcAt(pickX, pickY);
      if (npc) {
        npc->job = job;
        npc->sex = sex;
        npc->bodyPalette = bodyPalette;
        npc->head = head;
        npc->headPalette = headPalette;
        npc->accessory0 = accessory0;
        npc->accessory1 = accessory1;
        npc->accessory2 = accessory2;
        npc->robe = robe;
      }
    }
  }

  for (int map = 1; map < 999999; map += 3) {
    const char* mapName = nullptr;
    int count = 0;
    if (!script->Call("queryNavi_Distance_Map", "d>sd", map, &mapName, &count)) {
      std::sprintf(message, "워프 길 정보 읽기 실패 %d", map);
      MessageBoxA(hWnd, message, "Error", 0);
      break;
    }
    if (!mapName) {
      break;
    }
    CNaviInfo* info = this->FindObject(mapName);
    if (!info) {
      continue;
    }
    for (int entry = 1; entry < count + 1; ++entry) {
      int linkId = -1;
      if (!script->Call("queryNavi_Distance_Link", "dd>d", map, entry, &linkId)) {
        continue;
      }
      CNavi_Link* link = info->FindLink(linkId, true);
      if (!link) {
        continue;
      }
      for (int pass = 1;; ++pass) {
        const char* kind = nullptr;
        targetId = -1;
        cost = 0;
        if (!script->Call("queryNavi_Distance_Pass", "ddd>sdd", map, entry, pass, &kind, &targetId, &cost)) {
          continue;
        }
        if (std::strcmp("NULL", kind) == 0) {
          break;
        }
        if (std::strcmp(kind, "P") == 0) {
          CNavi_Link* target = info->FindLink(targetId, true);
          if (target) {
            link->AddLink(NaviLink{target, cost});
          }
        } else if (std::strcmp(kind, "E") == 0) {
          CNavi_Link* target = link->GetLinkedInfo()->FindLink(targetId, true);
          if (target) {
            link->AddLinkedLink(NaviLink{target, cost});
          }
        }
      }
    }
  }

  for (int map = 1; map < 999999; map += 3) {
    const char* mapName = nullptr;
    int count = 0;
    if (!script->Call("queryNavi_NpcDistance_Map", "d>sd", map, &mapName, &count)) {
      std::sprintf(message, "Npc 워프 길 정보 읽기 실패 %d", map);
      MessageBoxA(hWnd, message, "Error", 0);
      break;
    }
    if (!mapName) {
      break;
    }
    CNaviInfo* info = this->FindObject(mapName);
    if (!info) {
      continue;
    }
    for (int entry = 1; entry < count + 1; ++entry) {
      int npcId = -1;
      if (!script->Call("queryNavi_NpcDistance_Link", "dd>d", map, entry, &npcId)) {
        continue;
      }
      CNavi_Object* npc = info->FindNpc(npcId, true);
      if (!npc) {
        continue;
      }
      for (int pass = 1;; ++pass) {
        const char* targetMap = nullptr;
        targetId = -1;
        cost = 0;
        if (!script->Call("queryNavi_NpcDistance_Pass", "ddd>sdd", map, entry, pass, &targetMap, &targetId, &cost)) {
          continue;
        }
        if (targetId == 0 && cost == 0) {
          break;
        }
        CNaviInfo* targetInfo = this->FindObject(targetMap);
        CNavi_Link* target = targetInfo == nullptr ? this->data.FindLink(targetId, true) : targetInfo->FindLink(targetId, true);
        if (target) {
          npc->AddLink(NaviLink{target, cost});
        }
      }
    }
  }

  for (int index = 1; index < 200; ++index) {
    const char* name = nullptr;
    const char* mapName = nullptr;
    int id = 0;
    int objectIndex = 0;
    linkedX = 0;
    linkedY = 0;
    if (!script->Call("queryNavi_Scroll", "d>sddsdd", index, &name, &id, &objectIndex, &mapName, &linkedX, &linkedY)) {
      char text[260];
      std::memset(text, 0, sizeof(text));
      std::sprintf(text, "Scroll 정보 읽기 실패 %d", index);
      MessageBoxA(hWnd, text, "Error", 0);
      break;
    }
    if (std::strcmp("NULL", name) == 0 && std::strcmp("NULL", mapName) == 0) {
      break;
    }
    CNaviInfo* info = this->FindObject(mapName);
    if (!info) {
      continue;
    }
    CNavi_Link* link = new CNavi_Link(400, objectIndex, name, nullptr, 0, 0, nullptr, info, linkedX, linkedY);
    if (link) {
      link->SetId(id);
      this->data.AddObject(link);
      link->ClearLinkedLinks();
      for (int pass = 1;; ++pass) {
        targetId = -1;
        cost = 0;
        if (!script->Call("queryNavi_Scroll_Pass", "dd>dd", index, pass, &targetId, &cost)) {
          continue;
        }
        if (targetId == 0 && cost == 0) {
          break;
        }
        CNavi_Link* target = info->FindLink(targetId, true);
        if (target) {
          link->AddLinkedLink(NaviLink{target, cost});
        }
      }
    }
  }

  this->playerMapInfo = this->FindObject("prontera");
  this->data.ResetLinkDepths();
  return true;
}

// OFFSET: 0x00DDE770
// FUNCTION: fn_23399
const char* CSession::GetMapDisplayName(const char* mapName) {
  CMapInfoMgr* mapInfoMgr = g_mapInfoMgr;
  return mapInfoMgr->GetDisplayName(mapName);
}

// OFFSET: 0x00DDE800
// FUNCTION: fn_23400
std::uint32_t CSession::GetPartyMemberAid(const char* name) {
  for (const MessengerMember& member : this->m_partyMembers) {
    if (member.name == name) {
      return member.accountId;
    }
  }

  return 0;
}

// OFFSET: 0x00DE0A50
// FUNCTION: fn_23466
std::uint32_t CSession::GetServerTime() {
  return timeGetTime() - this->m_serverTimeOffset - 72;
}

// OFFSET: 0x00DFEF40
// FUNCTION: fn_23605
std::uint32_t CSession::AddPingSample(std::uint32_t sample) {
  this->m_pingSamples.push_back(sample);

  while (this->m_pingSamples.size() > 3) {
    this->m_pingSamples.erase(this->m_pingSamples.begin());
  }

  std::uint32_t sum = 0;
  for (std::uint32_t value : this->m_pingSamples) {
    sum += value;
  }

  this->m_averagePing = sum / this->m_pingSamples.size();
  return this->m_averagePing;
}

// OFFSET: 0x00E00130
// FUNCTION: fn_23637
std::uint32_t CSession::SetServerTime(std::uint32_t serverTime) {
  const std::uint32_t offset = timeGetTime() - serverTime;
  this->m_serverTimeOffset = offset;
  this->m_serverTimeAdjustCount = 0;
  return offset;
}

// OFFSET: 0x00E00160
// FUNCTION: fn_23638
void CSession::SetHotkeyDescription(const char* description, int index) {
  for (int slot = 9 * this->m_hotkeyPage; slot < 9 * (this->m_hotkeyPage + 1); ++slot) {
    if (this->m_hotkeyDescriptions[slot] == description) {
      this->m_hotkeyDescriptions[slot] = "nothing";
    }
  }

  if (std::strlen(description) == 0) {
    this->m_hotkeyDescriptions[index] = "nothing";
  } else {
    this->m_hotkeyDescriptions[index] = description;
  }
}

// OFFSET: 0x00E26430
// FUNCTION: fn_23914
void CSession::SetHotkeyDescription(const char* description, std::uint32_t skillLevel, int index) {
  for (int slot = 9 * this->m_hotkeyPage; slot < 9 * (this->m_hotkeyPage + 1); ++slot) {
    if (this->m_hotkeyDescriptions[slot] == description && this->hotkeySkillLevels[slot] == skillLevel) {
      this->m_hotkeyDescriptions[slot] = "nothing";
      this->hotkeySkillLevels[slot] = 0;
    }
  }

  if (std::strlen(description) == 0) {
    this->m_hotkeyDescriptions[index] = "nothing";
    this->hotkeySkillLevels[index] = 0;
  } else {
    this->m_hotkeyDescriptions[index] = description;
    this->hotkeySkillLevels[index] = skillLevel;
  }
}

// OFFSET: 0x00E26540
// FUNCTION: fn_23915
void CSession::SetHotkeySkillLevel(std::uint32_t skillLevel, int index) {
  this->hotkeySkillLevels[index] = skillLevel;
}

// OFFSET: 0x00E265C0
// FUNCTION: fn_23916
void CSession::RefreshTrackedAchievement(int id) {
  std::vector<SessionRecord6178>& tracked = this->trackedAchievements;
  auto found = std::find_if(tracked.begin(), tracked.end(), [id](SessionRecord6178 record) { return id == record.id; });
  if (found == tracked.end()) {
    return;
  }

  SessionRecord6144 progress = g_session.FindAchievementRecord(id);

  AchievementInfo info;
  g_achievementMgr->GetAchievement(info, id);
  found->completed = reinterpret_cast<const unsigned char*>(&progress.completionFlags)[1];

  for (unsigned int i = 0; i < info.rewards.size(); ++i) {
    if (info.progressMode == 0) {
      if (info.rewards[i].text.size() > 40) {
        found->objectives[i].text = static_cast<const std::string&>(FormatOwnedString("- %s...", info.rewards[i].text.substr(0, 40).c_str()));
      } else {
        found->objectives[i].text = static_cast<const std::string&>(FormatOwnedString("- %s", info.rewards[i].text.c_str()));
      }

      found->objectives[i].completed = static_cast<int>(progress.objectives[i]) > 0;
    } else if (info.progressMode == 1) {
      int value = progress.objectives[i];
      std::string count = FormatOwnedString("(%d/%d)", value, info.rewards[i].targetCount);
      if (info.rewards[i].text.size() + count.size() > 40) {
        found->objectives[i].text = static_cast<const std::string&>(FormatOwnedString("- %s...%s", info.rewards[i].text.substr(0, 40 - count.size()).c_str(), count.c_str()));
      } else {
        found->objectives[i].text = static_cast<const std::string&>(FormatOwnedString("- %s %s", info.rewards[i].text.c_str(), count.c_str()));
      }

      found->objectives[i].completed = value == info.rewards[i].targetCount;
    }
  }
}

// OFFSET: 0x00E00320
// FUNCTION: fn_23639
void CSession::AddItemSlot(ITEM_INFO item) {
  for (int slot = 0; slot < 10; ++slot) {
    if (this->m_itemSlots[slot].quantity == 0) {
      this->m_itemSlots[slot] = item;
      break;
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E003D0
// FUNCTION: fn_23640
SessionShop* CSession::GetShop(int shopId) {
  const auto found = this->m_shops.find(shopId);
  if (found != this->m_shops.end()) {
    return &found->second;
  }

  this->m_shops[shopId] = SessionShop{};
  return &this->m_shops[shopId];
}

// OFFSET: 0x00E004D0
// FUNCTION: fn_23641
void CSession::SetShopFlag(int shopId, char flag) {
  if (SessionShop* shop = this->GetShop(shopId)) {
    shop->flag = static_cast<unsigned char>(flag);
  }
}

// OFFSET: 0x00E004F0
// FUNCTION: fn_23642
void CSession::SetShopItem(int shopId, unsigned int index, int itemId, int first, int second) {
  SessionShop* shop = this->GetShop(shopId);
  if (shop == nullptr) {
    return;
  }

  if (shop->items.size() <= index) {
    shop->items.resize(index + 1);
  }

  std::map<int, std::pair<int, int>>& items = shop->items[index];
  if (items.find(itemId) == items.end()) {
    items[itemId] = index == 0 ? std::make_pair(1, second) : std::make_pair(first, second);
  }
}

// INLINED
static __forceinline int LookupShopId(CSession& session, std::string name) {
  std::string key = name;
  if (key.empty()) {
    key = session.m_navigationPlayerMap;
  }

  const auto found = session.m_nameToId.find(key);
  if (found == session.m_nameToId.end()) {
    return -1;
  }
  return found->second;
}

// OFFSET: 0x00E006E0
// FUNCTION: fn_23643
bool CSession::AddShopName(std::string name, int shopId) {
  if (LookupShopId(*this, name) != -1) {
    return true;
  }

  this->m_nameToId[name] = shopId;
  return false;
}

// OFFSET: 0x00E00910
// FUNCTION: fn_23644
void CSession::LoadNavigationScripts() {
  if (g_serviceMode == 0) {
    if (!this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_f_krpri", true, false)) {
      return;
    }
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Map_krpri", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Npc_krpri", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Mob_krpri", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Link_krpri", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_LinkDistance_krpri", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_NpcDistance_krpri", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Scroll_krpri", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\navi_picknpc_krpri", true, false);
  } else if (g_serviceMode == 1) {
    if (!this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_f_krSak", true, false)) {
      return;
    }
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Map_krSak", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Npc_krSak", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Mob_krSak", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Link_krSak", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_LinkDistance_krSak", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_NpcDistance_krSak", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Scroll_krSak", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\navi_picknpc_krSak", true, false);
  } else {
    if (!this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_f_krLoc", true, false)) {
      return;
    }
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Map_krLoc", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Npc_krLoc", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Mob_krLoc", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Link_krLoc", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_LinkDistance_krLoc", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_npcDistance_krLoc", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Scroll_krLoc", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\navi_picknpc_krLoc", true, false);
  }

  g_navigationManager.LoadScript(this->m_hotkeyScript);
}

// OFFSET: 0x00E00B30
// FUNCTION: fn_23645
void CSession::LoadRobeAlpha() {
  for (int index = 1; index < 1000; ++index) {
    int robeId = 0;
    int from = 0;
    int to = 0;
    int period = 0;
    LuaScript* script = this->m_hotkeyScript;

    const char signatureSource[] = {'\x15', '\x45', '\xC9', '\x0F', '\x21', '\x81', '\x73'};
    const char functionSource[] = {'\x36', '\x32', '\xDC', '\x11', '\x9E', '\x27', '\x6B', '\xB4', '\x29', '\x34', '\xC9', '\x02', '\x95', '\x36', '\x77', '\xA1', '\x2E', '\x33', '\xDC', '\x0E', '\x8E', '\x3D', '\x7F', '\xBA', '\x47'};

    if (!script->Call(DecryptName<0xD51953E763B94747ULL>(functionSource), DecryptName<0x6373E5456BAD7B71ULL>(signatureSource), index, &robeId, &from, &to, &period)) {
      char message[260];
      std::memset(message, 0, sizeof(message));
      const char formatSource[] = {'\xAA', '\x72', '\x76', '\xA1', '\x8C', '\x37', '\x93', '\x0A', '\xB5', '\x74', '\x63', '\xB2', '\x87', '\x26', '\x8F', '\x1F', '\xB2', '\x73', '\x76', '\xBE', '\x9C', '\x2D', '\x87', '\x04', '\xFB', '\x28', '\x3C', '\xF3', '\xD0', '\x27', '\xC1', '\xAB', '\x0B', '\xB6', '\xF1', '\xF3', '\x48', '\x84', '\x27', '\xBB', '\xDB'};
      std::sprintf(message, DecryptName<0x6BE143F5D31307DBULL>(formatSource), index);
      const char titleSource[] = {'\xC8', '\x0D', '\x8B', '\xC0', '\xA5', '\x91'};
      MessageBoxA(hWnd, message, DecryptName<0xAB5791D7AFF97F8DULL>(titleSource), 0);
      return;
    }

    if (!robeId) {
      return;
    }
    this->m_robeAlpha.insert({robeId, RobeAlpha{from, to, period}});
  }
}

// OFFSET: 0x00E010B0
// FUNCTION: fn_23646
int CSession::SetPlayerSkillMode(int value) {
  this->playerSkillMode = value;
  return value;
}

// OFFSET: 0x00DDBB50
// FUNCTION: fn_23359
int CSession::GetPlayerSkillMode() const {
  return this->playerSkillMode;
}

// OFFSET: 0x00E01670
// FUNCTION: fn_23647
void CSession::UnflagInventoryItem(int id) {
  for (ITEM_INFO& entry : this->m_inventoryItems) {
    if (entry.identity[1] == static_cast<std::uint32_t>(id)) {
      entry.flags[2] = 0;
      break;
    }
  }
}

// OFFSET: 0x00E016A0
// FUNCTION: fn_23648
void CSession::UnflagMerchantShopItem(int id) {
  for (ITEM_INFO& entry : this->merchantShopItems) {
    if (entry.identity[1] == static_cast<std::uint32_t>(id)) {
      entry.flags[2] = 0;
      break;
    }
  }
}

// OFFSET: 0x00E016D0
// FUNCTION: fn_23649
void CSession::UnflagCartItem(int id) {
  for (ITEM_INFO& entry : this->cartItems) {
    if (entry.identity[1] == static_cast<std::uint32_t>(id)) {
      entry.flags[2] = 0;
      break;
    }
  }
}

// OFFSET: 0x00E01700
// FUNCTION: fn_23650
std::uint32_t CSession::SyncServerTime(std::uint32_t serverTime) {
  const std::uint32_t current = timeGetTime() - this->m_serverTimeOffset - 72;
  if (current + 72 < serverTime) {
    this->m_serverTimeOffset = timeGetTime() - serverTime;
    this->m_serverTimeAdjustCount = 0;
    return this->m_serverTimeOffset;
  }

  if (current + 72 > serverTime + 144) {
    if (++this->m_serverTimeAdjustCount < 4) {
      return serverTime + 144;
    }

    this->m_serverTimeOffset = timeGetTime() + 72 - current;
    this->m_serverTimeAdjustCount = 0;
    return this->m_serverTimeOffset;
  }

  this->m_serverTimeAdjustCount = 0;
  return serverTime + 144;
}

// OFFSET: 0x00DE4D80
// FUNCTION: fn_23490
void CSession::LoadDamageSkinSettings() {
  g_session.m_hotkeyScript->Call("GetDamageSkinSize", ">d", &this->damageSkinCount);
  if (this->m_damageSkinIndex < 0 || this->m_damageSkinIndex >= this->damageSkinCount) {
    this->m_damageSkinIndex = 0;
  }
  if (this->m_damageSkinMode < 0 || this->m_damageSkinMode >= 4) {
    this->m_damageSkinMode = 0;
  }

  const char* resourceName = nullptr;
  const char* damageSpriteName = nullptr;
  const char* damageSkinAuxiliaryName = nullptr;
  g_session.m_hotkeyScript->Call("GetDamageResourceName", "d>sss", this->m_damageSkinIndex, &resourceName, &damageSpriteName, &damageSkinAuxiliaryName);
  if (resourceName) {
    this->m_damageResourceName = resourceName;
  }
  if (damageSpriteName) {
    this->damageSpriteName = damageSpriteName;
  }
  if (damageSkinAuxiliaryName) {
    this->damageSkinAuxiliaryName = damageSkinAuxiliaryName;
  }
  g_session.m_hotkeyScript->Call("GetDamageMillionUnit", "d>d", this->m_damageSkinIndex, &this->m_damageMillionUnit);
}

// OFFSET: 0x00DE53C0
// FUNCTION: fn_23491
bool CSession::LoadExceptionMinimapNameTable() {
  std::vector<char*> tokens;
  this->m_navigationForbiddenMaps.clear();
  this->m_navigationForbiddenMaps.reserve(512);

  CCommonObject& storage = reinterpret_cast<CCommonObject&>(this->minimapTokenStorage[0x30]);
  const bool loaded = storage.LoadTokens(tokens, "ExceptionMinimapNameTable.txt", 0);

  for (char* token : tokens) {
    this->m_navigationForbiddenMaps.push_back(std::string(token));
  }

  std::sort(this->m_navigationForbiddenMaps.begin(), this->m_navigationForbiddenMaps.end(), std::less<std::string>());
  return loaded;
}

// OFFSET: 0x00DE5620
// FUNCTION: fn_23492
bool CSession::LoadFogParameters() {
  this->m_fogParameters.clear();

  CFile file;
  if (!file.Open("fogParameterTable.txt", 0, true, false)) {
    return false;
  }

  const char* cursor = static_cast<const char*>(file.GetData());
  const char* end = cursor + file.GetSize();
  std::string name;
  std::vector<std::string> values;

  for (const char* start = cursor; cursor < end; ++cursor) {
    const char value = *cursor;
    if (value == '\n' || value == '\r') {
      start = cursor + 1;
      continue;
    }

    if (value != '#') {
      continue;
    }

    if (name.empty()) {
      name.assign(start, cursor - start);
    } else if (values.size() < 3) {
      values.push_back(std::string(start, cursor));
    } else {
      std::string last(start, cursor);
      values.push_back(std::move(last));
      this->m_fogParameters.push_back(std::make_pair(name, values));
      name.clear();
      values.clear();
    }

    start = cursor + 1;
  }

  std::sort(this->m_fogParameters.begin(), this->m_fogParameters.end(),
            [](const std::pair<std::string, std::vector<std::string>>& left,
               const std::pair<std::string, std::vector<std::string>>& right) {
              return std::strcmp(left.first.c_str(), right.first.c_str()) < 0;
            });
  file.Close();
  return true;
}

// OFFSET: 0x00DE5B40
// FUNCTION: fn_23493
void CSession::ClearHomunStatus() {
  this->moveHomunEnemy = 0;
  this->selectHomunEnemy = 0;
  this->attackHomunEnemy = 0;
  this->homunEnemyGid = 0;
  this->homunEnemyX = 0;
  this->homunEnemyY = 0;
}
