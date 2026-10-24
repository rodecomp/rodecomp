#include "Client/BrowserAddressBar.h"

#include <commctrl.h>

// OFFSET: 0x00AB27A0
// FUNCTION: fn_17416
bool BrowserAddressBar::Create(int width, int requestedHeight) {
  this->rebar = CreateWindowExA(
    0x80, "ReBarWindow32", "rebar", 0x56809648, 0, 0, width, 0, this->host, nullptr, GetModuleHandleA(nullptr), nullptr
  );
  if (!this->rebar) {
    return false;
  }

  REBARINFO bar{};
  bar.cbSize = sizeof(bar);
  if (!SendMessageA(this->rebar, RB_SETBARINFO, 0, reinterpret_cast<LPARAM>(&bar))) {
    return false;
  }

  HWND combo = CreateWindowExA(
    0x200, "COMBOBOX", "addressbar", 0x50000002, 10, 10, 160, 22, this->rebar, nullptr, GetModuleHandleA(nullptr),
    nullptr
  );

  REBARBANDINFOA band{};
  band.cbSize = sizeof(band);
  band.fMask = 0xF1;
  band.fStyle = 0x24;
  band.hwndChild = combo;
  band.cxMinChild = 0;
  band.cyMinChild = requestedHeight;
  band.cx = 160;
  band.hbmBack = LoadBitmapA(GetModuleHandleA(nullptr), nullptr);
  if (!SendMessageA(this->rebar, RB_INSERTBANDA, static_cast<WPARAM>(-1), reinterpret_cast<LPARAM>(&band))) {
    return false;
  }

  this->combo = combo;
  HWND edit = GetWindow(combo, GW_CHILD);
  this->previous =
    reinterpret_cast<WNDPROC>(SetWindowLongPtrA(edit, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(this->procedure)));

  RECT rectangle;
  GetClientRect(this->host, &rectangle);
  SetWindowPos(
    this->rebar, nullptr, 0, 0, rectangle.right - rectangle.left, rectangle.bottom - rectangle.top, SWP_NOMOVE
  );
  ShowWindow(this->rebar, SW_HIDE);

  this->height = 0;
  if (this->rebar && this->visible != 0) {
    GetClientRect(this->rebar, &rectangle);
    this->height = rectangle.bottom - rectangle.top + 4;
  }

  return true;
}
