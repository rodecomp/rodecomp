#ifndef RAGNAROK_SOURCE_UI_ITEM_UIPARAITEMSHOPWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIPARAITEMSHOPWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Item/UIItemBaseWnd.h"
#include "UI/Item/UIItemPurchaseWnd.h"
#include "UI/Item/UIItemShopWnd.h"

#include <cstdint>
#include <vector>

class UIStaticText;

extern int g_pendingAid;
extern int g_pendingGid;

class UIParaItemPurchaseWnd : public UIItemPurchaseWnd {
public:
  UIParaItemPurchaseWnd();
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIParaItemShopWnd : public UIItemShopWnd {
public:
  UIParaItemShopWnd();
  std::vector<unsigned char> BuildReplayRecords();
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIParaResultWnd : public UIItemBaseWnd {
public:
  UIParaResultWnd();
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void AddItem(const RawItemBasic& item, int quantity);
  void AddItem(const ITEM_INFO& item);

  /* 00F0 */ UIStaticText* title;
  /* 00F4 */ UIStaticText* total;
  /* 00F8 */ int hovered;
  /* 00FC */ int footerHeight;
  /* 0100 */ int headerHeight;
};

#endif
