#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMPURCHASEWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMPURCHASEWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

class UIEditCtrl;
class UIStaticText;

class UIItemPurchaseWnd : public UIItemBaseWnd {
public:
  std::vector<unsigned char> GetReplayData() override;
  UIItemPurchaseWnd();
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00F0 */ UIStaticText* title;
  /* 00F4 */ ItemPurchaseWndInfo* saved;
  /* 00F8 */ int hovered;
  /* 00FC */ int unknown_00FC;
  /* 0100 */ UIEditCtrl* quantityEdit;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemPurchaseWnd, saved) == 0xF4);
#endif

#endif
