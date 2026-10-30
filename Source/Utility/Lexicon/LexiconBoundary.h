#ifndef RAGNAROK_SOURCE_UTILITY_LEXICON_LEXICONBOUNDARY_H
#define RAGNAROK_SOURCE_UTILITY_LEXICON_LEXICONBOUNDARY_H

#include <cstdint>

struct LexiconSerializer;

struct LexiconBoundary {
  /* 0000 */ LexiconSerializer* serializer;
  /* 0004 */ std::uint32_t flags[256];

  bool Load();
  bool Matches(unsigned char previous, unsigned char current, unsigned char options) const;
};

extern std::uint32_t g_lexiconByteClasses[256];

#endif
