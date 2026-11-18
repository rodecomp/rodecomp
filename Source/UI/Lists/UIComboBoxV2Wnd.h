#ifndef RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXV2WND_H
#define RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXV2WND_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIListBoxEx;
class UIBitmapButton;
class UIComboBoxEx;

class UIComboBoxV2Wnd : public UIWindow {
public:
  UIComboBoxV2Wnd();
  void Paint() override;
  void OnLBtnUp(int x, int y) override;
  void OnWheel(int value) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  std::string SelectedText();
  void Initialize(int width, int height, std::string prefix);

  /* 007C */ std::string text;
  /* 0094 */ int selected;
  /* 0098 */ UIBitmapButton* auxiliaryWindow;
  /* 009C */ UIComboBoxEx* list;
};

#endif
