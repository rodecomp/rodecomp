#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIITEMMOVEINFO_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIITEMMOVEINFO_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Window/WindowManagerWindows.h"

struct WindowVisibility;

class UIItemMoveInfo : public UITextButton {
  friend class UIItemCollectionWnd;

public:
  UIItemMoveInfo(UIWindowContext& windows, UIButtonContext& buttons, WindowManagerWindows& windowList, WindowVisibility& visibility);
  void Paint() override;
  void Slot28(int x, int y) override;
  void Slot48() override;

protected:
  /* 00F8 */ int m_moveIndex;

private:
  WindowManagerWindows* m_windows;
  WindowVisibility* m_visibility;

protected:
  /* 0100 */ ITEM_INFO m_item;
};

#endif
