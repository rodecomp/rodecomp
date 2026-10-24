#ifndef RAGNAROK_SOURCE_UI_INPUTFOCUS_H
#define RAGNAROK_SOURCE_UI_INPUTFOCUS_H

class UIWindow;

// Original manager +0x1A0, distinct from mouse capture at +0x19C.
struct InputFocus {
  UIWindow*& window;
  void Set(UIWindow* value);
  void First();
  void Next();
  void Advance();
  // Common mouse tail: invalidate current focus, reread after callback, then
  // invalidate its root unless it is the manager's excluded frame.
  void Refresh(UIWindow* const& excludedRoot);
};

void SetPressedWindowRoot(UIWindow*& pressedRoot, UIWindow* window);

#endif
