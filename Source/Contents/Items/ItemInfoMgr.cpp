#include "Contents/Items/ItemInfoMgr.h"

#include "Contents/Items/ItemDescription.h"
#include "Script/NLuaState.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}
#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <new>
#include <windows.h>

// GLOBAL: gl_24729
CItemInfoMgr* g_itemInfoMgr;

// OFFSET: 0x00ADFDB0
// FUNCTION: fn_17976
bool CItemInfoMgr::Create() {
  if (g_itemInfoMgr == nullptr) {
    g_itemInfoMgr = new (std::nothrow) CItemInfoMgr();
    if (g_itemInfoMgr != nullptr) {
      if (g_itemInfoMgr->m_isValid) {
        return true;
      }
      delete g_itemInfoMgr;
      g_itemInfoMgr = nullptr;
    }
  }
  return false;
}

// OFFSET: 0x00ADFEA0
// FUNCTION: fn_17977
void CItemInfoMgr::Destroy() {
  if (g_itemInfoMgr != nullptr) {
    delete g_itemInfoMgr;
    g_itemInfoMgr = nullptr;
  }
}

// OFFSET: 0x00ADFEC0
// FUNCTION: fn_17978
int CItemInfoMgr::IsCostume(int id) {
  return this->m_items[id].isCostume;
}

// OFFSET: 0x00ADFEE0
// FUNCTION: fn_17979
int CItemInfoMgr::GetEffectId(int id) {
  std::map<int, lcData>::iterator it = this->m_items.find(id);
  if (it == this->m_items.end()) {
    return -1;
  }
  return it->second.effectId;
}

// OFFSET: 0x00ADFF30
// FUNCTION: fn_17980
const char* CItemInfoMgr::GetFileName() {
  return "System\\iteminfo_true.lub";
}

// OFFSET: 0x00ADFF40
// FUNCTION: fn_17981
int CItemInfoMgr::GetPackageId(int id) {
  std::map<int, lcData>::iterator it = this->m_items.find(id);
  if (it != this->m_items.end()) {
    return it->second.packageId;
  }
  return 0;
}

// OFFSET: 0x00ADFF90
// FUNCTION: fn_17982
bool CItemInfoMgr::Load() {
  NLuaState lua;
  if (lua.IsValid()) {
    luaL_openlibs(lua.GetState());
    lua_register(lua.GetState(), "AddItem", CItemInfoMgr::LuaAddItem);
    lua_register(lua.GetState(), "AddItemUnidentifiedDesc", CItemInfoMgr::LuaAddItemUnidentifiedDesc);
    lua_register(lua.GetState(), "AddItemIdentifiedDesc", CItemInfoMgr::LuaAddItemIdentifiedDesc);
    lua_register(lua.GetState(), "AddItemEffectInfo", CItemInfoMgr::LuaAddItemEffectInfo);
    lua_register(lua.GetState(), "AddItemIsCostume", CItemInfoMgr::LuaAddItemIsCostume);
    lua_register(lua.GetState(), "AddItemPackageID", CItemInfoMgr::LuaAddItemPackageID);
    if (luaL_loadfile(lua.GetState(), "SystemEN\\iteminfo.lua") != 0 || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
      MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CItemInfoMgr", 0);
    } else {
      lua_getglobal(lua.GetState(), "main");
      if (lua_pcall(lua.GetState(), 0, 2, 0) != 0) {
        MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CItemInfoMgr", 0);
      } else if (!lua_isstring(lua.GetState(), -1)) {
        MessageBoxA(nullptr, "second return value is not string...", "CItemInfoMgr", 0);
      } else {
        if (lua_toboolean(lua.GetState(), -2)) {
          return true;
        }
        std::string message = std::string("error found :") + lua_tostring(lua.GetState(), -1);
        MessageBoxA(nullptr, message.c_str(), "CItemInfoMgr", 0);
      }
    }
  }

  return false;
}

// OFFSET: 0x00AE0270
// FUNCTION: fn_17983
static __declspec(noinline) std::string FormatItemInfoMessage(const char* format, ...) {
  char buffer[2049];
  std::memset(buffer, 0, sizeof(buffer));
  va_list arguments;
  va_start(arguments, format);
  _vsnprintf(buffer, 2048, format, arguments);
  va_end(arguments);
  return std::string(buffer);
}

// OFFSET: 0x00AE0360
// FUNCTION: fn_17984
void CItemInfoMgr::SetItemInfo(std::map<int, ITEM_DESC>& descriptions) const {
  class lcMyInsertDesc {
  public:
    explicit lcMyInsertDesc(std::vector<const char*>* lines) : m_lines(lines) {
    }
    // OFFSET: 0x00ADF8B0
    // FUNCTION: fn_17971
    virtual ~lcMyInsertDesc() {
    }
    void operator()(const std::string& line) {
      this->m_lines->push_back(line.c_str());
    }

    std::vector<const char*>* m_lines;
  };

  for (std::map<int, lcData>::const_iterator it = this->m_items.begin(); it != this->m_items.end(); ++it) {
    ItemDescriptions table{descriptions};
    ITEM_DESC& desc = table.Ensure(it->first);
    desc.identifiedName = it->second.identifiedDisplayName.c_str();
    desc.identifiedResource = it->second.identifiedResourceName.c_str();
    std::for_each(it->second.identifiedDescription.begin(), it->second.identifiedDescription.end(),
                  lcMyInsertDesc(&desc.identifiedDescription));
    desc.unidentifiedName = it->second.unidentifiedDisplayName.c_str();
    desc.unidentifiedResource = it->second.unidentifiedResourceName.c_str();
    std::for_each(it->second.unidentifiedDescription.begin(), it->second.unidentifiedDescription.end(),
                  lcMyInsertDesc(&desc.unidentifiedDescription));
    desc.slots = it->second.slotCount;
    desc.unknown_0040 = it->second.classNum;
  }
}

// OFFSET: 0x00AE07A0
// FUNCTION: fn_17986
int CItemInfoMgr::LuaAddItem(lua_State* state) {
  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("7th argment is must number").c_str());
    return 2;
  }
  int id = static_cast<int>(lua_tonumber(state, -7));
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("6th argment is must number [itemID:%d]", id).c_str());
  } else if (!lua_isstring(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("5th argment is must string [itemID:%d]", id).c_str());
  } else if (!lua_isstring(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("4th argment is must string [itemID:%d]", id).c_str());
  } else if (!lua_isstring(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("3th argment is must string [itemID:%d]", id).c_str());
  } else if (!lua_isstring(state, -6)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("2nd argment is must string [itemID:%d]", id).c_str());
  } else if (!lua_isnumber(state, -7)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("1st argment is must number [itemID:%d]", id).c_str());
  } else {
    const char* unidentifiedDisplayName = lua_tostring(state, -6);
    const char* unidentifiedResourceName = lua_tostring(state, -5);
    const char* identifiedDisplayName = lua_tostring(state, -4);
    const char* identifiedResourceName = lua_tostring(state, -3);
    int slotCount = static_cast<int>(lua_tonumber(state, -2));
    int classNum = static_cast<int>(lua_tonumber(state, -1));
    g_itemInfoMgr->m_items[id].slotCount = slotCount;
    g_itemInfoMgr->m_items[id].classNum = classNum;
    g_itemInfoMgr->SetUnidentifiedName(id, unidentifiedDisplayName, unidentifiedResourceName);
    g_itemInfoMgr->SetIdentifiedName(id, identifiedDisplayName, identifiedResourceName);
    lua_pushboolean(state, 1);
    lua_pushstring(state, FormatItemInfoMessage("good").c_str());
  }
  return 2;
}

// OFFSET: 0x00AE0B70
// FUNCTION: fn_17987
int CItemInfoMgr::LuaAddItemEffectInfo(lua_State* state) {
  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("2nd argment is must number").c_str());
  } else if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("1st argment is must number").c_str());
  } else {
    int id = static_cast<int>(lua_tonumber(state, -2));
    int effectId = static_cast<int>(lua_tonumber(state, -1));
    g_itemInfoMgr->m_items[id].effectId = effectId;
    lua_pushboolean(state, 1);
    lua_pushstring(state, FormatItemInfoMessage("good").c_str());
  }
  return 2;
}

// OFFSET: 0x00AE0CB0
// FUNCTION: fn_17988
int CItemInfoMgr::LuaAddItemIdentifiedDesc(lua_State* state) {
  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("2nd argment is must string").c_str());
  } else if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("1st argment is must number").c_str());
  } else {
    int id = static_cast<int>(lua_tonumber(state, -2));
    std::string line = lua_tostring(state, -1);
    g_itemInfoMgr->m_items[id].identifiedDescription.push_back(line);
    lua_pushboolean(state, 1);
    lua_pushstring(state, FormatItemInfoMessage("good").c_str());
  }
  return 2;
}

// OFFSET: 0x00AE0E60
// FUNCTION: fn_17989
int CItemInfoMgr::LuaAddItemIsCostume(lua_State* state) {
  if (lua_type(state, -1) != LUA_TBOOLEAN) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("2nd argment is must boolean").c_str());
  } else if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("1st argment is must number").c_str());
  } else {
    int id = static_cast<int>(lua_tonumber(state, -2));
    bool isCostume = lua_toboolean(state, -1) != 0;
    g_itemInfoMgr->m_items[id].isCostume = isCostume;
    lua_pushboolean(state, 1);
    lua_pushstring(state, FormatItemInfoMessage("good").c_str());
  }
  return 2;
}

// OFFSET: 0x00AE0FA0
// FUNCTION: fn_17990
int CItemInfoMgr::LuaAddItemPackageID(lua_State* state) {
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("1st argment is must number").c_str());
  } else if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("2nd argment is must number").c_str());
  } else {
    int id = static_cast<int>(lua_tonumber(state, -2));
    int packageId = static_cast<int>(lua_tonumber(state, -1));
    g_itemInfoMgr->m_items[id].packageId = packageId;
    lua_pushboolean(state, 1);
    lua_pushstring(state, FormatItemInfoMessage("good").c_str());
  }
  return 2;
}

// OFFSET: 0x00AE10E0
// FUNCTION: fn_17991
int CItemInfoMgr::LuaAddItemUnidentifiedDesc(lua_State* state) {
  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("2nd argment is must string").c_str());
  } else if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatItemInfoMessage("1st argment is must number").c_str());
  } else {
    int id = static_cast<int>(lua_tonumber(state, -2));
    std::string line = lua_tostring(state, -1);
    g_itemInfoMgr->m_items[id].unidentifiedDescription.push_back(line);
    lua_pushboolean(state, 1);
    lua_pushstring(state, FormatItemInfoMessage("good").c_str());
  }
  return 2;
}
