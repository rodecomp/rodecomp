#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSELLWND2_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSELLWND2_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <vector>

class UIStaticText;

class UIItemSellWnd2 : public UIItemBaseWnd {
public:
  UIItemSellWnd2();
  std::vector<unsigned char> GetReplayData() override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void StoreInfo() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void OnCreate(int width, int height) override;

protected:
  /* 00F0 */ UIStaticText* total;

public:
  /* 00F4 */ ItemSellWndInfo* saved;
  /* 00F8 */ int hovered;
  /* 00FC */ int unknown_00FC;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemSellWnd2, saved) == 0xF4);
#endif

#endif
