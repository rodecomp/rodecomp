#ifndef RAGNAROK_SOURCE_UI_WINDOW_FRAMETEXTCACHE_H
#define RAGNAROK_SOURCE_UI_WINDOW_FRAMETEXTCACHE_H

#include <cstddef>
#include <cstring>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct FrameTextKeyView {
  /* 0000 */ std::uint32_t word0;
  /* 0004 */ std::uint32_t word1;
  /* 0008 */ const char* text;
};

struct FrameTextKey {
  /* 0000 */ std::uint32_t word0;
  /* 0004 */ std::uint32_t word1;
  /* 0008 */ std::string text;
  FrameTextKey(const FrameTextKeyView& key) : word0(key.word0), word1(key.word1), text(key.text, std::strlen(key.text)) {}
  bool operator==(const FrameTextKey& other) const;
};
struct FrameTextHash {
  std::size_t operator()(const FrameTextKey& key) const;
};
using FrameTextLines = std::vector<std::vector<std::pair<int, std::string>>>;
using FrameTextCache = std::unordered_map<FrameTextKey, FrameTextLines, FrameTextHash>;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(FrameTextKey) == 32);
static_assert(offsetof(FrameTextKey, word0) == 0x0000);
static_assert(offsetof(FrameTextKey, word1) == 0x0004);
static_assert(offsetof(FrameTextKey, text) == 0x0008);
static_assert(sizeof(FrameTextCache) == 32);
#endif

#endif
