#ifndef RAGNAROK_SOURCE_UI_UIITEMREFORMWND_H
#define RAGNAROK_SOURCE_UI_UIITEMREFORMWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UIBasicButton;

class UIItemReformWnd_List : public UIFrameWnd {
public:
  void Move(int x, int y) override;
  void Slot15(int width, int height) override;
  void Paint() override;
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
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;

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

class UIItemReformWnd : public UIFrameWnd {
public:
  void Slot19() override;
  void Paint() override;
  void SelectItem(ITEM_INFO* item);

  /* 00B4 */ UIBasicButton* reformButton;
  /* 00B8 */ UIItemReformWnd_List* list;
  /* 00BC */ UIItemReformWnd_Item* baseIcon;
  /* 00C0 */ UIItemReformWnd_Item* resultIcon;
  /* 00C4 */ std::vector<UIItemReformWnd_Item*> materialIcons;
  /* 00D0 */ UIItemReformWnd_DetailBtn* detailButton;
  /* 00D4 */ UIItemReformWnd_DetailMsg* detailMsg;
  /* 00D8 */ ITEM_INFO reformItem;
  /* 01D0 */ ITEM_INFO* selectedItem;
  /* 01D8 */ ITEM_INFO resultItem;
  /* 02D0 */ std::vector<ITEM_INFO> materials;
  /* 02DC */ std::string information;
  /* 02F4 */ std::string warning;
};

#endif
