#ifndef RAGNAROK_SOURCE_UI_WINDOW_INPUTFOCUS_H
#define RAGNAROK_SOURCE_UI_WINDOW_INPUTFOCUS_H

class UIWindow;

struct InputFocus {
  UIWindow*& window;
  void Set(UIWindow* value);
  void First();
  void Last();
  void Next();
  bool IsLast(UIWindow* value);
  void Previous();

  void Refresh(UIWindow* const& excludedRoot);
};

void SetPressedWindowRoot(UIWindow*& pressedRoot, UIWindow* window);

#endif
