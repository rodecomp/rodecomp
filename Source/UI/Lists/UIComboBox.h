#ifndef RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOX_H

#include "UI/Lists/UIListBox.h"

class UIComboBox : public UIListBox {
public:
  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Refresh() override;
};

#endif
