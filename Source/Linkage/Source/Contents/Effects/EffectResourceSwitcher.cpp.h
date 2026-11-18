#pragma once

#include <cstddef>
#include <string>
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Primitives/EffectPrimitiveState.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/World/World.h"
#include "File/File.h"
#include "Network/TextEncoding.h"
#include "Resource/World/TerrainResource.h"
#include "Resource/Json/JsonCppLoader.h"
#include <algorithm>
#include <cctype>
#include <map>
#include <new>
#include <vector>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template CEZ2STREffectMgr* Singleton<CEZ2STREffectMgr>::GetInstance();
