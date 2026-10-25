#ifndef RAGNAROK_SOURCE_UTILITY_ASYNC_NASYNCWORK_H
#define RAGNAROK_SOURCE_UTILITY_ASYNC_NASYNCWORK_H

#include "Utility/Async/NRecycleObjectPoolMgr.h"

class NAsyncWork : public NRecycleObject {
public:
  explicit NAsyncWork(std::string poolName);
  ~NAsyncWork() override;
  virtual bool Execute();
  virtual void Complete();

  // INLINED
  std::pair<std::uint32_t, std::uint32_t> GroupKey() const {
    return {m_group, m_key};
  }

protected:
  /* 001C */ std::uint32_t unknown_001C;
  /* 0020 */ std::uint32_t m_group;
  /* 0024 */ std::uint32_t m_key;
};

class NAsyncWorkPtr {
public:
  NAsyncWorkPtr(NAsyncWorkPtr& other);
  NAsyncWorkPtr(NRecycleObjectPoolMgr* manager, std::string poolName);
  virtual ~NAsyncWorkPtr();

  // INLINED
  NAsyncWork* Get() const {
    return this->m_work;
  }

protected:
  /* 0004 */ NRecycleObjectPoolMgr* m_manager;
  /* 0008 */ NAsyncWork* m_work;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NAsyncWork) == 40);
static_assert(sizeof(NAsyncWorkPtr) == 12);
#endif

#endif
