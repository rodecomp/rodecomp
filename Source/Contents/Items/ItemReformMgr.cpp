#include "Contents/Items/ItemReformMgr.h"

#include "Client/Application/ErrorDialog.h"
#include "Contents/Items/ItemDBToIDMgr.h"
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

  for (std::vector<std::uint64_t>::const_iterator it = item->second.begin(); it != item->second.end();
       ++it) {
    const auto reform = this->m_reformInfo.find(*it);
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

  for (const std::uint64_t& reformId : item->second) {
    const auto reform = this->m_reformInfo.find(reformId);
    if (reform == this->m_reformInfo.end()) {
      continue;
    }
    if (reform->second.baseItem == baseItem) {
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
  return this->m_reformItemList.end() != this->m_reformItemList.find(itemId);
}

// INLINED
static CItemDBToIDMgr* EnsureItemDbMgr() {
  if (g_itemDbToIdMgr == nullptr) {
    g_itemDbToIdMgr = new (std::nothrow) CItemDBToIDMgr;
  }
  return g_itemDbToIdMgr;
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
  if (lua_type(state, -1) == LUA_TTABLE) {
    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      const std::uint64_t reformId = lua_tointeger(state, -2);
      ItemReformInfo info;

      lua_pushstring(state, "BaseItem");
      lua_gettable(state, -2);
      if (lua_isstring(state, -1) != 0) {
        info.baseItem = EnsureItemDbMgr()->GetItemID(lua_tostring(state, -1));
      } else {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find BaseItem", reformId).c_str(), 0);
      }
      lua_pop(state, 1);

      lua_pushstring(state, "ResultItem");
      lua_gettable(state, -2);
      if (lua_isstring(state, -1) != 0) {
        info.resultItem = EnsureItemDbMgr()->GetItemID(lua_tostring(state, -1));
      } else {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find ResultItem", reformId).c_str(), 0);
      }
      lua_pop(state, 1);

      lua_pushstring(state, "Material");
      lua_gettable(state, -2);
      if (lua_type(state, -1) == LUA_TTABLE) {
        lua_pushnil(state);
        while (lua_next(state, -2) != 0) {
          if (this->m_maxMaterials <= info.materials.size()) {
            ShowErrorDialog(FormatOwnedString("Num of ReformInfo[ %I64u ].Material can't be more than %u", reformId, this->m_maxMaterials).c_str(), 0);
          }

          const int materialId = EnsureItemDbMgr()->GetItemID(lua_tostring(state, -2));
          info.materials[materialId] = static_cast<unsigned int>(lua_tointeger(state, -1));
          lua_pop(state, 1);
        }
      } else {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find Material", reformId).c_str(), 0);
      }
      lua_pop(state, 1);

      lua_pushstring(state, "NeedRefineMin");
      lua_gettable(state, -2);
      if (lua_isnumber(state, -1) != 0) {
        const std::int64_t needRefineMin = lua_tointeger(state, -1);
        if (needRefineMin < 0) {
          ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ].NeedRefineMin can't be less than 0", reformId).c_str(), 0);
        }
        info.needRefineMin = needRefineMin;
      } else {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find NeedRefineMin", reformId).c_str(), 0);
      }
      lua_pop(state, 1);

      lua_pushstring(state, "NeedRefineMax");
      lua_gettable(state, -2);
      if (lua_isnumber(state, -1) != 0) {
        const std::int64_t needRefineMax = lua_tointeger(state, -1);
        if (needRefineMax < info.needRefineMin || needRefineMax > 20) {
          ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ].needRefineMax must be between needRefineMin and 20", reformId).c_str(), 0);
        }
        info.needRefineMax = needRefineMax;
      } else {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find NeedRefineMax", reformId).c_str(), 0);
      }
      lua_pop(state, 1);

      lua_pushstring(state, "NeedOptionNumMin");
      lua_gettable(state, -2);
      if (lua_isnumber(state, -1) != 0) {
        const std::int64_t needOptionNumMin = lua_tointeger(state, -1);
        if (needOptionNumMin < 0) {
          ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ].needRanOptNum can't be less than 0", reformId).c_str(), 0);
        }
        info.needOptionNumMin = needOptionNumMin;
      } else {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find NeedOptionNumMin", reformId).c_str(), 0);
      }
      lua_pop(state, 1);

      lua_pushstring(state, "IsEmptySocket");
      lua_gettable(state, -2);
      if (lua_type(state, -1) == LUA_TBOOLEAN) {
        info.isEmptySocket = lua_toboolean(state, -1) > 0;
      } else {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find IsEmptySocket", reformId).c_str(), 0);
      }
      lua_pop(state, 1);

      lua_pushstring(state, "ChangeRefineValue");
      lua_gettable(state, -2);
      if (lua_isnumber(state, -1) != 0) {
        const std::int64_t changeRefineValue = lua_tointeger(state, -1);
        if (changeRefineValue < -20 || changeRefineValue > 20) {
          ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ].ChangeRefineValue can't be less than 0 and more than 20", reformId).c_str(), 0);
        }
        info.changeRefineValue = changeRefineValue;
      } else {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find ChangeRefineValue", reformId).c_str(), 0);
      }
      lua_pop(state, 1);

      lua_pushstring(state, "RandomOptionCode");
      lua_gettable(state, -2);
      info.randomOptionCode = lua_tostring(state, -1);
      lua_pop(state, 1);

      lua_pushstring(state, "PreserveSocketItem");
      lua_gettable(state, -2);
      if (lua_type(state, -1) == LUA_TBOOLEAN) {
        info.preserveSocketItem = lua_toboolean(state, -1) > 0;
      } else {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find PreserveSocketItem", reformId).c_str(), 0);
      }
      lua_pop(state, 1);

      lua_pushstring(state, "PreserveGrade");
      lua_gettable(state, -2);
      if (lua_type(state, -1) == LUA_TBOOLEAN) {
        info.preserveGrade = lua_toboolean(state, -1) > 0;
      } else {
        ShowErrorDialog(FormatOwnedString("ReformInfo[ %I64u ] : Can't find PreserveGrade", reformId).c_str(), 0);
      }
      lua_pop(state, 1);

      lua_pushstring(state, "InformationString");
      lua_gettable(state, -2);
      if (lua_type(state, -1) == LUA_TTABLE) {
        lua_pushnil(state);
        while (lua_next(state, -2) != 0) {
          info.informationString += lua_tostring(state, -1) + std::string("\n");
          lua_pop(state, 1);
        }
        if (!info.informationString.empty() && info.informationString.at(info.informationString.size() - 1) == '\n') {
          info.informationString.erase(info.informationString.size() - 1);
        }
      }
      lua_pop(state, 1);

      this->m_reformInfo[reformId] = info;
      lua_pop(state, 1);
    }
  } else {
    ShowErrorDialog("Can't find global table \"ReformInfo\"", 0);
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
    const int itemId = EnsureItemDbMgr()->GetItemID(itemName);

    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      const std::uint64_t reformId = static_cast<std::uint32_t>(lua_tointeger(state, -1));
      if (this->m_reformInfo.find(reformId) == this->m_reformInfo.end()) {
        ShowErrorDialog(FormatOwnedString("ReformItemList[ %s ] : Can't find [ %I64u ] in ReformInfo", itemName.c_str(), reformId), 0);
      } else {
        this->m_reformItemList[itemId].push_back(reformId);
      }
      lua_pop(state, 1);
    }

    lua_pop(state, 1);
  }

  lua_pop(state, 1);
}
