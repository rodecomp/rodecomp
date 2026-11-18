#pragma once

#include "Contents/Map/MapSkyMgr.h"
#include "UI/GameSettings/TalkType.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Effects/Skills/CloudEffect.h"
#include "Contents/World/World.h"
#include "File/MapName.h"
#include "Render/Renderer.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/AffineMatrix.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <new>

template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
