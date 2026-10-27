#include "Contents/ItemDBToIDMgr.h"

#include <new>

// GLOBAL: gl_24498
CItemDBToIDMgr* g_itemDbToIdMgr = nullptr;

// OFFSET: 0x006C7400
// FUNCTION: fn_06415
CItemDBToIDMgr::CItemDBToIDMgr() {
}

// OFFSET: 0x006C7450
// FUNCTION: fn_06416
void CItemDBToIDMgr::Init() {
}

// OFFSET: 0x006C74D0
// FUNCTION: fn_06418
CItemDBToIDMgr::~CItemDBToIDMgr() {
}

// OFFSET: 0x006C7540
// FUNCTION: fn_06419
bool CItemDBToIDMgr::Load() {
  return this->m_lua.LoadFile("LuaFiles514\\Lua Files\\ItemDBNameTbl.lub");
}

// OFFSET: 0x006C7670
// FUNCTION: fn_06420
int CItemDBToIDMgr::GetItemID(std::string itemDBName) {
  lua_State* state = this->m_lua.GetState();
  if (!state) {
    return 0;
  }

  lua_getglobal(state, "ItemDB_To_ItemID");
  lua_pushstring(state, itemDBName.c_str());
  if (lua_pcall(state, 1, 1, 0) != 0) {
    return 0;
  }

  return static_cast<int>(lua_tonumber(state, -1));
}

// OFFSET: 0x00E5CE60
// FUNCTION: fn_24373
CItemDBToIDMgr* CItemDBToIDMgr::GetInstance() {
  if (!g_itemDbToIdMgr) {
    g_itemDbToIdMgr = new (std::nothrow) CItemDBToIDMgr;
  }

  return g_itemDbToIdMgr;
}
