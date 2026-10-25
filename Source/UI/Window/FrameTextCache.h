#ifndef RAGNAROK_SOURCE_UI_WINDOW_FRAMETEXTCACHE_H
#define RAGNAROK_SOURCE_UI_WINDOW_FRAMETEXTCACHE_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

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
