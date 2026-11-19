#pragma once

#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "UI/Navigation/UIRoMapWnd.h"
#include "UI/Window/UIEnums.h"
#include "Script/LuaScript.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/Navigation/NavigationManager.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
#include "Render/Projection/ScreenGeometry.h"
#include "Render/Projection/ScreenRectangle.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Text/ColoredText.h"
#include "UI/Navigation/UINavigationWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Client/Replay/Replay.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "Resource/MSGSTRINGID.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>
#include <vector>
#include "Client/Mode/CGameMode.h"

extern template void std::string::resize(size_type, char);
template __declspec(noinline) std::vector<std::string>& std::map<std::string, std::vector<std::string>>::operator[](const std::string&);
template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
extern template std::string* std::vector<std::string>::_Ucopy<std::string*>(std::string*, std::string*, std::string*);
