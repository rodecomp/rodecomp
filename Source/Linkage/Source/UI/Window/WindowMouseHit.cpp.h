#pragma once

#include "UI/Window/WindowId.h"
#include "Client/Session/Session.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Browser/WebBrowser.h"
#include "Client/Input/Mouse.h"
#include "Client/Replay/Replay.h"
#include "Contents/Messenger/MessengerMember.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Lists/UIDropDownBox.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Layout/WindowCoordinates.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/CashEmotion/UICashEmotionListWnd.h"
#include "UI/Dialogs/UIMessageBox.h"
#include "UI/Guild/UIGuildWnd.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Items/UISecondCostumeWnd.h"
#include "UI/Lists/UIComboBoxWnd.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Login/UISelCharForUServerWnd.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Shop/UInCash_CallWnd.h"
#include "UI/Skill/UINewSkillListWnd.h"
#include <algorithm>

template __declspec(noinline) std::_List_node<UIWindow*, void*>* std::list<UIWindow*>::_Emplace<UIWindow* const&>(std::_List_node<UIWindow*, void*>* const, UIWindow* const&);

template __declspec(noinline) std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<HWND const, CWebBrowser*>>>, std::_Iterator_base0>&
std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<HWND const, CWebBrowser*>>>, std::_Iterator_base0>::operator++() noexcept;
