#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIEXCHANGEWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIEXCHANGEWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <list>
#include <string>

class UIBitmapButton;
class UIEditCtrl;
class UICheckBox;

class UIExchangeWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIBitmapButton* m_buttons[3];
  /* 00C0 */ UIEditCtrl* zenyEdit;
  /* 00C4 */ UIWindow* m_scrollBar;
  /* 00C8 */ unsigned char padding_00C8[0x04];
  /* 00CC */ int isLocalTrader;
  /* 00D0 */ int localConfirmed;
  /* 00D4 */ int remoteConfirmed;
  /* 00D8 */ int localAccepted;
  /* 00DC */ int remoteAccepted;
  /* 00E0 */ std::list<ITEM_INFO> m_myItems;
  /* 00E8 */ std::list<ITEM_INFO> m_otherItems;
  /* 00F0 */ std::string partnerName;
  /* 0108 */ int partnerId;
  /* 010C */ short partnerLevel;
  /* 010E */ unsigned char padding_010E[0x04];
  /* 0112 */ short m_rowHeight;
  /* 0114 */ short m_hoverX;
  /* 0116 */ unsigned char padding_0116[0x02];
  /* 0118 */ UICheckBox* screenshotCheckBox;
  /* 011C */ unsigned char takeScreenshot;
  /* 0120 */ int m_moneyLength;
  /* 0124 */ std::string m_moneyText;

  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnWheel(int delta) override;
  void OnProcess() override;
  std::vector<unsigned char> GetReplayData() override;

  void LayoutButtons();
  void CancelInput();
  ITEM_INFO ItemAt(int x, int y);
};

#endif
