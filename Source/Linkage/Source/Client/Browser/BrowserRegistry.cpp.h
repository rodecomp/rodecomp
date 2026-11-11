#pragma once

#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Browser/WebBrowser.h"
#include "UI/Window/UIWindowMgr.h"
#include <new>

using BrowserTree = std::_Tree<std::_Tmap_traits<HWND, CWebBrowser*, std::less<HWND>, std::allocator<std::pair<HWND const, CWebBrowser*>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<HWND const, CWebBrowser*>, void*>*>
BrowserTree::_Find_lower_bound<HWND>(HWND const&) const;
extern template std::pair<std::map<HWND, CWebBrowser*>::iterator, bool> std::map<HWND, CWebBrowser*>::insert<std::pair<HWND, CWebBrowser*>>(std::pair<HWND, CWebBrowser*>&&);
