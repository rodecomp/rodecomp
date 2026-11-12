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
  int result;
  va_list args;

  va_start(args, format);
  result = _vsnprintf_s_l(buffer, sizeInBytes, count, format, nullptr, args);
  va_end(args);

  return result;
}

namespace {
  class ThreadSafeBuffer {
  public:
    // OFFSET: 0x00733F00
    // FUNCTION: fn_07592
    __declspec(noinline) ThreadSafeBuffer() {
      InitializeCriticalSection(&this->m_criticalSection);
    }

    // INLINED
    virtual ~ThreadSafeBuffer() {
      DeleteCriticalSection(&this->m_criticalSection);
    }

    char* GetBuffer();

  private:
    /* 0004 */ CRITICAL_SECTION m_criticalSection;
    /* 001C */ std::map<int, std::array<char, 2048>> m_buffers;
  };

  // OFFSET: 0x00734390
  // FUNCTION: fn_07595
  char* ThreadSafeBuffer::GetBuffer() {
    NSyncObject lock(&this->m_criticalSection);
    const int thread = reinterpret_cast<int>(GetCurrentThread());
    std::map<int, std::array<char, 2048>>& buffers = this->m_buffers;

    if (buffers.find(thread) == buffers.end()) {
      std::array<char, 2048> buffer{};
      buffers[thread] = buffer;
    }

    return buffers[thread].data();
  }
}

// OFFSET: 0x00734470
// FUNCTION: fn_07596
const char* FormatString(const char* format, ...) {
  static ThreadSafeBuffer s_buffer;
  va_list args;
  va_start(args, format);
  _vsnprintf(s_buffer.GetBuffer(), 2048, format, args);
  va_end(args);

  return s_buffer.GetBuffer();
}

// OFFSET: 0x00AF62D0
// FUNCTION: fn_18252
std::string FormatOwnedString(const char* format, ...) {
  char buffer[4096];
  va_list arguments;

  va_start(arguments, format);
  _vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, format, arguments);
  va_end(arguments);

  return std::string(buffer);
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
    text.erase(text.begin(), text.end() - digits);
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
    std::size_t remaining = digits.size() - i;
    if (remaining % 3 == 0) {
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
