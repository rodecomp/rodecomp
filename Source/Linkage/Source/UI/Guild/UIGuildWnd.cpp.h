#pragma once

#include "Render/DeviceContext/DCSurface.h"
#include "Contents/Skin/SkinMgr.h"
#include "UI/Window/WindowId.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Guild/UIGuildWnd.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Guild/GuildMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "File/ShortcutFilename.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/Bitmap/EmblemResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Guild/UIAgitWnd.h"
#include "UI/Guild/UIGuildHelperWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"
#include <windows.h>
#include <timeapi.h>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include "Contents/Guild/GuildInfo.h"
#include "Resource/MSGSTRINGID.h"

template __declspec(noinline) std::string& std::string::operator=(const char* const);
template __declspec(noinline) std::vector<MiniPartyInfoEntry>::iterator std::vector<MiniPartyInfoEntry>::erase(const_iterator, const_iterator) noexcept;
