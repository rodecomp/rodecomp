#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SKILLDETAILCLOSE_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SKILLDETAILCLOSE_H

#include <deque>
#include <map>

class UIWindow;
struct WindowRetirement;
struct FrameFocus;

struct SavedWindowRectangle {
  int x;
  int y;
  int width;
  int height;
};

struct SkillDetailClose {
  UIWindow*& window;
  int& visible;
  std::map<int, SavedWindowRectangle>& saved;
  const std::deque<int>& modalStack;
  UIWindow*& reference_4F1D4;
  std::map<int, UIWindow*>& windowsById;
  WindowRetirement& retirement;
  FrameFocus& focus;

  bool Close();
};

#endif
