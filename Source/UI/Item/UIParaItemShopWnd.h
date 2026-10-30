#ifndef RAGNAROK_SOURCE_UI_ITEM_UIPARAITEMSHOPWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIPARAITEMSHOPWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Item/UIItemBaseWnd.h"
#include "UI/Item/UIItemPurchaseWnd.h"
#include "UI/Item/UIItemShopWnd.h"

#include <cstdint>

class UIParaItemPurchaseWnd : public UIItemPurchaseWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
};

class UIParaItemShopWnd : public UIItemShopWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
};

class UIParaResultWnd : public UIItemBaseWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;

  void AddItem(const RawItemBasic& item, int quantity);
  void AddItem(const ITEM_INFO& item);
};

#endif
