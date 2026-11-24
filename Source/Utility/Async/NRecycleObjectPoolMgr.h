#ifndef RAGNAROK_SOURCE_UTILITY_ASYNC_NRECYCLEOBJECTPOOLMGR_H
#define RAGNAROK_SOURCE_UTILITY_ASYNC_NRECYCLEOBJECTPOOLMGR_H

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include "Utility/Async/NRecycleObject.h"

#include <hash_map>
#include <string>

class NRecycleObjectPoolMgr {
public:
  NRecycleObjectPoolMgr();
  virtual ~NRecycleObjectPoolMgr();
  NRecycleObject* Acquire(std::string poolName);
  void Release(std::string poolName, NRecycleObject* object);

protected:
  /* 0004 */ bool m_enabled;
  /* 0008 */ stdext::hash_map<std::string, NRecycleObjectPoolInter*> m_pools;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NRecycleObjectPoolMgr) == 44);
#endif

#endif
