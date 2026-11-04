#include "UI/Window/FrameTextCache.h"

// OFFSET: 0x008C4CF0
// FUNCTION: fn_11892
std::size_t FrameTextHash::operator()(const FrameTextKey& key) const {
  std::size_t hash = std::hash<std::string>()(key.text);
  hash ^= std::hash<std::uint32_t>()(key.word1) + 0x9E3779B9u + (hash << 6) + (hash >> 2);
  hash ^= std::hash<std::uint32_t>()(key.word0) + 0x9E3779B9u + (hash << 6) + (hash >> 2);
  return hash;
}

// OFFSET: 0x008C5FE0
// FUNCTION: fn_11906
bool FrameTextKey::operator==(const FrameTextKey& other) const {
  return this->text == other.text && this->word1 == other.word1 && this->word0 == other.word0;
}
