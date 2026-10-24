#ifndef RAGNAROK_SOURCE_UTILITY_NASYNCWORKMGR_H
#define RAGNAROK_SOURCE_UTILITY_NASYNCWORKMGR_H

#include "Utility/NAsyncWorkThread.h"

#include <cstdint>
#include <vector>

class NAsyncWorkMgr {
public:
  explicit NAsyncWorkMgr(std::uint32_t workers);
  virtual void Update(std::uint32_t maximum);
  virtual ~NAsyncWorkMgr();
  void Submit(NAsyncWorkPtr& work);
  bool Start();
  void Stop();

protected:
  bool Initialize(std::uint32_t workers);
  void DestroyQueues();

  /* 0004 */ bool m_enabled;
  /* 0008 */ std::uint32_t m_workerCount;
  /* 000C */ NAsyncWorkRequestQueue* m_requests;
  /* 0010 */ NAsyncWorkResponseQueue* m_responses;
  /* 0014 */ std::vector<NAsyncWorkThread*> m_workers;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NAsyncWorkMgr) == 32);
#endif

#endif
