#include "Contents/Items/NeoPackageItemMgr.h"
#include "Contents/Items/ItemInfoMgr.h"
#include "Contents/Items/SelectPackageItemMgr.h"
#include "Script/LuaInstance.h"
extern "C" {
#include <lua.h>
}
#include <cstdlib>
#include <windows.h>

// GLOBAL: gl_24959
template <>
NEO_PACKAGEITEM::CNeoPackageItemMgr* Singleton<NEO_PACKAGEITEM::CNeoPackageItemMgr>::s_instance = nullptr;

// GLOBAL: gl_24960
template <>
NEO_PACKAGEITEM::CPaidPackageItemMgr* Singleton<NEO_PACKAGEITEM::CPaidPackageItemMgr>::s_instance = nullptr;

namespace NEO_PACKAGEITEM {

// OFFSET: 0x006D1600
// FUNCTION: fn_06518
CNeoPackageItemMgr::CNeoPackageItemMgr() {
}

// OFFSET: 0x006D1680
// FUNCTION: fn_06519
void CNeoPackageItemMgr::Init() {
}

// OFFSET: 0x006D2140
// FUNCTION: fn_06524
__declspec(noinline) std::optional<std::map<int, PackageGroup>*> CNeoPackageItemMgr::FindPackage(int itemId) {
  const int packageId = g_itemInfoMgr->GetPackageId(itemId);
  if (packageId != 0) {
    itemId = packageId;
  }

  std::map<int, std::map<int, PackageGroup>>::iterator package = this->m_packages.find(itemId);
  if (package != this->m_packages.end()) {
    return &package->second;
  }

  return std::nullopt;
}

// OFFSET: 0x006D24B0
// FUNCTION: fn_06526
__declspec(noinline) bool CNeoPackageItemMgr::HasPackage(int itemId) {
  const int packageId = g_itemInfoMgr->GetPackageId(itemId);
  if (packageId != 0) {
    itemId = packageId;
  }

  return this->m_packages.find(itemId) != this->m_packages.end();
}

// OFFSET: 0x006D2CB0
// FUNCTION: fn_06532
CPaidPackageItemMgr::CPaidPackageItemMgr() {
}

// OFFSET: 0x006D2D30
// FUNCTION: fn_06533
void CPaidPackageItemMgr::Init() {
}

// OFFSET: 0x006D2E30
// FUNCTION: fn_06536
void CPaidPackageItemMgr::AddItem(int itemId) {
  if (this->m_items.find(itemId) != this->m_items.end()) {
    return;
  }

  std::optional<std::map<int, PackageGroup>*> package = CNeoPackageItemMgr::GetInstance()->FindPackage(itemId);
  if (package) {
    this->m_items.insert(itemId);
    for (std::map<int, PackageGroup>::iterator group = (*package)->begin(); group != (*package)->end(); ++group) {
      for (std::list<PackageEntry>::iterator entry = group->second.items.begin(); entry != group->second.items.end();
           ++entry) {
        this->AddItem(entry->itemId);
      }
    }
  }

  CSelectPackageItemMgr* selectPackages = CSelectPackageItemMgr::GetInstance();
  std::map<int, std::map<int, SelectPackageTab>>::iterator selectPackage = selectPackages->m_packages.find(itemId);
  if (selectPackage == selectPackages->m_packages.end()) {
    return;
  }

  this->m_items.insert(itemId);
  for (std::map<int, SelectPackageTab>::iterator tab = selectPackage->second.begin(); tab != selectPackage->second.end();
       ++tab) {
    std::optional<std::vector<ITEM_INFO>*> items = CSelectPackageItemMgr::GetInstance()->FindItems(itemId, tab->first);
    if (items) {
      for (std::vector<ITEM_INFO>::iterator item = (*items)->begin(); item != (*items)->end(); ++item) {
        this->AddItem(std::atoi(item->itemId.c_str()));
      }
    }
  }
}

// OFFSET: 0x006D3110
// FUNCTION: fn_06537
bool CPaidPackageItemMgr::LoadScript() {
  CLuaInstance script;
  script.SetGlobal("C_AddItemSold", CPaidPackageItemMgr::LuaAddItemSold);
  if (script.LoadFile("LuaFiles514\\Lua Files\\ProbabilityInfo\\SimpleCashShopScript.lub") == true) {
    lua_State* state = script.GetState();
    if (state) {
      lua_getfield(state, LUA_GLOBALSINDEX, "LoadItemSoldTbl");
      if (lua_pcall(state, 0, 0, 0) != 0) {
        MessageBoxA(nullptr, lua_tolstring(state, -1, nullptr), "CPaidItemMgr", 0);
      } else {
        return true;
      }
    }
  }

  return false;
}

// OFFSET: 0x006D3370
// FUNCTION: fn_06538
bool CPaidPackageItemMgr::IsPaidItem(int itemId) {
  if (this->m_items.find(itemId) != this->m_items.end() && CNeoPackageItemMgr::GetInstance()->HasPackage(itemId)) {
    return true;
  }

  return false;
}

// OFFSET: 0x006D3440
// FUNCTION: fn_06539
int CPaidPackageItemMgr::LuaAddItemSold(lua_State* L) {
  if (!lua_isnumber(L, -1)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "argment 1st is must number");
    return 2;
  }

  const int itemId = static_cast<int>(lua_tonumber(L, -1));
  CPaidPackageItemMgr::GetInstance()->AddItem(itemId);
  return 2;
}

}
