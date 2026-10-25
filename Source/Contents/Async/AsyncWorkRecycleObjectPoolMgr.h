#ifndef RAGNAROK_SOURCE_CONTENTS_ASYNC_ASYNCWORKRECYCLEOBJECTPOOLMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ASYNC_ASYNCWORKRECYCLEOBJECTPOOLMGR_H

#include "Utility/Async/NRecycleObjectPoolMgr.h"

class CAsyncWorkRecycleObjectPoolMgr : public NRecycleObjectPoolMgr {
public:
  CAsyncWorkRecycleObjectPoolMgr();
  ~CAsyncWorkRecycleObjectPoolMgr() override;
  bool Ready() const {
    return this->m_registered;
  }

private:
  bool RegisterPools();
  /* 002C */ bool m_registered;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAsyncWorkRecycleObjectPoolMgr) == 48);
#endif

#endif
