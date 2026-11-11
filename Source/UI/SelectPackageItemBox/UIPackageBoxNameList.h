#ifndef RAGNAROK_SOURCE_UI_SELECTPACKAGEITEMBOX_UIPACKAGEBOXNAMELIST_H
#define RAGNAROK_SOURCE_UI_SELECTPACKAGEITEMBOX_UIPACKAGEBOXNAMELIST_H

#include "UI/Lists/UIListBox.h"

class UIPackageBoxNameList : public UIListBox {
public:
  UIPackageBoxNameList();
  void OnLBtnDown(int x, int y) override;
};

#endif
