#include "Script/NLuaState.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
}

// GLOBAL: gl_24849
void* g_entryQueueMgr = nullptr;

struct TreeSubNode {
  TreeSubNode* left;
  TreeSubNode* parent;
  TreeSubNode* right;
  char color;
  char isnil;
};

struct TreeSubMap {
  TreeSubNode* head;
  unsigned int size;

  void Clear();
  void EraseSubtree(TreeSubMap& al, TreeSubNode* node);
};

// OFFSET: 0x00CA4AE0
// FUNCTION: fn_20939
void TreeSubMap::Clear() {
  TreeSubNode* cur = this->head->parent;
  while (cur->isnil == 0) {
    this->EraseSubtree(*this, cur->right);
    TreeSubNode* next = cur->left;
    ::operator delete(cur, 0x18);
    cur = next;
  }
  this->head->parent = this->head;
  this->head->left = this->head;
  this->head->right = this->head;
  this->size = 0;
}

// OFFSET: 0x00CA4B60
// FUNCTION: fn_20940
void* sub_CA4B60() {
  return g_entryQueueMgr;
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
