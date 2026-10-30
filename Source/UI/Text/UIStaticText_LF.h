#ifndef RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXT_LF_H
#define RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXT_LF_H

#include "UI/Window/UIWindow.h"

class UIStaticText_LF : public UIWindow {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;
};

#endif
