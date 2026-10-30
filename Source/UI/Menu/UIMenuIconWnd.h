#ifndef RAGNAROK_SOURCE_UI_MENU_UIMENUICONWND_H
#define RAGNAROK_SOURCE_UI_MENU_UIMENUICONWND_H

#include "UI/Window/UIFrameWnd.h"

class UIMenuIconWnd : public UIFrameWnd {
public:
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
};

#endif
