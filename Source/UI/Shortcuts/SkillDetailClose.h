#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SKILLDETAILCLOSE_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SKILLDETAILCLOSE_H

#include "UI/Window/UIWindowMgr.h"

#include <deque>
#include <map>

class UIWindow;
struct FrameFocus;

struct SkillDetailClose {
  UIWindow*& window;
  int& visible;
  std::map<int, SavedWindowRectangle>& saved;
  const std::deque<int>& modalStack;
  UIWindow*& reference_4F1D4;
  std::map<int, UIWindow*>& windowsById;
  UIWindowMgr& retirement;
  FrameFocus& focus;

  bool Close();
};

#endif
