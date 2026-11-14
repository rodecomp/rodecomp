#include "UI/Window/FrameTextCache.h"

// OFFSET: 0x008C4CF0
// FUNCTION: fn_11892
std::size_t FrameTextHash::operator()(const FrameTextKey& key) const {
  std::size_t hash = std::hash<std::string>()(key.text);
  hash ^= std::hash<std::uint32_t>()(key.word1) + 0x9E3779B9u + (hash << 6) + (hash >> 2);
  hash ^= std::hash<std::uint32_t>()(key.word0) + 0x9E3779B9u + (hash << 6) + (hash >> 2);
  return hash;
}

// INLINED
bool FrameTextKey::operator==(const FrameTextKey& other) const {
  return this->text == other.text && this->word1 == other.word1 && this->word0 == other.word0;
}

// OFFSET: 0x008C5FE0
// FUNCTION: fn_11906
template <> template <> std::_Hash_find_last_result<FrameTextCache::_Nodeptr>
FrameTextCache::_Mybase::_Find_last<FrameTextKey>(const FrameTextKey& key, const std::size_t hash) const {
  const size_type bucket = hash & this->_Mask;
  _Nodeptr where = this->_Vec._Mypair._Myval2._Myfirst[(bucket << 1) + 1]._Ptr;
  const _Nodeptr end = this->_List._Mypair._Myval2._Myhead;
  if (where == end) {
    return {end, _Nodeptr{}};
  }

  const _Nodeptr first = this->_Vec._Mypair._Myval2._Myfirst[bucket << 1]._Ptr;
  for (;;) {
    if (!this->_Traitsobj(key, where->_Myval.first)) {
      return {where->_Next, where};
    }
    if (where == first) {
      return {where, _Nodeptr{}};
    }
    where = where->_Prev;
  }
}
