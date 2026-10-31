#ifndef RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOX_H

#include "UI/Lists/UIListBox.h"

#include <cstdint>

class UIComboBox : public UIListBox {
public:
  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Refresh() override;

  /* 00DC */ int targetId;
};


#endif
