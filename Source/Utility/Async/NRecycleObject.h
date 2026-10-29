#ifndef RAGNAROK_SOURCE_UTILITY_ASYNC_NRECYCLEOBJECT_H
#define RAGNAROK_SOURCE_UTILITY_ASYNC_NRECYCLEOBJECT_H

#include "Utility/Threading/NSyncObject.h"

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

// OFFSET: 0x005782C0
inline NRecycleObjectPoolInter::~NRecycleObjectPoolInter() = default;

template <class T> class NRecycleObjectPool : public NRecycleObjectPoolInter {
public:
  // OFFSET: 0x00575AC0
  // FUNCTION: fn_02529
  explicit NRecycleObjectPool(std::uint32_t minimumFree) : m_enabled(false), m_minimumFree(minimumFree) {
    InitializeCriticalSection(&this->m_criticalSection);
    this->m_enabled = true;
  }

  // INLINED
  ~NRecycleObjectPool() override {
    for (T* object : this->m_objects) {
      delete object;
    }
    DeleteCriticalSection(&this->m_criticalSection);
  }

  // OFFSET: 0x005782F0
  // OFFSET: 0x005783D0
  // OFFSET: 0x005784B0
  // OFFSET: 0x00578590
  // OFFSET: 0x00578670
  // OFFSET: 0x00578750
  // OFFSET: 0x00578830
  // OFFSET: 0x00578910
  // OFFSET: 0x005789F0
  // OFFSET: 0x00578AD0
  // OFFSET: 0x00578BB0
  // OFFSET: 0x00578C90
  // OFFSET: 0x00578D70
  // OFFSET: 0x00578E50
  // FUNCTION: fn_02552
  // FUNCTION: fn_02553
  // FUNCTION: fn_02554
  // FUNCTION: fn_02555
  // FUNCTION: fn_02556
  // FUNCTION: fn_02557
  // FUNCTION: fn_02558
  // FUNCTION: fn_02559
  // FUNCTION: fn_02560
  // FUNCTION: fn_02561
  // FUNCTION: fn_02562
  // FUNCTION: fn_02563
  // FUNCTION: fn_02564
  // FUNCTION: fn_02565
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
  // OFFSET: 0x0057A780
  // OFFSET: 0x0057A810
  // OFFSET: 0x0057A8A0
  // OFFSET: 0x0057A930
  // OFFSET: 0x0057A9C0
  // OFFSET: 0x0057AA50
  // OFFSET: 0x0057AAE0
  // OFFSET: 0x0057AB70
  // OFFSET: 0x0057AC00
  // OFFSET: 0x0057AC90
  // OFFSET: 0x0057AD20
  // OFFSET: 0x0057ADB0
  // OFFSET: 0x0057AE40
  // FUNCTION: fn_02567
  // FUNCTION: fn_02568
  // FUNCTION: fn_02569
  // FUNCTION: fn_02570
  // FUNCTION: fn_02571
  // FUNCTION: fn_02572
  // FUNCTION: fn_02573
  // FUNCTION: fn_02574
  // FUNCTION: fn_02575
  // FUNCTION: fn_02576
  // FUNCTION: fn_02577
  // FUNCTION: fn_02578
  // FUNCTION: fn_02579
  // FUNCTION: fn_02580
  void Release(NRecycleObject* object) override {
    NSyncObject held(&this->m_criticalSection);
    if (object) {
      auto owned = std::find(this->m_objects.begin(), this->m_objects.end(), object);
      if (owned != this->m_objects.end()) {

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
