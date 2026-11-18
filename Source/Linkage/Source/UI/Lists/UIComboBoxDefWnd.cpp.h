#pragma once

#include <cstddef>
#include <string>
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Replay/Replay.h"
#include "UI/Window/WindowPolicy.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Items/UIItemStoreComboBox.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Lists/UIComboBoxWnd.h"
#include <cstdio>
#include "UI/Text/UITextViewer.h"
#include "UI/Text/UIString.h"
#include "UI/Login/UIDescriptRace.h"
#include "Contents/Tables/MessageTable.h"
#include "Client/Input/InputCharset.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Sprites/SpriteRidingOffset.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/Sprites/ImfResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Resource/MSGSTRINGID.h"
#include <timeapi.h>

extern template std::size_t std::_Traits_find<std::char_traits<char>>(const char* const, const std::size_t, const std::size_t, const char* const, const std::size_t) noexcept;
extern template std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
template __declspec(noinline) std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
