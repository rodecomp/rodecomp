#include "Utility/Text/HexString.h"

#include <cstring>

// OFFSET: 0x00AF9F80
// FUNCTION: fn_18285
int ParseHexString(const char* text) {
  int value = 0;
  unsigned char shift = 0;
  for (int index = static_cast<int>(std::strlen(text)) - 1; index > 1; --index) {
    const int digit = text[index];
    switch (digit) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      value += (digit - '0') << shift;
      break;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      value += (digit - 'A' + 10) << shift;
      break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
      value += (digit - 'a' + 10) << shift;
      break;
    }
    shift += 4;
  }
  return value;
}
