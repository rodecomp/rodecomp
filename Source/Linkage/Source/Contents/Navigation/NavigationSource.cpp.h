#pragma once

#include "Contents/Navigation/NavigationManager.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/World/World.h"
#include "Resource/ResourceMgr.h"
#include "Utility/Text/FormatString.h"
#include "UI/Navigation/UINavigationWnd.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <shlwapi.h>

template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) std::vector<char>::vector(const std::size_t, const char&, const std::allocator<char>&);
template __declspec(noinline) void std::vector<PathCell>::push_back(const PathCell&);
template __declspec(noinline) void std::_Destroy_range<std::allocator<NaviRouteLine>>(NaviRouteLine*, NaviRouteLine* const, std::allocator<NaviRouteLine>&) noexcept;
