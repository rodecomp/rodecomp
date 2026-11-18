#pragma once

#include "UI/Chat/UINewChatWnd.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Input/Mouse.h"
#include "Client/Mode/CMode.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/TextEncoding.h"
#include "Render/Renderer.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Chat/UISubChatHisWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Navigation/NavigationLink.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Text/TextWrapping.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
#include <windows.h>
#include <mbstring.h>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>

template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) std::string std::string::substr(std::string::size_type, std::string::size_type) const;
template __declspec(noinline) std::string* std::_Move_unchecked<std::string*, std::string*>(std::string*, std::string*, std::string*);
template __declspec(noinline) std::map<int, UIWindow*>::map(const std::map<int, UIWindow*>&);
template __declspec(noinline) void std::map<int, UIWindow*>::_Mybase::clear() noexcept;
template __declspec(noinline) std::_Tree_unchecked_const_iterator<std::map<int, UIWindow*>::_Mybase::_Scary_val>& std::_Tree_unchecked_const_iterator<std::map<int, UIWindow*>::_Mybase::_Scary_val>::operator++() noexcept;
