#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIVIDEOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIVIDEOWND_H

#include "UI/Buttons/UIButton.h"
#include "UI/Window/UIWindow.h"

#include <cstdint>

class BinkMovie;
class CTexture;
class WindowManagerWindows;
struct WindowDialogs;

struct UIVideoWndContext {
  WindowManagerWindows& windows;
  WindowDialogs& dialogs;
  UIButtonContext& buttons;
};

class UIVideoSkipButton : public UIButton {
public:
  // INLINED
  UIVideoSkipButton(UIWindowContext& windowContext, UIButtonContext& buttonContext) : UIButton(windowContext, buttonContext) {
  }

  void Paint() override;
};

class UIVideoWnd : public UIWindow {
public:
  UIVideoWnd(UIWindowContext& windowContext, UIVideoWndContext& videoContext);
  ~UIVideoWnd() override;
  bool Slot02() override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ BinkMovie* m_movie;
  /* 0080 */ CTexture* m_texture;
  /* 0084 */ int m_savedVolume;
  UIVideoWndContext* m_videoContext;
};

#endif
