#pragma once

#include <cstddef>
#include <string>
#include "Contents/Navigation/NavigationManager.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/World/World.h"
#include "Client/Session/Session.h"
#include "Resource/ResourceMgr.h"
#include <cstdio>
#include <cstring>
#include <windows.h>
#include <shlwapi.h>

extern template bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
