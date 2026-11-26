#ifndef RAGNAROK_SOURCE_UTILITY_THREADING_CONCURRENCYRESOURCEMANAGER_H
#define RAGNAROK_SOURCE_UTILITY_THREADING_CONCURRENCYRESOURCEMANAGER_H

#include <cstdint>
#include <map>
#include <windows.h>
#include <mmiscapi2.h>

struct TimerLock : CRITICAL_SECTION {
  /* 0018 */ std::uint32_t initialized;

  TimerLock() noexcept;
  ~TimerLock();
};

// INLINED
inline TimerLock::TimerLock() noexcept {
  this->initialized = 0;
  InitializeCriticalSection(this);
  this->initialized = 1;
}

// INLINED
inline TimerLock::~TimerLock() {
  DeleteCriticalSection(this);
  this->initialized = 0;
}

class ScopedTimerLock {
public:
  explicit ScopedTimerLock(TimerLock* lock);
  ~ScopedTimerLock();

private:
  /* 0000 */ TimerLock* m_lock;
  /* 0004 */ bool m_owned;
};

// INLINED
inline ScopedTimerLock::ScopedTimerLock(TimerLock* lock) : m_lock(lock), m_owned(true) {
  if (this->m_lock->initialized != 0) {
    EnterCriticalSection(this->m_lock);
  }
}

// INLINED
inline ScopedTimerLock::~ScopedTimerLock() {
  if (this->m_lock->initialized != 0) {
    LeaveCriticalSection(this->m_lock);
  }
}

struct Rc4State {
  /* 0000 */ unsigned int x;
  /* 0004 */ unsigned int y;
  /* 0008 */ unsigned int data[256];
};

struct LogWriter {
  /* 0000 */ HANDLE file;
  /* 0004 */ TimerLock lock;
  /* 0020 */ char buffer[4096];
  /* 1020 */ SYSTEMTIME time;
  /* 1030 */ std::uint32_t unknown_1030;
  /* 1034 */ Rc4State cipher;
  /* 143C */ int xorKey;
  /* 1440 */ unsigned int lineLimit;
  /* 1444 */ unsigned int lineCount;
};

struct LogContext {
  /* 0000 */ std::uint8_t unknown_0000[0x24];
  /* 0024 */ LogWriter writer;
};

namespace Concurrency {
namespace details {

class Resource {
public:
  /* 0000 */ TimerLock* lock;
  /* 0004 */ void* unknown_0004;
  /* 0008 */ unsigned int* allocation;
  /* 000C */ unsigned int size;
  /* 0010 */ unsigned int pages;
  /* 0014 */ BOOL isProtected;
  /* 0018 */ unsigned int initialized;
  /* 001C */ unsigned int seed;
  /* 0020 */ int mode;
  /* 0024 */ unsigned int regionSize;
  /* 0028 */ MEMORY_BASIC_INFORMATION regionInfo;

  Resource(int mode);
  void sub_E88720();
  void sub_E8BAF0();
  void sub_E8CF10();
  BOOL sub_E90560();
};

class ResourceManager {
public:
  /* 0000 */ TimerLock* lock;
  /* 0004 */ std::map<unsigned int, Resource*> resources;
  /* 000C */ MMRESULT timer;

  ResourceManager();
  void sub_E903F0(unsigned int key, Resource* resource);
};

}
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TimerLock) == 0x1C);
static_assert(sizeof(LogWriter) == 0x1448);
static_assert(sizeof(Concurrency::details::Resource) == 0x44);
static_assert(sizeof(Concurrency::details::ResourceManager) == 0x10);
#endif

extern LogContext* g_logContext;

void CALLBACK fptc(UINT timerId, UINT message, DWORD_PTR user, DWORD_PTR reserved1, DWORD_PTR reserved2);

TimerLock* sub_E94520();
char* sub_E939D0(char* text);
char* sub_E93D70(char* text);
bool sub_E916F0(LogWriter* writer, const char* format, ...);
void sub_E97C60(Rc4State* key, unsigned int length, const unsigned char* input, unsigned char* output);

#endif
