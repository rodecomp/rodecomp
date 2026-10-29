#include "Script/LuaScript.h"
#include "Archive/PakRegistry.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectTypes.h"
#include "File/File.h"
#include "File/ResourcePath.h"
#include "Script/LuaFileOperations.h"
#include "UI/UILapineUpgradeBoxWnd.h"
#include "UI/Window/UIWindowMgr.h"

extern "C" {
#include <lauxlib.h>
#include <lualib.h>
}

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <windows.h>

struct LuaScriptPair {
  std::string left;
  std::string right;
};

class LuaScriptBag {
public:
  void Add(std::string left, std::string right);

  /* 0000 */ unsigned char unknown_0000[0x7C];
  /* 007C */ std::vector<LuaScriptPair> pairs;
};

// OFFSET: 0x00AFC8B0
// FUNCTION: fn_18325
void LuaScriptBag::Add(std::string left, std::string right) {
  this->pairs.push_back(LuaScriptPair{left, right});
}

int AddSkill(lua_State* state);
int AddNeededSkill(lua_State* state);

// OFFSET: 0x00AFF7E0
// FUNCTION: fn_18354
int TraceValue(lua_State* state) {
  std::string text = luaL_checkstring(state, 1);
  MessageBoxA(nullptr, text.c_str(), "Lua Error", 0);
  return 1;
}

// OFFSET: 0x00AFF140
// FUNCTION: fn_18352
int SetSubChatWndList(lua_State* state) {
  luaL_checkstring(state, 1);
  luaL_checkstring(state, 2);
  luaL_checknumber(state, 3);
  luaL_checknumber(state, 4);
  luaL_checknumber(state, 5);
  luaL_checknumber(state, 6);
  lua_pushnumber(state, 0.0);
  return 1;
}

// OFFSET: 0x00AFF630
// FUNCTION: fn_18353
int SetSubChatWndOption(lua_State* state) {
  luaL_checkstring(state, 1);
  luaL_checknumber(state, 2);
  luaL_checknumber(state, 3);
  luaL_checknumber(state, 4);
  return 1;
}

// OFFSET: 0x00AFF020
// FUNCTION: fn_18351
int SetCmdOnOffList(lua_State* state) {
  luaL_checkstring(state, 1);
  luaL_checknumber(state, 2);
  return 0;
}

// OFFSET: 0x00AFEF50
// FUNCTION: fn_18350
int GetCmdOnOffValue(lua_State* state) {
  luaL_checkstring(state, 1);
  lua_pushnumber(state, 0.0);
  return 1;
}

// OFFSET: 0x00AFEBD0
// FUNCTION: fn_18341
int LuaRGB(lua_State* state) {
  const int red = static_cast<int>(luaL_checknumber(state, 1));
  const int green = static_cast<int>(luaL_checknumber(state, 2));
  const int blue = static_cast<int>(luaL_checknumber(state, 3));
  lua_pushnumber(state, ((blue & 255) << 16) | ((green & 255) << 8) | (red & 255));
  return 1;
}

// OFFSET: 0x00B7FFC0
// FUNCTION: fn_19249
int SetEmitterInfo(lua_State* state) {
  const int index = static_cast<int>(luaL_checknumber(state, 1));
  CEffect* effect = g_effectMgr->At(EF_EMITTER, static_cast<unsigned>(index));
  if (effect) {
    const int key = static_cast<int>(luaL_checknumber(state, 2));
    if (key == 0) {
      effect->Slot07();
    }
  }
  return 0;
}

// OFFSET: 0x00B80640
// FUNCTION: fn_19251
int SetMagicFloorInfo(lua_State* state) {
  const int index = static_cast<int>(luaL_checknumber(state, 1));
  CEffect* effect = g_effectMgr->At(EF_MAGIC_FLOOR, static_cast<unsigned>(index));
  if (effect) {
    const int key = static_cast<int>(luaL_checknumber(state, 2));
    if (key == 0) {
      effect->Slot07();
    }
  }
  return 0;
}

// OFFSET: 0x00B804E0
// FUNCTION: fn_19250
int SetEvilsPawInfo(lua_State* state) {
  const int index = static_cast<int>(luaL_checknumber(state, 1));
  CEffect* effect = g_effectMgr->At(EF_EVILS_PAW, static_cast<unsigned>(index));
  if (effect) {
    const int key = static_cast<int>(luaL_checknumber(state, 2));
    if (key == 0) {
      effect->Slot07();
    }
  }
  return 0;
}

// OFFSET: 0x00B807E0
// FUNCTION: fn_19252
int SetOtherEffectInfo(lua_State* state) {
  const int id = static_cast<int>(luaL_checknumber(state, 1));
  const int index = static_cast<int>(luaL_checknumber(state, 2));
  CEffect* effect = g_effectMgr->At(id, static_cast<unsigned>(index));
  if (effect) {
    const int key = static_cast<int>(luaL_checknumber(state, 3));
    if (key == 0) {
      effect->Slot07();
    }
  }
  return 0;
}

// OFFSET: 0x00B7FA30
// FUNCTION: fn_19247
int SetAnimatedEmitterInfo(lua_State* state) {
  const int index = static_cast<int>(luaL_checknumber(state, 1));
  CEffect* effect = g_effectMgr->At(EF_ANIMATED_EMITTER, static_cast<unsigned>(index));
  if (effect) {
    const int key = static_cast<int>(luaL_checknumber(state, 2));
    if (key == 0) {
      effect->Slot07();
    }
  }
  return 0;
}

// OFFSET: 0x00B7FE50
// FUNCTION: fn_19248
int SetEZ2STRInfo(lua_State* state) {
  const int index = static_cast<int>(luaL_checknumber(state, 1));
  luaL_checknumber(state, 2);
  g_effectMgr->At(0x584, static_cast<unsigned>(index));
  return 0;
}

// OFFSET: 0x00B80990
// FUNCTION: fn_19253
int SetWindEffectInfo(lua_State* state) {
  const int index = static_cast<int>(luaL_checknumber(state, 1));
  CEffect* effect = g_effectMgr->At(0x927, static_cast<unsigned>(index));
  if (effect) {
    const int key = static_cast<int>(luaL_checknumber(state, 2));
    if (key == 0) {
      effect->Slot07();
    }
  }
  return 0;
}

// OFFSET: 0x00BA2F00
// FUNCTION: fn_19675
int AddEntryQueue(lua_State* state) {
  luaL_checknumber(state, 1);
  luaL_checkstring(state, 2);
  luaL_checkstring(state, 3);
  luaL_checknumber(state, 4);
  luaL_checknumber(state, 5);
  luaL_checknumber(state, 6);
  luaL_checknumber(state, 7);
  luaL_checknumber(state, 8);
  luaL_checknumber(state, 9);
  luaL_checknumber(state, 10);
  luaL_checknumber(state, 11);
  luaL_checknumber(state, 12);
  luaL_checkstring(state, 13);
  luaL_checkstring(state, 14);
  luaL_checkstring(state, 15);
  luaL_checkstring(state, 16);
  luaL_checkstring(state, 17);
  return 0;
}

// OFFSET: 0x00AFFF60
// FUNCTION: fn_18358
int AddIngredient(lua_State* state) {
  luaL_checknumber(state, 1);
  luaL_checknumber(state, 2);
  g_windowMgr.LookupWindow(0x122);
  return 0;
}

// OFFSET: 0x00AFFFC0
// FUNCTION: fn_18359
int AddIngredientLapineUpgradeBox(lua_State* state) {
  const int id = static_cast<int>(luaL_checknumber(state, 1));
  if (UIWindow* window = g_windowMgr.LookupWindow(0x12e)) {
    static_cast<UILapineUpgradeBoxWnd*>(window)->AddMaterial(id);
  }
  return 0;
}

// OFFSET: 0x00AFFB10
// FUNCTION: fn_18356
int AddCashshopBanner(lua_State* state) {
  luaL_checkstring(state, 1);
  luaL_checkstring(state, 2);
  g_windowMgr.LookupWindow(0x13e);
  return 0;
}

// OFFSET: 0x00AFF8F0
// FUNCTION: fn_18355
int AddCashshopBanner2(lua_State* state) {
  luaL_checkstring(state, 1);
  luaL_checkstring(state, 2);
  g_windowMgr.LookupWindow(0x13e);
  return 0;
}

// OFFSET: 0x00AFFD30
// FUNCTION: fn_18357
int AddCheckAttendanceBanner(lua_State* state) {
  luaL_checkstring(state, 1);
  luaL_checkstring(state, 2);
  lua_toboolean(state, 3);
  lua_toboolean(state, 4);
  g_windowMgr.LookupWindow(0x148);
  return 0;
}

// OFFSET: 0x009419B0
// FUNCTION: fn_12793
int AddItmeTokafraServiceMapList(lua_State* state) {
  luaL_checkstring(state, 1);
  luaL_checkstring(state, 2);
  return 0;
}

// OFFSET: 0x00941870
// FUNCTION: fn_12792
int AddItmeToExceptionMiniMapPosList(lua_State* state) {
  luaL_checkstring(state, 1);
  luaL_checknumber(state, 2);
  luaL_checknumber(state, 3);
  return 0;
}

// OFFSET: 0x00933CB0
// FUNCTION: fn_12736
__declspec(noinline) void RegisterLuaKafraServiceMaps(lua_State* state) {
  lua_register(state, "AddItmeTokafraServiceMapList", AddItmeTokafraServiceMapList);
  lua_register(state, "AddItmeToExceptionMiniMapPosList", AddItmeToExceptionMiniMapPosList);
}

// OFFSET: 0x00AFC830
// FUNCTION: fn_18323
LuaScript::LuaScript() : m_state(nullptr) {
}

// OFFSET: 0x00AFC860
// FUNCTION: fn_18324
LuaScript::~LuaScript() {
  this->Close();
}

// OFFSET: 0x00AFDDB0
// FUNCTION: fn_18336
void LuaScript::Close() {
  if (this->m_state) {
    lua_close(this->m_state);
    this->m_state = nullptr;
  }
}

// OFFSET: 0x00AFE4B0
// FUNCTION: fn_45455
void LuaScript::Register() {
  lua_State* state = this->m_state;
  lua_register(this->m_state, "TraceValue", TraceValue);
  lua_register(this->m_state, "SetSubChatWndList", SetSubChatWndList);
  lua_register(this->m_state, "SetSubChatWndOption", SetSubChatWndOption);
  lua_register(this->m_state, "c_SetCmdOnOffList", SetCmdOnOffList);
  lua_register(this->m_state, "c_GetCmdOnOffValue", GetCmdOnOffValue);
  lua_register(this->m_state, "c_AddSkillList", AddSkill);
  lua_register(this->m_state, "c_AddNeedSkillList", AddNeededSkill);
  lua_register(this->m_state, "RGB", LuaRGB);
  lua_register(this->m_state, "c_ChangeDirectoryOfDatas", LuaChangeDirectoryOfDatas);
  lua_register(this->m_state, "SetEmitterInfo", SetEmitterInfo);
  lua_register(this->m_state, "SetMagicFloorInfo", SetMagicFloorInfo);
  lua_register(this->m_state, "SetEvilsPawInfo", SetEvilsPawInfo);
  lua_register(this->m_state, "SetOtherEffectInfo", SetOtherEffectInfo);
  lua_register(this->m_state, "SetAnimatedEmitterInfo", SetAnimatedEmitterInfo);
  lua_register(this->m_state, "SetEZ2STRInfo", SetEZ2STRInfo);
  lua_register(this->m_state, "SetWindEffectInfo", SetWindEffectInfo);
  RegisterLuaKafraServiceMaps(this->m_state);
  lua_register(this->m_state, "AddEntryQueue", AddEntryQueue);
  lua_register(this->m_state, "add_ingredient", AddIngredient);
  lua_register(this->m_state, "add_ingredient_lapine_upgrade_box", AddIngredientLapineUpgradeBox);
  lua_register(this->m_state, "add_cashshop_banner", AddCashshopBanner);
  lua_register(this->m_state, "add_cashshop_banner2", AddCashshopBanner2);
  lua_register(this->m_state, "add_checkAttendance_banner", AddCheckAttendanceBanner);
  lua_pushnumber(state, 4.0);
  lua_setglobal(state, "SKFLAG_NOREDUCT");
  lua_pushnumber(state, 4096.0);
  lua_setglobal(state, "SKFLAG_DISABLE_FIXEDCASTING_REDUCTION");
}

// OFFSET: 0x00AFDDD0
// FUNCTION: fn_18337
void LuaScript::Open() {
  if (this->m_state) {
    lua_close(this->m_state);
    this->m_state = nullptr;
  }

  this->m_state = luaL_newstate();
  if (!this->m_state) {
    return;
  }

  luaL_openlibs(this->m_state);
  return this->Register();
}

// OFFSET: 0x00AFDD60
// FUNCTION: fn_18335
bool LuaScript::Execute(const void* bytes, std::size_t size) {
  return this->m_state && luaL_loadbuffer(this->m_state, static_cast<const char*>(bytes), size, "buf") == 0 && lua_pcall(this->m_state, 0, 0, 0) == 0;
}

// OFFSET: 0x00AFDE60
// FUNCTION: fn_18338
bool LuaScript::Load(const char* name, bool resourcePath, bool looseOnly) {
  if (!this->m_state) {
    return false;
  }

  std::string prefix;
  if (!looseOnly) {
    prefix = "LuaFiles514\\";
  }

  std::string path = prefix + name;
  path += ".lub";

  if (resourcePath) {
    if (!ResourceExists(path.c_str(), looseOnly)) {
      path = prefix + name + ".lua";
      if (!ResourceExists(path.c_str(), looseOnly)) {
        return false;
      }
    }
  } else {
    if (!g_pakRegistry.Exists(path.c_str(), looseOnly)) {
      path = prefix + name + ".lua";
      if (!g_pakRegistry.Exists(path.c_str(), looseOnly)) {
        return false;
      }
    }
  }

  CFile file;
  if (!file.Open(path.c_str(), 0, resourcePath, looseOnly)) {
    return false;
  }

  const std::uint32_t size = file.GetSize();
  char* buffer = new char[size + 1];
  file.Read(buffer, size);
  buffer[size] = '\0';
  file.Close();

  if (luaL_loadbuffer(this->m_state, buffer, size, "buf") != 0 || lua_pcall(this->m_state, 0, 0, 0) != 0) {
    std::string error = lua_tostring(this->m_state, -1);
    this->Error(name, "%s\n", error.c_str());
    lua_settop(this->m_state, -2);
    delete[] buffer;
    return false;
  }

  delete[] buffer;
  return true;
}

// OFFSET: 0x00AFD1D0
// FUNCTION: fn_18328
void LuaScript::Error(const char* caption, const char* format, ...) {
  char text[500];
  va_list arguments;
  va_start(arguments, format);
  _vsnprintf_s(text, sizeof(text), _TRUNCATE, format, arguments);
  va_end(arguments);
  MessageBoxA(nullptr, text, caption, MB_OK);
}

// OFFSET: 0x00AFDC60
// FUNCTION: fn_18334
bool LuaScript::TableField(const char* signature, ...) {
  va_list arguments;
  va_start(arguments, signature);

  if (*signature == 'd') {
    lua_pushnumber(this->m_state, va_arg(arguments, int));
    lua_gettable(this->m_state, -2);
  } else if (*signature == 's') {
    lua_pushstring(this->m_state, va_arg(arguments, const char*));
    lua_gettable(this->m_state, -2);
  }

  ++signature;

  if (*signature == 'd' && lua_isnumber(this->m_state, -1)) {
    *va_arg(arguments, int*) = static_cast<int>(lua_tonumber(this->m_state, -1));
    lua_pop(this->m_state, 1);
  }

  if (*signature == 's' && lua_isstring(this->m_state, -1)) {
    *va_arg(arguments, const char**) = lua_tostring(this->m_state, -1);
    lua_pop(this->m_state, 1);
  }

  if (*signature == 'l' && lua_isnumber(this->m_state, -1)) {
    *va_arg(arguments, double*) = lua_tonumber(this->m_state, -1);
    lua_pop(this->m_state, 1);
  }

  va_end(arguments);
  return true;
}

// OFFSET: 0x00AFC9A0
// FUNCTION: fn_18326
bool LuaScript::Call(std::string function, const char* signature, ...) {
  if (!this->m_state) {
    return false;
  }

  va_list arguments;
  va_start(arguments, signature);
  lua_getglobal(this->m_state, function.c_str());
  int inputCount = 0;

  while (*signature) {
    luaL_checkstack(this->m_state, 1, "too many arguments");
    char option = *signature++;
    if (option == '>') {
      break;
    }

    switch (option) {
    case 'b':
      lua_pushboolean(this->m_state, static_cast<unsigned char>(va_arg(arguments, int)));
      break;
    case 'd':
      lua_pushnumber(this->m_state, va_arg(arguments, int));
      break;
    case 'l':
      lua_pushnumber(this->m_state, va_arg(arguments, double));
      break;
    case 's':
      lua_pushstring(this->m_state, va_arg(arguments, const char*));
      break;
    default:
      this->Error(function.c_str(), "invalid option (%c)", option);
      break;
    }

    ++inputCount;
  }

  int outputCount = static_cast<int>(std::strlen(signature));
  if (lua_pcall(this->m_state, inputCount, outputCount, 0)) {
    std::string error = lua_tostring(this->m_state, -1);
    this->Error(function.c_str(), "%s\n", error.c_str());
    if (this->m_state) {
      lua_settop(this->m_state, 0);
    }

    va_end(arguments);
    return false;
  }

  for (int output = -outputCount; *signature; ++output) {
    switch (*signature++) {
    case 'b':
      *va_arg(arguments, bool*) = lua_toboolean(this->m_state, output) != 0;
      break;
    case 'd':
      *va_arg(arguments, int*) = static_cast<int>(lua_tonumber(this->m_state, output));
      break;
    case 'l':
      *va_arg(arguments, double*) = lua_tonumber(this->m_state, output);
      break;
    case 's':
      *va_arg(arguments, const char**) = lua_tostring(this->m_state, output);
      break;
    default:
      this->Error(function.c_str(), "invalid option (%c)", signature[-1]);
      break;
    }
  }

  if (outputCount) {
    lua_pop(this->m_state, outputCount);
    if (this->m_state) {
      lua_settop(this->m_state, 0);
    }
  }

  va_end(arguments);
  return true;
}

// OFFSET: 0x00AFCD90
// FUNCTION: fn_18327
bool LuaScript::CallTable(std::string table, std::string function, const char* signature, ...) {
  if (!this->m_state) {
    return false;
  }

  va_list arguments;
  va_start(arguments, signature);

  lua_getglobal(this->m_state, table.c_str());
  lua_getfield(this->m_state, -1, function.c_str());

  int inputCount = 0;

  while (*signature) {
    luaL_checkstack(this->m_state, 1, "too many arguments");

    char option = *signature++;
    if (option == '>') {
      break;
    }

    switch (option) {
    case 'b':
      lua_pushboolean(this->m_state, static_cast<unsigned char>(va_arg(arguments, int)));
      break;
    case 'd':
      lua_pushnumber(this->m_state, va_arg(arguments, int));
      break;
    case 'l':
      lua_pushnumber(this->m_state, va_arg(arguments, double));
      break;
    case 's':
      lua_pushstring(this->m_state, va_arg(arguments, const char*));
      break;
    default:
      this->Error(function.c_str(), "invalid option (%c)", option);
      break;
    }

    ++inputCount;
  }

  int outputCount = static_cast<int>(std::strlen(signature));
  if (lua_pcall(this->m_state, inputCount, outputCount, 0)) {
    std::string error = lua_tostring(this->m_state, -1);
    this->Error(function.c_str(), "%s\n", error.c_str());
    if (this->m_state) {
      lua_settop(this->m_state, 0);
    }

    va_end(arguments);
    return false;
  }

  for (int output = -outputCount; *signature; ++output) {
    switch (*signature++) {
    case 'b':
      *va_arg(arguments, bool*) = lua_toboolean(this->m_state, output) != 0;
      break;
    case 'd':
      *va_arg(arguments, int*) = static_cast<int>(lua_tonumber(this->m_state, output));
      break;
    case 'l':
      *va_arg(arguments, double*) = lua_tonumber(this->m_state, output);
      break;
    case 's':
      *va_arg(arguments, const char**) = lua_tostring(this->m_state, output);
      break;
    default:
      this->Error(function.c_str(), "invalid option (%c)", signature[-1]);
      break;
    }
  }

  if (outputCount) {
    lua_settop(this->m_state, -1 - outputCount);
    if (this->m_state) {
      lua_settop(this->m_state, 0);
    }
  }

  va_end(arguments);
  return true;
}

// OFFSET: 0x00AFD2A0
// FUNCTION: fn_18329
int LuaScript::GlobalInteger(const char* name) {
  if (!this->m_state) {
    return 0;
  }

  lua_getglobal(this->m_state, name);
  if (!lua_isnumber(this->m_state, -1)) {
    return -1;
  }

  int value = static_cast<int>(lua_tonumber(this->m_state, -1));
  lua_pop(this->m_state, 1);
  return value;
}

// OFFSET: 0x00AFD310
// FUNCTION: fn_18330
std::string LuaScript::GlobalString(const char* name) {
  if (!this->m_state) {
    return {};
  }

  lua_getglobal(this->m_state, name);
  if (!lua_isstring(this->m_state, -1)) {
    return {};
  }

  std::string value = lua_tostring(this->m_state, -1);
  lua_pop(this->m_state, 1);
  return value;
}

// OFFSET: 0x00AFD490
// FUNCTION: fn_18331
bool LuaScript::HasTable(const char* name) {
  if (!this->m_state) {
    return false;
  }

  lua_getglobal(this->m_state, name);

  return lua_type(this->m_state, -1) == LUA_TTABLE;
}

// OFFSET: 0x00AFD4D0
// FUNCTION: fn_18332
std::list<int> LuaScript::TableKeys(const char* name, int) {
  std::list<int> keys;

  if (!this->m_state) {
    return keys;
  }

  lua_getglobal(this->m_state, name);

  if (lua_type(this->m_state, -1) == LUA_TTABLE) {
    lua_pushnil(this->m_state);

    while (lua_next(this->m_state, -2)) {
      keys.push_back(static_cast<int>(lua_tonumber(this->m_state, -2)));
      lua_pop(this->m_state, 1);
    }

    lua_pop(this->m_state, 1);
    keys.sort();
  }

  return keys;
}

// OFFSET: 0x00AFD830
// FUNCTION: fn_18333
int LuaScript::TableInteger(std::string table, int keyType, std::intptr_t key) {
  if (!this->m_state) {
    return 0;
  }

  int value = -1;
  lua_getglobal(this->m_state, table.c_str());

  if (lua_type(this->m_state, -1) != LUA_TTABLE) {
    return value;
  }

  if (keyType == 0) {
    lua_pushnumber(this->m_state, static_cast<int>(key));
    lua_gettable(this->m_state, -2);
  } else if (keyType == 1) {
    lua_pushstring(this->m_state, reinterpret_cast<const char*>(key));
    lua_gettable(this->m_state, -2);
  } else if (keyType != 2) {
    lua_pushnumber(this->m_state, static_cast<int>(key));
    lua_gettable(this->m_state, -2);
    return value;
  }

  if (lua_isnumber(this->m_state, -1)) {
    value = static_cast<int>(lua_tonumber(this->m_state, -1));
    lua_pop(this->m_state, 1);
  }

  return value;
}

// OFFSET: 0x0059CD60
// FUNCTION: fn_02980
void LuaScript::Register(std::string name, int (*function)(lua_State*)) {
  lua_register(this->m_state, name.c_str(), function);
}
