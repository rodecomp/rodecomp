#include "Contents/EnchantMgr.h"
#include "Client/Session.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/ItemDBToIDMgr.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Enchant/EnchantUI.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>

namespace {
#pragma pack(push, 1)
struct CloseEnchantPacket {
  /* 0000 */ int16_t packetType;
};

struct RandomEnchantPacket {
  /* 0000 */ int16_t packetType;
  /* 0002 */ uint64_t group;
  /* 000A */ int16_t index;
};

struct PerfectEnchantPacket {
  /* 0000 */ int16_t packetType;
  /* 0002 */ uint64_t group;
  /* 000A */ int16_t index;
  /* 000C */ int32_t itemId;
};

struct UpgradeEnchantPacket {
  /* 0000 */ int16_t packetType;
  /* 0002 */ uint64_t group;
  /* 000A */ int16_t index;
  /* 000C */ int16_t slot;
};

struct UpgradePerfectEnchantPacket {
  /* 0000 */ int16_t packetType;
  /* 0002 */ uint64_t group;
  /* 000A */ int16_t index;
  /* 000C */ int16_t slot;
  /* 000E */ int32_t itemId;
};
#pragma pack(pop)
}

// OFFSET: 0x0067FF60
// FUNCTION: fn_05523
CEnchantMgr::CEnchantMgr() {
  m_request.packetType = 0;
  m_request.group = 0;
  m_request.index = -1;
  m_request.slot = -1;
  m_unknown_0030 = true;
}

// OFFSET: 0x00682960
// FUNCTION: fn_05540
void CEnchantMgr::SendCloseEnchant() {
  CloseEnchantPacket packet;
  packet.packetType = 0xBA0;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_unknown_0030 = true;
}

// OFFSET: 0x00683BC0
// FUNCTION: fn_05544
void CEnchantMgr::OpenEnchant(uint64_t group) {
  if (g_session.m_weight * 100 / g_session.m_maxWeight >= 70) {
    if (g_windowMgr.LookupWindow(10006)) {
      g_windowMgr.CloseWindow(10006);
    }
  } else {
    CUIEnchantUI* window = static_cast<CUIEnchantUI*>(g_windowMgr.EnsureWindow(10006));
    if (window) {
      window->sub_A06C40(group);
      this->m_unknown_0030 = false;
    }
  }
}

// OFFSET: 0x006841D0
// FUNCTION: fn_05551
void CEnchantMgr::HandleEnchantResult(int messageId, int itemId) {
  if (messageId == 0xF11) {
    auto enchant = this->m_enchants.find(this->m_request.group);
    if (enchant == this->m_enchants.end()) {
      return;
    }

    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, this->m_request.index);
    if (item.unknown_0000[1] == 0) {
      return;
    }

    if (this->m_request.packetType == 0xB9E) {
      for (int slot : enchant->second.slots) {
        if (static_cast<uint32_t>(slot) < 4) {
          item.SetCard(0, slot);
        }
      }
    } else {
      if (static_cast<uint16_t>(this->m_request.slot) > 3) {
        return;
      }
      item.SetCard(itemId, this->m_request.slot);
    }

    g_session.UpdateInventoryItem(item, 1);
    int effect = 0;
    switch (this->m_request.packetType) {
    case 0xB9B:
    case 0xB9C:
      effect = 0x908;
      break;
    case 0xB9D:
    case 0xBF0:
    case 0xBF1:
      effect = 0x90B;
      break;
    case 0xB9E:
      effect = 0x90E;
      break;
    }
    g_effectMgr->Spawn(g_session.m_aid, effect, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  } else if (messageId == 0xF12) {
    int effect = 0;
    switch (this->m_request.packetType) {
    case 0xB9B:
    case 0xB9C:
    case 0xB9D:
    case 0xBF0:
    case 0xBF1:
      effect = 0x909;
      break;
    case 0xB9E:
      effect = 0x90F;
      break;
    }
    g_effectMgr->Spawn(g_session.m_aid, effect, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  } else {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(messageId)), 0xFF, 0, 0, 0);
  }

  g_windowMgr.CloseWindow(10006);
  this->m_unknown_0030 = false;
}

// OFFSET: 0x00683C20
// FUNCTION: fn_05545
void CEnchantMgr::RequestPerfectEnchant(uint64_t group, int16_t index, int16_t slot, int32_t itemId) {
  if (g_session.m_weight * 100 / g_session.m_maxWeight >= 70) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3837)), 0xFF, 0, 0, 0);
    return;
  }

  PerfectEnchantPacket packet;
  packet.packetType = 0xB9C;
  packet.group = group;
  packet.index = index;
  packet.itemId = itemId;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_request = EnchantRequest{0xB9C, group, index, slot};
  m_unknown_0030 = true;
}

// OFFSET: 0x00683D10
// FUNCTION: fn_05546
void CEnchantMgr::RequestRandomEnchant(uint64_t group, int16_t index, int16_t slot) {
  if (g_session.m_weight * 100 / g_session.m_maxWeight >= 70) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3837)), 0xFF, 0, 0, 0);
    return;
  }

  RandomEnchantPacket packet;
  packet.packetType = 0xB9B;
  packet.group = group;
  packet.index = index;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_request = EnchantRequest{0xB9B, group, index, slot};
  m_unknown_0030 = true;
}

// OFFSET: 0x00683E00
// FUNCTION: fn_05547
void CEnchantMgr::RequestResetEnchant(uint64_t group, int16_t index) {
  if (g_session.m_weight * 100 / g_session.m_maxWeight >= 70) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3837)), 0xFF, 0, 0, 0);
    return;
  }

  RandomEnchantPacket packet;
  packet.packetType = 0xB9E;
  packet.group = group;
  packet.index = index;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_request = EnchantRequest{0xB9E, group, index, -1};
  m_unknown_0030 = true;
}

// OFFSET: 0x00683EF0
// FUNCTION: fn_05548
void CEnchantMgr::RequestUpgradePerfectEnchant(uint64_t group, int16_t index, int16_t slot, int32_t itemId) {
  if (g_session.m_weight * 100 / g_session.m_maxWeight >= 70) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3837)), 0xFF, 0, 0, 0);
    return;
  }

  UpgradePerfectEnchantPacket packet;
  packet.packetType = 0xBF1;
  packet.group = group;
  packet.index = index;
  packet.slot = slot;
  packet.itemId = itemId;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_request = EnchantRequest{0xBF1, group, index, slot};
  m_unknown_0030 = true;
}

// OFFSET: 0x00683FF0
// FUNCTION: fn_05549
void CEnchantMgr::RequestUpgradeEnchant(uint64_t group, int16_t index, int16_t slot) {
  if (g_session.m_weight * 100 / g_session.m_maxWeight >= 70) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3837)), 0xFF, 0, 0, 0);
    return;
  }

  UpgradeEnchantPacket packet;
  packet.packetType = 0xB9D;
  packet.group = group;
  packet.index = index;
  packet.slot = slot;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_request = EnchantRequest{0xB9D, group, index, slot};
  m_unknown_0030 = true;
}

// OFFSET: 0x006840E0
// FUNCTION: fn_05550
void CEnchantMgr::RequestUpgradeRandomEnchant(uint64_t group, int16_t index, int16_t slot) {
  if (g_session.m_weight * 100 / g_session.m_maxWeight >= 70) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3837)), 0xFF, 0, 0, 0);
    return;
  }

  UpgradeEnchantPacket packet;
  packet.packetType = 0xBF0;
  packet.group = group;
  packet.index = index;
  packet.slot = slot;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_request = EnchantRequest{0xBF0, group, index, slot};
  m_unknown_0030 = true;
}

// OFFSET: 0x00686BF0
// FUNCTION: fn_05568
int L_AddRandomUpgradeEnchant(lua_State* L) {
  if (!lua_isnumber(L, -5)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddRandomUpgradeEnchant : 1th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -4)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddRandomUpgradeEnchant : 2th argument must be number");
    return 2;
  }

  if (!lua_isstring(L, -3)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddRandomUpgradeEnchant : 3th argument must be string");
    return 2;
  }

  if (!lua_isstring(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddRandomUpgradeEnchant : 4th argument must be string");
    return 2;
  }

  if (!lua_isnumber(L, -1)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddRandomUpgradeEnchant : 5th argument must be number");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -5));
  uint32_t optionKey = static_cast<uint32_t>(lua_tonumber(L, -4));
  std::string firstItemName(lua_tostring(L, -3));
  std::string secondItemName(lua_tostring(L, -2));
  uint64_t rate = static_cast<uint64_t>(lua_tonumber(L, -1));

  CEnchantMgr* mgr = CEnchantMgr::GetInstance();
  int firstItemId = CItemDBToIDMgr::GetInstance()->GetItemID(firstItemName);
  if (firstItemId != 0) {
    int secondItemId = CItemDBToIDMgr::GetInstance()->GetItemID(secondItemName);
    if (secondItemId != 0) {
      mgr->m_enchants[id].options[optionKey].randomUpgrades[firstItemId].rates[secondItemId] = rate;
    }
  }
  return 2;
}

// OFFSET: 0x00686F30
// FUNCTION: fn_05569
int L_AddTargetItem(lua_State* L) {
  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTargetItem : 1th argument must be number");
    return 2;
  }

  if (!lua_isstring(L, -1)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTargetItem : 2th argument must be string");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -2));
  CEnchantMgr* mgr = CEnchantMgr::GetInstance();
  std::string itemName(lua_tostring(L, -1));
  int itemId = CItemDBToIDMgr::GetInstance()->GetItemID(itemName);
  if (itemId != 0) {
    mgr->m_enchants[id].targetItems.push_back(itemId);
  }
  return 2;
}

// OFFSET: 0x006875D0
// FUNCTION: fn_05571
int L_ApproveRandomOption(lua_State* L) {
  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_ApproveRandomOption : 1th argument must be number");
    return 2;
  }

  if (lua_type(L, -1) != LUA_TBOOLEAN) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_ApproveRandomOption : 2th argument must be boolean");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -2));
  bool approve = lua_toboolean(L, -1) != 0;
  CEnchantMgr::GetInstance()->m_enchants[id].approveRandomOption = approve;
  return 2;
}

// OFFSET: 0x006876D0
// FUNCTION: fn_05572
int L_GetSlotCount(lua_State* L) {
  if (lua_tostring(L, -1) != nullptr) {
    int itemId = CItemDBToIDMgr::GetInstance()->GetItemID(lua_tostring(L, -1));
    if (itemId != 0) {
      ITEM_INFO item;
      item.SetItemId(itemId);
      lua_pushnumber(L, ItemSlotsCount(item));
      return 1;
    }
  }

  lua_pushnumber(L, -1);
  return 1;
}

// OFFSET: 0x00687870
// FUNCTION: fn_05573
int L_SetCaution(lua_State* L) {
  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetCaution : 1th argument must be number");
    return 2;
  }

  if (!lua_isstring(L, -1)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetCaution : 2th argument must be string");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -2));
  std::string caution(lua_tostring(L, -1));
  CEnchantMgr::GetInstance()->m_enchants[id].caution = caution;
  return 2;
}

// OFFSET: 0x00687A50
// FUNCTION: fn_05574
int L_SetCondition(lua_State* L) {
  if (!lua_isnumber(L, -3)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetCondition : 1th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetCondition : 2th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -1)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetCondition : 3th argument must be number");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -3));
  uint32_t condition1 = static_cast<uint32_t>(lua_tonumber(L, -2));
  uint32_t condition2 = static_cast<uint32_t>(lua_tonumber(L, -1));
  CEnchantMgr* mgr = CEnchantMgr::GetInstance();
  mgr->m_enchants[id].condition1 = condition1;
  mgr->m_enchants[id].condition2 = condition2;
  return 2;
}

// OFFSET: 0x00687EF0
// FUNCTION: fn_05576
int L_SetGradeBonus(lua_State* L) {
  if (!lua_isnumber(L, -3)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetGradeBonus : 1th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetGradeBonus : 2th argument must be number");
    return 2;
  }

  if (lua_type(L, -1) != LUA_TTABLE) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetGradeBonus : 3th argument must be table");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -3));
  uint32_t optionKey = static_cast<uint32_t>(lua_tonumber(L, -2));
  std::map<uint32_t, uint64_t> gradeBonus;
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    uint32_t grade = static_cast<uint32_t>(lua_tonumber(L, -2));
    uint64_t bonus = static_cast<uint64_t>(lua_tonumber(L, -1));
    gradeBonus[grade] = bonus;
    lua_pop(L, 1);
  }

  CEnchantMgr::GetInstance()->m_enchants[id].options[optionKey].gradeBonus = gradeBonus;
  return 2;
}

// OFFSET: 0x00688E70
// FUNCTION: fn_05580
int L_SetSlotOrder(lua_State* L) {
  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetSlotOrder : 1th argument must be number");
    return 2;
  }

  if (lua_type(L, -1) != LUA_TTABLE) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetSlotOrder : 2th argument must be table");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -2));
  std::vector<int> slots;
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    slots.push_back(static_cast<uint32_t>(lua_tonumber(L, -1)));
    lua_pop(L, 1);
  }

  CEnchantMgr::GetInstance()->m_enchants[id].slots = slots;
  return 2;
}

// OFFSET: 0x006890A0
// FUNCTION: fn_05581
int L_SetSuccessRate(lua_State* L) {
  if (!lua_isnumber(L, -3)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetSuccessRate : 1th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetSuccessRate : 2th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -1)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetSuccessRate : 3th argument must be number");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -3));
  uint32_t optionKey = static_cast<uint32_t>(lua_tonumber(L, -2));
  uint64_t rate = static_cast<uint64_t>(lua_tonumber(L, -1));
  CEnchantMgr::GetInstance()->m_enchants[id].options[optionKey].successRate = rate;
  return 2;
}

// OFFSET: 0x006821B0
// FUNCTION: fn_05537
void CEnchantMgr::AddPerfectEnchant(uint64_t group, uint32_t optionKey, const std::string& itemName, uint64_t cost,
                                    const std::map<std::string, int>& materials) {
  int itemId = CItemDBToIDMgr::GetInstance()->GetItemID(itemName);
  if (itemId != 0) {
    m_enchants[group].options[optionKey].perfectEnchants[itemId].cost = cost;
    m_enchants[group].options[optionKey].perfectEnchants[itemId].materials = ConvertMaterials(materials);
  }
}

// OFFSET: 0x00682310
// FUNCTION: fn_05538
void CEnchantMgr::AddPerfectUpgradeEnchant(uint64_t group, uint32_t optionKey, const std::string& baseItemName, const std::string& itemName,
                                           uint64_t cost, const std::map<std::string, int>& materials) {
  int baseItemId = CItemDBToIDMgr::GetInstance()->GetItemID(baseItemName);
  if (baseItemId != 0) {
    int itemId = CItemDBToIDMgr::GetInstance()->GetItemID(itemName);
    if (itemId != 0) {
      m_enchants[group].options[optionKey].perfectUpgrades[baseItemId][itemId].cost = cost;
      m_enchants[group].options[optionKey].perfectUpgrades[baseItemId][itemId].materials = ConvertMaterials(materials);
    }
  }
}

// OFFSET: 0x006827B0
// FUNCTION: fn_05539
bool CEnchantMgr::CallScript(const std::string& name) {
  lua_State* L = m_lua.GetState();
  if (L == nullptr) {
    return false;
  }

  lua_getglobal(L, name.c_str());
  if (lua_pcall(L, 0, 2, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(L, -1), "CEnchantMgr", MB_OK);
    return false;
  }

  if (lua_type(L, -2) != LUA_TBOOLEAN) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "1th argument must be boolean");
    return false;
  }

  if (!lua_isstring(L, -1)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "2th argument must be string");
    return false;
  }

  int success = lua_toboolean(L, -2);
  std::string message(lua_tostring(L, -1));
  if (!success) {
    MessageBoxA(nullptr, lua_tostring(L, -1), "CEnchantMgr", MB_OK);
    return false;
  }
  return true;
}

// OFFSET: 0x006829A0
// FUNCTION: fn_05541
EnchantInfo* CEnchantMgr::GetEnchantInfo(uint64_t group) {
  auto it = m_enchants.find(group);
  if (it == m_enchants.end()) {
    return nullptr;
  }
  return &it->second;
}

// OFFSET: 0x00682A00
// FUNCTION: fn_05542
std::map<int, int> CEnchantMgr::ConvertMaterials(std::map<std::string, int> materials) {
  std::map<int, int> result;
  for (auto it = materials.begin(); it != materials.end(); ++it) {
    int itemId = CItemDBToIDMgr::GetInstance()->GetItemID(it->first);
    if (itemId != 0) {
      result[itemId] = it->second;
    }
  }
  return result;
}

// OFFSET: 0x00682D10
// FUNCTION: fn_05543
bool CEnchantMgr::Init() {
  m_lua.SetGlobal("MAX_REFINE_LEVEL", 20.0);
  m_lua.SetGlobal("MAX_GRADE_LEVEL", 7.0);
  m_lua.SetGlobal("MAX_SLOT_NUM", 4.0);
  m_lua.SetGlobal("MAX_MATERIAL_NUM", 8.0);
  m_lua.SetGlobal("IS_CLIENT", true);
  m_lua.SetGlobal("C_SetSlotOrder", L_SetSlotOrder);
  m_lua.SetGlobal("C_AddTargetItem", L_AddTargetItem);
  m_lua.SetGlobal("C_SetCondition", L_SetCondition);
  m_lua.SetGlobal("C_ApproveRandomOption", L_ApproveRandomOption);
  m_lua.SetGlobal("C_SetReset", L_SetReset);
  m_lua.SetGlobal("C_SetCaution", L_SetCaution);
  m_lua.SetGlobal("C_SetRequire", L_SetRequire);
  m_lua.SetGlobal("C_SetSuccessRate", L_SetSuccessRate);
  m_lua.SetGlobal("C_SetGradeBonus", L_SetGradeBonus);
  m_lua.SetGlobal("C_SetEnchant", L_SetEnchant);
  m_lua.SetGlobal("C_AddPerfectEnchant", L_AddPerfectEnchant);
  m_lua.SetGlobal("C_AddUpgradeEnchant", L_AddUpgradeEnchant);
  m_lua.SetGlobal("C_SetRandomUpgradeRequire", L_SetRandomUpgradeRequire);
  m_lua.SetGlobal("C_AddRandomUpgradeEnchant", L_AddRandomUpgradeEnchant);
  m_lua.SetGlobal("C_AddPerfectUpgradeEnchant", L_AddPerfectUpgradeEnchant);
  m_lua.SetGlobal("C_GetSlotCount", L_GetSlotCount);

  if (!m_lua.LoadFile("LuaFiles514\\Lua Files\\Enchant\\EnchantList_f.lub")) {
    return false;
  }
  if (!m_lua.LoadFile("LuaFiles514\\Lua Files\\Enchant\\EnchantList.lub")) {
    return false;
  }
  if (!CallScript("CheckFile")) {
    return false;
  }
  return CallScript("LoadAllData");
}

// OFFSET: 0x006845C0
// FUNCTION: fn_05552
void CEnchantMgr::SetEnchant(uint64_t group, uint32_t optionKey, uint32_t slot, const std::map<std::string, uint64_t>& enchants) {
  std::map<int, uint64_t> items;
  for (auto it = enchants.begin(); it != enchants.end(); ++it) {
    int itemId = CItemDBToIDMgr::GetInstance()->GetItemID(it->first);
    if (itemId != 0) {
      items[itemId] = it->second;
    }
  }
  m_enchants[group].options[optionKey].enchants[slot] = items;
}

// OFFSET: 0x006864D0
// FUNCTION: fn_05566
int L_AddPerfectEnchant(lua_State* L) {
  if (!lua_isnumber(L, -5)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddPerfectEnchant : 1th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -4)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddPerfectEnchant : 2th argument must be number");
    return 2;
  }

  if (!lua_isstring(L, -3)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddPerfectEnchant : 3th argument must be string");
    return 2;
  }

  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddPerfectEnchant : 4th argument must be number");
    return 2;
  }

  if (lua_type(L, -1) != LUA_TTABLE) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddPerfectEnchant : 5th argument must be table");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -5));
  uint32_t optionKey = static_cast<uint32_t>(lua_tonumber(L, -4));
  std::string itemName(lua_tostring(L, -3));
  uint64_t cost = static_cast<uint64_t>(lua_tonumber(L, -2));
  std::map<std::string, int> materials;
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    uint32_t count = static_cast<uint32_t>(lua_tonumber(L, -1));
    materials[lua_tostring(L, -2)] = count;
    lua_pop(L, 1);
  }

  CEnchantMgr::GetInstance()->AddPerfectEnchant(id, optionKey, itemName, cost, materials);
  return 2;
}

// OFFSET: 0x00686830
// FUNCTION: fn_05567
int L_AddPerfectUpgradeEnchant(lua_State* L) {
  if (!lua_isnumber(L, -6)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddPerfectEnchant : 1th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -5)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddPerfectEnchant : 2th argument must be number");
    return 2;
  }

  if (!lua_isstring(L, -4)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddPerfectEnchant : 3th argument must be string");
    return 2;
  }

  if (!lua_isstring(L, -3)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddPerfectEnchant : 4th argument must be string");
    return 2;
  }

  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddPerfectEnchant : 5th argument must be number");
    return 2;
  }

  if (lua_type(L, -1) != LUA_TTABLE) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddPerfectEnchant : 6th argument must be table");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -6));
  uint32_t optionKey = static_cast<uint32_t>(lua_tonumber(L, -5));
  std::string baseItemName(lua_tostring(L, -4));
  std::string itemName(lua_tostring(L, -3));
  uint64_t cost = static_cast<uint64_t>(lua_tonumber(L, -2));
  std::map<std::string, int> materials;
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    uint32_t count = static_cast<uint32_t>(lua_tonumber(L, -1));
    materials[lua_tostring(L, -2)] = count;
    lua_pop(L, 1);
  }

  CEnchantMgr::GetInstance()->AddPerfectUpgradeEnchant(id, optionKey, baseItemName, itemName, cost, materials);
  return 2;
}

// OFFSET: 0x006871E0
// FUNCTION: fn_05570
int L_AddUpgradeEnchant(lua_State* L) {
  if (!lua_isnumber(L, -6)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddUpgradeEnchant : 1th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -5)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddUpgradeEnchant : 2th argument must be number");
    return 2;
  }

  if (!lua_isstring(L, -4)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddUpgradeEnchant : 3th argument must be string");
    return 2;
  }

  if (!lua_isstring(L, -3)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddUpgradeEnchant : 4th argument must be string");
    return 2;
  }

  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddUpgradeEnchant : 5th argument must be number");
    return 2;
  }

  if (lua_type(L, -1) != LUA_TTABLE) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddUpgradeEnchant : 6th argument must be table");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -6));
  uint32_t optionKey = static_cast<uint32_t>(lua_tonumber(L, -5));
  std::string baseItemName(lua_tostring(L, -4));
  std::string itemName(lua_tostring(L, -3));
  uint64_t cost = static_cast<uint64_t>(lua_tonumber(L, -2));
  std::map<std::string, int> materials;
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    uint32_t count = static_cast<uint32_t>(lua_tonumber(L, -1));
    materials[lua_tostring(L, -2)] = count;
    lua_pop(L, 1);
  }

  CEnchantMgr* mgr = CEnchantMgr::GetInstance();
  mgr->m_enchants[id].options[optionKey].isUpgrade = true;
  mgr->AddPerfectUpgradeEnchant(id, optionKey, baseItemName, itemName, cost, materials);
  return 2;
}

// OFFSET: 0x00687BB0
// FUNCTION: fn_05575
int L_SetEnchant(lua_State* L) {
  if (!lua_isnumber(L, -4)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetEnchant : 1th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -3)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetEnchant : 2th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetEnchant : 3th argument must be number");
    return 2;
  }

  if (lua_type(L, -1) != LUA_TTABLE) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetEnchant : 4th argument must be table");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -4));
  uint32_t optionKey = static_cast<uint32_t>(lua_tonumber(L, -3));
  uint32_t slot = static_cast<uint32_t>(lua_tonumber(L, -2));
  std::map<std::string, uint64_t> enchants;
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    uint64_t rate = static_cast<uint64_t>(lua_tonumber(L, -1));
    enchants[lua_tostring(L, -2)] = rate;
    lua_pop(L, 1);
  }

  CEnchantMgr::GetInstance()->SetEnchant(id, optionKey, slot, enchants);
  return 2;
}

// OFFSET: 0x00688290
// FUNCTION: fn_05577
int L_SetRandomUpgradeRequire(lua_State* L) {
  if (!lua_isnumber(L, -5)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetRandomUpgradeRequire : 1th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -4)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetRandomUpgradeRequire : 2th argument must be number");
    return 2;
  }

  if (!lua_isstring(L, -3)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetRandomUpgradeRequire : 3th argument must be string");
    return 2;
  }

  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetRandomUpgradeRequire : 4th argument must be number");
    return 2;
  }

  if (lua_type(L, -1) != LUA_TTABLE) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetRandomUpgradeRequire : 4th argument must be table");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -5));
  uint32_t optionKey = static_cast<uint32_t>(lua_tonumber(L, -4));
  std::string itemName(lua_tostring(L, -3));
  uint64_t cost = static_cast<uint64_t>(lua_tonumber(L, -2));
  std::map<std::string, int> materials;
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    uint32_t count = static_cast<uint32_t>(lua_tonumber(L, -1));
    materials[lua_tostring(L, -2)] = count;
    lua_pop(L, 1);
  }

  CEnchantMgr* mgr = CEnchantMgr::GetInstance();
  int itemId = CItemDBToIDMgr::GetInstance()->GetItemID(itemName);
  if (itemId != 0) {
    mgr->m_enchants[id].options[optionKey].randomUpgrades[itemId].cost = cost;
    mgr->m_enchants[id].options[optionKey].randomUpgrades[itemId].materials = mgr->ConvertMaterials(materials);
  }
  return 2;
}

// OFFSET: 0x006886E0
// FUNCTION: fn_05578
int L_SetRequire(lua_State* L) {
  if (!lua_isnumber(L, -4)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetRequire : 1th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -3)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetRequire : 2th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetRequire : 3th argument must be number");
    return 2;
  }

  if (lua_type(L, -1) != LUA_TTABLE) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetRequire : 4th argument must be table");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -4));
  uint32_t optionKey = static_cast<uint32_t>(lua_tonumber(L, -3));
  uint64_t cost = static_cast<uint64_t>(lua_tonumber(L, -2));
  std::map<std::string, int> materials;
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    uint32_t count = static_cast<uint32_t>(lua_tonumber(L, -1));
    materials[lua_tostring(L, -2)] = count;
    lua_pop(L, 1);
  }

  CEnchantMgr* mgr = CEnchantMgr::GetInstance();
  mgr->m_enchants[id].options[optionKey].requireCost = cost;
  mgr->m_enchants[id].options[optionKey].requireMaterials = mgr->ConvertMaterials(materials);
  return 2;
}

// OFFSET: 0x00688A70
// FUNCTION: fn_05579
int L_SetReset(lua_State* L) {
  if (!lua_isnumber(L, -5)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetReset : 1th argument must be number");
    return 2;
  }

  if (lua_type(L, -4) != LUA_TBOOLEAN) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetReset : 2th argument must be boolean");
    return 2;
  }

  if (!lua_isnumber(L, -3)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetReset : 3th argument must be number");
    return 2;
  }

  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetReset : 4th argument must be number");
    return 2;
  }

  if (lua_type(L, -1) != LUA_TTABLE) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_SetReset : 5th argument must be table");
    return 2;
  }

  uint64_t id = static_cast<uint64_t>(lua_tonumber(L, -5));
  bool enabled = lua_toboolean(L, -4) != 0;
  uint64_t rate = static_cast<uint64_t>(lua_tonumber(L, -3));
  uint64_t cost = static_cast<uint64_t>(lua_tonumber(L, -2));
  std::map<std::string, int> materials;
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    uint32_t count = static_cast<uint32_t>(lua_tonumber(L, -1));
    materials[lua_tostring(L, -2)] = count;
    lua_pop(L, 1);
  }

  CEnchantMgr* mgr = CEnchantMgr::GetInstance();
  mgr->m_enchants[id].resetEnabled = enabled;
  mgr->m_enchants[id].resetRate = rate;
  mgr->m_enchants[id].resetCost = cost;
  mgr->m_enchants[id].resetMaterials = mgr->ConvertMaterials(materials);
  return 2;
}
