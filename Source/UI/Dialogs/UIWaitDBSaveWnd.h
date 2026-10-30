#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIWAITDBSAVEWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIWAITDBSAVEWND_H

#include "UI/Window/UIWindow.h"

#include <cstdint>

class CActRes;
class CSprRes;
class WindowManagerWindows;
struct UIButtonContext;

struct UIWaitDBSaveWndContext {
  WindowManagerWindows& windows;
  UIButtonContext& buttons;
};

class UIWaitDBSaveWnd : public UIWindow {
public:
  UIWaitDBSaveWnd(UIWindowContext& windowContext, UIWaitDBSaveWndContext& waitContext);
  void Slot15(int width, int height) override;
  void Slot18() override;
  void Slot19() override;
  void Paint() override;
  void Slot27(int x, int y) override;

  /* 007C */ int m_animation;
  /* 0080 */ int m_frame;
  /* 0084 */ unsigned int m_loopLimit;
  /* 0088 */ CSprRes* m_sprite;
  /* 008C */ CActRes* m_action;
  /* 0090 */ std::uint32_t m_startTime;
  /* 0094 */ unsigned int m_loop;
  /* 0098 */ std::uint32_t m_closeTime;
  UIWaitDBSaveWndContext* m_waitContext;
};

#endif
