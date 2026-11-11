#pragma once

#include "UI/Window/UIEnums.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Browser/WebBrowser.h"
#include "Contents/Tables/MessageTable.h"
#include "Resource/MSGSTRINGID.h"
#include "UI/Window/UIWindowMgr.h"
#include <new>

extern template std::pair<std::map<HWND, CWebBrowser*>::iterator, bool> std::map<HWND, CWebBrowser*>::insert<std::pair<HWND, CWebBrowser*>>(std::pair<HWND, CWebBrowser*>&&);
