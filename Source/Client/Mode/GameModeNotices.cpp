#include "Client/Mode/CGameMode.h"
#include "Contents/MessageTable.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>

// OFFSET: 0x00D6D200
// FUNCTION: fn_22087
void CGameMode::ReceiveResultNotice(int result) {
  switch (result) {
  case 0:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4125)), 0xFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4126)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4127)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6E100
// FUNCTION: fn_22096
void CGameMode::RefreshWindow16(const unsigned char*) {
  UIWindow* window = g_windowMgr.LookupWindow(16);
  if (window) {
    window->SendMsg(nullptr, 75, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D6E9F0
// FUNCTION: fn_22110
void CGameMode::ReceiveWindow154Close(std::int16_t value) {
  if (value == 0) {
    UIWindow* window = g_windowMgr.LookupWindow(340);
    if (window) {
      window->SendMsg(nullptr, 64, 2, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D6EB00
// FUNCTION: fn_22113
void CGameMode::MoveWindow16(int x, int y) {
  UIWindow* window = g_windowMgr.LookupWindow(16);
  if (window) {
    window->Move(x, y);
  }
}

// OFFSET: 0x00D6EB20
// FUNCTION: fn_22114
void CGameMode::ResizeWindow16(int width, int height) {
  UIWindow* window = g_windowMgr.LookupWindow(16);
  if (window) {
    window->Resize(width, height);
  }
}

// OFFSET: 0x00D6EC10
// FUNCTION: fn_22118
void CGameMode::ReceiveNotice1950(const unsigned char*) {
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1950)), 0xFF, 0, 0, 0);
}
