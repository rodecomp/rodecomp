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
  /* 0000 */ LexiconNode* root;
  /* 0004 */ int wordCount;
  /* 0008 */ int nodeCount;
  /* 000C */ std::uint32_t unknown_000C;
  /* 0010 */ unsigned char* buffer;

  bool Insert(const unsigned char* word, int length);
  void Destroy(LexiconNode* node);
  std::uint32_t Serialize(LexiconNode* node, std::uint32_t offset, unsigned char terminal);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(LexiconNode) == 12);
#endif

#endif
