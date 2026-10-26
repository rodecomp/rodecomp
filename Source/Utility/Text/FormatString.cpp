#include "Utility/Text/FormatString.h"
#include "Utility/Threading/NSyncObject.h"

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
  const int result = __stdio_common_vsnprintf_s(_CRT_INTERNAL_LOCAL_PRINTF_OPTIONS, buffer, sizeInBytes, count, format, nullptr, args);
  va_end(args);

  if (result < 0) {
    return -1;
  }

  return result;
}

namespace {
  class ThreadSafeBuffer {
  public:
    // OFFSET: 0x00733F00
    // FUNCTION: fn_07592
    ThreadSafeBuffer() {
      InitializeCriticalSection(&this->m_criticalSection);
    }

    // INLINED
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

// FUNCTION: fn_18252
std::string FormatOwnedString(const char* format, ...) {
  char buffer[4096];
  va_list arguments;
  va_start(arguments, format);
  _vsnprintf_s(buffer, sizeof(buffer), static_cast<std::size_t>(-1), format, arguments);
  va_end(arguments);
  return std::string(buffer, std::strlen(buffer));
}
