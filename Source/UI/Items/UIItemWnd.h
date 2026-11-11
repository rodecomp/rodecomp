#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Items/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

class UIBitmapButton;
class UIButton;
class UICheckBox;
class UITabControl;

class UIItemWnd : public UIItemBaseWnd {
public:
  UIItemWnd();
  __declspec(noinline) ~UIItemWnd() override;
  // INLINED
  bool NeedsFrameRefresh() override {
    return true;
  }
  void StoreInfo() override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnDestroy() override;
  void OnProcess() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void Paint() override;
  void OnCreate(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  ITEM_INFO GetItemAt(int x, int y);
  void ReleaseSelection(unsigned char keep);
  void DrawNewItemMark(int x, int y);
  void RefreshOptionBox();
  void RefreshSortButton();
  void RefreshItems();

  /* 00F0 */ ItemWndInfo* saved;
  /* 00F4 */ UIBitmapButton* baseButton;
  /* 00F8 */ UIBitmapButton* minimizeButton;
  /* 00FC */ UIBitmapButton* closeButton;
  /* 0100 */ int usableFilter;
  /* 0104 */ int equipmentFilter;
  /* 0108 */ int miscFilter;
  /* 010C */ int categoryFilter;
  /* 0110 */ int expandedHeight;
  /* 0114 */ UICheckBox* lockDropBox;
  /* 0118 */ UITabControl* tabs;
  /* 011C */ UIWindow* optionBox;
  /* 0120 */ UIBitmapButton* countButton;
  /* 0124 */ UIButton* dealLockButton;
  /* 0128 */ UIWindow* filterWindow;
  /* 012C */ UIButton* filterButton;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemWnd, saved) == 0xF0);
static_assert(offsetof(UIItemWnd, expandedHeight) == 0x110);
static_assert(offsetof(UIItemWnd, tabs) == 0x118);
static_assert(offsetof(UIItemWnd, filterWindow) == 0x128);
#endif

#endif
