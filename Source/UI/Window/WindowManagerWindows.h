#ifndef RAGNAROK_SOURCE_UI_WINDOW_WINDOWMANAGERWINDOWS_H
#define RAGNAROK_SOURCE_UI_WINDOW_WINDOWMANAGERWINDOWS_H

class UIWindow;

class WindowManagerWindows {
public:
  virtual UIWindow* LookupWindow(int id) = 0;
  virtual UIWindow* EnsureWindow(int id) = 0;

  virtual bool CloseWindow(int id) = 0;

protected:
  ~WindowManagerWindows() = default;
};

#endif
