#include "Contents/EnchantMgr.h"
#include "Client/Session.h"
#include "Contents/ItemDBToIDMgr.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
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
