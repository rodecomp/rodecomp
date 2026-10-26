#include "Utility/GuidParse.h"
#include "Utility/Text/FormatString.h"

#include <cstdarg>
#include <cstdio>

namespace {
  // OFFSET: 0x00AFA440
  // FUNCTION: fn_18290
  int Scan(const char* text, std::size_t bytes, const char* format, ...) {
    va_list arguments;
    va_start(arguments, format);
    const int count = __stdio_common_vsscanf(_CRT_INTERNAL_LOCAL_SCANF_OPTIONS | _CRT_INTERNAL_SCANF_SECURECRT, text, bytes, format, nullptr, arguments);
    va_end(arguments);

    return count;
  }
}

// OFFSET: 0x00AF7610
// FUNCTION: fn_18260
int ParseGuid(const char* text, std::size_t bytes, GUID& output, bool braced) {
  const char* format = braced ? "{%8X-%4hX-%4hX-%2hhX%2hhX-%2hhX%2hhX%2hhX%2hhX%2hhX%2hhX}" : "%8X-%4hX-%4hX-%2hhX%2hhX-%2hhX%2hhX%2hhX%2hhX%2hhX%2hhX";
  const int count = Scan(
    text, bytes, format, &output.Data1, &output.Data2, &output.Data3, &output.Data4[0], &output.Data4[1], &output.Data4[2], &output.Data4[3], &output.Data4[4],
    &output.Data4[5], &output.Data4[6], &output.Data4[7]
  );
  return count == 11 ? 1 : count == EOF ? -1 : 0;
}

// FUNCTION: fn_18259
int FormatGuid(char* text, std::size_t bytes, const GUID& value, bool braced) {
  const char* format = braced ? "{%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}" : "%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX";
  return StringFormat(
    text, bytes, static_cast<std::size_t>(-1), format, value.Data1, value.Data2, value.Data3, value.Data4[0], value.Data4[1], value.Data4[2], value.Data4[3],
    value.Data4[4], value.Data4[5], value.Data4[6], value.Data4[7]
  );
}
