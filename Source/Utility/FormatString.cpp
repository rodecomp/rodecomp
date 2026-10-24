#include "Utility/FormatString.h"
#include "Utility/NSyncObject.h"

#include <array>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <map>
#include <windows.h>

// OFFSET: 0x005BAEF0
// FUNCTION: fn_03451
int StringFormat(char* buffer, std::size_t sizeInBytes, std::size_t count, const char* format, ...) {
  va_list args;
  va_start(args, format);
  const int result = _vsnprintf_s(buffer, sizeInBytes, count, format, args);
  va_end(args);

  if (result < 0) {
    return -1;
  }

  return result;
}

namespace {

  class ThreadSafeBuffer {
  public:
    ThreadSafeBuffer() {
      InitializeCriticalSection(&this->m_criticalSection);
    }

    virtual ~ThreadSafeBuffer() {
      DeleteCriticalSection(&this->m_criticalSection);
    }

    char* GetBuffer();

  private:
    /* 0004 */ CRITICAL_SECTION m_criticalSection;
    /* 001C */ std::map<HANDLE, std::array<char, 2048>> m_buffers;
  };

  // OFFSET: 0x00734390
  // FUNCTION: fn_07595
  char* ThreadSafeBuffer::GetBuffer() {
    NSyncObject lock(&this->m_criticalSection);
    HANDLE thread = GetCurrentThread();

    auto it = this->m_buffers.find(thread);
    if (it == this->m_buffers.end()) {
      it = this->m_buffers.emplace(thread, std::array<char, 2048>{}).first;
    }

    return it->second.data();
  }

}

// OFFSET: 0x00734470
// FUNCTION: fn_07596
const char* FormatString(const char* format, ...) {
  static ThreadSafeBuffer s_buffer;
  char* buffer = s_buffer.GetBuffer();

  va_list args;
  va_start(args, format);
  std::vsnprintf(buffer, 2048, format, args);
  va_end(args);

  return s_buffer.GetBuffer();
}
