#ifndef RAGNAROK_SOURCE_UI_WINDOWMOUSEHIT_H
#define RAGNAROK_SOURCE_UI_WINDOWMOUSEHIT_H

#include <array>
#include <cstdint>
#include <deque>
#include <list>
#include <map>

class UIWindow;
class CMouse;
struct WindowAncestry;
struct WindowOrdering;
class WindowManagerWindows;
struct WindowDropDown;

// Borrowed manager rendered list, overlay map and hover UID fields. Overlay
// hits deliberately bypass the rendered-window enter/leave transition.
// Observed rendered-list rejection retries the same top window with the hit
// pointer's low DWORD as y (zero on a miss); a persistent miss does not return.
struct WindowMouseHit {
  std::list<UIWindow*>& rendered;
  std::map<int, UIWindow*>& overlays;
  std::uint32_t& previous;
  std::uint32_t& current;

  UIWindow* FindUid(std::uint32_t uid);
  UIWindow* HitTest(int x, int y);
};

// Borrowed captured-window branch of mouse polling, not a second manager.
struct WindowMouseCapture {
  WindowAncestry*& capture;
  WindowOrdering& ordering;
  const CMouse& mouse;
  const int& previousX;
  const int& previousY;
  const int& doubleClickGate;
  const int& alternateDoubleClickGate;

  bool Dispatch();
};

// Uncaptured branch and its hover handoff. CloseWindow remains the real
// manager's complete operation; this view does not invent window factories.
struct WindowMouseUncaptured {
  WindowMouseHit& hit;
  WindowOrdering& ordering;
  WindowDropDown& dropdown;
  WindowManagerWindows& windows;
  const std::deque<UIWindow*>& modal;
  const CMouse& mouse;
  const int& previousX;
  const int& previousY;
  const int& doubleClickGate;
  const int& alternateDoubleClickGate;
  UIWindow*& pressedRoot;
  UIWindow* const& dismiss26;
  UIWindow* const& dismiss260;
  WindowAncestry*& hover;
  // Manager refresh order: +3D4,+1E0,+1E4,+3FC,+3D8,+2C4,+4F170.
  std::array<UIWindow**, 7> refresh;
  const int& refreshGate;

  int Dispatch();
};

#endif
