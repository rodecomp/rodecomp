#include "Utility/Lexicon/LexiconBoundary.h"

// OFFSET: 0x00AC4EA0
// FUNCTION: fn_17634
bool LexiconBoundary::Matches(unsigned char previous, unsigned char current, unsigned char options) const {
  if (previous == 0xDD) {
    return true;
  }

  if ((this->flags[previous] & 1) != 0 && (this->flags[current] & 0x2100) != 0) {
    return true;
  }

  if (current == 32) {
    return true;
  }

  if (previous == 32) {
    return false;
  }

  const std::uint32_t before = this->flags[previous];
  const std::uint32_t after = this->flags[current];
  if ((options & 2) != 0) {
    return ((before & 0x470) != 0) == ((after & 0x470) != 0) &&
           (((before & 0x1000) != 0 && (after & 1) != 0) || ((before & 0x470) != 0 && (after & 0x470) != 0) ||
            ((before & 2) != 0 && (after & 2) != 0));
  }

  if ((after & 0x10) != 0 && (before & 0x10) == 0) {
    return false;
  }

  if ((after & 0x60) != 0 || (before & 0x10) != 0 || ((before & 0x1000) != 0 && (after & 1) != 0)) {
    return true;
  }

  return ((before & 0x400) != 0 && (after & 0x10002) != 0) || (after & 0x400) != 0;
}
