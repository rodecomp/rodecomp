#ifndef RAGNAROK_SOURCE_UI_UISTATICITEM_H
#define RAGNAROK_SOURCE_UI_UISTATICITEM_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIWindow.h"

#include "UI/Window/WindowManagerWindows.h"

class UIStaticItem : public UIWindow {
public:
  UIStaticItem();
  ~UIStaticItem() override;
  void Paint() override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;

protected:
  /* 0080 */ ITEM_INFO m_item;
};

class UIStaticItem_Illust : public UIStaticItem {
public:
  void Paint() override;
  void Slot28(int x, int y) override;
};

#endif
