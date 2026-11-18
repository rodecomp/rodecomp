#pragma once

#include "UI/Cooldowns/CooldownState.h"
#include "Client/Replay/Replay.h"
#include "UI/Cooldowns/CooldownClock.h"
#include <cmath>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

template __declspec(noinline) std::_Tree<std::_Tmap_traits<unsigned, CooldownEdge, std::less<unsigned>, std::allocator<std::pair<const unsigned, CooldownEdge>>, false>>::~_Tree() noexcept;
