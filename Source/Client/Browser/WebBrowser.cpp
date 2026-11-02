#include "Client/Application/ApplicationActivation.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Browser/BrowserClientSite.h"
#include "UI/Window/UIWindowMgr.h"
#include "Client/Browser/WebBrowser.h"
#include "Client/Browser/WebEventReceiver.h"
#include "Client/Input/Mouse.h"
#include "Contents/Configuration/ServiceOptions.h"

#include <new>
#include <utility>

// OFFSET: 0x00AB1DE0
// FUNCTION: fn_17409
CWebBrowser::CWebBrowser()
    : browser(nullptr), externalEvents(nullptr), site(nullptr), host(nullptr), browserWindow(nullptr), cursorState(0), auxiliaryVisible(0),
      screenWidth(GetSystemMetrics(SM_CXSCREEN)), screenHeight(GetSystemMetrics(SM_CYSCREEN)), cookie(0), rebar(nullptr), combo(nullptr), barHeight(0),
      width(0), height(0), restoreOption(0) {
  OutputDebugStringA("CWebBrowser() \n");
  if (g_serviceOptions.Get(ServiceOption::ConfineCursor) == 0) {
    return;
  }

  POINT position{g_mouse.x, g_mouse.y};
  g_serviceOptions.Set(ServiceOption::ConfineCursor, 0);
  g_mouse.SetX(position.x);
  g_mouse.SetY(position.y);
  this->restoreOption = 1;
  ClientToScreen(hWnd, &position);
  SetCursorPos(position.x, position.y);
}

// OFFSET: 0x00AB1F20
// FUNCTION: fn_17410
CWebBrowser::~CWebBrowser() {
  OutputDebugStringA("~CWebBrowser() \n");
  if (this->restoreOption != 0) {
    POINT position;
    GetCursorPos(&position);
    ScreenToClient(hWnd, &position);
    g_serviceOptions.Set(ServiceOption::ConfineCursor, 1);
    g_mouse.SetX(position.x);
    g_mouse.SetY(position.y);
    SetActiveWindow(hWnd);
    SetApplicationActive(1);
  }

  if (this->cursorState != 0) {
    SetCursor(nullptr);
    g_mouse.Reset();
    ShowCursor(FALSE);
    this->cursorState = 0;
  }

  if (this->site) {
    IOleObject* object;
    if (SUCCEEDED(this->browser->QueryInterface(IID_IOleObject, reinterpret_cast<void**>(&object)))) {
      UnadviseBrowserEvents(object, DIID_DWebBrowserEvents2, this->cookie);
      object->Release();
    }

    delete this->site;
    this->site = nullptr;
  }

  if (this->externalEvents) {
    this->externalEvents->Release();
    this->externalEvents = nullptr;
  }

  if (this->browser) {
    this->browser->Stop();
    SetWindowLongA(this->browserWindow, GWL_STYLE, WS_CHILD);
    SetParent(this->browserWindow, hWnd);
    this->browser->Quit();
    this->browser->Release();
    this->browser = nullptr;
  }

  if (this->host) {
    DestroyWindow(this->host);
    this->host = nullptr;
  }
}

// OFFSET: 0x00AB2950
// FUNCTION: fn_45434
bool CWebBrowser::AttachExternalEvents() {
  auto* receiver = new (std::nothrow) CWebEventReceiver();
  this->externalEvents = receiver;
  receiver->AddRef();

  return receiver->Register(this->browser, this->host);
}

// OFFSET: 0x00AB3030
// FUNCTION: fn_17420
bool CWebBrowser::MatchesWindow(HWND window) const {
  return this->host == window || this->host == GetParent(window);
}

// OFFSET: 0x00AB26D0
// FUNCTION: fn_17415
int CWebBrowser::AddressBarHeight() {
  if (this->rebar && this->auxiliaryVisible) {
    RECT rectangle;
    GetClientRect(this->rebar, &rectangle);
    return rectangle.bottom - rectangle.top + 4;
  }

  return 0;
}
