#ifndef RAGNAROK_SOURCE_UTILITY_NMESSAGEQUEUE_H
#define RAGNAROK_SOURCE_UTILITY_NMESSAGEQUEUE_H

#include <cstdint>
#include <deque>
#include <windows.h>

// Reconstructed result name; original pop writes these three words then valid.
struct NMessage {
  std::uint32_t id;
  std::uint32_t argumentHigh;
  std::uint32_t argumentLow;
  bool valid;
};

// Original RTTI class; field and operation names are reconstructed.
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
    std::uint32_t argumentLow;
    std::uint32_t argumentHigh;
    std::uint32_t id;
  };

  /* 0004 */ CRITICAL_SECTION m_criticalSection;
  /* 001C */ std::deque<Record> m_messages;
  /* 0030 */ HANDLE m_semaphore;
  /* 0034 */ bool m_enabled;
};

static_assert(sizeof(NMessage) == 16);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NMessageQueue) == 56);
#endif

#endif
