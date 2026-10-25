#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_WEBBROWSER_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_WEBBROWSER_H

#include <windows.h>
#include <ole2.h>
#include <exdisp.h>
#include <string>
#include <cstddef>

class BrowserClientSite;
class CMouse;
class CRenderSystemBase;
struct ServiceOptions;
struct BrowserRegistry;
struct BrowserFactory;
struct ApplicationActivation;

struct WebBrowserContext {
  HWND const& applicationWindow;
  HINSTANCE const& instance;
  const char* const& title;
  CMouse& mouse;
  CRenderSystemBase& renderSystem;
  ServiceOptions& service;
  ApplicationActivation& activation;
  BrowserFactory& popupFactory;
  const int& renderWidth;
  const int& renderHeight;
  WNDPROC hostProcedure;
  WNDPROC editProcedure;
  WNDPROC& previousEditProcedure;
};

class CWebBrowser {
public:
  explicit CWebBrowser(WebBrowserContext& context);
  ~CWebBrowser();
  CWebBrowser(const CWebBrowser&) = delete;
  CWebBrowser& operator=(const CWebBrowser&) = delete;

  bool Create(int width, int height);
  bool Close();
  bool AttachExternalEvents(BrowserRegistry& registry);
  void Resize(int width, int height);
  void Poll();
  bool Navigate(std::string address);
  void ShowAuxiliary(unsigned char value);
  bool MatchesWindow(HWND window) const;

  /* 0000 */ IWebBrowser2* browser;
  /* 0004 */ IUnknown* externalEvents;
  /* 0008 */ BrowserClientSite* site;
  /* 000C */ HWND host;
  /* 0010 */ HWND browserWindow;
  /* 0014 */ int cursorState;
  /* 0018 */ unsigned char auxiliaryVisible;
  unsigned char unknown_0019[3];
  /* 001C */ int screenWidth;
  /* 0020 */ int screenHeight;
  /* 0024 */ DWORD cookie;
  /* 0028 */ HWND rebar;
  /* 002C */ HWND combo;
  /* 0030 */ int barHeight;
  /* 0034 */ int width;
  /* 0038 */ int height;
  /* 003C */ unsigned char restoreOption;
  unsigned char unknown_003D[3];
  WebBrowserContext* context;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CWebBrowser, host) == 0xC);
static_assert(offsetof(CWebBrowser, cookie) == 0x24);
static_assert(offsetof(CWebBrowser, restoreOption) == 0x3C);
static_assert(offsetof(CWebBrowser, context) == 0x40);
#endif

#endif
