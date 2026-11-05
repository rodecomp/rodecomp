#include "Utility/Async/NMessageQueue.h"
#include "Utility/Threading/NSyncObject.h"

// OFFSET: 0x00736340
// FUNCTION: fn_07619
NMessageQueue::NMessageQueue() : m_semaphore(nullptr), m_enabled(false) {
  InitializeCriticalSection(&this->m_criticalSection);

  this->m_semaphore = CreateSemaphoreA(nullptr, 0, 0x7FFFFFFF, "");
  if (this->m_semaphore) {
    this->m_enabled = true;
  }
}

// OFFSET: 0x00736460
// FUNCTION: fn_07620
NMessageQueue::~NMessageQueue() {
  if (this->m_semaphore) {
    CloseHandle(this->m_semaphore);
    this->m_semaphore = nullptr;
  }

  DeleteCriticalSection(&this->m_criticalSection);
}

// OFFSET: 0x00736590
// FUNCTION: fn_07622
NMessage NMessageQueue::Pop() {
  NSyncObject held(&this->m_criticalSection);
  if (this->m_messages.empty()) {
    return NMessage();
  }

  Record record = this->m_messages.front();
  this->m_messages.pop_front();
  return NMessage(record.id, record.argumentHigh, record.argumentLow);
}

// OFFSET: 0x00736660
// FUNCTION: fn_07624
void NMessageQueue::Push(std::uint32_t argumentLow, std::uint32_t argumentHigh, std::uint32_t id) {
  NSyncObject held(&this->m_criticalSection);
  this->m_messages.push_back({argumentLow, argumentHigh, id});
  ReleaseSemaphore(this->m_semaphore, 1, nullptr);
}

// OFFSET: 0x00736650
// FUNCTION: fn_07623
void NMessageQueue::Clear() {
  std::deque<Record>().swap(this->m_messages);
}
