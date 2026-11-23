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
  // INLINED
  explicit NRecycleObject(std::string poolName) : m_poolName(poolName) {
  }

  virtual void Reset() = 0;

  // OFFSET: 0x005BA3C0
  // FUNCTION: fn_46401
  virtual ~NRecycleObject() {
  }

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

inline NRecycleObjectPoolInter::~NRecycleObjectPoolInter() = default;

template <class T> class NRecycleObjectPool : public NRecycleObjectPoolInter {
public:
  // OFFSET: 0x00575AC0
  // FUNCTION: fn_02529
  // OFFSET: 0x00576140
  // FUNCTION: fn_46379
  // OFFSET: 0x00575C60
  // FUNCTION: fn_46373
  // OFFSET: 0x00576550
  // FUNCTION: fn_46384
  // OFFSET: 0x00576480
  // FUNCTION: fn_46383
  // OFFSET: 0x00576070
  // FUNCTION: fn_46378
  // OFFSET: 0x00576210
  // FUNCTION: fn_46380
  // OFFSET: 0x005763B0
  // FUNCTION: fn_46382
  // OFFSET: 0x005762E0
  // FUNCTION: fn_46381
  // OFFSET: 0x00575ED0
  // FUNCTION: fn_46376
  // OFFSET: 0x00575FA0
  // FUNCTION: fn_46377
  // OFFSET: 0x00575E00
  // FUNCTION: fn_46375
  // OFFSET: 0x00575D30
  // FUNCTION: fn_46374
  // OFFSET: 0x00575B90
  // FUNCTION: fn_46372
  explicit NRecycleObjectPool(std::uint32_t minimumFree) : m_minimumFree(minimumFree) {
    this->m_enabled = false;
    InitializeCriticalSection(&this->m_criticalSection);
    this->m_enabled = true;
  }

  // OFFSET: 0x00576CD0
  // FUNCTION: fn_46392
  // OFFSET: 0x00576850
  // FUNCTION: fn_46386
  // OFFSET: 0x00577090
  // FUNCTION: fn_46397
  // OFFSET: 0x00576FD0
  // FUNCTION: fn_46396
  // OFFSET: 0x00576C10
  // FUNCTION: fn_46391
  // OFFSET: 0x00576D90
  // FUNCTION: fn_46393
  // OFFSET: 0x00576F10
  // FUNCTION: fn_46395
  // OFFSET: 0x00576E50
  // FUNCTION: fn_46394
  // OFFSET: 0x00576A90
  // FUNCTION: fn_46389
  // OFFSET: 0x00576B50
  // FUNCTION: fn_46390
  // OFFSET: 0x005769D0
  // FUNCTION: fn_46388
  // OFFSET: 0x00576910
  // FUNCTION: fn_46387
  // OFFSET: 0x00576790
  // FUNCTION: fn_46385
  // OFFSET: 0x005766D0
  // FUNCTION: fn_46371
  __declspec(noinline) ~NRecycleObjectPool() override {
    for (auto object = this->m_objects.begin(); object != this->m_objects.end(); ++object) {
      delete *object;
    }
    DeleteCriticalSection(&this->m_criticalSection);
  }

  // OFFSET: 0x005782F0
  // FUNCTION: fn_02552
  // OFFSET: 0x005783D0
  // FUNCTION: fn_02553
  // OFFSET: 0x005784B0
  // FUNCTION: fn_02554
  // OFFSET: 0x00578590
  // FUNCTION: fn_02555
  // OFFSET: 0x00578670
  // FUNCTION: fn_02556
  // OFFSET: 0x00578750
  // FUNCTION: fn_02557
  // OFFSET: 0x00578830
  // FUNCTION: fn_02558
  // OFFSET: 0x00578910
  // FUNCTION: fn_02559
  // OFFSET: 0x005789F0
  // FUNCTION: fn_02560
  // OFFSET: 0x00578AD0
  // FUNCTION: fn_02561
  // OFFSET: 0x00578BB0
  // FUNCTION: fn_02562
  // OFFSET: 0x00578C90
  // FUNCTION: fn_02563
  // OFFSET: 0x00578D70
  // FUNCTION: fn_02564
  // OFFSET: 0x00578E50
  // FUNCTION: fn_02565
  T* Acquire() override {
    NSyncObject held(&this->m_criticalSection);
    T* object;
    if (this->m_free.size() < this->m_minimumFree) {
      object = new (std::nothrow) T;
      if (object == nullptr) {
        return nullptr;
      }
      this->m_objects.push_back(object);
    } else {
      object = static_cast<T*>(this->m_free.front());
      this->m_free.pop_front();
    }
    object->Reset();
    return object;
  }

  // OFFSET: 0x0057A6F0
  // FUNCTION: fn_02567
  // OFFSET: 0x0057A780
  // FUNCTION: fn_02568
  // OFFSET: 0x0057A810
  // FUNCTION: fn_02569
  // OFFSET: 0x0057A8A0
  // FUNCTION: fn_02570
  // OFFSET: 0x0057A930
  // FUNCTION: fn_02571
  // OFFSET: 0x0057A9C0
  // FUNCTION: fn_02572
  // OFFSET: 0x0057AA50
  // FUNCTION: fn_02573
  // OFFSET: 0x0057AAE0
  // FUNCTION: fn_02574
  // OFFSET: 0x0057AB70
  // FUNCTION: fn_02575
  // OFFSET: 0x0057AC00
  // FUNCTION: fn_02576
  // OFFSET: 0x0057AC90
  // FUNCTION: fn_02577
  // OFFSET: 0x0057AD20
  // FUNCTION: fn_02578
  // OFFSET: 0x0057ADB0
  // FUNCTION: fn_02579
  // OFFSET: 0x0057AE40
  // FUNCTION: fn_02580
  void Release(NRecycleObject* object) override {
    NSyncObject held(&this->m_criticalSection);
    if (object) {
      auto owned = std::find(this->m_objects.begin(), this->m_objects.end(), object);
      if (owned != this->m_objects.end()) {
        this->m_free.push_back(object);
      }
    }
  }

protected:
  /* 0004 */ bool m_enabled;
  /* 0008 */ std::deque<NRecycleObject*> m_free;
  /* 001C */ std::list<NRecycleObject*> m_objects;
  /* 0024 */ std::uint32_t m_minimumFree;
  /* 0028 */ CRITICAL_SECTION m_criticalSection;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NRecycleObject) == 28);
static_assert(sizeof(NRecycleObjectPoolInter) == 4);
#endif

#endif
