#include "Resource/GifResource.h"

#include <cstdlib>

namespace {
  struct GifLzwEntry {
    /* 0000 */ unsigned char value;
    /* 0001 */ unsigned char padding_0001[3];
    /* 0004 */ int prefix;
    /* 0008 */ int length;
  };

  static_assert(sizeof(GifLzwEntry) == 12);
}

// OFFSET: 0x0074A840
// FUNCTION: fn_07868
int __stdcall DecodeGifLzw(int codeSize, const unsigned char* input, int inputBytes, unsigned char* output) {
  int previousCode = -1;
  unsigned int bitMask = 1;
  int matchLength = 0;

  const int clearCode = 1 << codeSize;
  const int endCode = clearCode + 1;
  const int resetCodeSize = codeSize;

  GifLzwEntry* dictionary = static_cast<GifLzwEntry*>(std::malloc(sizeof(GifLzwEntry) * (1 << (codeSize + 1))));
  int nextCode;
  for (nextCode = 0; nextCode < (1 << codeSize); ++nextCode) {
    dictionary[nextCode].value = static_cast<unsigned char>(nextCode);
    dictionary[nextCode].prefix = -1;
    dictionary[nextCode].length = 1;
  }

  ++nextCode;
  ++nextCode;

  while (inputBytes) {
    int code = 0;
    for (int bit = 0; bit < codeSize + 1; ++bit) {
      const int value = (*input & bitMask) ? 1 : 0;
      bitMask <<= 1;

      if (bitMask == 0x100) {
        bitMask = 1;
        ++input;
        --inputBytes;
      }

      code = code | (value << bit);
    }

    if (code == clearCode) {
      codeSize = resetCodeSize;
      dictionary = static_cast<GifLzwEntry*>(std::realloc(dictionary, sizeof(GifLzwEntry) * (1 << (codeSize + 1))));

      for (nextCode = 0; nextCode < (1 << codeSize); ++nextCode) {
        dictionary[nextCode].value = static_cast<unsigned char>(nextCode);
        dictionary[nextCode].prefix = -1;
        dictionary[nextCode].length = 1;
      }

      ++nextCode;
      ++nextCode;
      previousCode = -1;
      continue;
    } else if (code == endCode) {
      if (inputBytes > 1) {
        if (dictionary) {
          std::free(dictionary);
        }
        return -1;
      }
      break;
    }

    if (previousCode > -1 && codeSize < 12) {
      if (code > nextCode) {
        if (dictionary) {
          std::free(dictionary);
        }
        return -1;
      }

      if (code == nextCode) {
        int root = previousCode;
        while (dictionary[root].prefix != -1) {
          root = dictionary[root].prefix;
        }
        dictionary[nextCode].value = dictionary[root].value;
      } else {
        int root = code;
        while (dictionary[root].prefix != -1) {
          root = dictionary[root].prefix;
        }
        dictionary[nextCode].value = dictionary[root].value;
      }

      dictionary[nextCode].prefix = previousCode;
      dictionary[nextCode].length = dictionary[previousCode].length + 1;
      ++nextCode;

      if (nextCode == (1 << (codeSize + 1)) && codeSize < 11) {
        ++codeSize;
        dictionary = static_cast<GifLzwEntry*>(std::realloc(dictionary, sizeof(GifLzwEntry) * (1 << (codeSize + 1))));
      }
    }

    previousCode = code;

    matchLength = dictionary[code].length;
    while (code != -1) {
      output[dictionary[code].length - 1] = dictionary[code].value;
      if (dictionary[code].prefix == code) {
        if (dictionary) {
          std::free(dictionary);
        }
        return -1;
      }
      code = dictionary[code].prefix;
    }

    output += matchLength;
  }

  if (dictionary) {
    std::free(dictionary);
  }
  return matchLength;
}
