#ifndef RAGNAROK_SOURCE_UI_WINDOW_WINDOWRETIREMENT_H
#define RAGNAROK_SOURCE_UI_WINDOW_WINDOWRETIREMENT_H

#include <deque>

class UIWindow;
class UIWindowMgr;

struct WindowRetirement {
  UIWindowMgr& windows;

  void DrainPending(const std::deque<UIWindow*>& modal, UIWindow*& reference_4F16C, UIWindow*& capture, UIWindow*& input, UIWindow*& hover);
};

#endif
