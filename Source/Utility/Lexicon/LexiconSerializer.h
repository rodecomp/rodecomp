#ifndef RAGNAROK_SOURCE_UTILITY_LEXICON_LEXICONSERIALIZER_H
#define RAGNAROK_SOURCE_UTILITY_LEXICON_LEXICONSERIALIZER_H

#include <cstdint>

struct LexiconNode {
  /* 0000 */ unsigned char value;
  /* 0001 */ unsigned char terminal;
  /* 0002 */ unsigned char unknown_0002[2];
  /* 0004 */ LexiconNode* sibling;
  /* 0008 */ LexiconNode* child;
};

struct LexiconSerializer {
  /* 0000 */ std::uint32_t unknown_0000[4];
  /* 0010 */ unsigned char* buffer;

  void Destroy(LexiconNode* node);
  std::uint32_t Serialize(LexiconNode* node, std::uint32_t offset, unsigned char terminal);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(LexiconNode) == 12);
#endif

#endif
