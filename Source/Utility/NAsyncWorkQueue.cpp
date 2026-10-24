#include "Utility/NAsyncWorkQueue.h"
#include "Utility/NSyncObject.h"

#include <algorithm>

// OFFSET: 0x00731D70
// FUNCTION: fn_07565
NAsyncWorkRequestQueue::NAsyncWorkRequestQueue() : m_waiting(0), m_semaphore(nullptr), m_enabled(false) {
  InitializeCriticalSection(&this->m_criticalSection);
  this->m_semaphore = CreateSemaphoreA(nullptr, 0, 0x7FFFFFFF, "");
  if (this->m_semaphore) {
    this->m_enabled = true;
  }
}

// OFFSET: 0x00732140
// FUNCTION: fn_07567
NAsyncWorkRequestQueue::~NAsyncWorkRequestQueue() {
  if (this->m_semaphore) {
    CloseHandle(this->m_semaphore);
    this->m_semaphore = nullptr;
  }
  DeleteCriticalSection(&this->m_criticalSection);
}

// OFFSET: 0x007322D0
// FUNCTION: fn_07569
NAsyncWorkPtr NAsyncWorkRequestQueue::Pop() {
  NSyncObject held(&this->m_criticalSection);
  if (!this->m_pending.empty()) {
    unsigned examined = 1;
    for (auto work = this->m_pending.begin(); work != this->m_pending.end(); ++work) {
      if (examined > 40) {
        break;
      }
      auto key = work->Get()->GroupKey();
      if (std::find(this->m_active.begin(), this->m_active.end(), key) == this->m_active.end()) {
        this->m_active.push_back(key);
        NAsyncWorkPtr result(*work);
        this->m_pending.erase(work);
        return result;
      }
      ++examined;
    }
  }
  ++this->m_waiting;
  return NAsyncWorkPtr(nullptr, "");
}

// OFFSET: 0x007324D0
// FUNCTION: fn_07570
void NAsyncWorkRequestQueue::PushBack(NAsyncWorkPtr& work) {
  NSyncObject held(&this->m_criticalSection);
  this->m_pending.emplace_back(work);
  ReleaseSemaphore(this->m_semaphore, 1, nullptr);
}

// OFFSET: 0x00732550
// FUNCTION: fn_07571
void NAsyncWorkRequestQueue::PushFront(NAsyncWorkPtr& work) {
  NSyncObject held(&this->m_criticalSection);
  this->m_pending.emplace_front(work);
  ReleaseSemaphore(this->m_semaphore, 1, nullptr);
}

// OFFSET: 0x00732210
// FUNCTION: fn_07568
void NAsyncWorkRequestQueue::Complete(std::uint32_t group, std::uint32_t key) {
  NSyncObject held(&this->m_criticalSection);
  auto active = std::find(this->m_active.begin(), this->m_active.end(), std::pair{group, key});
  if (active != this->m_active.end()) {
    this->m_active.erase(active);
  }
  if (this->m_waiting) {
    --this->m_waiting;
    ReleaseSemaphore(this->m_semaphore, 1, nullptr);
  }
}

// OFFSET: 0x007325D0
// FUNCTION: fn_07572
NAsyncWorkResponseQueue::NAsyncWorkResponseQueue() : m_enabled(false) {
  InitializeCriticalSection(&this->m_criticalSection);
  this->m_enabled = true;
}

// OFFSET: 0x007326C0
// FUNCTION: fn_07573
NAsyncWorkResponseQueue::~NAsyncWorkResponseQueue() {
  DeleteCriticalSection(&this->m_criticalSection);
}

// OFFSET: 0x007328B0
// FUNCTION: fn_07575
void NAsyncWorkResponseQueue::Push(NAsyncWorkPtr& work) {
  NSyncObject held(&this->m_criticalSection);
  this->m_pending.emplace_back(work);
}

// OFFSET: 0x00732740
// FUNCTION: fn_07574
NAsyncWorkResponse NAsyncWorkResponseQueue::Pop() {
  NSyncObject held(&this->m_criticalSection);
  if (this->m_pending.empty()) {
    return {false, NAsyncWorkPtr(nullptr, "")};
  }
  NAsyncWorkPtr work(this->m_pending.front());
  this->m_pending.pop_front();
  return {true, work};
}
