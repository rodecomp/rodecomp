#include "Contents/Items/SelectPackageItemMgr.h"
#include "Network/TextEncoding.h"

extern "C" {
#include <lua.h>
}
#include <new>

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

// OFFSET: 0x0061F9A0
// FUNCTION: fn_04454
template <>
CSelectPackageItemMgr* Singleton<CSelectPackageItemMgr>::GetInstance() {
  if (!s_instance) {
    s_instance = new (std::nothrow) CSelectPackageItemMgr;
  }

  return s_instance;
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
