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

// OFFSET: 0x006D1680
// FUNCTION: fn_06519
void CNeoPackageItemMgr::Init() {
}

// OFFSET: 0x006D2140

// OFFSET: 0x006D24B0

// OFFSET: 0x006D2CB0

// OFFSET: 0x006D2D30
// FUNCTION: fn_06533
void CPaidPackageItemMgr::Init() {
}

// OFFSET: 0x006D2E30

// OFFSET: 0x006D3110

// OFFSET: 0x006D3370

// OFFSET: 0x006D3440

}
