#include "File/ShortcutFilename.h"

// OFFSET: 0x00DD9430
// FUNCTION: fn_23335
std::string ShortcutFilename(std::uint32_t identifier) {
  std::string result;
  constexpr char digits[] = "3867012495";
  for (std::uint32_t divisor = 1000000000; divisor; divisor /= 10) {
    if (!result.empty() && divisor == 100) {
      result.push_back('-');
    }
    unsigned digit = identifier / divisor;
    identifier %= divisor;
    if (digit || !result.empty()) {
      result.push_back(digits[digit]);
    }
  }
  return result;
}
