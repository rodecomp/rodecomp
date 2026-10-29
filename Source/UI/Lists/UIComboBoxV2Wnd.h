#ifndef RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXV2WND_H
#define RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXV2WND_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIListBoxEx;

class UIComboBoxV2Wnd : public UIWindow {
public:
  void Slot31(int x, int y) override;
  void Slot35(int value) override;

  /* 007C */ std::string text;
  /* 0094 */ int selected;
  /* 0098 */ UIWindow* unknown_0098;
  /* 009C */ UIListBoxEx* list;
};

#endif
