#ifndef RAGNAROK_SOURCE_UTILITY_ASYNC_NRECYCLEOBJECTPOOLMGR_H
#define RAGNAROK_SOURCE_UTILITY_ASYNC_NRECYCLEOBJECTPOOLMGR_H

#include "Utility/Async/NRecycleObject.h"

#include <string>
#include <unordered_map>

class NRecycleObjectPoolMgr {
public:
  NRecycleObjectPoolMgr();
  virtual ~NRecycleObjectPoolMgr();
  NRecycleObject* Acquire(std::string poolName);
  void Release(std::string poolName, NRecycleObject* object);

protected:
  struct NameHash {

    std::uint8_t unknown_0000 = 0;
    std::size_t operator()(const std::string& name) const;
  };

  /* 0004 */ bool m_enabled;
  /* 0008 */ std::unordered_map<std::string, NRecycleObjectPoolInter*, NameHash> m_pools;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NRecycleObjectPoolMgr) == 44);
#endif

#endif
