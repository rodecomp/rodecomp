#pragma once

#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Session/Session.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/MessengerMember.h"
#include "Contents/Messenger/MessengerWindows.h"
#include "UI/Party/MiniPartyGrid.h"
#include "UI/Party/MiniPartyPlacement.h"
#include "UI/Dialogs/UIMenuWnd.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Party/MiniPartyWndAreaMgr.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Login/UIChangeNameWnd.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Login/UISelCharForUServerWnd.h"
#include "UI/Login/UIWaitDBSaveWnd.h"
#include <new>

template __declspec(noinline) std::_List_node<UIWindow*, void*>* std::list<UIWindow*>::_Emplace<UIWindow* const&>(std::_List_node<UIWindow*, void*>* const, UIWindow* const&);
