#include "Utility/GroupedInteger.h"

#include <cstdio>
#include <cstring>

namespace {

  // OFFSET: 0x00AF8730
  // FUNCTION: fn_18263
  void GroupDigits(const char* digits, int length, char* output, int capacity) {
    int written = 0;

    if (digits[0] == '-') {
      written = 1;
      output[0] = '-';
      --length;
    }

    const char* next = digits + written;
    int phase = 3 * (length / 3) - length + 2;

    for (; *next != '\0'; phase = (phase + 1) % 3) {
      if (written >= capacity) {
        break;
      }

      output[written++] = *next++;

      if (phase == 1) {
        if (written >= capacity) {
          break;
        }

        output[written++] = ',';
      }
    }

    // The original overwrites the final comma, or the last copied digit if the
    // buffer filled. Do not replace this with conventional snprintf truncation.
    output[written - 1] = '\0';
  }

}

// OFFSET: 0x00AF60E0
// FUNCTION: fn_18249
char* FormatGroupedInteger(std::int64_t value, char* output, int capacity) {
  if (output && capacity > 0) {
    char digits[32];
    const int length = std::snprintf(digits, sizeof(digits), "%lld", static_cast<long long>(value));
    GroupDigits(digits, length, output, capacity);
  }

  return output;
}

// OFFSET: 0x00AF6140
// FUNCTION: fn_18250
char* FormatGroupedInteger(int value, char* output, int capacity) {
  if (output && capacity > 0) {
    char digits[32];
    const int length = std::snprintf(digits, sizeof(digits), "%d", value);
    GroupDigits(digits, length, output, capacity);
  }

  return output;
}

// OFFSET: 0x00AF82D0
// FUNCTION: fn_18262
const char* ReverseFindChar(const char* text, char target) {
  for (const char* current = text + std::strlen(text) - 1; current >= text; --current) {
    if (*current == target) {
      return current;
    }
  }

  return nullptr;
}
