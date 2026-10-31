#ifndef RAGNAROK_SOURCE_UI_LISTS_UIPACKAGEBOXNAMELIST_H
#define RAGNAROK_SOURCE_UI_LISTS_UIPACKAGEBOXNAMELIST_H

#include "UI/Lists/UIListBox.h"

class UIPackageBoxNameList : public UIListBox {
public:
  UIPackageBoxNameList(UIWindowContext& windows, UIListBoxContext& list);
  void Slot25(int x, int y) override;
};

#endif
