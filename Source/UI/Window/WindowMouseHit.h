#ifndef RAGNAROK_SOURCE_UI_WINDOW_WINDOWMOUSEHIT_H
#define RAGNAROK_SOURCE_UI_WINDOW_WINDOWMOUSEHIT_H

#include <array>
#include <cstdint>
#include <deque>
#include <list>
#include <map>

class UIWindow;
class CMouse;
struct WindowOrdering;
class WindowManagerWindows;
struct WindowDropDown;

struct WindowMouseHit {
  std::list<UIWindow*>& rendered;
  std::map<int, UIWindow*>& overlays;
  std::uint32_t& previous;
  std::uint32_t& current;

  UIWindow* FindUid(std::uint32_t uid);
  UIWindow* HitTest(int x, int y);
};

struct WindowMouseCapture {
  UIWindow*& capture;
  WindowOrdering& ordering;
  const CMouse& mouse;
  const int& previousX;
  const int& previousY;
  const int& doubleClickGate;
  const int& alternateDoubleClickGate;

  bool Dispatch();
};

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
  UIWindow*& hover;

  std::array<UIWindow**, 7> refresh;
  const int& refreshGate;

  int Dispatch();
};

extern int g_alternateDoubleClickGate;
extern int g_mouseRefreshGate;

#endif
