#pragma once

#include <map>
#include <xtree>
#include "UI/Window/WindowId.h"
#include "UI/GameSettings/TalkType.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/GameSettingsMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Skin/SkinMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Device/DisplayMode.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "Render/Device/DeviceSelection.h"
#include "Render/Renderer.h"
#include "Render/Textures/TextureCache.h"
#include "Render/Device/RenderAdapter.h"
#include "Render/Device/RenderBackends.h"
#include "Resource/ResourcePreload.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Dialogs/UITipboxWnd.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Events/UIRoulletteWnd.h"
#include "UI/Lists/UIDropDownBox.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/StringUtil.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Resource/MSGSTRINGID.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/Obfuscate.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iterator>
#include <list>
#include <string>
#include <vector>

using OptionHotkeyTree = std::_Tree<std::_Tmap_traits<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, int>, void*>*>
OptionHotkeyTree::_Find_lower_bound<int>(const int&) const;
template __declspec(noinline) std::list<RenderAdapter>::~list() noexcept;
template __declspec(noinline) std::list<DisplayMode>::~list() noexcept;
template __declspec(noinline) std::list<RenderBackend>::~list() noexcept;
template void CUIComboBox<GameSettingsUI::CUIComboItem, std::string>::Paint();
template void CUIComboBox<GameSettingsUI::CUIComboItem, RenderBackend>::Paint();
template void CUIComboBox<GameSettingsUI::CUIComboItem, DisplayMode>::Paint();
template void CUIComboBox<GameSettingsUI::CUIComboItem, RenderAdapter>::Paint();
template unsigned int CUIComboBox<GameSettingsUI::CUIComboItem, RenderAdapter>::Selected() const;
template unsigned int CUIComboBox<GameSettingsUI::CUIComboItem, DisplayMode>::Selected() const;
template unsigned int CUIComboBox<GameSettingsUI::CUIComboItem, RenderBackend>::Selected() const;
template unsigned int CUIComboBox<GameSettingsUI::CUIComboItem, std::string>::Selected() const;
template void CUIComboBox<GameSettingsUI::CUIComboItem, RenderAdapter>::UpdateListHeight();
template void CUIComboBox<GameSettingsUI::CUIComboItem, DisplayMode>::UpdateListHeight();
template void CUIComboBox<GameSettingsUI::CUIComboItem, RenderBackend>::UpdateListHeight();
template void CUIComboBox<GameSettingsUI::CUIComboItem, std::string>::UpdateListHeight();
template int CUIComboBox<GameSettingsUI::CUIComboItem, std::string>::AddItem(const std::string& value, bool refresh);
template void CUIComboBox<GameSettingsUI::CUIComboItem, RenderAdapter>::Create(int width, int itemHeight, int visibleItems);
template void CUIComboBox<GameSettingsUI::CUIComboItem, DisplayMode>::Create(int width, int itemHeight, int visibleItems);
template void CUIComboBox<GameSettingsUI::CUIComboItem, RenderBackend>::Create(int width, int itemHeight, int visibleItems);
template void CUIComboBox<GameSettingsUI::CUIComboItem, std::string>::Create(int width, int itemHeight, int visibleItems);
