#ifndef RAGNAROK_SOURCE_UI_UISTATICITEM_H
#define RAGNAROK_SOURCE_UI_UISTATICITEM_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIWindow.h"

struct ItemDescriptions;
struct UIButtonContext;
class WindowManagerWindows;

class UIStaticItem : public UIWindow {
public:
  void Paint() override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;

protected:
  /* 0080 */ ITEM_INFO m_item;
  ItemDescriptions* m_descriptions;
  UIButtonContext* m_controls;
  WindowManagerWindows* m_windows;
};

class UIStaticItem_Illust : public UIStaticItem {
public:
  void Paint() override;
  void Slot28(int x, int y) override;
};

#endif
