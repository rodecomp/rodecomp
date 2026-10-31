#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINOTEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINOTEWND_H

#include "UI/UIScrollWnd.h"
#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UINoteWnd_Scroll : public UIScrollWnd {
public:
  // INLINED
  UINoteWnd_Scroll(UIWindowContext& windows, int width, int height) : UIScrollWnd(windows, width, height, 0), unknown_00B0(0) {
    this->SetSize(width, height);
  }
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;

  /* 00B0 */ int unknown_00B0;
};

class UINoteWnd : public UIFrameWnd {
public:
  UINoteWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot15(int width, int height) override;
  void Paint() override;

protected:
  /* 00B4 */ UINoteWnd_Scroll* m_scroll;
  UIButtonContext* m_controls;
};

#endif
