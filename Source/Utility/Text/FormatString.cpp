#include "Utility/Text/FormatString.h"
#include "Utility/Threading/NSyncObject.h"

#include <array>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <windows.h>

// GLOBAL: gl_24365
static char s_numberBuffer[1024];

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

// OFFSET: 0x00A95590
// FUNCTION: fn_17181
std::string IntToString(int value, unsigned int digits) {
  if (digits == 0) {
    _itoa(value, s_numberBuffer, 10);
    return std::string(s_numberBuffer);
  }

  std::sprintf(s_numberBuffer, "%.10d", value);
  std::string text(s_numberBuffer);
  if (text.size() > digits) {
    text.erase(0, text.size() - digits);
  }

  return text;
}

// OFFSET: 0x00A95790
// FUNCTION: fn_17182
std::string IntToSignedString(int value, bool grouped, bool showPlus) {
  if (!grouped) {
    return IntToString(value, 0);
  }

  const unsigned int bits = static_cast<unsigned int>(value);
  const unsigned int magnitude = value < 0 ? 0u - bits : bits;
  std::string digits = IntToString(static_cast<int>(magnitude), 0);
  std::string text;
  const bool negative = value < 0;
  for (std::size_t i = 0; i < digits.size(); ++i) {
    if ((digits.size() - i) % 3 == 0) {
      text += std::string(",");
    }
    text += digits.at(i);
  }

  if (text.at(0) == ',') {
    text.erase(0, 1);
  }

  if (negative) {
    text.insert(text.begin(), '-');
  } else if (showPlus && value != 0) {
    text.insert(text.begin(), '+');
  }

  return text;
}
