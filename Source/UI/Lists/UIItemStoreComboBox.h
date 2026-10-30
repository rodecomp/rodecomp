#ifndef RAGNAROK_SOURCE_UI_LISTS_UIITEMSTORECOMBOBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UIITEMSTORECOMBOBOX_H

#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Lists/UIComboBoxEx.h"

class UIItemStoreComboBox : public UIComboBoxEx {
public:
  void Slot15(int width, int height) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
};

class UIItemStoreComboBoxWnd : public UIComboBoxDefWnd {
public:
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

#endif
