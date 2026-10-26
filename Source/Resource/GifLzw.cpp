#include "Resource/GifLzw.h"

#include <cstdlib>

namespace {
  struct GifLzwEntry {
    /* 0000 */ unsigned char value;
    /* 0001 */ unsigned char unknown_0001[3];
    /* 0004 */ int prefix;
    /* 0008 */ int length;
  };

  static_assert(sizeof(GifLzwEntry) == 12);
}

// FUNCTION: fn_07868
int __stdcall DecodeGifLzw(int minimumCodeSize, const unsigned char* input, int inputBytes, unsigned char* output) {
  const int clearCode = 1 << minimumCodeSize;
  const int endCode = clearCode + 1;
  const unsigned int initialBytes = 12u * (1u << (minimumCodeSize + 1));
  auto* dictionary = static_cast<GifLzwEntry*>(std::malloc(initialBytes));
  int initial = 0;
  for (; initial < clearCode; ++initial) {
    dictionary[initial].value = static_cast<unsigned char>(initial);
    dictionary[initial].prefix = -1;
    dictionary[initial].length = 1;
  }

  int nextCode = initial + 2;
  int previousCode = -1;
  int codeSize = minimumCodeSize;
  int bitMask = 1;
  int lastLength = 0;
  while (inputBytes != 0) {
    const int bits = codeSize + 1;
    int code = 0;
    for (int bit = 0; bit < bits; ++bit) {
      code |= ((*input & bitMask) != 0) << bit;
      bitMask *= 2;
      if (bitMask == 256) {
        ++input;
        --inputBytes;
        bitMask = 1;
      }
    }

    if (code == clearCode) {
      codeSize = minimumCodeSize;
      dictionary = static_cast<GifLzwEntry*>(std::realloc(dictionary, initialBytes));
      initial = 0;
      for (; initial < clearCode; ++initial) {
        dictionary[initial].value = static_cast<unsigned char>(initial);
        dictionary[initial].prefix = -1;
        dictionary[initial].length = 1;
      }

      nextCode = initial + 2;
      previousCode = -1;
      continue;
    }

    if (code == endCode) {
      if (inputBytes > 1) {
        std::free(dictionary);
        return -1;
      }

      break;
    }

    if (previousCode > -1 && codeSize < 12) {
      if (code > nextCode) {
        std::free(dictionary);
        return -1;
      }

      int first = code == nextCode ? previousCode : code;
      while (dictionary[first].prefix != -1) {
        first = dictionary[first].prefix;
      }

      dictionary[nextCode].value = dictionary[first].value;
      dictionary[nextCode].prefix = previousCode;
      dictionary[nextCode].length = dictionary[previousCode].length + 1;
      ++nextCode;
      if (nextCode == (1 << bits) && codeSize < 11) {
        ++codeSize;
        dictionary = static_cast<GifLzwEntry*>(std::realloc(dictionary, 12u * (1u << (bits + 1))));
      }
    }

    previousCode = code;
    lastLength = dictionary[code].length;
    while (code != -1) {
      output[dictionary[code].length - 1] = dictionary[code].value;
      const int prefix = dictionary[code].prefix;
      if (prefix == code) {
        std::free(dictionary);
        return -1;
      }

      code = prefix;
    }

    output += lastLength;
  }

  std::free(dictionary);
  return lastLength;
}
