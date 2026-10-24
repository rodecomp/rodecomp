#include "UI/SnapMgr.h"
#include "UI/UIWindow.h"
#include "UI/WindowSnapping.h"

// OFFSET: 0x00AAB0D0
// FUNCTION: fn_17280
void WindowSnapping::Store(UIWindow* window) {
  for (UIWindow* entry : this->excluded) {
    if (entry == window) {
      return;
    }
  }
  if (!this->enabled || !window) {
    return;
  }
  SnapRectangle& rectangle = this->rectangles[window];
  rectangle.Set(window->geometry.x, window->geometry.y, window->geometry.width, window->geometry.height);
}

// OFFSET: 0x00A914A0
// FUNCTION: fn_17161
void WindowSnapping::Constrain(UIWindow* window, int& dx, int& dy, int groupMember) {
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
  const bool root = this->graph.IsRoot(window);
  for (auto& [other, rectangle] : this->rectangles) {
    // Query order matters: connectivity calls reset and mutate visit markers.
    const bool connected = this->graph.Connected(window, other);
    const bool otherRoot = this->graph.IsRoot(other);
    const bool otherConnected = this->graph.ConnectedToRoot(other);
    if (groupMember != 0) {
      if (window == other || otherRoot || connected || otherConnected) {
        continue;
      }
    } else {
      if (window == other || (root && (otherConnected || connected))) {
        continue;
      }
    }
    rectangle.Snap(moving, x, y);
    if (x != 0 && needX) {
      needX = false;
      dx = x;
    }
    if (y != 0 && needY) {
      needY = false;
      dy = y;
    }
    if (!needX && !needY) {
      return;
    }
  }
}
