#ifndef RAGNAROK_SOURCE_UTILITY_LEXICON_LEXICONSERIALIZER_H
#define RAGNAROK_SOURCE_UTILITY_LEXICON_LEXICONSERIALIZER_H

#include <cstdint>

struct LexiconNode {
  /* 0000 */ unsigned char value;
  /* 0001 */ unsigned char terminal;
  /* 0002 */ unsigned char padding_0002[2];
  /* 0004 */ LexiconNode* sibling;
  /* 0008 */ LexiconNode* child;
};

struct LexiconHeader {
  /* 0000 */ unsigned char padding_0000[0x104];
  /* 0104 */ std::uint32_t wordCount;
  /* 0108 */ std::uint32_t nodeCount;
  /* 010C */ std::uint32_t total;
  /* 0110 */ std::uint32_t size;
  /* 0114 */ unsigned char loaded;
};

struct LexiconSerializer {
  LexiconSerializer();
  ~LexiconSerializer();

  /* 0000 */ LexiconNode* root;
  /* 0004 */ int wordCount;
  /* 0008 */ int nodeCount;
  /* 000C */ std::uint32_t serializedSize;
  /* 0010 */ unsigned char* buffer;
  /* 0014 */ LexiconHeader header;

  bool Load(const char* path);
  bool Insert(const unsigned char* word, int length);
  void Destroy(LexiconNode* node);
  unsigned char UnbranchedLength(const LexiconNode* node) const;
  int SiblingCount(const LexiconNode* node) const;
  std::uint32_t Serialize(LexiconNode* node, std::uint32_t offset, unsigned char terminal);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(LexiconNode) == 12);
static_assert(sizeof(LexiconHeader) == 0x118);
static_assert(sizeof(LexiconSerializer) == 300);
#endif

#endif
