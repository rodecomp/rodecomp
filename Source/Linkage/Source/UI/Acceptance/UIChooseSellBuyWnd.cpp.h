#pragma once

#include "UI/Window/FrameTextCache.h"

template <> template <> std::_Hash_find_last_result<FrameTextCache::_Nodeptr>
FrameTextCache::_Mybase::_Find_last<FrameTextKey>(const FrameTextKey& key, const std::size_t hash) const;
