#ifndef RAGNAROK_SOURCE_UI_LISTS_UITREEVIEW_H
#define RAGNAROK_SOURCE_UI_LISTS_UITREEVIEW_H

#include "UI/Window/UIWindow.h"

class UIScrollBar;

class UITreeView : public UIWindow {
public:
  void Resize(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int value) override;
  virtual void Slot53();

protected:
  /* 007C */ UIScrollBar* verticalBar;
  /* 0080 */ int verticalOverflow;
  /* 0084 */ unsigned char unknown_0084[0x4];
  /* 0088 */ int unknown_0088;
  /* 008C */ int unknown_008C;
  /* 0090 */ int unknown_0090;
  /* 0094 */ unsigned char unknown_0094[0xC];
  /* 00A0 */ int itemHeight;
  /* 00A4 */ int itemCount;
  /* 00A8 */ int unknown_00A8;
  /* 00AC */ unsigned char unknown_00AC;
};

#endif
