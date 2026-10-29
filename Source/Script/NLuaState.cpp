#include "Script/NLuaState.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
}

// OFFSET: 0x00CA4CF0
// FUNCTION: fn_20941
NLuaState::NLuaState() : m_state(nullptr), m_isValid(false) {
  this->m_state = luaL_newstate();
  if (this->m_state != nullptr) {
    this->m_isValid = true;
  }
}

NLuaState::~NLuaState() {
  if (this->m_state != nullptr) {
    lua_close(this->m_state);
    this->m_state = nullptr;
  }
}

NLuaThread::NLuaThread(NLuaState& parent) : m_state(nullptr), m_isValid(false) {
  this->m_state = lua_newthread(parent.m_state);
  if (this->m_state != nullptr) {
    this->m_isValid = true;
  }
}

// OFFSET: 0x00CA4E80
// FUNCTION: fn_20944
NLuaThread::~NLuaThread() {
  this->m_state = nullptr;
}
