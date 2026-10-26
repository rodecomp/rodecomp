#ifndef RAGNAROK_SOURCE_UTILITY_LEXICON_LEXICONBOUNDARY_H
#define RAGNAROK_SOURCE_UTILITY_LEXICON_LEXICONBOUNDARY_H

#include <cstdint>

struct LexiconBoundary {
  /* 0000 */ std::uint32_t unknown_0000;
  /* 0004 */ std::uint32_t flags[256];

  bool Matches(unsigned char previous, unsigned char current, unsigned char options) const;
};

#endif
