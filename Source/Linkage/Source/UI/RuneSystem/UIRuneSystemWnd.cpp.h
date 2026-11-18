#pragma once

#include <cstddef>
#include <string>
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/RuneSystem/RuneSystem.h"
#include "Contents/Tables/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/RuneSystem/UIRuneSystemWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Client/Mode/CMode.h"
#include "Network/Packets/PacketId.h"
#include "Resource/MSGSTRINGID.h"

extern template bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
