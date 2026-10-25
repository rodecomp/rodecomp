#include "Utility/Async/NAsyncWork.h"

#include <utility>

// OFFSET: 0x005B9E50
// FUNCTION: fn_03440
NAsyncWork::NAsyncWork(std::string poolName) : NRecycleObject(std::move(poolName)), m_group(0), m_key(0) {
}

// OFFSET: 0x005BA4F0
// FUNCTION: fn_03443
NAsyncWork::~NAsyncWork() = default;

// OFFSET: 0x005BAEE0
// FUNCTION: fn_03450
bool NAsyncWork::Execute() {
  return false;
}

// OFFSET: 0x005BA900
// FUNCTION: fn_03448
void NAsyncWork::Complete() {
}

// OFFSET: 0x00731A90
// FUNCTION: fn_07559
NAsyncWorkPtr::NAsyncWorkPtr(NAsyncWorkPtr& other) : m_manager(other.m_manager), m_work(other.m_work) {
  other.m_manager = nullptr;
  other.m_work = nullptr;
}

// OFFSET: 0x00731AC0
// FUNCTION: fn_07560
NAsyncWorkPtr::NAsyncWorkPtr(NRecycleObjectPoolMgr* manager, std::string poolName) : m_manager(manager), m_work(nullptr) {
  if (manager) {
    this->m_work = static_cast<NAsyncWork*>(manager->Acquire(std::move(poolName)));
  }
}

// OFFSET: 0x00731B50
// FUNCTION: fn_07561
// OFFSET: 0x00731BD0
// FUNCTION: fn_07562
NAsyncWorkPtr::~NAsyncWorkPtr() {
  if (this->m_work) {
    this->m_manager->Release(this->m_work->PoolName(), this->m_work);
    this->m_work = nullptr;
  }
}
