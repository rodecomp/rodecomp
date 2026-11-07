#include "File/ShortcutFilename.h"

// OFFSET: 0x00DD9430
// FUNCTION: fn_23335
std::string ShortcutFilename(std::uint32_t identifier) {
  std::string result = "";

  for (std::uint32_t divisor = 1000000000; divisor; divisor /= 10) {
    if (!result.empty() && divisor == 100) {
      result += '-';
    }

    std::uint32_t digit = identifier / divisor;
    identifier %= divisor;
    switch (digit) {
    case 0:
      if (!result.empty()) {
        result += '3';
      }
      break;
    case 1:
      result += '8';
      break;
    case 2:
      result += '6';
      break;
    case 3:
      result += '7';
      break;
    case 4:
      result += '0';
      break;
    case 5:
      result += '1';
      break;
    case 6:
      result += '2';
      break;
    case 7:
      result += '4';
      break;
    case 8:
      result += '9';
      break;
    case 9:
      result += '5';
      break;
    }
  }

  return result;
}
