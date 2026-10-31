#ifndef RAGNAROK_SOURCE_UI_SHOP_UINCASH_CALLWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UINCASH_CALLWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <list>

struct WindowPlacement;

class UInCashWnd : public UIFrameWnd {
public:
  bool Slot03() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot11() override;
  void Slot25(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot35(int delta) override;
  ITEM_INFO ItemAt(int x, int y);

  /* 00B4 */ unsigned char unknown_00B4[0x34];
  /* 00E8 */ std::list<ITEM_INFO> m_items;
  /* 00F0 */ unsigned char unknown_00F0[0x08];
  /* 00F8 */ std::list<ITEM_INFO> m_otherItems;
  /* 0100 */ WindowPlacement* m_placement;
  /* 0104 */ unsigned char unknown_0104[0x54];
  /* 0158 */ int m_firstOther;
  /* 015C */ int m_selected;
  /* 0160 */ unsigned char unknown_0160[0x04];
  /* 0164 */ int m_page;
  /* 0168 */ int m_tab;
  /* 016C */ unsigned char unknown_016C[0x08];
  /* 0174 */ int m_selectedIndex;
  /* 0178 */ unsigned char unknown_0178[0x04];
  /* 017C */ int m_rowHeight;
  /* 0180 */ int m_listTop;
  /* 0184 */ int m_listHeight;
};

class UInCash_CallWnd : public UIFrameWnd {
public:
  UInCash_CallWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

#endif
