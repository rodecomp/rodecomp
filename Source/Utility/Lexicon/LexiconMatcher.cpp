#include "Utility/Lexicon/LexiconMatcher.h"

// OFFSET: 0x00AC48E0
// FUNCTION: fn_17631
int __stdcall MatchLexicon(const unsigned char* data, int, const unsigned char* text, const unsigned char* end, int) {
  if (*text == 0) {
    return 1000;
  }

  int accepted = 0;
  int matched = 0;
  bool failed = false;
  const unsigned char* entry = data;
  while (text < end && !failed) {
    if ((*entry & 0x40) != 0) {
      return matched;
    }

    int offset = ((entry[0] & 0x3F) << 16) | (entry[1] << 8) | entry[2];
    entry += 3;
    const unsigned char* candidate = data + offset;
    const int count = *candidate;
    if (count < 0x20 && candidate[1] == *text) {
      ++offset;
      for (int index = 0; index < count; ++index) {
        if (data[offset++] != *text) {
          failed = true;
          break;
        }

        ++matched;
        ++text;
        const unsigned char marker = data[offset];
        if (marker < 0x20) {
          if ((marker & 2) != 0) {
            accepted = matched;
          }

          if ((marker & 4) != 0) {
            entry = data + offset + 1;
            break;
          }

          ++offset;
        }
      }
    } else if (count >= *text) {
      if (count != *text) {
        return accepted;
      }

      ++matched;
      entry = candidate + 1;
      ++text;
      if ((*entry & 0x80) != 0) {
        accepted = matched;
      }
    }

    if (*text == 0) {
      return accepted;
    }
  }

  return accepted;
}
