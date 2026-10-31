#include "Client/Application/ApplicationWindow.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MapInfoMgr.h"
#include "Contents/NavigationManager.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <timeapi.h>
#include <windows.h>

// GLOBAL: gl_24705
int g_serviceMode;

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  std::uint8_t m_text[Length];
  bool m_encrypted;
};

}

struct NaviUnknownBlock {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ int unknown_000C;
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ int unknown_0018;
  /* 001C */ int unknown_001C;
  /* 0020 */ int unknown_0020;

  NaviUnknownBlock();
};

// OFFSET: 0x00B8C870
// FUNCTION: fn_19426
CNavi_Object::CNavi_Object()
    : id(-1), type(101), index(0), info(nullptr), cellX(0), cellY(0), unknown_0058(0), unknown_005C(0), unknown_0060(0), unknown_0064(0), unknown_0068(0),
      unknown_006C(0), unknown_0070(0), unknown_0074(0), unknown_0078(0) {
}

// OFFSET: 0x00B8C990
// FUNCTION: fn_19427
NaviUnknownBlock::NaviUnknownBlock()
    : unknown_0000(0), unknown_0004(0), unknown_0008(0), unknown_000C(0), unknown_0010(0), unknown_0014(0), unknown_0018(0), unknown_001C(0),
      unknown_0020(0) {
}

// OFFSET: 0x00B8C710
// FUNCTION: fn_19425
CNavi_Object::CNavi_Object(int type, int index, const char* name, const char* title, int cellX, int cellY, CNaviInfo* info)
    : id(-1), type(type), index(index), unknown_0058(0), unknown_005C(0), unknown_0060(0), unknown_0064(0), unknown_0068(0), unknown_006C(0),
      unknown_0070(0), unknown_0074(0), unknown_0078(0) {
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

// OFFSET: 0x00B8C2B0
// FUNCTION: fn_19422
CNavi_Link::CNavi_Link(
  int type, int index, const char* name, const char* title, int cellX, int cellY, CNaviInfo* info, CNaviInfo* linkedInfo, int unknown80, int unknown84
)
    : unknown_0080(0), unknown_0084(0) {
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
  this->unknown_0080 = unknown80;
  this->unknown_0084 = unknown84;
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
  if (!info) {
    return false;
  }
  this->destinations.push_back(info);
  return true;
}

// OFFSET: 0x00B886E0
// FUNCTION: fn_19373
bool CNaviData::AddObject(CNavi_Link* link) {
  if (!link) {
    return false;
  }
  this->objects.push_back(link);
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
    this->data.AddDestination(new CNaviInfo(static_cast<int>(this->data.destinations.size()), name, title, cellX, cellY, type));
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
    int linkedX = 0;
    int linkedY = 0;
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

  int pick[9] = {};
  for (int index = 1; index < 9999; ++index) {
    const char* mapName = nullptr;
    int pickX = 0;
    int pickY = 0;
    if (!script->Call(
          "queryNavi_PickNpc", "d>sddddddddddd", index, &mapName, &pickX, &pickY, &pick[0], &pick[1], &pick[2], &pick[3], &pick[4], &pick[5], &pick[6],
          &pick[7], &pick[8]
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
        npc->unknown_0058 = pick[0];
        npc->unknown_005C = pick[1];
        npc->unknown_0060 = pick[2];
        npc->unknown_0064 = pick[3];
        npc->unknown_0068 = pick[4];
        npc->unknown_006C = pick[5];
        npc->unknown_0070 = pick[6];
        npc->unknown_0074 = pick[7];
        npc->unknown_0078 = pick[8];
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
        int targetId = -1;
        int cost = 0;
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
        int targetId = -1;
        int cost = 0;
        if (!script->Call("queryNavi_NpcDistance_Pass", "ddd>sdd", map, entry, pass, &targetMap, &targetId, &cost)) {
          continue;
        }
        if (targetId == 0 && cost == 0) {
          break;
        }
        CNaviInfo* targetInfo = this->FindObject(targetMap);
        CNavi_Link* target = targetInfo ? targetInfo->FindLink(targetId, true) : this->data.FindLink(targetId, true);
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
    int linkedX = 0;
    int linkedY = 0;
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
    link->SetId(id);
    this->data.AddObject(link);
    link->ClearLinkedLinks();
    for (int pass = 1;; ++pass) {
      int targetId = -1;
      int cost = 0;
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

  this->unknown_1244 = this->FindObject("prontera");
  this->data.ResetLinkDepths();
  return true;
}

// OFFSET: 0x00DDE770
// FUNCTION: fn_23399
const char* CSession::GetMapDisplayName(const char* mapName) {
  return g_mapInfoMgr->GetDisplayName(mapName);
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
    this->m_pingSamples.pop_front();
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

  if (description[0] == '\0') {
    this->m_hotkeyDescriptions[index] = "nothing";
  } else {
    this->m_hotkeyDescriptions[index] = description;
  }
}

// OFFSET: 0x00E26430
// FUNCTION: fn_23914
void CSession::SetHotkeyDescription(const char* description, std::uint32_t skillLevel, int index) {
  for (int slot = 9 * this->m_hotkeyPage; slot < 9 * (this->m_hotkeyPage + 1); ++slot) {
    if (this->m_hotkeyDescriptions[slot] == description && this->unknown_0420[slot] == skillLevel) {
      this->m_hotkeyDescriptions[slot] = "nothing";
      this->unknown_0420[slot] = 0;
    }
  }

  if (std::strlen(description) == 0) {
    this->m_hotkeyDescriptions[index] = "nothing";
    this->unknown_0420[index] = 0;
  } else {
    this->m_hotkeyDescriptions[index] = description;
    this->unknown_0420[index] = skillLevel;
  }
}

// OFFSET: 0x00E26540
// FUNCTION: fn_23915
void CSession::SetHotkeySkillLevel(std::uint32_t skillLevel, int index) {
  this->unknown_0420[index] = skillLevel;
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

  if (UIWindow* window = g_windowMgr.LookupWindow(10011)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
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
    shop->unknown_0000 = static_cast<unsigned char>(flag);
  }
}

// OFFSET: 0x00E004F0
// FUNCTION: fn_23642
void CSession::SetShopItem(int shopId, unsigned int index, int itemId, int first, int second) {
  SessionShop* shop = this->GetShop(shopId);
  if (shop == nullptr) {
    return;
  }

  if (shop->unknown_0004.size() <= index) {
    shop->unknown_0004.resize(index + 1);
  }

  std::map<int, std::pair<int, int>>& items = shop->unknown_0004[index];
  if (items.find(itemId) == items.end()) {
    items[itemId] = index == 0 ? std::make_pair(1, second) : std::make_pair(first, second);
  }
}

// OFFSET: 0x00E006E0
// FUNCTION: fn_23643
bool CSession::AddShopName(std::string name, int shopId) {
  std::string key = name;
  if (key.empty()) {
    key = this->m_navigationPlayerMap;
  }

  const auto found = this->m_nameToId.find(key);
  if (found != this->m_nameToId.end() && found->second != -1) {
    return true;
  }

  this->m_nameToId[name] = shopId;
  return false;
}

// OFFSET: 0x00E00910
// FUNCTION: fn_23644
void CSession::LoadNavigationScripts() {
  const char* npcDistance;
  const char* scroll;
  const char* pickNpc;

  if (g_serviceMode == 0) {
    if (!this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_f_krpri", true, false)) {
      return;
    }
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Map_krpri", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Npc_krpri", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Mob_krpri", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Link_krpri", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_LinkDistance_krpri", true, false);
    npcDistance = "Lua Files\\Navigation\\Navi_NpcDistance_krpri";
    scroll = "Lua Files\\Navigation\\Navi_Scroll_krpri";
    pickNpc = "Lua Files\\Navigation\\navi_picknpc_krpri";
  } else if (g_serviceMode == 1) {
    if (!this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_f_krSak", true, false)) {
      return;
    }
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Map_krSak", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Npc_krSak", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Mob_krSak", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Link_krSak", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_LinkDistance_krSak", true, false);
    npcDistance = "Lua Files\\Navigation\\Navi_NpcDistance_krSak";
    scroll = "Lua Files\\Navigation\\Navi_Scroll_krSak";
    pickNpc = "Lua Files\\Navigation\\navi_picknpc_krSak";
  } else {
    if (!this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_f_krLoc", true, false)) {
      return;
    }
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Map_krLoc", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Npc_krLoc", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Mob_krLoc", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_Link_krLoc", true, false);
    this->m_hotkeyScript->Load("Lua Files\\Navigation\\Navi_LinkDistance_krLoc", true, false);
    npcDistance = "Lua Files\\Navigation\\Navi_npcDistance_krLoc";
    scroll = "Lua Files\\Navigation\\Navi_Scroll_krLoc";
    pickNpc = "Lua Files\\Navigation\\navi_picknpc_krLoc";
  }

  this->m_hotkeyScript->Load(npcDistance, true, false);
  this->m_hotkeyScript->Load(scroll, true, false);
  this->m_hotkeyScript->Load(pickNpc, true, false);
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

    const std::uint8_t signatureSource[] = {0x15, 0x45, 0xC9, 0x0F, 0x21, 0x81, 0x73};
    thread_local EncryptedName<sizeof(signatureSource), 0x6373E5456BAD7B71ULL> signature(signatureSource);
    const std::uint8_t functionSource[] = {0x36, 0x32, 0xDC, 0x11, 0x9E, 0x27, 0x6B, 0xB4, 0x29, 0x34, 0xC9, 0x02, 0x95,
                                           0x36, 0x77, 0xA1, 0x2E, 0x33, 0xDC, 0x0E, 0x8E, 0x3D, 0x7F, 0xBA, 0x47};
    thread_local EncryptedName<sizeof(functionSource), 0xD51953E763B94747ULL> function(functionSource);

    if (!script->Call(function.Text(), signature.Text(), index, &robeId, &from, &to, &period)) {
      char message[260];
      std::memset(message, 0, sizeof(message));
      const std::uint8_t formatSource[] = {0xAA, 0x72, 0x76, 0xA1, 0x8C, 0x37, 0x93, 0x0A, 0xB5, 0x74, 0x63, 0xB2, 0x87, 0x26,
                                           0x8F, 0x1F, 0xB2, 0x73, 0x76, 0xBE, 0x9C, 0x2D, 0x87, 0x04, 0xFB, 0x28, 0x3C, 0xF3,
                                           0xD0, 0x27, 0xC1, 0xAB, 0x0B, 0xB6, 0xF1, 0xF3, 0x48, 0x84, 0x27, 0xBB, 0xDB};
      thread_local EncryptedName<sizeof(formatSource), 0x6BE143F5D31307DBULL> format(formatSource);
      std::sprintf(message, format.Text(), index);
      const std::uint8_t titleSource[] = {0xC8, 0x0D, 0x8B, 0xC0, 0xA5, 0x91};
      thread_local EncryptedName<sizeof(titleSource), 0xAB5791D7AFF97F8DULL> title(titleSource);
      MessageBoxA(hWnd, message, title.Text(), 0);
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
int CSession::SetUnknown8134(int value) {
  this->unknown_8134 = value;
  return value;
}

// OFFSET: 0x00DDBB50
// FUNCTION: fn_23359
int CSession::GetUnknown8134() const {
  return this->unknown_8134;
}

// OFFSET: 0x00E01670
// FUNCTION: fn_23647
void CSession::ClearUnknown1710(int id) {
  for (ITEM_INFO& entry : this->m_inventoryItems) {
    if (entry.unknown_0000[1] == static_cast<std::uint32_t>(id)) {
      entry.unknown_0074[2] = 0;
      break;
    }
  }
}

// OFFSET: 0x00E016A0
// FUNCTION: fn_23648
void CSession::ClearUnknown1740(int id) {
  for (ITEM_INFO& entry : this->unknown_1740) {
    if (entry.unknown_0000[1] == static_cast<std::uint32_t>(id)) {
      entry.unknown_0074[2] = 0;
      break;
    }
  }
}

// OFFSET: 0x00E016D0
// FUNCTION: fn_23649
void CSession::ClearUnknown1738(int id) {
  for (ITEM_INFO& entry : this->unknown_1738) {
    if (entry.unknown_0000[1] == static_cast<std::uint32_t>(id)) {
      entry.unknown_0074[2] = 0;
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
  g_luaScript->Call("GetDamageSkinSize", ">d", &this->unknown_80C8);
  if (this->m_damageSkinIndex < 0 || this->m_damageSkinIndex >= this->unknown_80C8) {
    this->m_damageSkinIndex = 0;
  }
  if (this->m_damageSkinMode < 0 || this->m_damageSkinMode >= 4) {
    this->m_damageSkinMode = 0;
  }

  const char* resourceName = nullptr;
  const char* name80EC = nullptr;
  const char* name8104 = nullptr;
  g_luaScript->Call("GetDamageResourceName", "d>sss", this->m_damageSkinIndex, &resourceName, &name80EC, &name8104);
  if (resourceName) {
    this->m_damageResourceName = resourceName;
  }
  if (name80EC) {
    this->unknown_80EC = name80EC;
  }
  if (name8104) {
    this->unknown_8104 = name8104;
  }
  g_luaScript->Call("GetDamageMillionUnit", "d>d", this->m_damageSkinIndex, &this->m_damageMillionUnit);
}

// OFFSET: 0x00DE5B40
// FUNCTION: fn_23493
void CSession::ClearHomunStatus() {
  this->unknown_5608 = 0;
  this->unknown_560C = 0;
  this->unknown_5610 = 0;
  this->unknown_55FC = 0;
  this->unknown_5600 = 0;
  this->unknown_5604 = 0;
}
