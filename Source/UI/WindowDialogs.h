#ifndef RAGNAROK_SOURCE_UI_WINDOWDIALOGS_H
#define RAGNAROK_SOURCE_UI_WINDOWDIALOGS_H

#include <deque>
#include <map>
#include <set>

class UIWindow;
class UIMessageBox;
struct UIWindowContext;
struct UIFrameContext;
struct UIMessageBoxContext;
struct WindowAncestry;

// The current mode's virtual frame operation at slot 4. A modal dialog runs
// the real frame operation; it does not replace it with a private event loop.
class ModalFramePump {
public:
  virtual void Frame() = 0;

protected:
  ~ModalFramePump() = default;
};

// Borrowed UIWindowMgr state: result+4, suspended roots+368, rendered+380,
// capture+412, input+416, modal stack+424, alternate result+1140 and latest
// dialog+323948. No duplicate window manager or ownership container.
struct WindowDialogs {
  UIWindowContext& windows;
  UIFrameContext& frames;
  UIMessageBoxContext& dialog;
  int& result;
  std::set<UIWindow*>& suspendedRoots;
  WindowAncestry*& capture;
  WindowAncestry*& input;
  std::deque<UIWindow*>& modal;
  int& alternateResult;
  UIMessageBox*& latest;
  ModalFramePump*& mode;
  const bool& suppressed;
  const int& immediate;
  const std::map<int, int>& frameOptions;

  UIWindow* Top() const;
  void RestoreInput();
  int Confirm(
    const char* text,
    int type,
    bool defaultAccept,
    bool replace,
    int timeout,
    const char* caption,
    int width,
    int height,
    unsigned char protect
  );
};

#endif
