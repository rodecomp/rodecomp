#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMSHOPWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMSHOPWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Items/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>
#include <vector>

class UICheckBox;
class UIStaticText;

class UIItemShopWnd : public UIItemBaseWnd {
public:
  UIItemShopWnd();
  std::vector<unsigned char> BuildReplayRecords();
  void StoreInfo() override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  /* 00F0 */ UIStaticText* title;

public:
  /* 00F4 */ UICheckBox* checkBox;
  /* 00F8 */ int sellMode;
  /* 00FC */ ItemShopWndInfo* saved;
  /* 0100 */ int hovered;
  /* 0104 */ int bottomPadding;
  /* 0108 */ int footerHeight;
  /* 010C */ int showFooter;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemShopWnd, saved) == 0xFC);
#endif

#endif
