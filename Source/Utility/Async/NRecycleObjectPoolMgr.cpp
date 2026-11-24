#include "Utility/Async/NRecycleObjectPoolMgr.h"

// OFFSET: 0x00736B30
// FUNCTION: fn_07632
NRecycleObjectPoolMgr::NRecycleObjectPoolMgr() {
  this->m_enabled = true;
}

// OFFSET: 0x00736DE0
// FUNCTION: fn_07635
NRecycleObjectPoolMgr::~NRecycleObjectPoolMgr() = default;

// OFFSET: 0x00736EE0
// FUNCTION: fn_07637
NRecycleObject* NRecycleObjectPoolMgr::Acquire(std::string poolName) {
  auto pool = this->m_pools.find(poolName);
  if (pool != this->m_pools.end()) {
    return pool->second->Acquire();
  }

  return nullptr;
}

// OFFSET: 0x00736F60
// FUNCTION: fn_07638
void NRecycleObjectPoolMgr::Release(std::string poolName, NRecycleObject* object) {
  auto pool = this->m_pools.find(poolName);
  if (pool != this->m_pools.end()) {
    pool->second->Release(object);
  }
}
