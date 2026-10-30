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
