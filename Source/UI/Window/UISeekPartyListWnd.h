#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYLISTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYLISTWND_H

#include "Contents/Party/PartyBookingInfo.h"
#include "UI/Window/UIFrameWnd.h"

class UICheckBox;
class UIScrollBar;

class UISeekPartyListWnd : public UIFrameWnd {
public:
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot35(int delta) override;
  void NotifyNewAd();
  bool MatchesFilter(PARTY_BOOKING_AD_INFO info);

protected:
  int HitTestRow(int x, int y);

  /* 00BC */ unsigned char unknown_00BC[0x34];
  /* 00F0 */ UICheckBox* m_alarmCheck;
  /* 00F4 */ unsigned char unknown_00F4[0x14];
  /* 0108 */ UIScrollBar* m_scrollBar;
  /* 010C */ unsigned char unknown_010C[0x98];
  /* 01A4 */ int m_firstRow;
  /* 01A8 */ int m_hoverIndex;
  /* 01AC */ int m_columnCount;
  /* 01B0 */ int m_visibleRows;
  /* 01B4 */ int unknown_01B4;
  /* 01B8 */ int m_scrollable;
  /* 01BC */ unsigned char unknown_01BC[0x8];
  /* 01C4 */ short m_filterLevel;
  /* 01C6 */ short m_filterMap;
  /* 01C8 */ short m_filterJob;
};

#endif
