#include "UI/FrameTextCache.h"

namespace {
  std::uint32_t HashWord(std::uint32_t value) {
    std::uint32_t hash = 0x811C9DC5u;
    for (unsigned shift = 0; shift < 32; shift += 8) {
      hash = 16777619u * (hash ^ ((value >> shift) & 255));
    }
    return hash;
  }
}

// OFFSET: 0x008C4CF0
// FUNCTION: fn_11892
std::size_t FrameTextHash::operator()(const FrameTextKey& key) const {
  std::uint32_t hash = 0x811C9DC5u;
  for (unsigned char value : key.text) {
    hash = 16777619u * (hash ^ value);
  }
  hash ^= HashWord(key.word1) + 0x9E3779B9u + (hash << 6) + (hash >> 2);
  hash ^= HashWord(key.word0) + 0x9E3779B9u + (hash << 6) + (hash >> 2);
  return hash;
}

// OFFSET: 0x008C5FE0
// FUNCTION: fn_11906
bool FrameTextKey::operator==(const FrameTextKey& other) const {
  return this->text == other.text && this->word1 == other.word1 && this->word0 == other.word0;
}
