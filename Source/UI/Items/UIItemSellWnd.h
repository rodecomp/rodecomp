#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMSELLWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMSELLWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Items/UIItemBaseWnd.h"

#include <cstddef>

class UIStaticText;

class UIItemSellWnd : public UIItemBaseWnd {
public:
  std::vector<unsigned char> GetReplayData() override;
  UIItemSellWnd();
  void StoreInfo() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  /* 00F0 */ UIStaticText* total;

public:
  /* 00F4 */ ItemSellWndInfo* saved;
  /* 00F8 */ int hovered;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemSellWnd, saved) == 0xF4);
#endif

#endif
