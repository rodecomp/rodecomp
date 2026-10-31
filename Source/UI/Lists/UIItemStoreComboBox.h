#ifndef RAGNAROK_SOURCE_UI_LISTS_UIITEMSTORECOMBOBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UIITEMSTORECOMBOBOX_H

#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Lists/UIComboBoxEx.h"

class UIItemStoreComboBox : public UIComboBoxEx {
public:
  UIItemStoreComboBox();
  UIItemStoreComboBox(UIWindowContext& windows, UIComboBoxContext& combo) : UIComboBoxEx(windows, combo), pink(false) {}
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;

  /* 00C4 */ bool pink;
};

class UIItemStoreComboBoxWnd : public UIComboBoxDefWnd {
public:
  UIItemStoreComboBoxWnd(UIWindowContext& windows, UIComboBoxHeaderContext& controls);
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00A8 */ unsigned char pink;
};

#endif
