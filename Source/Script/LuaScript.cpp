#include "Script/LuaScript.h"

extern "C" {
#include <lauxlib.h>
}

#include <cstdio>
#include <cstring>
#include <windows.h>

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

// OFFSET: 0x00AFDD60
// FUNCTION: fn_18335
bool LuaScript::Execute(const void* bytes, std::size_t size) {
  return this->m_state && luaL_loadbuffer(this->m_state, static_cast<const char*>(bytes), size, "buf") == 0 && lua_pcall(this->m_state, 0, 0, 0) == 0;
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
