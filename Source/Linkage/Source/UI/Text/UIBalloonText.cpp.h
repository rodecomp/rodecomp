#pragma once

#include "Contents/Items/ItemDescription.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/ActorNameCard.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/EmblemResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Text/UIBalloonText.h"
#include "UI/Text/UINameBalloonText.h"
#include "Utility/Text/FormatString.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>
#include <utility>

template __declspec(noinline) char* std::allocator<char>::allocate(const std::size_t);
template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
