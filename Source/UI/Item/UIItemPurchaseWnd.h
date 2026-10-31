#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMPURCHASEWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMPURCHASEWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

class UIStaticText;

class UIItemPurchaseWnd : public UIItemBaseWnd {
public:
  UIItemPurchaseWnd();
  UIItemPurchaseWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot11() override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;

  /* 00F0 */ UIStaticText* title;
  /* 00F4 */ ItemPurchaseWndInfo* saved;
  /* 00F8 */ int hovered;
  /* 00FC */ int unknown_00FC;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemPurchaseWnd, saved) == 0xF4);
#endif

#endif
