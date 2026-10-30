#ifndef RAGNAROK_SOURCE_UI_WINDOW_WINDOWDIALOGS_H
#define RAGNAROK_SOURCE_UI_WINDOW_WINDOWDIALOGS_H

#include <deque>
#include <map>
#include <set>

class UIWindow;
class UIMessageBox;
struct UIWindowContext;
struct UIFrameContext;
struct UIMessageBoxContext;

class ModalFramePump {
public:
  virtual void Frame() = 0;

protected:
  ~ModalFramePump() = default;
};

struct WindowDialogs {
  UIWindowContext& windows;
  UIFrameContext& frames;
  UIMessageBoxContext& dialog;
  int& result;
  std::set<UIWindow*>& suspendedRoots;
  UIWindow*& capture;
  UIWindow*& input;
  std::deque<UIWindow*>& modal;
  int& alternateResult;
  UIMessageBox*& latest;
  ModalFramePump*& mode;
  const bool& suppressed;
  const int& immediate;
  const std::map<int, int>& frameOptions;

  UIWindow* Top() const;
  void RestoreInput();
  int Confirm(const char* text, int type, bool defaultAccept, bool replace, int timeout, const char* caption, int width, int height, unsigned char protect);
  void AddMessageBox(
    const char* text, int type, bool defaultAccept, int unused, int timeout, const char* caption, int width, int height, const char* icon, bool track
  );
  int RunModal(UIWindow* window);
};

extern int g_dialogKeyboardEnabled;
extern int g_dialogKeyboardBlocked;

#endif
