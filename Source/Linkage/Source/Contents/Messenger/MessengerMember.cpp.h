#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "Contents/Messenger/MessengerMember.h"
#include "Contents/Skin/SkinMgr.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Input/Mouse.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Contents/Tables/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Gauges/UIPcGage.h"
#include "UI/Lists/UIScrollBar.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/World/World.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Navigation/UIRoMapWnd.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Render/Text/TextContext.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/WindowBorder.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Utility/Text/FormatString.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "UI/Text/UIStaticText.h"
#include "File/ShortcutFilename.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Buttons/UIRadioBtn.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/WindowBorder.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Utility/Text/FormatString.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Client/Mode/CMode.h"
#include "Resource/MSGSTRINGID.h"
#include "UI/Party/UIPartySettingWnd.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <timeapi.h>

extern template std::string::basic_string(const std::string&);
extern template std::vector<unsigned char>::vector(std::size_t, const unsigned char&, const std::allocator<unsigned char>&);
template __declspec(noinline) std::vector<unsigned char>::vector(const std::size_t, const unsigned char&, const std::allocator<unsigned char>&);
template __declspec(noinline) std::vector<unsigned char>::vector(const std::vector<unsigned char>&);
