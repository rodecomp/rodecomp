#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINOTEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINOTEWND_H

#include "UI/UIScrollWnd.h"
#include "UI/Window/UIFrameWnd.h"

class UINoteWnd_Scroll : public UIScrollWnd {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;
};

class UINoteWnd : public UIFrameWnd {
public:
  void Paint() override;
};

#endif
