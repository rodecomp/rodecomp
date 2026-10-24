#ifndef RAGNAROK_SOURCE_UI_WINDOWMANAGERWINDOWS_H
#define RAGNAROK_SOURCE_UI_WINDOWMANAGERWINDOWS_H

class UIWindow;

// Source boundary for UIWindowMgr lookup and factory. Concrete owner/factory must implement all required window types.
class WindowManagerWindows {
public:
  virtual UIWindow* LookupWindow(int id) = 0;
  virtual UIWindow* EnsureWindow(int id) = 0;
  // Close operation, including modal and retirement policy.
  virtual bool CloseWindow(int id) = 0;

protected:
  ~WindowManagerWindows() = default;
};

#endif
