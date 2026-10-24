#ifndef RAGNAROK_SOURCE_UTILITY_NASYNCWORKTHREAD_H
#define RAGNAROK_SOURCE_UTILITY_NASYNCWORKTHREAD_H

#include "Utility/NAsyncWorkQueue.h"
#include "Utility/NMessageMgr.h"
#include "Utility/NMessageQueue.h"
#include "Utility/NObject.h"
#include "Utility/NThread.h"

class NAsyncWorkThread : public NObject, public NInter, public NThread {
  friend class NAsyncWorkMgr;

public:
  NAsyncWorkThread(NAsyncWorkRequestQueue* requests, NAsyncWorkResponseQueue* responses);
  ~NAsyncWorkThread() override;
  void Receive(std::uint64_t argument, std::uint32_t id) override;
  unsigned Run() override;
  void RequestStop() override;
  bool Start();
  void Stop();

protected:
  /* 0038 */ bool m_registered;
  /* 003C */ NMessageQueue m_messages;
  /* 0074 */ NAsyncWorkRequestQueue* m_requests;
  /* 0078 */ NAsyncWorkResponseQueue* m_responses;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NAsyncWorkThread) == 124);
#endif

#endif
