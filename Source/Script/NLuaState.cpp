#include "Script/NLuaState.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
}
#include <utility>

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

  void Clear() noexcept;
  __declspec(noinline) void EraseSubtree(TreeSubMap& al, TreeSubNode* node);
};

// OFFSET: 0x00BA1A20
// FUNCTION: fn_19656
void TreeSubMap::EraseSubtree(TreeSubMap& al, TreeSubNode* node) {
  while (node->isnil == 0) {
    this->EraseSubtree(al, node->right);
    ::operator delete(std::exchange(node, node->left), 0x18);
  }
}

// OFFSET: 0x00CA4AE0
// FUNCTION: fn_20939
void TreeSubMap::Clear() noexcept {
  TreeSubNode* head = this->head;
  TreeSubNode* cur = head->parent;
  while (cur->isnil == 0) {
    this->EraseSubtree(*this, cur->right);
    ::operator delete(std::exchange(cur, cur->left), 0x18);
  }
  head->parent = head;
  head->left = head;
  head->right = head;
  this->size = 0;
}

// OFFSET: 0x00CA4B60
// FUNCTION: fn_20940
void* GetEntryQueueMgr() {
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
