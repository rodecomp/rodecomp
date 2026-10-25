#include "Utility/Async/NMessageMgr.h"
#include "Utility/Threading/NSyncObject.h"

#include <new>

// STATIC: 0x0138D3EC
// GLOBAL: gl_24361
NMessageMgr* g_messageMgr;

// OFFSET: 0x00732BF0
NInter::~NInter() = default;

// INLINED
NMessageMgr::NMessageMgr() : m_enabled(false) {
  InitializeCriticalSection(&this->m_criticalSection);
  this->m_enabled = true;
}

// INLINED
NMessageMgr::~NMessageMgr() {
  DeleteCriticalSection(&this->m_criticalSection);
}

// OFFSET: 0x007359A0
// FUNCTION: fn_07610
bool NMessageMgr::Initialize() {
  g_messageMgr = new (std::nothrow) NMessageMgr;
  if (!g_messageMgr) {
    return false;
  }

  if (!g_messageMgr->m_enabled) {
    delete g_messageMgr;
    g_messageMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x00735A90
// FUNCTION: fn_07611
void NMessageMgr::Shutdown() {
  if (g_messageMgr) {
    delete g_messageMgr;
    g_messageMgr = nullptr;
  }
}

// OFFSET: 0x00735AB0
// FUNCTION: fn_07612
bool NMessageMgr::Register(std::uint32_t objectId, NInter* object) {
  NSyncObject held(&this->m_criticalSection);
  return this->m_objects.emplace(objectId, object).second;
}

// OFFSET: 0x00735BC0
// FUNCTION: fn_07613
bool NMessageMgr::Send(std::uint32_t objectId, std::uint64_t argument, std::uint32_t id) {
  NSyncObject held(&this->m_criticalSection);
  auto object = this->m_objects.find(objectId);
  if (object == this->m_objects.end()) {
    return false;
  }

  object->second->Receive(argument, id);
  return true;
}

// OFFSET: 0x00735C60
// FUNCTION: fn_07614
void NMessageMgr::Unregister(std::uint32_t objectId, NInter* object) {
  NSyncObject held(&this->m_criticalSection);
  auto registered = this->m_objects.find(objectId);
  if (registered != this->m_objects.end() && registered->second == object) {
    this->m_objects.erase(registered);
  }
}
