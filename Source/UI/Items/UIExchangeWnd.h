#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIEXCHANGEWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIEXCHANGEWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <list>
#include <string>

class UIEditCtrl;
class UICheckBox;

class UIExchangeWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIWindow* m_buttons[3];
  /* 00C0 */ UIEditCtrl* unknown_00C0;
  /* 00C4 */ UIWindow* m_scrollBar;
  /* 00C8 */ unsigned char unknown_00C8[0x04];
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ int unknown_00DC;
  /* 00E0 */ std::list<ITEM_INFO> m_myItems;
  /* 00E8 */ std::list<ITEM_INFO> m_otherItems;
  /* 00F0 */ std::string unknown_00F0;
  /* 0108 */ int unknown_0108;
  /* 010C */ short unknown_010C;
  /* 010E */ unsigned char unknown_010E[0x04];
  /* 0112 */ short m_rowHeight;
  /* 0114 */ short m_hoverX;
  /* 0116 */ unsigned char unknown_0116[0x02];
  /* 0118 */ UICheckBox* unknown_0118;
  /* 011C */ unsigned char unknown_011C;

  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot35(int delta) override;

  void LayoutButtons();
  void CancelInput();
  ITEM_INFO ItemAt(int x, int y);
};

#endif
