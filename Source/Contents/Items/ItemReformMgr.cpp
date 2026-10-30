#include "Contents/Items/ItemReformMgr.h"

#include "Client/Application/ErrorDialog.h"
#include "Contents/ItemDBToIDMgr.h"
#include "Utility/Text/FormatString.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <new>
#include <utility>

// GLOBAL: gl_24923
CItemReformMgr* g_itemReformMgr = nullptr;

// STRING: st_01024
static const char kItemReformSystemPath[] = "LuaFiles514\\Lua Files\\ItemReform\\ItemReformSystem.lub";

// OFFSET: 0x005F92A0
// FUNCTION: fn_04074
ItemReformInfo::ItemReformInfo() {
}

// OFFSET: 0x005F91D0
// FUNCTION: fn_04073
CItemReformMgr::CItemReformMgr() : m_maxMaterials(6) {
}

CItemReformMgr::~CItemReformMgr() {
}

// OFFSET: 0x005F9350
// FUNCTION: fn_04075
void CItemReformMgr::NullSub() {
}

// OFFSET: 0x005F9DF0
// FUNCTION: fn_04081
std::vector<int> CItemReformMgr::GetReformBaseItems(int itemId) {
  std::vector<int> result;
  const auto item = this->m_reformItemList.find(itemId);
  if (item == this->m_reformItemList.end()) {
    return result;
  }

  for (std::vector<std::pair<std::uint32_t, std::uint32_t>>::const_iterator it = item->second.begin(); it != item->second.end();
       ++it) {
    const std::uint64_t key = (static_cast<std::uint64_t>(it->second) << 32) | it->first;
    const auto reform = this->m_reformInfo.find(key);
    if (reform != this->m_reformInfo.end()) {
      result.push_back(reform->second.baseItem);
    }
  }

  return result;
}

// OFFSET: 0x005F9F60
// FUNCTION: fn_04082
ItemReformInfo* CItemReformMgr::FindReform(int itemId, int baseItem) {
  const auto item = this->m_reformItemList.find(itemId);
  if (item == this->m_reformItemList.end()) {
    return nullptr;
  }

  for (std::vector<std::pair<std::uint32_t, std::uint32_t>>::const_iterator it = item->second.begin(); it != item->second.end();
       ++it) {
    const std::uint64_t key = (static_cast<std::uint64_t>(it->second) << 32) | it->first;
    const auto reform = this->m_reformInfo.find(key);
    if (reform != this->m_reformInfo.end() && reform->second.baseItem == baseItem) {
      return &reform->second;
    }
  }

  return nullptr;
}

// OFFSET: 0x005F9FF0
// FUNCTION: fn_04083
bool CItemReformMgr::Load() {
  if (!this->m_lua.LoadFile(kItemReformSystemPath)) {
    return false;
  }

  this->LoadReformInfo();
  this->LoadReformItemList();
  return true;
}

// OFFSET: 0x005FA140
// FUNCTION: fn_04084
bool CItemReformMgr::HasItem(int itemId) {
  return this->m_reformItemList.find(itemId) != this->m_reformItemList.end();
}

static void EnsureItemDbMgr() {
  if (g_itemDbToIdMgr == nullptr) {
    g_itemDbToIdMgr = new (std::nothrow) CItemDBToIDMgr;
  }
}

// OFFSET: 0x005FA180
// FUNCTION: fn_04085
void CItemReformMgr::LoadReformInfo() {
  this->m_reformInfo.clear();

  lua_State* state = this->m_lua.GetState();
  if (state == nullptr) {
    return;
  }

  lua_getglobal(state, "ReformInfo");
  if (lua_type(state, -1) != LUA_TTABLE) {
    ShowErrorDialog("Can't find global table \"ReformInfo\"", 0);
  } else {
    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      const std::int64_t reformId = static_cast<std::int64_t>(static_cast<int>(lua_tointeger(state, -2)));
      ItemReformInfo info;

      lua_pushstring(state, "BaseItem");
      lua_gettable(state, -2);
      if (lua_isstring(state, -1) == 0) {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find BaseItem", static_cast<std::uint64_t>(reformId)), 0);
      } else {
        EnsureItemDbMgr();
        info.baseItem = g_itemDbToIdMgr->GetItemID(lua_tostring(state, -1));
      }
      lua_pop(state, 1);

      lua_pushstring(state, "ResultItem");
      lua_gettable(state, -2);
      if (lua_isstring(state, -1) == 0) {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find ResultItem", static_cast<std::uint64_t>(reformId)), 0);
      } else {
        EnsureItemDbMgr();
        info.resultItem = g_itemDbToIdMgr->GetItemID(lua_tostring(state, -1));
      }
      lua_pop(state, 1);

      lua_pushstring(state, "Material");
      lua_gettable(state, -2);
      if (lua_type(state, -1) != LUA_TTABLE) {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find Material", static_cast<std::uint64_t>(reformId)), 0);
      } else {
        lua_pushnil(state);
        while (lua_next(state, -2) != 0) {
          if (this->m_maxMaterials <= info.materials.size()) {
            ShowErrorDialog(FormatOwnedString("Num of ReformInfo[ %I64u ].Material can't be more than %u",
                                              static_cast<std::uint64_t>(reformId), this->m_maxMaterials),
                            0);
          }

          EnsureItemDbMgr();
          const int materialId = g_itemDbToIdMgr->GetItemID(lua_tostring(state, -2));
          ItemReformMaterial material;
          material.count = static_cast<int>(lua_tointeger(state, -1));
          material.unknown_0004 = 0;
          info.materials[materialId] = material;
          lua_pop(state, 1);
        }
      }
      lua_pop(state, 1);

      lua_pushstring(state, "NeedRefineMin");
      lua_gettable(state, -2);
      if (lua_isnumber(state, -1) == 0) {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find NeedRefineMin", static_cast<std::uint64_t>(reformId)), 0);
      } else {
        info.needRefineMin = static_cast<std::int64_t>(static_cast<int>(lua_tointeger(state, -1)));
        if (info.needRefineMin < 0) {
          ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ].NeedRefineMin can't be less than 0", static_cast<std::uint64_t>(reformId)),
                          0);
        }
      }
      lua_pop(state, 1);

      lua_pushstring(state, "NeedRefineMax");
      lua_gettable(state, -2);
      if (lua_isnumber(state, -1) == 0) {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find NeedRefineMax", static_cast<std::uint64_t>(reformId)), 0);
      } else {
        info.needRefineMax = static_cast<std::int64_t>(static_cast<int>(lua_tointeger(state, -1)));
        if (info.needRefineMax < info.needRefineMin || info.needRefineMax > 20) {
          ShowErrorDialog(
              FormatOwnedString("ReformInfo[ %I64u ].needRefineMax must be between needRefineMin and 20", static_cast<std::uint64_t>(reformId)),
              0);
        }
      }
      lua_pop(state, 1);

      lua_pushstring(state, "NeedOptionNumMin");
      lua_gettable(state, -2);
      if (lua_isnumber(state, -1) == 0) {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find NeedOptionNumMin", static_cast<std::uint64_t>(reformId)), 0);
      } else {
        info.needOptionNumMin = static_cast<std::int64_t>(static_cast<int>(lua_tointeger(state, -1)));
        if (info.needOptionNumMin < 0) {
          ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ].needRanOptNum can't be less than 0", static_cast<std::uint64_t>(reformId)),
                          0);
        }
      }
      lua_pop(state, 1);

      lua_pushstring(state, "IsEmptySocket");
      lua_gettable(state, -2);
      if (lua_type(state, -1) != LUA_TBOOLEAN) {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find IsEmptySocket", static_cast<std::uint64_t>(reformId)), 0);
      } else {
        info.isEmptySocket = lua_toboolean(state, -1) != 0;
      }
      lua_pop(state, 1);

      lua_pushstring(state, "ChangeRefineValue");
      lua_gettable(state, -2);
      if (lua_isnumber(state, -1) == 0) {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find ChangeRefineValue", static_cast<std::uint64_t>(reformId)), 0);
      } else {
        info.changeRefineValue = static_cast<std::int64_t>(static_cast<int>(lua_tointeger(state, -1)));
        if (info.changeRefineValue < -20 || info.changeRefineValue > 20) {
          ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ].ChangeRefineValue can't be less than 0 and more than 20",
                                            static_cast<std::uint64_t>(reformId)),
                          0);
        }
      }
      lua_pop(state, 1);

      lua_pushstring(state, "RandomOptionCode");
      lua_gettable(state, -2);
      info.randomOptionCode = lua_tostring(state, -1);
      lua_pop(state, 1);

      lua_pushstring(state, "PreserveSocketItem");
      lua_gettable(state, -2);
      if (lua_type(state, -1) != LUA_TBOOLEAN) {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find PreserveSocketItem", static_cast<std::uint64_t>(reformId)), 0);
      } else {
        info.preserveSocketItem = lua_toboolean(state, -1) != 0;
      }
      lua_pop(state, 1);

      lua_pushstring(state, "PreserveGrade");
      lua_gettable(state, -2);
      if (lua_type(state, -1) != LUA_TBOOLEAN) {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find PreserveGrade", static_cast<std::uint64_t>(reformId)), 0);
      } else {
        info.preserveGrade = lua_toboolean(state, -1) != 0;
      }
      lua_pop(state, 1);

      lua_pushstring(state, "InformationString");
      lua_gettable(state, -2);
      if (lua_type(state, -1) == LUA_TTABLE) {
        lua_pushnil(state);
        while (lua_next(state, -2) != 0) {
          info.informationString += lua_tostring(state, -1);
          info.informationString += '\n';
          lua_pop(state, 1);
        }
        if (!info.informationString.empty() && info.informationString.back() == '\n') {
          info.informationString.pop_back();
        }
      }
      lua_pop(state, 1);

      this->m_reformInfo[static_cast<std::uint64_t>(reformId)] = info;
      lua_pop(state, 1);
    }
  }

  lua_pop(state, 1);
}

// OFFSET: 0x005FB1E0
// FUNCTION: fn_04086
void CItemReformMgr::LoadReformItemList() {
  this->m_reformItemList.clear();

  lua_State* state = this->m_lua.GetState();
  if (state == nullptr) {
    return;
  }

  lua_getglobal(state, "ReformItemList");
  if (lua_type(state, -1) != LUA_TTABLE) {
    ShowErrorDialog("Can't find global table \"ReformItemList\"", 0);
  }

  lua_pushnil(state);
  while (lua_next(state, -2) != 0) {
    const std::string itemName(lua_tostring(state, -2));
    EnsureItemDbMgr();
    const int itemId = g_itemDbToIdMgr->GetItemID(itemName);

    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      const std::uint32_t reformId = static_cast<std::uint32_t>(static_cast<int>(lua_tointeger(state, -1)));
      if (this->m_reformInfo.find(reformId) == this->m_reformInfo.end()) {
        ShowErrorDialog(FormatOwnedString("ReformItemList[ %s ] : Can't find [ %I64u ] in ReformInfo", itemName.c_str(),
                                          static_cast<std::uint64_t>(reformId)),
                        0);
      } else {
        this->m_reformItemList[itemId].push_back(std::make_pair(reformId, 0u));
      }
      lua_pop(state, 1);
    }

    lua_pop(state, 1);
  }

  lua_pop(state, 1);
}
