#ifndef RAGNAROK_SOURCE_UTILITY_NASYNCWORKQUEUE_H
#define RAGNAROK_SOURCE_UTILITY_NASYNCWORKQUEUE_H

#include "Utility/NAsyncWork.h"

#include <list>
#include <utility>
#include <windows.h>

class NAsyncWorkRequestQueue {
  friend class NAsyncWorkThread;
  friend class NAsyncWorkMgr;

public:
  NAsyncWorkRequestQueue();
  virtual ~NAsyncWorkRequestQueue();
  NAsyncWorkPtr Pop();
  void PushBack(NAsyncWorkPtr& work);
  void PushFront(NAsyncWorkPtr& work);
  void Complete(std::uint32_t group, std::uint32_t key);

protected:
  /* 0004 */ std::list<NAsyncWorkPtr> m_pending;
  /* 000C */ std::list<std::pair<std::uint32_t, std::uint32_t>> m_active;
  /* 0014 */ std::uint32_t m_waiting;
  /* 0018 */ HANDLE m_semaphore;
  /* 001C */ CRITICAL_SECTION m_criticalSection;
  /* 0034 */ bool m_enabled;
};

struct NAsyncWorkResponse {
  bool valid;
  NAsyncWorkPtr work;
};

class NAsyncWorkResponseQueue {
  friend class NAsyncWorkMgr;

public:
  NAsyncWorkResponseQueue();
  virtual ~NAsyncWorkResponseQueue();
  NAsyncWorkResponse Pop();
  void Push(NAsyncWorkPtr& work);

protected:
  /* 0004 */ std::list<NAsyncWorkPtr> m_pending;
  /* 000C */ CRITICAL_SECTION m_criticalSection;
  /* 0024 */ bool m_enabled;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NAsyncWorkRequestQueue) == 56);
static_assert(sizeof(NAsyncWorkResponseQueue) == 40);
static_assert(sizeof(NAsyncWorkResponse) == 16);
#endif

#endif
