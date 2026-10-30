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

// GLOBAL: gl_24923
CItemReformMgr* g_itemReformMgr = nullptr;

// STRING: st_01024
static const char kItemReformSystemPath[] = "LuaFiles514\\Lua Files\\ItemReform\\ItemReformSystem.lub";

// OFFSET: 0x005F92A0

// OFFSET: 0x005F91D0

CItemReformMgr::~CItemReformMgr() {
}

// OFFSET: 0x005F9350
// FUNCTION: fn_04075
void CItemReformMgr::NullSub() {
}

// OFFSET: 0x005F9DF0

// OFFSET: 0x005F9F60

// OFFSET: 0x005F9FF0

// OFFSET: 0x005FA140

static void ReformError(std::uint64_t id, const char* format) {
  ShowErrorDialog(FormatOwnedString(format, id), 0);
}

static void ReformError(std::uint64_t id, unsigned int limit, const char* format) {
  ShowErrorDialog(FormatOwnedString(format, id, limit), 0);
}

static int EnsureItemDbId(const char* name) {
  if (!g_itemDbToIdMgr) {
    g_itemDbToIdMgr = new (std::nothrow) CItemDBToIDMgr;
  }
  if (!g_itemDbToIdMgr) {
    return 0;
  }
  return g_itemDbToIdMgr->GetItemID(name);
}

// OFFSET: 0x005FA180

// OFFSET: 0x005FB1E0
