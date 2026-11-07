#ifndef RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOX_H

#include "UI/Lists/UIListBox.h"

#include <cstdint>

class UIComboBox : public UIListBox {
public:
  UIComboBox();
  void Resize(int width, int height) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Refresh() override;

  /* 00D4 */ int targetId;
};

extern int g_closeChatAfterGroups;

#endif
