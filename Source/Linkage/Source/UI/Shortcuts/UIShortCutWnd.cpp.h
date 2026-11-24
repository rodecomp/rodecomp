#pragma once

#include <string>
#include <xutility>
#include "Contents/Configuration/ServiceEnums.h"
#include "Contents/Items/ItemType.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Session/Session.h"
#include "Client/Input/Mouse.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Shortcuts/ShortcutActivation.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Shortcuts/ShortcutCooldowns.h"
#include "UI/Window/UIWindowMgr.inl"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Chat/UISubChatHisWnd.h"
#include "UI/Chat/UISubChatMiniWnd.h"
#include "UI/Navigation/UIMemorialDunWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Buttons/UIWindowCallBtnWnd.h"
#include "UI/Navigation/UIRoMapWnd.h"
#include "UI/Navigation/UINavigationWnd.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "Contents/Navigation/NavigationManager.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Client/Application/Launcher.h"
#include "Client/Replay/Replay.h"
#include "Contents/Navigation/NavigationManager.h"
#include "UI/Navigation/UINavigationWnd.h"
#include "Utility/Text/FormatString.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/PixelTextWrapping.h"
#include "Network/TextEncoding.h"
#include "File/File.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Window/UIResizer.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/CashEmotion/UICashEmotionMiniWnd.h"
#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <map>
#include <new>
#include <vector>
#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include "UI/Edit/UIEditCtrl.h"
#include <mbstring.h>

extern template std::string* std::_Move_unchecked<std::string*, std::string*>(std::string*, std::string*, std::string*);
template __declspec(noinline) char* std::allocator<char>::allocate(const std::size_t);
template __declspec(noinline) std::map<int, SubChatWndInfo>::iterator std::map<int, SubChatWndInfo>::_Mybase::find(const int&);
template __declspec(noinline) std::map<int, SubChatWndInfo>::iterator std::map<int, SubChatWndInfo>::_Mybase::erase(std::map<int, SubChatWndInfo>::iterator) noexcept;
template __declspec(noinline) std::map<int, SubChatWndInfo>::map(const std::map<int, SubChatWndInfo>&);
template __declspec(noinline) void std::map<int, SubChatWndInfo>::_Mybase::clear() noexcept;
template __declspec(noinline) std::map<int, SubChatWndInfo>::_Mybase::~_Tree() noexcept;
template __declspec(noinline) SubChatWndInfo& std::map<int, SubChatWndInfo>::operator[](const int&);
template __declspec(noinline) std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const int, SubChatWndInfo>>>, std::_Iterator_base0>& std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const int, SubChatWndInfo>>>, std::_Iterator_base0>::operator++() noexcept;
template __declspec(noinline) std::map<int, UIWindow*>::map(const std::map<int, UIWindow*>&);
template __declspec(noinline) void std::map<int, UIWindow*>::_Mybase::clear() noexcept;
