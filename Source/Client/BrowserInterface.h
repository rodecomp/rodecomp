#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERINTERFACE_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERINTERFACE_H

// clang-format off
#include <windows.h>
#include <ole2.h>
#include <exdisp.h>
// clang-format on

#include <string>

// Caller supplies the original owner fields and controls COM initialization
// and release. Successful creation publishes HWND only when browser is nonnull;
// get_HWND failure does not change the creation result.
bool CreateBrowserInterface(IWebBrowser2*& browser, HWND& window);

// Distinct original local-server route; never falls back to the in-process
// control. Success reparents its HWND and clears Top, Left and StatusBar.
bool CreateExternalBrowserInterface(IWebBrowser2*& browser, HWND& window, HWND const& host);

// Navigation's four optional arguments alias one VT_EMPTY VARIANT. Host show,
// browser visibility and title publication occur even when Navigate fails.
bool NavigateBrowser(IWebBrowser2* const& browser, HWND const& host, HWND const& title, std::string address);

// Connection-point helpers retain native HRESULTs; cookie writes belong to
// Advise. Release the connection point before its container on every path.
HRESULT AdviseBrowserEvents(IUnknown* source, IUnknown* sink, REFIID events, DWORD* cookie);
HRESULT UnadviseBrowserEvents(IUnknown* source, REFIID events, DWORD cookie);

#endif
