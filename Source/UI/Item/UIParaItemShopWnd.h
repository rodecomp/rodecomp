#ifndef RAGNAROK_SOURCE_UI_ITEM_UIPARAITEMSHOPWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIPARAITEMSHOPWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Item/UIItemBaseWnd.h"
#include "UI/Item/UIItemPurchaseWnd.h"
#include "UI/Item/UIItemShopWnd.h"

#include <cstdint>

class UIStaticText;

class UIParaItemPurchaseWnd : public UIItemPurchaseWnd {
public:
  void Slot25(int x, int y) override;
  void Slot10(int x, int y, std::intptr_t value) override;
};

class UIParaItemShopWnd : public UIItemShopWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot32(int x, int y) override;
};

class UIParaResultWnd : public UIItemBaseWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Slot28(int x, int y) override;

  void AddItem(const RawItemBasic& item, int quantity);
  void AddItem(const ITEM_INFO& item);

  /* 00F0 */ UIStaticText* title;
  /* 00F4 */ UIStaticText* total;
  /* 00F8 */ int unknown_00F8;
  /* 00FC */ int unknown_00FC;
  /* 0100 */ int unknown_0100;
};

#endif
