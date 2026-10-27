#ifndef RAGNAROK_SOURCE_UTILITY_ASYNC_NMESSAGEQUEUE_H
#define RAGNAROK_SOURCE_UTILITY_ASYNC_NMESSAGEQUEUE_H

#include <cstddef>
#include <cstdint>
#include <deque>
#include <windows.h>

struct NMessage {
  /* 0000 */ std::uint32_t id;
  /* 0004 */ std::uint32_t argumentHigh;
  /* 0008 */ std::uint32_t argumentLow;
  /* 000C */ bool valid;
};

class NMessageQueue {
  friend class NAsyncWorkThread;

public:
  NMessageQueue();
  virtual ~NMessageQueue();

  NMessage* Pop(NMessage* message);
  void Push(std::uint64_t argument, std::uint32_t id);
  void Clear();

protected:
  struct Record {
    /* 0000 */ std::uint32_t argumentLow;
    /* 0004 */ std::uint32_t argumentHigh;
    /* 0008 */ std::uint32_t id;
  };

  /* 0004 */ CRITICAL_SECTION m_criticalSection;
  /* 001C */ std::deque<Record> m_messages;
  /* 0030 */ HANDLE m_semaphore;
  /* 0034 */ bool m_enabled;
};

static_assert(sizeof(NMessage) == 16);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(NMessage, argumentHigh) == 4);
static_assert(offsetof(NMessage, argumentLow) == 8);
static_assert(offsetof(NMessage, valid) == 12);
static_assert(sizeof(NMessageQueue) == 56);
#endif

#endif
