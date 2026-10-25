#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERINTERFACE_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERINTERFACE_H

#include <windows.h>
#include <ole2.h>
#include <exdisp.h>
#include <string>

bool CreateBrowserInterface(IWebBrowser2*& browser, HWND& window);
bool CreateExternalBrowserInterface(IWebBrowser2*& browser, HWND& window, HWND const& host);
bool NavigateBrowser(IWebBrowser2* const& browser, HWND const& host, HWND const& title, std::string address);
HRESULT AdviseBrowserEvents(IUnknown* source, IUnknown* sink, REFIID events, DWORD* cookie);
HRESULT UnadviseBrowserEvents(IUnknown* source, REFIID events, DWORD cookie);

#endif
