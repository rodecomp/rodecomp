#pragma once

#include <cstddef>
#include <string>
#include "Contents/Movies/BinkMovies.h"
#include "Render/Textures/TextureCache.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/World/World.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Sound/SoundMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"
#include <algorithm>
#include <cmath>
#include <new>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
