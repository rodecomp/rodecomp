#include "Client/Browser/BrowserClose.h"
#include "Client/Browser/BrowserCreation.h"
#include "Client/Browser/BrowserFrame.h"
#include "Client/Browser/BrowserInputHandoff.h"
#include "Client/Browser/BrowserInterface.h"
#include "Client/Browser/WebBrowser.h"
#include "Client/Browser/WebEventReceiver.h"

#include <new>
#include <utility>

namespace {
  // INLINED
  BrowserWindowSizing Sizing(CWebBrowser& owner) {
    return {owner.browser, owner.screenWidth, owner.screenHeight, owner.rebar, owner.auxiliaryVisible, owner.width, owner.height};
  }
}

// OFFSET: 0x00AB1DE0
// FUNCTION: fn_17409
CWebBrowser::CWebBrowser(WebBrowserContext& context)
    : browser(nullptr), externalEvents(nullptr), site(nullptr), host(nullptr), browserWindow(nullptr), cursorState(0), auxiliaryVisible(0),
      screenWidth(GetSystemMetrics(SM_CXSCREEN)), screenHeight(GetSystemMetrics(SM_CYSCREEN)), cookie(0), rebar(nullptr), combo(nullptr), barHeight(0),
      width(0), height(0), restoreOption(0), context(&context) {
  OutputDebugStringA("CWebBrowser() \n");
  BrowserInputHandoff handoff{this->restoreOption, context.applicationWindow, context.service, context.mouse, context.activation};
  handoff.Enter();
}

// OFFSET: 0x00AB1F20
// FUNCTION: fn_17410
CWebBrowser::~CWebBrowser() {
  OutputDebugStringA("~CWebBrowser() \n");
  auto& dependencies = *this->context;
  BrowserInputHandoff handoff{this->restoreOption, dependencies.applicationWindow, dependencies.service, dependencies.mouse, dependencies.activation};
  handoff.Restore();
  this->Close();
}

// OFFSET: 0x00AB22B0
bool CWebBrowser::Create(int requestedWidth, int requestedHeight) {
  auto& dependencies = *this->context;
  auto sizing = Sizing(*this);
  BrowserHostWindow window{this->host, this->browserWindow, dependencies.title, dependencies.hostProcedure};
  BrowserSiteAttachment attachment{
    sizing, this->host, this->site, this->cookie, dependencies.popupFactory, dependencies.renderWidth, dependencies.renderHeight
  };
  BrowserAddressBar addressBar{
    this->host, this->rebar, this->combo, this->barHeight, this->auxiliaryVisible, dependencies.previousEditProcedure, dependencies.editProcedure
  };
  BrowserCreation creation{this->browser, this->browserWindow, window, attachment, addressBar};

  return creation.Create(dependencies.applicationWindow, dependencies.instance, requestedWidth, requestedHeight);
}

// OFFSET: 0x00AB24D0
bool CWebBrowser::Close() {
  auto& dependencies = *this->context;
  auto sizing = Sizing(*this);
  BrowserSiteAttachment attachment{
    sizing, this->host, this->site, this->cookie, dependencies.popupFactory, dependencies.renderWidth, dependencies.renderHeight
  };
  BrowserClose operation{attachment,        this->browser,     this->externalEvents, this->host, this->browserWindow, dependencies.applicationWindow,
                         this->cursorState, dependencies.mouse};

  return operation.Close();
}

// OFFSET: 0x00AB2950
// FUNCTION: fn_45434
bool CWebBrowser::AttachExternalEvents(BrowserRegistry& registry) {
  auto* receiver = new (std::nothrow) CWebEventReceiver(registry);
  this->externalEvents = receiver;
  receiver->AddRef();

  return receiver->Register(this->browser, this->host);
}

// OFFSET: 0x00AB3060
void CWebBrowser::Resize(int requestedWidth, int requestedHeight) {
  auto sizing = Sizing(*this);
  sizing.Resize(requestedWidth, requestedHeight);
}

// OFFSET: 0x00AB2E70
void CWebBrowser::Poll() {
  auto& dependencies = *this->context;
  auto sizing = Sizing(*this);
  BrowserCursor cursor{this->cursorState, dependencies.mouse, dependencies.renderSystem, dependencies.applicationWindow};
  const bool hasExternalEvents = this->externalEvents != nullptr;
  BrowserFrame frame{sizing, cursor, this->host, this->browserWindow, hasExternalEvents};
  frame.Poll();
}

// OFFSET: 0x00AB2D50
bool CWebBrowser::Navigate(std::string address) {
  return NavigateBrowser(this->browser, this->host, this->combo, std::move(address));
}

// OFFSET: 0x00AB3120
void CWebBrowser::ShowAuxiliary(unsigned char value) {
  auto sizing = Sizing(*this);

  const bool hasExternalEvents = this->externalEvents != nullptr;
  BrowserAuxiliaryVisibility visibility{sizing, hasExternalEvents, this->host, this->auxiliaryVisible};
  visibility.Show(value);
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
