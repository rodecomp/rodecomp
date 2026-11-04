#include "File/ShortcutFilename.h"

// OFFSET: 0x00DD9430
// FUNCTION: fn_23335
std::string ShortcutFilename(std::uint32_t identifier) {
  std::string result = "";

  for (std::uint32_t divisor = 1000000000; divisor; divisor /= 10) {
    if (!result.empty() && divisor == 100) {
      result.push_back('-');
    }

    std::uint32_t digit = identifier / divisor;
    identifier %= divisor;
    switch (digit) {
    case 0:
      if (!result.empty()) {
        result.push_back('3');
      }
      break;
    case 1:
      result.push_back('8');
      break;
    case 2:
      result.push_back('6');
      break;
    case 3:
      result.push_back('7');
      break;
    case 4:
      result.push_back('0');
      break;
    case 5:
      result.push_back('1');
      break;
    case 6:
      result.push_back('2');
      break;
    case 7:
      result.push_back('4');
      break;
    case 8:
      result.push_back('9');
      break;
    case 9:
      result.push_back('5');
      break;
    }
  }

  return result;
}
