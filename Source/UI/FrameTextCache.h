#ifndef RAGNAROK_SOURCE_UI_FRAMETEXTCACHE_H
#define RAGNAROK_SOURCE_UI_FRAMETEXTCACHE_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// Original52-byte map node: links8,key32,value12. Key words are preserved
// without claiming semantics for the first two arguments to text layout.
struct FrameTextKey {
  std::uint32_t word0;
  std::uint32_t word1;
  std::string text;
  bool operator==(const FrameTextKey& other) const;
};
struct FrameTextHash {
  std::size_t operator()(const FrameTextKey& key) const;
};
using FrameTextLines = std::vector<std::vector<std::pair<int, std::string>>>;
using FrameTextCache = std::unordered_map<FrameTextKey, FrameTextLines, FrameTextHash>;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(FrameTextKey) == 32);
static_assert(sizeof(FrameTextCache) == 32);
#endif

#endif
