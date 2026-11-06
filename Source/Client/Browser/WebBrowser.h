#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_WEBBROWSER_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_WEBBROWSER_H

#include <windows.h>
#include <ole2.h>
#include <exdisp.h>
#include <string>
#include <cstddef>

class BrowserClientSite;

class CWebBrowser {
public:
  CWebBrowser();
  ~CWebBrowser();
  CWebBrowser(const CWebBrowser&) = delete;
  CWebBrowser& operator=(const CWebBrowser&) = delete;

  bool Create(HWND parent, HINSTANCE instance, int width, int height);
  bool Close();
  bool CreateInterface();
  bool CreateExternalInterface();
  bool AttachExternalEvents();
  bool CreateHostWindow(HWND parent, HINSTANCE instance, int width, int height);
  bool AttachSite();
  void DetachSite();
  bool CreateAddressBar(int width, int height);
  void Resize(int width, int height);
  void RefreshSize();
  int AuxiliaryHeight();
  void Normalize();
  void Poll();
  void SetCursorState(int value);
  void PollCursor();
  void EnterInput();
  void RestoreInput();
  bool Navigate(std::string address);
  void ShowAuxiliary(unsigned char value);
  bool MatchesWindow(HWND window) const;
  int AddressBarHeight();

  /* 0000 */ IWebBrowser2* browser;
  /* 0004 */ IUnknown* externalEvents;
  /* 0008 */ BrowserClientSite* site;
  /* 000C */ HWND host;
  /* 0010 */ HWND browserWindow;
  /* 0014 */ int cursorState;
  /* 0018 */ unsigned char auxiliaryVisible;
  /* 0019 */ unsigned char unknown_0019[3];
  /* 001C */ int screenWidth;
  /* 0020 */ int screenHeight;
  /* 0024 */ DWORD cookie;
  /* 0028 */ HWND rebar;
  /* 002C */ HWND combo;
  /* 0030 */ int barHeight;
  /* 0034 */ int width;
  /* 0038 */ int height;
  /* 003C */ unsigned char restoreOption;
  /* 003D */ unsigned char unknown_003D[3];
};

extern WNDPROC lpPrevWndFunc;

LRESULT CALLBACK BrowserHostProc(HWND window, UINT message, WPARAM value, LPARAM flags);
LRESULT CALLBACK BrowserAddressEditProc(HWND window, UINT message, WPARAM value, LPARAM flags);
HRESULT __stdcall AdviseBrowserEvents(IUnknown* source, IUnknown* sink, REFIID events, DWORD* cookie);
HRESULT __stdcall UnadviseBrowserEvents(IUnknown* source, REFIID events, DWORD cookie);

enum class BrowserCommand : WORD { Navigate = 1000 };

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CWebBrowser, host) == 0xC);
static_assert(offsetof(CWebBrowser, unknown_0019) == 0x19);
static_assert(offsetof(CWebBrowser, cookie) == 0x24);
static_assert(offsetof(CWebBrowser, restoreOption) == 0x3C);
static_assert(sizeof(CWebBrowser) == 0x40);
#endif

#endif
