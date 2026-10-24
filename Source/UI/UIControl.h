#ifndef RAGNAROK_SOURCE_UI_UICONTROL_H
#define RAGNAROK_SOURCE_UI_UICONTROL_H

#include "UI/UIWindow.h"

// RTTI CUIControl. Coordinate callbacks translate into the immediate parent's
// space; invalidation dirties the root subtree instead of only this control.
class CUIControl : public UIWindow {
public:
  explicit CUIControl(UIWindowContext& context);
  ~CUIControl() override;
  bool CanHitTest() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot34(int x, int y) override;
  void Slot35(int value) override;
  void Invalidate() override;
  void Slot52(int value) override;
  virtual void Slot53(int value);

  /* 007C */ unsigned char hitEnabled;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUIControl) == 132);
#endif

#endif
