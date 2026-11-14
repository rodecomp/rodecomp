#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMSHOPWND2_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMSHOPWND2_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Items/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

class UICheckBox;
class UIStaticText;

class UIItemShopWnd2 : public UIItemBaseWnd {
public:
  UIItemShopWnd2();
  void StoreInfo() override;
  void Paint() override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnCreate(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00F0 */ UIStaticText* title;

public:
  /* 00F4 */ UICheckBox* checkBox;
  /* 00F8 */ int interactionDisabled;
  /* 00FC */ ItemShopWndInfo* saved;
  /* 0100 */ int hovered;
  /* 0104 */ int bottomBarHeight;
  /* 0108 */ int extraBottomHeight;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemShopWnd2, saved) == 0xFC);
#endif

#endif
