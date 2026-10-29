#ifndef RAGNAROK_SOURCE_UI_LISTS_UIVSCROLLBAR_H
#define RAGNAROK_SOURCE_UI_LISTS_UIVSCROLLBAR_H

#include "UI/Window/UIControl.h"

#include <cstddef>
#include <functional>

class CUIVScrollBar : public CUIControl {
public:
  void Show(int visible) override;
  void Slot25(int x, int y) override;
  void Slot35(int value) override;
  virtual void Slot54();

  void SetTotal(int value);
  void SetVisible(int value);
  void SetPosition(int value);

  /* 0084 */ UIWindow* upButton;
  /* 0088 */ CUIControl* thumb;
  /* 008C */ int unknown_008C;
  /* 0090 */ int total;
  /* 0094 */ int visible;
  /* 0098 */ int step;
  /* 009C */ int position;
  /* 00A0 */ int maximum;
  /* 00A4 */ int trackLength;
  /* 00A8 */ unsigned char dragging;
  /* 00A9 */ unsigned char unknown_00A9[0x1F];
  /* 00C8 */ std::function<void(int)> onChange;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CUIVScrollBar, total) == 0x90);
static_assert(offsetof(CUIVScrollBar, dragging) == 0xA8);
static_assert(offsetof(CUIVScrollBar, onChange) == 0xC8);
#endif

#endif
