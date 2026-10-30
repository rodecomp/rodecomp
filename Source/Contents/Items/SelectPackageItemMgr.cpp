#include "Contents/Items/SelectPackageItemMgr.h"
#include "Network/TextEncoding.h"
#include "Script/LuaInstance.h"
extern "C" {
#include <lua.h>
}
#include <new>
#include <windows.h>

// GLOBAL: gl_24790
template <>
CSelectPackageItemMgr* Singleton<CSelectPackageItemMgr>::s_instance = nullptr;

// OFFSET: 0x0061E280
// FUNCTION: fn_04439
CSelectPackageItemMgr::CSelectPackageItemMgr() {
}

// OFFSET: 0x0061EC30
// FUNCTION: fn_04445
void CSelectPackageItemMgr::AddTab(int packageId, int tabIndex, const std::string& name, int itemCount) {
  if (this->m_packages.find(packageId) == this->m_packages.end()) {
    this->m_packages[packageId] = std::map<int, SelectPackageTab>();
  }

  if (this->m_packages[packageId].find(tabIndex) == this->m_packages[packageId].end()) {
    this->m_packages[packageId][tabIndex] = SelectPackageTab();
  }

  this->m_packages[packageId][tabIndex].name = name;
  this->m_packages[packageId][tabIndex].items.resize(itemCount);
}

// OFFSET: 0x0061EF60
// FUNCTION: fn_04446
std::optional<std::vector<ITEM_INFO>*> CSelectPackageItemMgr::FindItems(int packageId, int tabIndex) {
  std::map<int, std::map<int, SelectPackageTab>>::iterator package = this->m_packages.find(packageId);
  if (package != this->m_packages.end()) {
    std::map<int, SelectPackageTab>::iterator tab = package->second.find(tabIndex);
    if (tab != package->second.end()) {
      return &tab->second.items;
    }
  }

  return std::nullopt;
}

// OFFSET: 0x0061EFF0
// FUNCTION: fn_04447
bool CSelectPackageItemMgr::LoadScript() {
  CLuaInstance script;
  script.SetGlobal("AddTab", CSelectPackageItemMgr::LuaAddTab);
  script.SetGlobal("AddItem", CSelectPackageItemMgr::LuaAddItem);
  if (script.LoadFile("LuaFiles514\\Lua Files\\SelectPackage\\SelectPackageItem_Cln.lub") == true) {
    lua_State* state = script.GetState();
    if (state) {
      lua_getfield(state, LUA_GLOBALSINDEX, "LoadTable");
      if (lua_pcall(state, 0, 0, 0) != 0) {
        MessageBoxA(nullptr, lua_tolstring(state, -1, nullptr), "CSelectPackageItemMgr", 0);
      } else {
        lua_settop(state, -2);
      }
    }
  }

  return false;
}

// OFFSET: 0x0061F9A0
// FUNCTION: fn_04454
template <>
CSelectPackageItemMgr* Singleton<CSelectPackageItemMgr>::GetInstance() {
  if (!s_instance) {
    s_instance = new (std::nothrow) CSelectPackageItemMgr;
  }

  return s_instance;
}

// GLOBAL: gl_24907
short g_itemOptionPlaceholderId = 9999;

// OFFSET: 0x0061FA50
// FUNCTION: fn_04455
int CSelectPackageItemMgr::LuaAddItem(lua_State* L) {
  if (!lua_isnumber(L, -10)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 1st argument must be number");
    return 2;
  }
  if (!lua_isnumber(L, -9)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 2nd argument must be number");
    return 2;
  }
  if (!lua_isnumber(L, -8)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 3rd argument must be number");
    return 2;
  }
  if (!lua_isnumber(L, -7)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 4th argument must be number");
    return 2;
  }
  if (!lua_isnumber(L, -6)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 5th argument must be number");
    return 2;
  }
  if (!lua_isnumber(L, -5)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 6th argument must be number");
    return 2;
  }
  if (!lua_isnumber(L, -4)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 7th argument must be number");
    return 2;
  }
  if (lua_type(L, -3) != LUA_TBOOLEAN) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 8th argument must be boolean");
    return 2;
  }
  if (!lua_isnumber(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 9th argument must be number");
    return 2;
  }
  if (!lua_isnumber(L, -1)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 10th argument must be number");
    return 2;
  }

  int packageId = static_cast<int>(lua_tonumber(L, -10));
  int tabIndex = static_cast<int>(lua_tonumber(L, -9));
  const double index = lua_tonumber(L, -8);
  const double itemId = lua_tonumber(L, -7);
  const double quantity = lua_tonumber(L, -6);
  const double option = lua_tonumber(L, -5);
  const double value = lua_tonumber(L, -4);
  const bool placeholder = lua_toboolean(L, -3) != 0;
  const double refine = lua_tonumber(L, -2);
  const double kind = lua_tonumber(L, -1);

  std::vector<ITEM_INFO>& items = CSelectPackageItemMgr::GetInstance()->m_packages[packageId][tabIndex].items;
  const int slot = static_cast<int>(index);
  if (slot >= 0 && static_cast<std::size_t>(slot) < items.size()) {
    ITEM_INFO& item = items[slot];
    item.Reset();
    item.SetItemId(static_cast<int>(itemId));
    item.identified = 1;
    item.quantity = static_cast<int>(quantity);
    item.unknown_0000[0] = static_cast<unsigned char>(static_cast<int>(kind));
    item.unknown_0060 = static_cast<int>(value);
    item.unknown_0088 = static_cast<short>(static_cast<int>(refine));
    const int optionValue = static_cast<int>(option);
    if (optionValue != 0) {
      item.unknown_0066 = 1;
      item.unknown_0068 = optionValue;
    }
    if (placeholder) {
      *reinterpret_cast<short*>(item.unknown_009C) = g_itemOptionPlaceholderId;
      item.unknown_0098 = 3;
    }
  }

  lua_pushboolean(L, 1);
  lua_pushstring(L, "good");
  return 2;
}

// OFFSET: 0x0061FD90
// FUNCTION: fn_04456
int CSelectPackageItemMgr::LuaAddTab(lua_State* L) {
  if (!lua_isnumber(L, -4)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 1st argument must be number");
    return 2;
  }
  if (!lua_isnumber(L, -3)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 2nd argument must be number");
    return 2;
  }
  if (!lua_isstring(L, -2)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 3rd argument must be string");
    return 2;
  }
  if (!lua_isnumber(L, -1)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "L_AddTab : 4th argument must be number");
    return 2;
  }

  int packageId = static_cast<int>(lua_tonumber(L, -4));
  int tabIndex = static_cast<int>(lua_tonumber(L, -3));
  std::string name = FromUtf8(lua_tostring(L, -2), g_textCodePage);
  int itemCount = static_cast<int>(lua_tonumber(L, -1));
  CSelectPackageItemMgr::GetInstance()->AddTab(packageId, tabIndex, name, itemCount);

  lua_pushboolean(L, 1);
  lua_pushstring(L, "good");
  return 2;
}
