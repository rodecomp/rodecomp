#include "UI/Layout/SnapMgr.h"
#include "UI/Window/UIWindow.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00AAB0D0
// FUNCTION: fn_17280
void UIWindowMgr::StoreSnapRectangle(UIWindow* window) {
  for (UIWindow* entry : this->snapExcluded) {
    if (entry == window) {
      return;
    }
  }

  if (!g_serviceOptions.Get(141) || !window) {
    return;
  }

  SnapRectangle& rectangle = this->snapRectangles[window];
  rectangle.Set(window->geometry.x, window->geometry.y, window->geometry.width, window->geometry.height);
}

// OFFSET: 0x00A914A0
// FUNCTION: fn_17161
void UIWindowMgr::ConstrainSnap(UIWindow* window, int& dx, int& dy, int groupMember) {
  dx = 0;
  dy = 0;
  SnapRectangle moving;
  moving.Set(window->geometry.x, window->geometry.y, window->geometry.width, window->geometry.height);
  SnapRectangle screen;
  screen.Set(0, 0, this->screenWidth, this->screenHeight);
  screen.ReverseEdges();
  int x = 0, y = 0;
  screen.Snap(moving, x, y);
  bool needX = true, needY = true;

  if (x != 0) {
    needX = false;
    dx = x;
  }

  if (y != 0) {
    needY = false;
    dy = y;
  }

  if (!needX && !needY) {
    return;
  }

  int snapX = 0, snapY = 0;
  const bool root = g_snapMgr.IsRoot(window);

  for (auto it = this->snapRectangles.begin(); it != this->snapRectangles.end(); ++it) {
    auto& [other, rectangle] = *it;
    const bool connected = g_snapMgr.Connected(window, other);
    const bool otherRoot = g_snapMgr.IsRoot(other);
    const bool otherConnected = g_snapMgr.ConnectedToRoot(other);

    if (groupMember != 0) {
      if (window == other || otherRoot || connected || otherConnected) {
        continue;
      }
    } else {
      if (window == other || (root && (otherConnected || connected))) {
        continue;
      }
    }

    rectangle.Snap(moving, snapX, snapY);
    if (snapX != 0 && needX) {
      needX = false;
      dx = snapX;
    }

    if (snapY != 0 && needY) {
      needY = false;
      dy = snapY;
    }

    if (!needX && !needY) {
      return;
    }
  }
}
