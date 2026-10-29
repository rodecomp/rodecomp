#include "Contents/Items/ChangeMaterialMgr.h"
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

// GLOBAL: gl_24855
CChangeMaterialMgr* g_changeMaterialMgr = nullptr;

// OFFSET: 0x005DC550
// FUNCTION: fn_03809
MaterialKey::MaterialKey(const std::vector<ChangeMaterialEntry>& materials) {
  for (std::vector<ChangeMaterialEntry>::const_iterator it = materials.begin(); it != materials.end(); ++it) {
    this->itemIds.push_back(it->itemId);
  }
}

// OFFSET: 0x005DCBF0
// FUNCTION: fn_03814
bool MaterialKeyLess::operator()(const MaterialKey& left, const MaterialKey& right) const {
  const int leftCount = static_cast<int>(left.itemIds.size());
  const int rightCount = static_cast<int>(right.itemIds.size());
  const int count = leftCount < rightCount ? rightCount : leftCount;

  for (int i = 0; i < count; ++i) {
    const int leftId = i < leftCount ? left.itemIds[i] : 0;
    const int rightId = i < rightCount ? right.itemIds[i] : 0;
    if (leftId < rightId) {
      return true;
    }
    if (rightId < leftId) {
      break;
    }
  }

  return false;
}

// OFFSET: 0x005DCF60
// FUNCTION: fn_03816
bool CChangeMaterialMgr::CreateInstance() {
  if (g_changeMaterialMgr == nullptr) {
    g_changeMaterialMgr = new (std::nothrow) CChangeMaterialMgr;
    if (g_changeMaterialMgr != nullptr) {
      if (!g_changeMaterialMgr->m_isValid) {
        delete g_changeMaterialMgr;
      } else {
        if (g_changeMaterialMgr->Load()) {
          return true;
        }
        if (g_changeMaterialMgr != nullptr) {
          delete g_changeMaterialMgr;
        }
      }
      g_changeMaterialMgr = nullptr;
    }
  }

  return false;
}

// OFFSET: 0x005DD090
// FUNCTION: fn_03817
void CChangeMaterialMgr::DestroyInstance() {
  if (g_changeMaterialMgr != nullptr) {
    delete g_changeMaterialMgr;
    g_changeMaterialMgr = nullptr;
  }
}

// OFFSET: 0x005DD110
// FUNCTION: fn_03818
bool CChangeMaterialMgr::Load() {
  NLuaState lua;
  if (lua.IsValid()) {
    luaL_openlibs(lua.GetState());
    lua_register(lua.GetState(), "InsertRecipe", CChangeMaterialMgr::LuaInsertRecipe);
    if (luaL_loadfile(lua.GetState(), "System\\ChangeMaterial.lub") != 0 || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
      MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CChangeMaterialMgr", 0);
    } else {
      lua_getglobal(lua.GetState(), "main");
      if (lua_pcall(lua.GetState(), 0, 2, 0) != 0) {
        MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CChangeMaterialMgr", 0);
      } else if (!lua_isstring(lua.GetState(), -1)) {
        MessageBoxA(nullptr, "second return value is not string...", "CChangeMaterialMgr", 0);
      } else {
        if (lua_toboolean(lua.GetState(), -2)) {
          return true;
        }
        std::string message = std::string("error found unknown item :") + lua_tostring(lua.GetState(), -1);
        MessageBoxA(nullptr, message.c_str(), "CChangeMaterialMgr", 0);
      }
    }
  }

  return false;
}

// OFFSET: 0x005DD360
// FUNCTION: fn_03819
ChangeMaterialResult CChangeMaterialMgr::InsertRecipe(std::vector<ChangeMaterialEntry> materials, const std::vector<ChangeMaterialProduct>& products, int type) {
  std::sort(materials.begin(), materials.end());
  MaterialKey key(materials);

  std::map<MaterialKey, ChangeMaterialRecipeList, MaterialKeyLess>::iterator it = this->m_library.find(key);
  if (it == this->m_library.end()) {
    ChangeMaterialRecipeList recipes;
    std::pair<std::map<MaterialKey, ChangeMaterialRecipeList, MaterialKeyLess>::iterator, bool> inserted =
        this->m_library.insert(std::map<MaterialKey, ChangeMaterialRecipeList, MaterialKeyLess>::value_type(key, recipes));
    if (!inserted.second) {
      return ChangeMaterialResult(false, "error m_Library.insert()");
    }
    it = inserted.first;
  }

  return it->second.Add(materials, products, type);
}

// OFFSET: 0x005DD6A0
// FUNCTION: fn_03820
ChangeMaterialResult ChangeMaterialRecipeList::Add(std::vector<ChangeMaterialEntry> materials, std::vector<ChangeMaterialProduct> products, int type) {
  ChangeMaterialRecipe recipe(materials, products, type);

  for (std::vector<ChangeMaterialRecipe>::iterator it = this->m_recipes.begin(); it != this->m_recipes.end(); ++it) {
    const unsigned int count = recipe.materials.size();
    if (count == it->materials.size()) {
      unsigned int i = 0;
      for (; i < count; ++i) {
        if (recipe.materials[i].itemId != it->materials[i].itemId || recipe.materials[i].count != it->materials[i].count) {
          break;
        }
      }
      if (i >= count) {
        return ChangeMaterialResult(false, "found same recipe");
      }
    }
  }

  this->m_recipes.push_back(recipe);
  return ChangeMaterialResult(true, "");
}

// OFFSET: 0x005DD8A0
// FUNCTION: fn_03821
std::string ChangeMaterialFormat(const char* format, ...) {
  char buffer[0x801];
  std::memset(buffer, 0, sizeof(buffer));

  va_list arguments;
  va_start(arguments, format);
  _vsnprintf(buffer, 0x800, format, arguments);
  va_end(arguments);

  return std::string(buffer);
}

// OFFSET: 0x005DE160
// FUNCTION: fn_03840
int CChangeMaterialMgr::LuaInsertRecipe(lua_State* L) {
  if (lua_type(L, -3) != LUA_TTABLE) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, ChangeMaterialFormat("argment 1st is must table").c_str());
    return 2;
  }
  if (lua_type(L, -2) != LUA_TTABLE) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, ChangeMaterialFormat("argment 2nd is must table").c_str());
    return 2;
  }
  if (!lua_isnumber(L, -1)) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, ChangeMaterialFormat("argment 3rd is must number").c_str());
    return 2;
  }

  double type = lua_tonumber(L, -1);
  lua_pop(L, 1);

  std::vector<ChangeMaterialEntry> materials;
  int index = 1;
  lua_pushinteger(L, index);
  lua_gettable(L, -3);
  while (lua_type(L, -1) != LUA_TNIL) {
    if (lua_type(L, -1) != LUA_TTABLE) {
      lua_pushboolean(L, 0);
      lua_pushstring(L, ChangeMaterialFormat("Material element(%d) is must table", index).c_str());
      return 2;
    }

    lua_pushinteger(L, 1);
    lua_gettable(L, -2);
    double itemId = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_pushinteger(L, 2);
    lua_gettable(L, -2);
    std::string name = lua_tostring(L, -1);
    lua_pop(L, 1);

    lua_pushinteger(L, 3);
    lua_gettable(L, -2);
    double count = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_pop(L, 1);

    ChangeMaterialEntry entry = {static_cast<int>(itemId), static_cast<int>(count)};
    materials.push_back(entry);

    ++index;
    lua_pushinteger(L, index);
    lua_gettable(L, -3);
  }
  lua_pop(L, 1);

  std::vector<ChangeMaterialProduct> products;
  index = 1;
  lua_pushinteger(L, index);
  lua_gettable(L, -2);
  while (lua_type(L, -1) != LUA_TNIL) {
    if (lua_type(L, -1) != LUA_TTABLE) {
      lua_pushboolean(L, 0);
      lua_pushstring(L, ChangeMaterialFormat("Product element(%d) is must table", index).c_str());
      return 2;
    }

    lua_pushinteger(L, 1);
    lua_gettable(L, -2);
    double itemId = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_pushinteger(L, 2);
    lua_gettable(L, -2);
    std::string name = lua_tostring(L, -1);
    lua_pop(L, 1);

    lua_pushinteger(L, 3);
    lua_gettable(L, -2);
    double count = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_pushinteger(L, 4);
    lua_gettable(L, -2);
    double rate = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_pop(L, 1);

    ChangeMaterialProduct product = {static_cast<int>(itemId), static_cast<int>(count), static_cast<int>(rate)};
    products.push_back(product);

    ++index;
    lua_pushinteger(L, index);
    lua_gettable(L, -2);
  }
  lua_pop(L, 1);

  g_changeMaterialMgr->InsertRecipe(materials, products, static_cast<int>(type));

  lua_pushboolean(L, 1);
  lua_pushstring(L, ChangeMaterialFormat("good").c_str());
  return 2;
}
