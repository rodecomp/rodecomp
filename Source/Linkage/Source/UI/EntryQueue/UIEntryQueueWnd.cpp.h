#pragma once

#include <cstddef>
#include <string>
#include "UI/EntryQueue/UIEntryQueueWnd.h"
#include "Contents/Skin/SkinMgr.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Guild/GuildInfo.h"
#include "Contents/Guild/GuildMgr.h"
#include "Contents/EntryQueue/EntryQueueMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Render/Renderer.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Resource/MSGSTRINGID.h"
#include <cstdio>
#include <cstring>
#include <map>
#include <utility>
#include <windows.h>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
