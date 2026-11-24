#pragma once

#include <cstddef>
#include <string>
#include "Contents/Navigation/NavigationManager.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/Tables/MessageTable.h"
#include "Utility/Text/FormatString.h"
#include "Client/Application/Registry.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Resource/ResourceMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Window/UIWindow.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Navigation/UINavigationWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Navigation/UIRoMapWnd.h"
#include "UI/CashShop/UIPayConfirmWnd.h"
#include "Client/Application/RegistryKey.h"
#include <cstdint>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <timeapi.h>
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/World/World.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"
#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"

extern template bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
extern template std::string& std::string::operator=(std::string&&) noexcept;
template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) void std::vector<NavigationTarget>::push_back(const NavigationTarget&);
template __forceinline void std::_Destroy_range(NavigationTarget*, NavigationTarget* const, std::allocator<NavigationTarget>&) noexcept;
