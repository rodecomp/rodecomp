#ifndef RAGNAROK_SOURCE_UI_UIITEMREFORMWND_H
#define RAGNAROK_SOURCE_UI_UIITEMREFORMWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIFrameWnd.h"

#include <vector>

class UIItemReformWnd_List : public UIFrameWnd {
public:
  void Move(int x, int y) override;
  void Slot15(int width, int height) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int delta) override;

  /* 00B4 */ unsigned int itemHeight;
  /* 00B8 */ unsigned int visibleCount;
  /* 00BC */ unsigned int selectedIndex;
  /* 00C0 */ unsigned int hoverIndex;
  /* 00C4 */ unsigned int scrollPosition;
  /* 00C8 */ UIScrollBar* scrollBar;
  /* 00CC */ std::vector<ITEM_INFO> items;

  void UpdateScrollBar();
};

class UIItemReformWnd_Item : public UIWindow {
public:
  void Paint() override;

  /* 007C */ ITEM_INFO* item;
};

class UIItemReformWnd_DetailBtn : public UIButton {
public:
  void Paint() override;
  void Slot47() override;
  void Slot48() override;
};

class UIItemReformWnd_DetailMsg : public UIWindow {
public:
  void Paint() override;
};

#endif
