#ifndef RAGNAROK_SOURCE_UTILITY_ASYNC_NMESSAGEMGR_H
#define RAGNAROK_SOURCE_UTILITY_ASYNC_NMESSAGEMGR_H

#include <cstdint>
#include <map>
#include <windows.h>

class NInter {
public:
  virtual void Receive(std::uint64_t argument, std::uint32_t id) = 0;
  virtual ~NInter();
};

class NMessageMgr {
public:
  NMessageMgr();
  virtual ~NMessageMgr();

  static bool Initialize();
  static void Shutdown();
  bool Register(std::uint32_t objectId, NInter* object);
  bool Send(std::uint32_t objectId, std::uint64_t argument, std::uint32_t id);
  void Unregister(std::uint32_t objectId, NInter* object);

protected:
  /* 0004 */ CRITICAL_SECTION m_criticalSection;
  /* 001C */ bool m_enabled;
  /* 0020 */ std::map<std::uint32_t, NInter*> m_objects;
};

// STATIC: 0x0138D3EC
// GLOBAL: gl_24361
extern NMessageMgr* g_messageMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NInter) == 4);
static_assert(sizeof(NMessageMgr) == 40);
#endif

#endif
