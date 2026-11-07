#include "Script/LuaInstance.h"
#include "File/File.h"

extern "C" {
#include <lauxlib.h>
#include <lualib.h>
}
#include <cstdlib>
#include <windows.h>

// STATIC: 0x010D9630
// STRING: st_00223
static const char luaErrorCaption[] = "Lua Error";

// OFFSET: 0x005221E0
// FUNCTION: fn_01123
CLuaInstance::CLuaInstance() : m_state(nullptr) {
  this->m_state = luaL_newstate();
  if (this->m_state) {
    luaL_openlibs(this->m_state);
    lua_register(this->m_state, "MessageBox", LuaMessageBox);
  }
}

// OFFSET: 0x00522290
// FUNCTION: fn_01125
CLuaInstance::~CLuaInstance() {
  if (this->m_state) {
    lua_close(this->m_state);
    this->m_state = nullptr;
  }
}

// OFFSET: 0x005223D0
// FUNCTION: fn_01128
bool CLuaInstance::Execute(const std::string& name) {
  lua_getglobal(this->m_state, name.c_str());

  const int result = lua_pcall(this->m_state, 0, 0, 0);

  if (result != 0) {
    MessageBoxA(nullptr, lua_tostring(this->m_state, -1), luaErrorCaption, MB_OK);
    return false;
  }

  return true;
}

// OFFSET: 0x00522460
// FUNCTION: fn_01129
int CLuaInstance::LuaMessageBox(lua_State* state) {
  const std::string text = lua_tostring(state, -1);
  MessageBoxA(nullptr, text.c_str(), "CLuaInstance", MB_OK);
  return 0;
}

// OFFSET: 0x005225F0
// FUNCTION: fn_01130
bool CLuaInstance::LoadFile(const std::string& path, bool showError) {
  if (luaL_loadfile(this->m_state, path.c_str()) == 0 && lua_pcall(this->m_state, 0, LUA_MULTRET, 0) == 0) {
    return true;
  }

  if (showError) {
    MessageBoxA(nullptr, lua_tostring(this->m_state, -1), path.c_str(), MB_OK);
  }

  return false;
}

// OFFSET: 0x00522660
// FUNCTION: fn_01131
bool CLuaInstance::LoadFile(const std::string& path) {
  CFile file;
  if (!file.Open(path.c_str(), 0, true, false)) {
    return false;
  }

  char* buffer = new char[file.GetSize() + 1];
  SecureZeroMemory(buffer, file.GetSize() + 1);
  file.Read(buffer, file.GetSize());
  file.Close();

  if (luaL_loadbuffer(this->m_state, buffer, file.GetSize(), "buf") == 0 && lua_pcall(this->m_state, 0, 0, 0) == 0) {
    delete[] buffer;
    return true;
  }

  const char* error = lua_tostring(this->m_state, -1);
  MessageBoxA(nullptr, error, path.c_str(), MB_OK);
  lua_pop(this->m_state, 1);
  delete[] buffer;

  return false;
}

// OFFSET: 0x00522870
// FUNCTION: fn_01132
void CLuaInstance::SetGlobal(const std::string& name, bool value) {
  lua_pushboolean(this->m_state, value);
  lua_setglobal(this->m_state, name.c_str());
}

// OFFSET: 0x005228B0
// FUNCTION: fn_01133
void CLuaInstance::SetGlobal(const std::string& name, lua_CFunction function) {
  lua_pushcfunction(this->m_state, function);
  lua_setglobal(this->m_state, name.c_str());
}

// OFFSET: 0x005228F0
// FUNCTION: fn_01134
void CLuaInstance::SetGlobal(const std::string& name, double value) {
  lua_pushnumber(this->m_state, value);
  lua_setglobal(this->m_state, name.c_str());
}

// OFFSET: 0x00E27850
// FUNCTION: fn_23942
int CLuaInstance::ReadStrings(std::vector<std::string>& values, const char* global) {
  lua_getglobal(this->m_state, global);
  int type = lua_type(this->m_state, -1);
  if (type != LUA_TTABLE) {
    lua_pop(this->m_state, 1);
    return (type != LUA_TNIL) + 1;
  }

  values.resize(lua_objlen(this->m_state, -1));
  std::size_t index = 0;

  for (lua_pushnil(this->m_state); lua_next(this->m_state, -2); lua_pop(this->m_state, 1)) {
    std::string value;
    type = lua_type(this->m_state, -1);
    if (type != LUA_TSTRING) {
      lua_pop(this->m_state, 3);
      return (type != LUA_TNIL) + 1;
    }

    value = lua_tostring(this->m_state, -1);
    values[index++] = value;
  }

  lua_pop(this->m_state, 1);
  return 0;
}
