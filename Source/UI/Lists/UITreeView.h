#ifndef RAGNAROK_SOURCE_UI_LISTS_UITREEVIEW_H
#define RAGNAROK_SOURCE_UI_LISTS_UITREEVIEW_H

#include "UI/Window/UIWindow.h"

class UITreeView : public UIWindow {
public:
  void Slot31(int x, int y) override;

protected:
  /* 007C */ unsigned char unknown_007C[0xC];
  /* 0088 */ int unknown_0088;
  /* 008C */ int unknown_008C;
  /* 0090 */ int unknown_0090;
  /* 0094 */ unsigned char unknown_0094[0x18];
  /* 00AC */ unsigned char unknown_00AC;
};

#endif
