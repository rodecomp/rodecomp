#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTSHOPMAKEWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTSHOPMAKEWND_H

#include "Contents/Items/ItemInfo.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <cstdint>
#include <list>
#include <string>

class UIBasicButton;
class UIEditCtrl;
struct UIButtonContext;

class UIMerchantShopMakeWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot25(int x, int y) override;
  void Slot35(int value) override;

  __declspec(noinline) ITEM_INFO GetItemAt(int x, int y);
  void SetSelection(short row, short column);
  bool HasSelection() const;
  void SendShopRequest();
  void AddActionButton();
  void SetSuspended(unsigned char suspend);

  /* 00BC */ unsigned char unknown_00BC[0x10];
  /* 00CC */ UIEditCtrl* unknown_00CC;
  /* 00D0 */ unsigned char unknown_00D0[0x58];
  /* 0128 */ UIEditCtrl* unknown_0128;
  /* 012C */ int unknown_012C;
  /* 0130 */ int mode;
  /* 0134 */ UIWindow* scrollBar;
  /* 0138 */ int scroll;
  /* 013C */ int columns;
  /* 0140 */ int unknown_0140;
  /* 0144 */ int unknown_0144;
  /* 0148 */ std::list<ITEM_INFO> items;
  /* 0150 */ MerchantShopMakeWndInfo* saved;
  /* 0154 */ int unknown_0154;
  /* 0158 */ int unknown_0158;
  /* 015C */ int visibleRows;
  /* 0160 */ int unknown_0160;
  /* 0164 */ int unknown_0164;
  /* 0168 */ UIBasicButton* unknown_0168;
  /* 016C */ int unknown_016C;
  /* 0170 */ std::string unknown_0170;
  /* 0188 */ unsigned char unknown_0188;
  /* 0189 */ unsigned char suspended;
  /* 018A */ short unknown_018A;
  /* 018C */ short unknown_018C;

protected:
  UIButtonContext* m_controls;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantShopMakeWnd, unknown_00CC) == 0xCC);
static_assert(offsetof(UIMerchantShopMakeWnd, items) == 0x148);
static_assert(offsetof(UIMerchantShopMakeWnd, saved) == 0x150);
static_assert(offsetof(UIMerchantShopMakeWnd, unknown_018C) == 0x18C);
#endif

#endif
