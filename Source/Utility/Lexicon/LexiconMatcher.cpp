#include "Utility/Lexicon/LexiconMatcher.h"

// OFFSET: 0x00AC48E0
// FUNCTION: fn_17631
int __stdcall MatchLexicon(const unsigned char* data, int, const unsigned char* text, const unsigned char* end, int) {
  bool failed = false;
  int matched = 0;
  int accepted = 0;
  if (*text == 0) {
    return 1000;
  }

  const unsigned char* entry = data;
  int offset = 0;
  unsigned char* offsetBytes = reinterpret_cast<unsigned char*>(&offset);
  while (*text != 0) {
    if (text >= end || failed) {
      break;
    }

    if ((*entry & 0x40) != 0) {
      return matched;
    }

    offsetBytes[2] = entry[0] & 0x3F;
    offsetBytes[1] = entry[1];
    offsetBytes[0] = entry[2];
    entry += 3;
    const unsigned char* node = data + offset;
    const int count = *node;
    if (count < 0x20 && node[1] == *text) {
      ++offset;
      for (int index = 0; index < count; ++index) {
        if (data[offset++] != *text) {
          failed = true;
          break;
        }

        ++matched;
        ++text;
        node = data + offset;
        if (*node < 0x20) {
          if ((*node & 2) != 0) {
            accepted = matched;
          }

          if ((*node & 4) != 0) {
            entry = node + 1;
            break;
          }

          ++offset;
        }
      }
    } else if (*node >= *text) {
      if (*node != *text) {
        return accepted;
      }

      ++matched;
      entry = node + 1;
      ++text;
      if ((*entry & 0x80) != 0) {
        accepted = matched;
      }
    }
  }

  return accepted;
}
