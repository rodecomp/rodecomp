#ifndef RAGNAROK_SOURCE_UTILITY_NRECYCLEOBJECT_H
#define RAGNAROK_SOURCE_UTILITY_NRECYCLEOBJECT_H

#include "Utility/NSyncObject.h"

#include <algorithm>
#include <cstdint>
#include <deque>
#include <list>
#include <new>
#include <string>
#include <utility>
#include <windows.h>

class NRecycleObject {
public:
  explicit NRecycleObject(std::string poolName);
  virtual void Reset() = 0;
  virtual ~NRecycleObject();

  // OFFSET: 0x00731C60
  // FUNCTION: fn_07563
  std::string PoolName() const {
    return this->m_poolName;
  }

protected:
  /* 0004 */ std::string m_poolName;
};

class NRecycleObjectPoolInter {
public:
  virtual NRecycleObject* Acquire() = 0;
  virtual void Release(NRecycleObject* object) = 0;
  virtual ~NRecycleObjectPoolInter();
};

// Family established by multiple RTTI specializations and matching operations.
// Address tags below identify the observed account-data-load specialization;
// they do not claim original addresses for unobserved template arguments.
template <class T> class NRecycleObjectPool : public NRecycleObjectPoolInter {
public:
  // OFFSET: 0x00575AC0
  // FUNCTION: fn_02529
  explicit NRecycleObjectPool(std::uint32_t minimumFree) : m_enabled(false), m_minimumFree(minimumFree) {
    InitializeCriticalSection(&this->m_criticalSection);
    this->m_enabled = true;
  }

  // OFFSET: 0x005775F0
  // FUNCTION: fn_02536
  ~NRecycleObjectPool() override {
    for (T* object : this->m_objects) {
      delete object;
    }
    DeleteCriticalSection(&this->m_criticalSection);
  }

  // OFFSET: 0x005782F0
  // FUNCTION: fn_02552
  T* Acquire() override {
    NSyncObject held(&this->m_criticalSection);
    T* object;
    if (this->m_free.size() >= this->m_minimumFree) {
      object = this->m_free.front();
      this->m_free.pop_front();
    } else {
      object = new (std::nothrow) T;
      if (object) {
        this->m_objects.push_back(object);
      }
    }
    if (object) {
      object->Reset();
    }
    return object;
  }

  // OFFSET: 0x0057A6F0
  // FUNCTION: fn_02567
  void Release(NRecycleObject* object) override {
    NSyncObject held(&this->m_criticalSection);
    if (object) {
      auto owned = std::find(this->m_objects.begin(), this->m_objects.end(), object);
      if (owned != this->m_objects.end()) {
        // The original does not reject duplicate releases.
        this->m_free.push_back(*owned);
      }
    }
  }

protected:
  /* 0004 */ bool m_enabled;
  /* 0008 */ std::deque<T*> m_free;
  /* 001C */ std::list<T*> m_objects;
  /* 0024 */ std::uint32_t m_minimumFree;
  /* 0028 */ CRITICAL_SECTION m_criticalSection;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NRecycleObject) == 28);
static_assert(sizeof(NRecycleObjectPoolInter) == 4);
#endif

#endif
