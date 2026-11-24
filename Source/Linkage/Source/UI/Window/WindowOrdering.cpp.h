#pragma once

#include "UI/Events/UIRoulletteWnd.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Clan/UIClanWnd.h"
#include "UI/Guild/UIGuildWnd.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include <algorithm>

template __declspec(noinline) std::_Tree_find_result<std::map<UIWindow*, SnapRectangle>::_Mybase::_Nodeptr> std::map<UIWindow*, SnapRectangle>::_Mybase::_Find_lower_bound<UIWindow*>(UIWindow* const&) const;
template __declspec(noinline) void std::map<UIWindow*, SnapRectangle>::_Mybase::clear() noexcept;
template __declspec(noinline) std::_List_node<UIWindow*, void*>* std::list<UIWindow*>::_Emplace<UIWindow* const&>(std::_List_node<UIWindow*, void*>* const, UIWindow* const&);
