#include "Utility/NRecycleObjectPoolMgr.h"

// OFFSET: 0x00736A10
// FUNCTION: fn_07631
std::size_t NRecycleObjectPoolMgr::NameHash::operator()(const std::string& name) const {
  std::uint32_t hash = 2166136261;
  for (unsigned char byte : name) {
    hash = 16777619 * (hash ^ byte);
  }
  std::uint32_t value = hash & 0x7FFFFFFF;
  std::int32_t reduced =
    static_cast<std::int32_t>(16807 * (value % 127773)) - static_cast<std::int32_t>(2836 * (value / 127773));
  if (reduced < 0) {
    reduced += 0x7FFFFFFF;
  }
  return static_cast<std::uint32_t>(reduced);
}

// OFFSET: 0x00736B30
// FUNCTION: fn_07632
NRecycleObjectPoolMgr::NRecycleObjectPoolMgr() : m_enabled(false) {
  this->m_pools.rehash(8);
  this->m_enabled = true;
}

// OFFSET: 0x00736DE0
// FUNCTION: fn_07635
// OFFSET: 0x00736E50
NRecycleObjectPoolMgr::~NRecycleObjectPoolMgr() = default;

// OFFSET: 0x00736EE0
NRecycleObject* NRecycleObjectPoolMgr::Acquire(std::string poolName) {
  auto pool = this->m_pools.find(poolName);
  if (pool == this->m_pools.end()) {
    return nullptr;
  }
  return pool->second->Acquire();
}

// OFFSET: 0x00736F60
void NRecycleObjectPoolMgr::Release(std::string poolName, NRecycleObject* object) {
  auto pool = this->m_pools.find(poolName);
  if (pool != this->m_pools.end()) {
    pool->second->Release(object);
  }
}
