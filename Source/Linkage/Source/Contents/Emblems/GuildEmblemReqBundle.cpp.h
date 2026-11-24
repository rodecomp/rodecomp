#pragma once

#include "Contents/Emblems/GuildEmblemReqBundle.h"
#include <deque>

template __forceinline void std::deque<CGuildEmblemReqBundle::Recent>::push_back(CGuildEmblemReqBundle::Recent&&);
template __forceinline void std::deque<CGuildEmblemReqBundle::Recent>::_Emplace_back_internal(CGuildEmblemReqBundle::Recent&&);
