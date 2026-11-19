#pragma once

#include "UI/Chat/UINewChatWnd.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Navigation/NavigationManager.h"
#include "UI/Dialogs/UITipboxWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Contents/Items/ItemInfo.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Contents/Items/ItemNaming.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Navigation/NavigationLink.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Text/TextWrapping.h"
#include "Render/Text/TextContext.h"
#include "UI/Text/CreateLineInfo.h"
#include <cstring>
#include <bit>
#include <cstdlib>
#include <shellapi.h>

template __declspec(noinline) std::string::~basic_string() noexcept;

template __declspec(noinline) std::string::basic_string(const char* const);
template __declspec(noinline) std::string::basic_string(const std::string&);
template __declspec(noinline) std::string& std::string::operator=(const std::string&);
template __declspec(noinline) std::string& std::string::operator=(std::string&&) noexcept;
