#include "UI/Layout/SnapMgr.h"
#include "UI/GameSettings/TalkType.h"
#include "UI/Window/UIWindow.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00AAB0D0
// FUNCTION: fn_17280
void UIWindowMgr::StoreSnapRectangle(UIWindow* window) {
  const auto entryEnd = this->snapExcluded.end();
  for (auto entryIt = this->snapExcluded.begin(); entryIt != entryEnd; ++entryIt) {
    UIWindow* entry = *entryIt;
    if (entry == window) {
      return;
    }
  }

  if (!g_serviceOptions.Get(TT_WINDOW_SNAP_ON_OFF) || !window) {
    return;
  }

  SnapRectangle& rectangle = this->snapRectangles[window];
  rectangle.Set(window->geometry.x, window->geometry.y, window->geometry.width, window->geometry.height);
}

// OFFSET: 0x00A914A0
// FUNCTION: fn_17161
void UIWindowMgr::ConstrainSnap(UIWindow* window, int& dx, int& dy, int groupMember) {
  bool needX = true, needY = true;
  dx = 0;
  dy = 0;
  int x = 0, y = 0;
  SnapRectangle moving;
  moving.Set(window->geometry.x, window->geometry.y, window->geometry.width, window->geometry.height);
  SnapRectangle screen;
  screen.Set(0, 0, this->screenWidth, this->screenHeight);
  screen.ReverseEdges();
  screen.Snap(moving, x, y);

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
    SnapRectangle& rectangle = it->second;
    const bool connected = g_snapMgr.Connected(window, it->first);
    const bool otherRoot = g_snapMgr.IsRoot(it->first);
    UIWindow* const other = it->first;
    const bool otherConnected = g_snapMgr.ConnectedToRoot(other);

    const bool skip = groupMember != 0 ? window == other || otherRoot || connected || otherConnected
                                       : window == other || (root && (otherConnected || connected));
    if (skip) {
      continue;
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
