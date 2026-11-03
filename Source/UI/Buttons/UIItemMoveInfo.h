#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIITEMMOVEINFO_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIITEMMOVEINFO_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Buttons/UITextButton.h"

class UIItemMoveInfo : public UITextButton {
  friend class UIItemCollectionWnd;

public:
  UIItemMoveInfo();
  void Paint() override;
  void Slot28(int x, int y) override;
  void Slot48() override;

protected:
  /* 00F8 */ int m_moveIndex;

protected:
  /* 0100 */ ITEM_INFO m_item;
};

#endif
