#ifndef RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXV2WND_H
#define RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXV2WND_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIListBoxEx;

class UIComboBoxV2Wnd : public UIWindow {
public:
  explicit UIComboBoxV2Wnd(UIWindowContext& windows);
  void Paint() override;
  void Slot31(int x, int y) override;
  void Slot35(int value) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  std::string SelectedText();

  /* 007C */ std::string text;
  /* 0094 */ int selected;
  /* 0098 */ UIWindow* unknown_0098;
  /* 009C */ UIListBoxEx* list;
};

#endif
